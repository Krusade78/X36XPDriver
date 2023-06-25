/*++

Copyright (c) 2005 Alfredo Costalago

Module Name:

    pnp.c

Abstract: This module contains routines Generate the HID report and
    configure the joystick.

Environment:

    Kernel mode


--*/

#include <wdm.h>
#include <hidport.h>
#include <hidtoken.h>
#include <hidusage.h>
#include "hidgame.h"
#include "registro.h"
#include "i386\timing.h"
#include "analog_def.h"
#define _HIDJOYC_
#include "hidjoy.h"


#ifdef ALLOC_PRAGMA
    #pragma alloc_text (PAGE, HGM_MapAxesFromDevExt)
    #pragma alloc_text (PAGE, HGM_GenerateReport)
    #pragma alloc_text (PAGE, HGM_JoystickConfig)
    #pragma alloc_text (PAGE, HGM_InitAnalog)
#endif

/*****************************************************************************
 *
 *  @func   NTSTATUS  | HGM_InitAnalog |
 *
 *          Check that the configuration is valid whilst there is still time 
 *          to refuse it.  
 *          <nl>Detect and validate sibling relationships and call 
 *          HGM_JoystickConfig for the rest of the work.
 *
 *  @parm   IN PDEVICE_OBJECT | DeviceObject |
 *
 *          Pointer to the device object
 *
 *  @rvalue   STATUS_SUCCESS  | success
 *  @rvalue   STATUS_DEVICE_CONFIGURATION_ERROR  | Invalid configuration specified
 *
 *****************************************************************************/
/*
 *  Disable warning for variable used before set as it is hard for a compiler 
 *  to see that the use of DeviceExtension_Sibling is gated by a flag which 
 *  can only be set after DeviceExtension_Sibling is initialized.
 */
#pragma warning( disable:4701 )
NTSTATUS HGM_InitAnalog
    (
    IN PDEVICE_OBJECT         DeviceObject
    )
{
    NTSTATUS            ntStatus;
    PDEVICE_EXTENSION   DeviceExtension;
    PDEVICE_EXTENSION   DeviceExtension_Sibling;
    PLIST_ENTRY         pEntry;

#define ARE_WE_RELATED(_x_, _y_)                                \
    (                                                           \
        (_x_)->GameContext     == (_y_)->GameContext      &&    \
        (_x_)->WriteAccessor   == (_y_)->WriteAccessor    &&    \
        (_x_)->ReadAccessor    == (_y_)->ReadAccessor           \
    )

    PAGED_CODE ();
    
    /*
     * Get a pointer to the device extension
     */
    DeviceExtension = GET_MINIDRIVER_DEVICE_EXTENSION(DeviceObject);
    

    /*
     *  No modifications to the Global List while we are looking at it
     */
    ExAcquireFastMutex (&Global.Mutex);

    /*
     *  For two joysticks interface two fdos are created to service them 
     *  but physically they both share the same port.
     *  For the second sibling certain extra rules must be applied so we 
     *  search our list of devices for another device using the same port 
     *  and if we find one mark this one as a sibling.
     */
    for(pEntry = Global.DeviceListHead.Flink;
       pEntry != &Global.DeviceListHead;
       pEntry = pEntry->Flink)
    {

        /*
         * Obtain the device Extension of the Sibling
         */
        DeviceExtension_Sibling = CONTAINING_RECORD(pEntry, DEVICE_EXTENSION, Link);

        if(       DeviceExtension_Sibling != DeviceExtension
               && TRUE == ARE_WE_RELATED(DeviceExtension, DeviceExtension_Sibling)
               && TRUE == DeviceExtension_Sibling->fStarted )
        {

                DeviceExtension->fSiblingFound = TRUE;
           break;
        }
    }

    /*
     *  We are done, release the Mutex
     */
    ExReleaseFastMutex (&Global.Mutex);

    /*
     * check the axis and button configuration for the joystick
     */
    ntStatus = HGM_JoystickConfig(DeviceObject);

    if( NT_SUCCESS( ntStatus ) )
    {
        /*
         *  Make sure that sibling axes are not overlapped
         */
        if(  DeviceExtension->fSiblingFound &&
             (DeviceExtension_Sibling->resistiveInputMask & DeviceExtension->resistiveInputMask) != 0x0 )
        {

            ntStatus = STATUS_DEVICE_CONFIGURATION_ERROR;

        }
    }

    return( ntStatus );

} /* HGM_InitAnalog */

/*****************************************************************************
 *
 *  @func   NTSTATUS  | HGM_JoystickConfig |
 *
 *          Check that the configuration is valid whilst there is still time 
 *          to refuse it.
 *          <nl>HGM_GenerateReport uses the results generated here if the 
 *          settings are OK.
 *
 *  @parm   IN PDEVICE_OBJECT | DeviceObject |
 *
 *          Pointer to the device object
 *
 *  @rvalue   STATUS_SUCCESS  | success
 *  @rvalue   STATUS_DEVICE_CONFIGURATION_ERROR  | Invalid configuration specified
 *
 *****************************************************************************/
