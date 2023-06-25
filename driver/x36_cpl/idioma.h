#pragma once

class Traduce
{
public:
	static HRESULT Msg(HWND hWnd,short ID1, short ID2,UINT tipo);
	static HRESULT Msg(HWND hWnd,short ID1, const char* tit,UINT tipo);
	static void Dlg(HWND hWnd,short ID);
	static LPCSTR Txt(short ID);
};

	#define LG_ERROR1		13
	#define LG_ERROR2		14

// CPL

	#define LG_CPL_ERROR0 120
	#define LG_CPL_ERROR1 121
	#define LG_CPL_ERROR2 122
	#define LG_CPL_ERROR3 123
	#define LG_CPL_ERROR4 124
	#define LG_CPL_ERROR5 125
	#define LG_CPL_ERROR6 126
	#define LG_CPL_ERROR7 127
	#define LG_CPL_ERROR8 128
	#define LG_CPL_ERROR9 129
	#define LG_CPL_ERROR10 130
	#define LG_CPL_ERROR11 131
	#define LG_CPL_TXT0	132
	#define LG_CPL_TXT1 133
	#define LG_CPL_TXT2 134
	#define LG_CPL_TXT3 135
	#define LG_CPL_C_GUI0	136
	#define LG_CPL_C_GUI1	137
	#define LG_CPL_C_GUI2	138
	#define LG_CPL_C_GUI3	139
	#define LG_CPL_C_GUI4	140
	#define LG_CPL_C_GUI5	141
	#define LG_CPL_C_GUI6	142
	#define LG_CPL_C_GUI7	143
	#define LG_CPL_C_GUI8	144
	#define LG_CPL_C_GUI9	145
	#define LG_CPL_C_GUI10	146
	#define LG_CPL_C_GUI11	147
	#define LG_CPL_C_GUI12	148
	#define LG_CPL_C_GUI13	149
	#define LG_CPL_C_GUI14	150
	#define LG_CPL_C_GUI15	151
	#define LG_CPL_OK0		152
	#define LG_CPL_TXT4		153
	#define LG_CPL_TXT5		154
	#define LG_CPL_TXT6		155
	#define LG_CPL_TXT7		156
	#define LG_CPL_TXT8		157
	#define LG_CPL_TXT9		158
	#define LG_CPL_TXT10	159
	#define LG_CPL_TXT11	160
	#define LG_CPL_TXT12	161
	#define LG_CPL_TXT13	162
	#define LG_CPL_TXT14	163
	#define LG_CPL_TXT15	164
	#define LG_CPL_D2_GUI0	165
	#define LG_CPL_D2_GUI1	166
	#define LG_CPL_D2_GUI2	167
	#define LG_CPL_D2_GUI3	168
	#define LG_CPL_D2_GUI4	169
	#define LG_CPL_D2_GUI5	170
	#define LG_CPL_D2_GUI6	171
	#define LG_CPL_D1_GUI0	172
	#define LG_CPL_D1_GUI1	173
	#define LG_CPL_D1_GUI2	174
	#define LG_CPL_D1_GUI3	175
	#define LG_CPL_D1_GUI4	176
	#define LG_CPL_D1_GUI5	177
	#define LG_CPL_D1_GUI6	178
	#define LG_CPL_D1_GUI7	179
	#define LG_CPL_D1_GUI8	180
	#define LG_CPL_D1_GUI9	181
	#define LG_CPL_D1_GUI10	182
	#define LG_CPL_D1_GUI11	183
	#define LG_CPL_D1_GUI12	184
	#define LG_CPL_D1_GUI13	185
	#define LG_CPL_D1_GUI14	186
	#define LG_CPL_D1_GUI15	187
	#define LG_CPL_TXT16	188
	#define LG_CPL_TXT17	189
	#define LG_CPL_TXT18	190
	#define LG_CPL_TXT19	191
	#define LG_CPL_TXT20	192
	#define LG_CPL_TXT21	193