NTSTATUS HGM_JoystickConfig 
    (
    IN PDEVICE_OBJECT         DeviceObject
    )
{
    PDEVICE_EXTENSION   DeviceExtension;
    POEMDATA            OemData;
    NTSTATUS            ntStatus;
    int                 Idx;

    PAGED_CODE();


    /*
     * Get a pointer to the device extension
     */

    DeviceExtension = GET_MINIDRIVER_DEVICE_EXTENSION(DeviceObject);

    ntStatus = HGM_MapAxesFromDevExt( DeviceExtension );

    {
        /*
         * Calculate time thresholds for analog device
         */
        if( ( DeviceExtension->HidGameOemData.OemData[0].Timeout < ANALOG_POLL_TIMEOUT_MIN )
          ||( DeviceExtension->HidGameOemData.OemData[0].Timeout > ANALOG_POLL_TIMEOUT_MAX ) )
        {
            DeviceExtension->ScaledTimeout = (ULONG)( ( (ULONGLONG)ANALOG_POLL_TIMEOUT_DFT
                                                      * (ULONGLONG)(AXIS_FULL_SCALE<<SCALE_SHIFT) )
                                                    / (ULONGLONG)ANALOG_POLL_TIMEOUT_MAX );
        }
        else
        {
            DeviceExtension->ScaledTimeout = (ULONG)( ( (ULONGLONG)DeviceExtension->HidGameOemData.OemData[0].Timeout
                                                      * (ULONGLONG)(AXIS_FULL_SCALE<<SCALE_SHIFT) )
                                                    / (ULONGLONG)ANALOG_POLL_TIMEOUT_MAX );
        }

        /*
         *  Use one quarter of the minimum poll timeout as a starting value 
         *  for the time between two polls which will be considered to have 
         *  been interrupted.
         */
        DeviceExtension->ScaledThreshold = (ULONG)( ( (ULONGLONG)ANALOG_POLL_TIMEOUT_MIN
                                                    * (ULONGLONG)AXIS_FULL_SCALE )
                                                  / (ULONGLONG)ANALOG_POLL_TIMEOUT_MAX )>>2;
    }


    /*
     *  Set initial values of LastGoodAxis so that the device will not show
     *  up as present until we get at least one valid poll.
     */
    for( Idx = MAX_AXES; Idx >= 0; Idx-- )
    {
        DeviceExtension->LastGoodAxis[Idx] = AXIS_TIMEOUT;
    }

    return ntStatus;
} /* HGM_JoystickConfig */





/*****************************************************************************
 *
 *  @func   NTSTATUS  | HGM_MapAxesFromDevExt |
 *
 *          Use the flags in the DeviceExtension to generate mappings for each 
 *          axis.  
 *          <nl>This is called both from HGM_JoystickConfig to validate the 
 *          configuration and HGM_GenerateReport to use the axis maps.
 *          
 *
 *  @parm   IN OUT PDEVICE_EXTENSION | DeviceExtension |
 *
 *          Pointer to the minidriver device extension
 *
 *  @rvalue   STATUS_SUCCESS | success
 *  @rvalue   STATUS_DEVICE_CONFIGURATION_ERROR | The configuration is invalid
 *
 *****************************************************************************/
NTSTATUS HGM_MapAxesFromDevExt
    (
    IN OUT PDEVICE_EXTENSION DeviceExtension 
    )
{
//=====================Definiciones==============================

const unsigned char XLU[8] = { X1,Y1,X2,NA,Y2,NA,NA,NA };
const unsigned char YLU[8] = { Y1,X1,X2,NA,Y2,NA,NA,NA };
const unsigned char RLU[8] = { X2,Y1,X1,NA,Y2,NA,NA,NA };
const unsigned char ZLU[8] = { Y2,Y1,X1,NA,X2,NA,NA,NA };
const unsigned char cAxisIndexTable[9] = { 0, 0, 1, 0, 2, 0, 0, 0, 3 };

//==============================================================
    NTSTATUS    ntStatus;
    ULONG       dwFlags;
    int         nAxis;
    UCHAR       AxisMask;

    ntStatus = STATUS_SUCCESS;

    dwFlags = DeviceExtension->HidGameOemData.OemData[(DeviceExtension->fSiblingFound!=0)].joy_hws_dwFlags;  

    /* 
     *  Check X and Y last as Z, R and POV must not overlap
     *  The are no flags to indicate the presence of X or Y so if they 
     *  overlap, this indicates that they are not used,
     */

    DeviceExtension->resistiveInputMask = 0;
    for( nAxis=MAX_AXES; nAxis>=0; nAxis-- )
    {
        DeviceExtension->AxisMap[nAxis] = INVALID_INDEX;
    }
    nAxis = 0;

    if( dwFlags & JOY_HWS_HASZ )
    {
        AxisMask = ZLU[(dwFlags & ZMAPBITS) >> ZMAPSHFT];
        if( AxisMask >= NA )
        {
            ntStatus = STATUS_DEVICE_CONFIGURATION_ERROR; 
        }
        else
        {
            nAxis = 1;
            DeviceExtension->resistiveInputMask = AxisMask;
            DeviceExtension->AxisMap[ZIS] = cAxisIndexTable[AxisMask];
        }
    }


    if( dwFlags & JOY_HWS_HASR )
    {
        AxisMask = RLU[(dwFlags & RMAPBITS) >> RMAPSHFT];
        if( AxisMask >= NA )
        {
            ntStatus = STATUS_DEVICE_CONFIGURATION_ERROR; 
        }
        else
        {
            if( DeviceExtension->resistiveInputMask & AxisMask )
            {
                ntStatus = STATUS_DEVICE_CONFIGURATION_ERROR ; 
            }
            else
            {
                nAxis++;
                DeviceExtension->resistiveInputMask |= AxisMask;
                DeviceExtension->AxisMap[RIS] = cAxisIndexTable[AxisMask];
            }
        }
    }

    AxisMask = XLU[( dwFlags & XMAPBITS ) >> XMAPSHFT];
    if( AxisMask >= NA )
    {
        ntStatus = STATUS_DEVICE_CONFIGURATION_ERROR ; 
    }
    else
    {
        if( DeviceExtension->resistiveInputMask & AxisMask )
        {
            //("HGM_MapAxesFromDevExt: X axis mapped to same as another axis") );
        }
        else
        {
            nAxis++;
            DeviceExtension->resistiveInputMask |= AxisMask;
            DeviceExtension->AxisMap[XIS] = cAxisIndexTable[AxisMask];
        }
    }

    AxisMask = YLU[( dwFlags & YMAPBITS ) >> YMAPSHFT];
    if( AxisMask >= NA )
    {
        ntStatus = STATUS_DEVICE_CONFIGURATION_ERROR ; 
    }
    else
    {
        if( DeviceExtension->resistiveInputMask & AxisMask )
        {
            //("HGM_MapAxesFromDevExt: Y axis mapped to same as another axis") );
        }
        else
        {
            nAxis++;
            DeviceExtension->resistiveInputMask |= AxisMask;
            DeviceExtension->AxisMap[YIS] = cAxisIndexTable[AxisMask];
        }
    }

    if( nAxis != DeviceExtension->nAxes )
    {
        ntStatus = STATUS_DEVICE_CONFIGURATION_ERROR ;
    }

#include "analog_udef.h"

    return( ntStatus );
} /* HGM_MapAxesFromDevExt */










/*****************************************************************************
 *
 *  @doc    EXTERNAL
 *
 *  @func   NTSTATUS  | HGM_GenerateReport |
 *
 *          Generates a hid report descriptor for a n-axis, m-button joystick,
 *          depending on number of buttons and joy_hws_flags field.
 *
 *  @parm   IN PDEVICE_OBJECT | DeviceObject |
 *
 *          Pointer to the device object
 *
 *  @parm   IN OUT UCHAR * | rgGameReport[MAXBYTES_GAME_REPORT] |
 *
 *          Array that receives the HID report descriptor
 *
 *  @parm   OUT PUSHORT | pCbReport |
 *          
 *          Address of a short integer that receives size of 
 *          HID report descriptor. 
 *
 *  @rvalue   STATUS_SUCCESS  | success
 *  @rvalue   STATUS_BUFFER_TOO_SMALL  | Need more memory for HID descriptor
 *
 *****************************************************************************/
NTSTATUS HGM_GenerateReport
    (
    IN PDEVICE_OBJECT       DeviceObject, 
    OUT UCHAR               rgGameReport[MAXBYTES_GAME_REPORT],
    OUT PUSHORT             pCbReport
    )
{
    NTSTATUS    ntStatus;
    PDEVICE_EXTENSION DeviceExtension;
    UCHAR       *pucReport; 
    int         Idx;
    int         UsageIdx;
    ULONG       dwFlags; 

    int         InitialAxisMappings[MAX_AXES];


    typedef struct _USAGES
    {
        UCHAR UsagePage;
        UCHAR Usage;
    } USAGES, *PUSAGE;
  
    PUSAGE pJoyParams;

	USAGES JoystickMando[4] = {
		{ HID_USAGE_PAGE_GENERIC, HID_USAGE_GENERIC_X} , 
        { HID_USAGE_PAGE_GENERIC, HID_USAGE_GENERIC_Y} , 
        { HID_USAGE_PAGE_GENERIC, HID_USAGE_GENERIC_Z} ,
        { HID_USAGE_PAGE_SIMULATION, HID_USAGE_SIMULATION_RUDDER}
    };


    PAGED_CODE();

    /*
     *  Get a pointer to the device extension
     */

    DeviceExtension = GET_MINIDRIVER_DEVICE_EXTENSION(DeviceObject);

    /*
     *  Although the axes have already been validated and mapped in 
     *  HGM_JoystickConfig this function destroys the mapping when it compacts 
     *  the axes towards the start of the descriptor report.  Since this 
     *  function will be called once to find the descriptor length and then 
     *  again to read the report, the mappings are regenerated again each 
     *  time through.  Although this results in the parameters being 
     *  interpreted three times (for validation, descriptor size and 
     *  descriptor content) it avoids the possibility of a discrepancy in 
     *  implementation of separate functions.
     */

    ntStatus = HGM_MapAxesFromDevExt( DeviceExtension );

    pucReport = rgGameReport;

    dwFlags = DeviceExtension->HidGameOemData.OemData[(DeviceExtension->fSiblingFound!=0)].joy_hws_dwFlags;  


    /* 
     *  What manner of beast have we ?
     */
        pJoyParams = JoystickMando;

#define NEXT_BYTE( pReport, Data )   \
            *pReport++ = Data;    

#define NEXT_LONG( pReport, Data )   \
            *(((LONG UNALIGNED*)(pReport))++) = Data;

#define ITEM_DEFAULT        0x00 /* Data, Array, Absolute, No Wrap, Linear, Preferred State, Has no NULL */
#define ITEM_VARIABLE       0x02 /* as ITEM_DEFAULT but value is a variable, not an array */
#define ITEM_HASNULL        0x40 /* as ITEM_DEFAULT but values out of range are considered NULL */
#define ITEM_ANALOG_AXIS    ITEM_VARIABLE
#define ITEM_ROTARY			ITEM_VARIABLE
#define ITEM_DIGITAL_POV    (ITEM_VARIABLE|ITEM_HASNULL)
#define ITEM_BUTTON         ITEM_VARIABLE
#define ITEM_PADDING        0x01 /* Constant (nothing else applies) */


    /* USAGE_PAGE (Generic Desktop) */
    NEXT_BYTE(pucReport,    HIDP_GLOBAL_USAGE_PAGE_1);
    NEXT_BYTE(pucReport,    HID_USAGE_PAGE_GENERIC);

    /* USAGE (Joystick | GamePad ) */
	NEXT_BYTE(pucReport,    HIDP_LOCAL_USAGE_1);
	if( dwFlags & JOY_HWS_ISGAMEPAD ) {
	    NEXT_BYTE(pucReport,    HID_USAGE_GENERIC_GAMEPAD);
	} else {
	    NEXT_BYTE(pucReport,    HID_USAGE_GENERIC_JOYSTICK);
	}

	// Collection - Application
    NEXT_BYTE(pucReport,    HIDP_MAIN_COLLECTION); 
    NEXT_BYTE(pucReport,    0x1 ); 

    /* Logical Min is the smallest value that could be produced by a poll */
    NEXT_BYTE(pucReport,    HIDP_GLOBAL_LOG_MIN_4);
    NEXT_LONG(pucReport,    0 );

    /* Logical Max is the largest value that could be produced by a poll */
    NEXT_BYTE(pucReport,    HIDP_GLOBAL_LOG_MAX_4);
    NEXT_LONG(pucReport,    AXIS_FULL_SCALE );


    /* Define one axis at a time */
    NEXT_BYTE(pucReport,    HIDP_GLOBAL_REPORT_COUNT_1);
    NEXT_BYTE(pucReport,    0x1);  

    /* Each axis is a 32 bits value */
    NEXT_BYTE(pucReport,    HIDP_GLOBAL_REPORT_SIZE);
    NEXT_BYTE(pucReport,    8 * sizeof(ULONG) );

    /* 
     *  Do the axis 
     *  Although HID could cope with the "active" axes being mixed with the 
     *  dummy ones, it makes life simpler to move them to the start.
     *  Pass through all the axis maps generated by HGM_JoystickConfig 
     *  and map all the active ones into the descriptor, copying the usages 
     *  appropriate for the type of device.
     *  Since a polled POV is nothing more than a different interpretation 
     *  of axis data, this is added after any axes.
     */
    RtlCopyMemory( InitialAxisMappings, DeviceExtension->AxisMap, sizeof( InitialAxisMappings ) );



    Idx = 0;
    for( UsageIdx = 0; UsageIdx < MAX_AXES; UsageIdx++ )
    {
        if( InitialAxisMappings[UsageIdx] >= INVALID_INDEX )
        {
            continue;
        }

        DeviceExtension->AxisMap[Idx] = InitialAxisMappings[UsageIdx];

        NEXT_BYTE(pucReport,    HIDP_GLOBAL_USAGE_PAGE_1);
		NEXT_BYTE(pucReport,    pJoyParams[UsageIdx].UsagePage);
        NEXT_BYTE(pucReport,    HIDP_LOCAL_USAGE_1);
        NEXT_BYTE(pucReport,    pJoyParams[UsageIdx].Usage);

        /* Data Field */
        NEXT_BYTE(pucReport,    HIDP_MAIN_INPUT_1);
        NEXT_BYTE(pucReport,    ITEM_ANALOG_AXIS);

        Idx++;
    }

	/*
     *  Now fill in any remaining axis values as dummys
     */
    while( Idx < MAX_AXES )
    {
        /* Constant Field */
        NEXT_BYTE(pucReport,    HIDP_MAIN_INPUT_1);
        NEXT_BYTE(pucReport,    ITEM_PADDING);

        Idx++;
    }
        
		NEXT_BYTE(pucReport,    HIDP_GLOBAL_USAGE_PAGE_1);
		NEXT_BYTE(pucReport,    HID_USAGE_PAGE_GENERIC);

    //  POVS digitales

        NEXT_BYTE(pucReport,    HIDP_GLOBAL_REPORT_SIZE);
        NEXT_BYTE(pucReport,    0x8);

        NEXT_BYTE(pucReport,    HIDP_GLOBAL_LOG_MIN_1);
        NEXT_BYTE(pucReport,    1 );
        NEXT_BYTE(pucReport,    HIDP_GLOBAL_LOG_MAX_1);
        NEXT_BYTE(pucReport,    8 );

        NEXT_BYTE(pucReport,    HIDP_LOCAL_USAGE_1);
        NEXT_BYTE(pucReport,    HID_USAGE_GENERIC_HATSWITCH);
        NEXT_BYTE(pucReport,    HIDP_MAIN_INPUT_1);
        NEXT_BYTE(pucReport,    ITEM_DIGITAL_POV);
		NEXT_BYTE(pucReport,    HIDP_LOCAL_USAGE_1);
		NEXT_BYTE(pucReport,    HID_USAGE_GENERIC_HATSWITCH);
		NEXT_BYTE(pucReport,    HIDP_MAIN_INPUT_1);
		NEXT_BYTE(pucReport,    ITEM_DIGITAL_POV);

        NEXT_BYTE(pucReport,    HIDP_GLOBAL_LOG_MAX_1);
        NEXT_BYTE(pucReport,    4 );

        NEXT_BYTE(pucReport,    HIDP_LOCAL_USAGE_1);
		NEXT_BYTE(pucReport,    HID_USAGE_GENERIC_HATSWITCH);
		NEXT_BYTE(pucReport,    HIDP_MAIN_INPUT_1);
		NEXT_BYTE(pucReport,    ITEM_DIGITAL_POV);
		NEXT_BYTE(pucReport,    HIDP_LOCAL_USAGE_1);
		NEXT_BYTE(pucReport,    HID_USAGE_GENERIC_HATSWITCH);
		NEXT_BYTE(pucReport,    HIDP_MAIN_INPUT_1);
		NEXT_BYTE(pucReport,    ITEM_DIGITAL_POV);

	//=====================Saitek X36================================
	{
	    POEMDATA    OemData;
        OemData = &DeviceExtension->HidGameOemData.OemData[0];
        if(DeviceExtension->fSiblingFound)
        {
            OemData = &DeviceExtension->HidGameOemData.OemData[1];
        }

		// Rotatorios

        NEXT_BYTE(pucReport,    HIDP_GLOBAL_LOG_MIN_1);
        NEXT_BYTE(pucReport,    0 );
        NEXT_BYTE(pucReport,    HIDP_GLOBAL_LOG_MAX_1);
        NEXT_BYTE(pucReport,    255 );
		if(OemData->VID==0x06A3 && OemData->PID==0x3635)
		{
	        NEXT_BYTE(pucReport,    HIDP_LOCAL_USAGE_1);
			NEXT_BYTE(pucReport,    HID_USAGE_GENERIC_DIAL);
			NEXT_BYTE(pucReport,    HIDP_MAIN_INPUT_1);
			NEXT_BYTE(pucReport,    ITEM_ROTARY);
	        NEXT_BYTE(pucReport,    HIDP_LOCAL_USAGE_1);
			NEXT_BYTE(pucReport,    HID_USAGE_GENERIC_DIAL);
			NEXT_BYTE(pucReport,    HIDP_MAIN_INPUT_1);
			NEXT_BYTE(pucReport,    ITEM_ROTARY);
		} else {
			NEXT_BYTE(pucReport,    HIDP_MAIN_INPUT_1);
			NEXT_BYTE(pucReport,    ITEM_PADDING);
			NEXT_BYTE(pucReport,    HIDP_MAIN_INPUT_1);
			NEXT_BYTE(pucReport,    ITEM_PADDING);
		}
	}
	//=============================================================

    // Botones

    NEXT_BYTE(pucReport,    HIDP_GLOBAL_USAGE_PAGE_1);
    NEXT_BYTE(pucReport,    HID_USAGE_PAGE_BUTTON);
    NEXT_BYTE(pucReport,    HIDP_LOCAL_USAGE_MIN_1);
    NEXT_BYTE(pucReport,    1);
    NEXT_BYTE(pucReport,    HIDP_LOCAL_USAGE_MAX_1);
    NEXT_BYTE(pucReport,    40 );
    NEXT_BYTE(pucReport,    HIDP_GLOBAL_LOG_MIN_1);
    NEXT_BYTE(pucReport,    0 );
    NEXT_BYTE(pucReport,    HIDP_GLOBAL_LOG_MAX_1);
    NEXT_BYTE(pucReport,    1 );
	NEXT_BYTE(pucReport,    HIDP_GLOBAL_REPORT_SIZE);
    NEXT_BYTE(pucReport,    1);
    NEXT_BYTE(pucReport,    HIDP_GLOBAL_REPORT_COUNT_1);
    NEXT_BYTE(pucReport,    40);  
    NEXT_BYTE(pucReport,    HIDP_MAIN_INPUT_1);
    NEXT_BYTE(pucReport,    ITEM_BUTTON);

    // End of collection,  We're done ! 
    NEXT_BYTE(pucReport,  HIDP_MAIN_ENDCOLLECTION); 


#undef NEXT_BYTE
#undef NEXT_LONG

    if( pucReport - rgGameReport > MAXBYTES_GAME_REPORT)
    {
        ntStatus   = STATUS_BUFFER_TOO_SMALL;
        *pCbReport = 0x0;
        RtlZeroMemory(rgGameReport, sizeof(rgGameReport));
    } else
    {
        *pCbReport = (USHORT) (pucReport - rgGameReport);
        ntStatus = STATUS_SUCCESS;
    }

    return ( ntStatus );
} /* HGM_GenerateReport */







/*****************************************************************************
 *
 *  @func   VOID  | HGM_Game2HID |
 *
 *          Process the data returned from polling the gameport into values 
 *          and buttons for returning to HID.
 *          <nl>The meaning of the data is interpreted according to the 
 *          characteristics of the device described in the hardware settings
 *          flags.
 *
 *  @parm   IN PDEVICE_OBJECT | DeviceObject |
 *
 *          Pointer to the device object
 *
 *  @parm   IN      PDEVICE_EXTENSION | DeviceExtension | 
 *
 *          Pointer to the mini-driver device extension.
 *
 *  @parm   IN  OUT PUHIDGAME_INPUT_DATA | pHIDData | 
 *
 *          Pointer to the buffer into which the HID report should be written.
 *          This buffer must be assumed to be unaligned.
 *
 *****************************************************************************/
VOID HGM_Game2HID
    (
    IN PDEVICE_OBJECT      DeviceObject,
    IN  OUT PUHIDGAME_INPUT_DATA    pHIDData
    )
{
	LONG    Idx;
	PDEVICE_EXTENSION DeviceExtension = GET_MINIDRIVER_DEVICE_EXTENSION(DeviceObject);

    /*
     *  Use a local buffer to assemble the report as the real buffer may not 
     *  be aligned.
     */
	HIDGAME_INPUT_DATA aux;

	// Remap axis
	for(Idx = 0x0; Idx < DeviceExtension->nAxes; Idx++ )
	{
		DeviceExtension->hidBuffer.Ejes[Idx] = DeviceExtension->LastGoodAxis[DeviceExtension->AxisMap[Idx]];
    }

	HGM_CogerDatosX36(DeviceObject,&DeviceExtension->hidBuffer);
	RtlCopyMemory(&aux,&DeviceExtension->hidBuffer,sizeof(HIDGAME_INPUT_DATA));
	HGM_Filtrar(DeviceObject,&aux);
	RtlCopyMemory(pHIDData,&aux,sizeof(HIDGAME_INPUT_DATA));

	if(DeviceExtension->nuevaConfiguracion) {
		HGM_CargarConfiguracion(DeviceObject);
	} else if(DeviceExtension->nuevaCalibracion) {
		HGM_CargarCalibracion(DeviceObject);
	} else {
		HGM_ComprobarEstado(DeviceObject);
	}

} /* HGM_Game2HID */


VOID HGM_Filtrar
    (
    IN      PDEVICE_OBJECT       DeviceObject,
	IN  OUT PHIDGAME_INPUT_DATA    pHIDData
    )
{
	PDEVICE_EXTENSION devExt = GET_MINIDRIVER_DEVICE_EXTENSION(DeviceObject);
	HIDGAME_INPUT_DATA hidAux;
	UCHAR idx;

	RtlZeroMemory(&hidAux,sizeof(HIDGAME_INPUT_DATA));

	// Filtrado de ejes
	for(idx=0;idx<MAX_AXES;idx++) {
		if(devExt->jitter[idx].antiv) {
		// Antivibraciones
			if (pHIDData->Ejes[idx]>devExt->jitter[idx].PosElegida)
			{
			    if (pHIDData->Ejes[idx]>(devExt->jitter[idx].PosElegida+devExt->jitter[idx].Margen)) {
					devExt->jitter[idx].PosElegida=pHIDData->Ejes[idx];
				} else {
					if(devExt->jitter[idx].PosRepetida<3) {
						pHIDData->Ejes[idx]=devExt->jitter[idx].PosElegida;
					} else {
						devExt->jitter[idx].PosRepetida=0;
						devExt->jitter[idx].PosElegida=pHIDData->Ejes[idx];
					}
				}
			} else {
			    if (pHIDData->Ejes[idx]<(devExt->jitter[idx].PosElegida-devExt->jitter[idx].Margen)) {
					devExt->jitter[idx].PosElegida=pHIDData->Ejes[idx];
				} else {
					if(devExt->jitter[idx].PosRepetida<3) {
						pHIDData->Ejes[idx]=devExt->jitter[idx].PosElegida;
					} else {
						devExt->jitter[idx].PosRepetida=0;
						devExt->jitter[idx].PosElegida=pHIDData->Ejes[idx];
					}
				}
			}
		}

		if(devExt->limites[idx].cal) {
		// Calibrado
			ULONG ancho1,ancho2;
			if(pHIDData->Ejes[idx]==0 ||((pHIDData->Ejes[idx]>=(devExt->limites[idx].c-devExt->limites[idx].n)) && (pHIDData->Ejes[idx]<=(devExt->limites[idx].c+devExt->limites[idx].n))))
			{
				pHIDData->Ejes[idx]=0;
			} else {
		        if(pHIDData->Ejes[idx]<devExt->limites[idx].i)
				{
					pHIDData->Ejes[idx]=devExt->limites[idx].i;
				} else {
					if(pHIDData->Ejes[idx]>devExt->limites[idx].d)
					{
						pHIDData->Ejes[idx]=devExt->limites[idx].d;
					}
				}
				ancho1=(devExt->limites[idx].c-devExt->limites[idx].n)-devExt->limites[idx].i;
				ancho2=devExt->limites[idx].d-(devExt->limites[idx].c+devExt->limites[idx].n);
				if(pHIDData->Ejes[idx]<devExt->limites[idx].c)
				{
					pHIDData->Ejes[idx]=((devExt->limites[idx].c-devExt->limites[idx].n)-pHIDData->Ejes[idx]);
					if(ancho1>ancho2)
					{
						pHIDData->Ejes[idx]=(pHIDData->Ejes[idx]*ancho2)/ancho1;
					}
					pHIDData->Ejes[idx]=0-pHIDData->Ejes[idx];
				} else {
					pHIDData->Ejes[idx]-=(devExt->limites[idx].c+devExt->limites[idx].n);
					if(ancho2>ancho1)
					{
						pHIDData->Ejes[idx]=(pHIDData->Ejes[idx]*ancho1)/ancho2;
					}
				}
			}
		}
	}


	// Mapeados
	for(idx=0;idx<MAX_AXES;idx++)
	{	hidAux.Ejes[idx]=pHIDData->Ejes[devExt->Configuracion.MapaEjes[idx]]; }

	if((devExt->Configuracion.MapaRotatorios&3)!=0) {
		hidAux.Rotatorios[0]=pHIDData->Rotatorios[(devExt->Configuracion.MapaRotatorios&3)-1];
	}
	if((devExt->Configuracion.MapaRotatorios>>2)!=0) {
		hidAux.Rotatorios[1]=pHIDData->Rotatorios[(devExt->Configuracion.MapaRotatorios>>2)-1];
	}
/*
	for(idx=0;idx<40;idx++) {
		if(devExt->Configuracion.MapaBotones[0][idx]!=0) {
			UCHAR boton=devExt->Configuracion.MapaBotones[0][idx]-1;
			hidAux.Botones[boton/8]|=((pHIDData->Botones[idx/8]>>(idx%8))&1)<<(boton%8);
		}
		if(devExt->Configuracion.MapaBotones[1][idx]!=0) {
			UCHAR seta=devExt->Configuracion.MapaBotones[1][idx]&3;
			UCHAR pos=devExt->Configuracion.MapaBotones[1][idx]>>2;
			if(((pHIDData->Botones[idx/8]>>(idx%8))&1)==1) hidAux.Setas[seta]=pos;
		}
	}*/
	RtlCopyMemory(hidAux.Botones,pHIDData->Botones,sizeof(hidAux.Botones));
	RtlCopyMemory(hidAux.Setas,pHIDData->Setas,sizeof(hidAux.Setas));

	RtlCopyMemory(pHIDData,&hidAux,sizeof(HIDGAME_INPUT_DATA));
}

/************************************************************+
				LLamadas externas
*************************************************************/

//#define IOCTL_HID_LEERX36	CTL_CODE(FILE_DEVICE_UNKNOWN, 0x0100, METHOD_NEITHER, FILE_READ_ACCESS)
#define IOCTL_HID_LEERX36	CTL_CODE(FILE_DEVICE_UNKNOWN, 0x0108, METHOD_BUFFERED, FILE_READ_ACCESS)
VOID HGM_CogerDatosX36
    (
    IN      PDEVICE_OBJECT       DeviceObject,
    IN  OUT PHIDGAME_INPUT_DATA    pHIDData
    )
{
	PDEVICE_EXTENSION devExt = GET_MINIDRIVER_DEVICE_EXTENSION(DeviceObject);

    if(devExt->vfo!=NULL && KeGetCurrentIrql()==PASSIVE_LEVEL)
	{
		NTSTATUS                ntStatus = STATUS_SUCCESS;
		KEVENT                  IoctlCompleteEvent;
		IO_STATUS_BLOCK         IoStatus;
		PIRP                    pIrp;
		struct {
			UCHAR   Ejes[2];
			UCHAR   Setas[4];
			UCHAR   Rotatorios[2];
			UCHAR	Botones[5];
		} buffer;

		KeInitializeEvent(&IoctlCompleteEvent, NotificationEvent, FALSE);
		pIrp=IoBuildDeviceIoControlRequest(IOCTL_HID_LEERX36,devExt->vdo,NULL,0,&buffer,sizeof(buffer),TRUE,&IoctlCompleteEvent,&IoStatus);
		if(pIrp!=NULL)
		{
			ntStatus = IoCallDriver(devExt->vdo, pIrp);
			if (ntStatus == STATUS_PENDING) {
				KeWaitForSingleObject (&IoctlCompleteEvent,Executive,KernelMode,FALSE,NULL);
			}
			RtlCopyMemory(pHIDData->Setas,buffer.Setas,sizeof(buffer.Setas));
			RtlCopyMemory(pHIDData->Rotatorios,buffer.Rotatorios,sizeof(buffer.Rotatorios));
			RtlCopyMemory(pHIDData->Botones,buffer.Botones,sizeof(buffer.Botones));
		}
	}
}


#define IOCTL_HID_ESTADO	CTL_CODE(FILE_DEVICE_UNKNOWN, 0x0100, METHOD_BUFFERED, FILE_READ_ACCESS)
VOID HGM_ComprobarEstado
(
    IN      PDEVICE_OBJECT       DeviceObject
)
{
	PDEVICE_EXTENSION devExt = GET_MINIDRIVER_DEVICE_EXTENSION(DeviceObject);

    if(devExt->vfo!=NULL && KeGetCurrentIrql()==PASSIVE_LEVEL)
	{
		NTSTATUS                ntStatus = STATUS_SUCCESS;
		KEVENT                  IoctlCompleteEvent;
		IO_STATUS_BLOCK         IoStatus;
		PIRP                    pIrp;
		CHAR					buffer=0;

		KeInitializeEvent(&IoctlCompleteEvent, NotificationEvent, FALSE);
		pIrp=IoBuildDeviceIoControlRequest(IOCTL_HID_ESTADO,devExt->vdo,NULL,0,&buffer,1,TRUE,&IoctlCompleteEvent,&IoStatus);
		if(pIrp!=NULL)
		{
			ntStatus = IoCallDriver(devExt->vdo, pIrp);
			if (ntStatus == STATUS_PENDING) {
				KeWaitForSingleObject (&IoctlCompleteEvent,Executive,KernelMode,FALSE,NULL);
			}
			if((buffer&1)==1) devExt->nuevaConfiguracion=TRUE;
			if((buffer&2)==2) devExt->nuevaCalibracion=TRUE;
			if((buffer&4)==4) {
				UCHAR idx;
				for(idx=0;idx<4;idx++) {
					devExt->EjesAntiguos[idx]=devExt->Configuracion.MapaEjes[idx];
					devExt->Configuracion.MapaEjes[idx]=idx;
					devExt->limites[idx].cal=FALSE;
					devExt->jitter[idx].antiv=FALSE;
				}
			}
		}
	}
}

#define IOCTL_HID_LEERCONF	CTL_CODE(FILE_DEVICE_UNKNOWN, 0x0101, METHOD_BUFFERED, FILE_READ_ACCESS)
VOID HGM_CargarConfiguracion
    (
    IN      PDEVICE_OBJECT       DeviceObject
    )
{
	PDEVICE_EXTENSION devExt = GET_MINIDRIVER_DEVICE_EXTENSION(DeviceObject);

    if(devExt->vfo!=NULL && KeGetCurrentIrql()==PASSIVE_LEVEL)
	{
		NTSTATUS                ntStatus = STATUS_SUCCESS;
		KEVENT                  IoctlCompleteEvent;
		IO_STATUS_BLOCK         IoStatus;
		PIRP                    pIrp;
		CONF					buffer;

		KeInitializeEvent(&IoctlCompleteEvent, NotificationEvent, FALSE);
		pIrp=IoBuildDeviceIoControlRequest(IOCTL_HID_LEERCONF,devExt->vdo,NULL,0,&buffer,sizeof(CONF),TRUE,&IoctlCompleteEvent,&IoStatus);
		if(pIrp!=NULL)
		{
			ntStatus = IoCallDriver(devExt->vdo, pIrp);
			if (ntStatus == STATUS_PENDING) {
				KeWaitForSingleObject (&IoctlCompleteEvent,Executive,KernelMode,FALSE,NULL);
			}
			RtlCopyMemory(&devExt->Configuracion,&buffer,sizeof(CONF));
			devExt->nuevaConfiguracion=FALSE;
		}
	}
}