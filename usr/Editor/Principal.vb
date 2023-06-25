Public Class Principal

    Private Declare Ansi Function Compile Lib "x36_svc.dll" (ByVal mdf As String, ByVal jdf As String) As Byte
    Private Declare Ansi Function CompileAndLaunch Lib "x36_svc.dll" (ByVal mdf As String, ByVal jdf As String) As Byte
    Public datos As Datos
    Public actual As Byte = 0
    Private esNuevo As Boolean = True
    Public eventos As Boolean = False

#Region "Eventos"

    Private Sub Form1_Load(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.Load
        Traducir()
        Vista.padre = Me
        datos = New Datos()
        eventos = True
        If My.Application.CommandLineArgs.Count = 1 Then
            Abrir(My.Application.CommandLineArgs(0) & ".jdf")
        Else
            Vista.Boton(0, Button22)
        End If
    End Sub
    Private Sub Principal_FormClosing(ByVal sender As Object, ByVal e As System.Windows.Forms.FormClosingEventArgs) Handles Me.FormClosing
        If datos.estaModificado Then
            Dim r As Microsoft.VisualBasic.MsgBoxResult = Traduce.Msg(80, 79, MsgBoxStyle.YesNoCancel Or MsgBoxStyle.Exclamation)
            If r = MsgBoxResult.Cancel Then
                e.Cancel = True
                Exit Sub
            Else
                If r = MsgBoxResult.Yes Then
                    If Not OnFileSave2() Then e.Cancel = True : Exit Sub
                End If
            End If
        End If
    End Sub

    Private Sub MenuItem5_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MenuItem5.Click
        Nuevo()
    End Sub
    Private Sub MenuItem6_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MenuItem6.Click
        Abrir()
    End Sub
    Private Sub MenuItem8_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MenuItem8.Click
        Guardar()
    End Sub
    Private Sub MenuItem11_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MenuItem11.Click
        GuardarComo()
    End Sub
    Private Sub MenuItem13_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MenuItem13.Click
        Me.Close()
    End Sub
    Private Sub MenuItem20_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MenuItem20.Click
        Dim about As New AcercaDe
        Me.AddOwnedForm(about)
        about.Show()
    End Sub

    Private Sub ToolBar1_ItemClicked(ByVal sender As System.Object, ByVal e As System.Windows.Forms.ToolStripItemClickedEventArgs) Handles ToolBar1.ItemClicked
        Select Case e.ClickedItem.Tag.ToString
            Case "Nuevo"
                Nuevo()
            Case "Abrir"
                Abrir()
            Case "Guardar"
                Guardar()
            Case "Compilar"
                Compilar()
            Case "Lanzar"
                Lanzar()
        End Select
    End Sub

    Private Sub Principal_Resize(ByVal sender As Object, ByVal e As System.EventArgs) Handles MyBase.Resize
        Panel3.Top = (x36Tab.Height - Panel3.Height) / 2
        Panel4.Top = (x35Tab.Height - Panel4.Height) / 2
    End Sub

    Private Sub TabControl1_SelectedIndexChanged(ByVal sender As Object, ByVal e As System.EventArgs) Handles TabControl1.SelectedIndexChanged
        If TabControl1.SelectedIndex = 0 Then
            x36Tab.Visible = True
            x35Tab.Visible = False
            Panel3.Top = (x36Tab.Height - Panel3.Height) / 2
        Else
            x36Tab.Visible = False
            x35Tab.Visible = True
            Panel4.Top = (x35Tab.Height - Panel4.Height) / 2
        End If
    End Sub

#End Region

#Region "Metodos"

    Public Sub GetModos(ByRef p As Byte, ByRef m As Byte, ByRef a As Byte)
        Vista.GetModos(p, m, a)
    End Sub

    Private Sub Traducir()
        Me.Text = Traduce.Txt(13) & " - [" & Traduce.Txt(14) & "]"
        'Menus
        MenuItem1.Text = Traduce.Txt(15)
        MenuItem5.Text = Traduce.Txt(16)
        MenuItem6.Text = Traduce.Txt(17)
        MenuItem8.Text = Traduce.Txt(18)
        MenuItem11.Text = Traduce.Txt(19)
        MenuItem13.Text = Traduce.Txt(20)
        MenuItem2.Text = Traduce.Txt(21)
        MenuItem3.Text = Traduce.Txt(22)
        MenuItem4.Text = Traduce.Txt(23)
        MenuItem19.Text = Traduce.Txt(24)
        MenuItem20.Text = Traduce.Txt(25)
        ' Dialogos
        OpenFileDialog1.Filter = Traduce.Txt(27)
        SaveFileDialog1.Filter = Traduce.Txt(27)
        'OpenFileDialog1.FileName = Traduce.Txt(26)
        SaveFileDialog1.FileName = Traduce.Txt(26)
        ' Barra botones
        ToolBar1.Items(0).ToolTipText = Traduce.Txt(28)
        ToolBar1.Items(1).ToolTipText = Traduce.Txt(29)
        ToolBar1.Items(2).ToolTipText = Traduce.Txt(30)
        ToolBar1.Items(4).ToolTipText = Traduce.Txt(31)
        ToolBar1.Items(5).ToolTipText = Traduce.Txt(32)
        'Elementos
        ToolStripLabel2.Text = Traduce.Txt(33)
        Button22.Text = Traduce.Txt(34)
        Button1.Text = Traduce.Txt(35)
        Button2.Text = Traduce.Txt(36)
        Button21.Text = Traduce.Txt(37)
        Button20.Text = Traduce.Txt(38)
        Button23.Text = Traduce.Txt(39)
        Label1.Text = Traduce.Txt(41)
        Label2.Text = Traduce.Txt(40)
        Button39.Text = Traduce.Txt(42)
        Button40.Text = Traduce.Txt(43)
        Button25.Text = Traduce.Txt(44)
        Button30.Text = Traduce.Txt(45)
        Button24.Text = Traduce.Txt(46)
        Button45.Text = Traduce.Txt(47)
        Button19.Text = Traduce.Txt(48)
        Button46.Text = Traduce.Txt(49)
        Label3.Text = Traduce.Txt(50)
        Label4.Text = Traduce.Txt(51)
        Button35.Text = Traduce.Txt(52)
        Button38.Text = Traduce.Txt(53)
        Button37.Text = Traduce.Txt(54)
        Button36.Text = Traduce.Txt(55)
        Button12.Tag = Traduce.Txt(86)
        Button11.Tag = Traduce.Txt(87)
        Button17.Tag = Traduce.Txt(88)
        Button14.Tag = Traduce.Txt(89)
        Button16.Tag = Traduce.Txt(90)
        Button18.Tag = Traduce.Txt(91)
        Button13.Tag = Traduce.Txt(92)
        Button15.Tag = Traduce.Txt(93)
        Button3.Tag = Traduce.Txt(94)
        Button8.Tag = Traduce.Txt(95)
        Button5.Tag = Traduce.Txt(96)
        Button10.Tag = Traduce.Txt(97)
        Button6.Tag = Traduce.Txt(98)
        Button9.Tag = Traduce.Txt(99)
        Button4.Tag = Traduce.Txt(100)
        Button7.Tag = Traduce.Txt(101)
        Button26.Tag = Traduce.Txt(102)
        Button28.Tag = Traduce.Txt(103)
        Button29.Tag = Traduce.Txt(104)
        Button27.Tag = Traduce.Txt(105)
        Button31.Tag = Traduce.Txt(106)
        Button33.Tag = Traduce.Txt(107)
        Button34.Tag = Traduce.Txt(108)
        Button32.Tag = Traduce.Txt(109)
        'Propiedades
        Label5.Text = Traduce.Txt(56)
        Label6.Text = Traduce.Txt(57)
        Label7.Text = Traduce.Txt(58)
        Label8.Text = Traduce.Txt(59)
        Label9.Text = Traduce.Txt(60)
        Label10.Text = Traduce.Txt(61)
        RadioButton23.Text = Traduce.Txt(42)
        RadioButton24.Text = Traduce.Txt(43)
        RadioButton21.Text = Traduce.Txt(52)
        RadioButton22.Text = Traduce.Txt(53)
        RadioButton19.Text = Traduce.Txt(54)
        RadioButton20.Text = Traduce.Txt(55)
        RadioButton17.Text = Traduce.Txt(62)
        RadioButton18.Text = Traduce.Txt(63)
        RadioButton1.Text = Traduce.Txt(64)
        RadioButton2.Text = Traduce.Txt(65)
        Button44.Text = Traduce.Txt(66)
        CheckBox1.Text = Traduce.Txt(39)
        CheckBox2.Text = Traduce.Txt(67)
        CheckBox3.Text = Traduce.Txt(68)
        CheckBox4.Text = Traduce.Txt(69)
        RadioButton11.Text = Traduce.Txt(70)
        RadioButton12.Text = Traduce.Txt(71)
        Button41.Text = Traduce.Txt(74)
        Button42.Text = Traduce.Txt(75)
        Button43.Text = Traduce.Txt(76)
        Label11.Text = Traduce.Txt(77)
        ComboBox1.Items.Add(Traduce.Txt(78))
        ComboBox1.SelectedIndex = 0
    End Sub

    Private Sub Nuevo()
        If datos.estaModificado Then
            Dim r As Microsoft.VisualBasic.MsgBoxResult = Traduce.Msg(80, 79, MsgBoxStyle.YesNoCancel Or MsgBoxStyle.Exclamation)
            If r = MsgBoxResult.Cancel Then
                Exit Sub
            Else
                If r = MsgBoxResult.Yes Then
                    If Not OnFileSave2() Then Exit Sub
                End If
            End If
        End If
        ComboBox1.Items.Clear()
        ComboBox1.Items.Add(Traduce.Txt(78))
        ComboBox1.SelectedIndex = 0
        datos = Nothing
        GC.Collect()
        datos = New Datos()
        Vista.Boton(0, Button22)
        Me.Text = Traduce.Txt(13) & " - [" & Traduce.Txt(14) & "]"
        esNuevo = True
    End Sub

    Private Sub Abrir(Optional ByVal archivo As String = Nothing)
        If archivo Is Nothing Then
            If datos.estaModificado Then
                Dim r As Microsoft.VisualBasic.MsgBoxResult = Traduce.Msg(80, 79, MsgBoxStyle.YesNoCancel Or MsgBoxStyle.Exclamation)
                If r = MsgBoxResult.Cancel Then
                    Exit Sub
                Else
                    If r = MsgBoxResult.Yes Then
                        If Not OnFileSave2() Then Exit Sub
                    End If
                End If
            End If
            If OpenFileDialog1.ShowDialog() = Windows.Forms.DialogResult.OK Then
                archivo = OpenFileDialog1.FileName
            End If
        End If
        If Not archivo Is Nothing Then
            ComboBox1.Items.Clear()
            ComboBox1.Items.Add(Traduce.Txt(78))
            ComboBox1.SelectedIndex = 0
            datos = Nothing
            GC.Collect()
            datos = New Datos()
            If datos.CargarArchivo(archivo, ComboBox1) Then
                Me.Text = Traduce.Txt(13) & " - [" & archivo.Remove(0, archivo.LastIndexOf("\") + 1) & "]"
                esNuevo = False
            Else
                ComboBox1.Items.Clear()
                ComboBox1.Items.Add(Traduce.Txt(78))
                ComboBox1.SelectedIndex = 0
                datos = Nothing
                GC.Collect()
                datos = New Datos()
                Me.Text = Traduce.Txt(13) & " - [" & Traduce.Txt(14) & "]"
                esNuevo = True
            End If
            Vista.Boton(0, Button22)
        End If
    End Sub

    Private Sub Guardar()
        OnFileSave2()
    End Sub

    Private Function OnFileSave2() As Boolean
        If esNuevo Then
            Return OnFileSaveAs2()
        Else
            return datos.Guardar(ComboBox1)
        End If
    End Function

    Private Sub GuardarComo()
        OnFileSaveAs2()
    End Sub

    Private Function OnFileSaveAs2() As Boolean
        SaveFileDialog1.FileName = Traduce.Txt(26)
        If SaveFileDialog1.ShowDialog() = Windows.Forms.DialogResult.OK Then
            If datos.Guardar(SaveFileDialog1.FileName, ComboBox1) Then
                esNuevo = False
                Me.Text = Traduce.Txt(13) & " - [" & SaveFileDialog1.FileName.Remove(0, SaveFileDialog1.FileName.LastIndexOf("\") + 1) & "]"
                Return True
            Else
                Return False
            End If
        Else
            Return False
        End If
    End Function

    Private Sub Lanzar()
        If datos.estaModificado Then
            Dim r As Microsoft.VisualBasic.MsgBoxResult = Traduce.Msg(81, 79, MsgBoxStyle.OkCancel Or MsgBoxStyle.Exclamation)
            If r = MsgBoxResult.Cancel Then
                Exit Sub
            Else
                If Not OnFileSave2() Then Exit Sub
            End If
        End If
        Try
            If CompileAndLaunch(datos.GetRutaPerfil() & ".mdf", datos.GetRutaPerfil() & ".jdf") = 1 Then
                Traduce.Msg(10, 32, MsgBoxStyle.Information)
            End If
        Catch
            Traduce.Msg(9, 32, MsgBoxStyle.Exclamation)
        End Try
    End Sub

    Private Sub Compilar()
        If datos.estaModificado Then
            Dim r As Microsoft.VisualBasic.MsgBoxResult = Traduce.Msg(81, 79, MsgBoxStyle.OkCancel Or MsgBoxStyle.Exclamation)
            If r = MsgBoxResult.Cancel Then
                Exit Sub
            Else
                If Not OnFileSave2() Then Exit Sub
            End If
        End If
        Try
            If Compile(datos.GetRutaPerfil() & ".mdf", datos.GetRutaPerfil() & ".jdf") = 1 Then
                Traduce.Msg(82, 31, MsgBoxStyle.Information)
            End If
        Catch
            Traduce.Msg(9, 31, MsgBoxStyle.Exclamation)
        End Try
    End Sub
#End Region

#Region "Elementos X36"
    Private Sub Button22_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button22.Click
        Vista.Boton(0, Button22)
    End Sub
    Private Sub Button1_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button1.Click
        Vista.Boton(1, Button1)
    End Sub
    Private Sub Button2_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button2.Click
        Vista.Boton(2, Button2)
    End Sub
    Private Sub Button21_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button21.Click
        Vista.Boton(3, Button21)
    End Sub
    Private Sub Button20_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button20.Click
        Vista.Boton(4, Button20)
    End Sub
    Private Sub Button23_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button23.Click
        Vista.Boton(5, Button23)
    End Sub
    Private Sub Button3_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button3.Click
        Vista.Boton(14, Button3)
    End Sub
    Private Sub Button6_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button6.Click
        Vista.Boton(15, Button6)
    End Sub
    Private Sub Button4_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button4.Click
        Vista.Boton(16, Button4)
    End Sub
    Private Sub Button5_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button5.Click
        Vista.Boton(17, Button5)
    End Sub
    Private Sub Button7_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button7.Click
        Vista.Boton(18, Button7)
    End Sub
    Private Sub Button8_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button8.Click
        Vista.Boton(19, Button8)
    End Sub
    Private Sub Button9_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button9.Click
        Vista.Boton(20, Button9)
    End Sub
    Private Sub Button10_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button10.Click
        Vista.Boton(21, Button10)
    End Sub
    Private Sub Button12_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button12.Click
        Vista.Boton(6, Button12)
    End Sub
    Private Sub Button16_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button16.Click
        Vista.Boton(7, Button16)
    End Sub
    Private Sub Button13_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button13.Click
        Vista.Boton(8, Button13)
    End Sub
    Private Sub Button17_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button17.Click
        Vista.Boton(9, Button17)
    End Sub
    Private Sub Button15_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button15.Click
        Vista.Boton(10, Button15)
    End Sub
    Private Sub Button11_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button11.Click
        Vista.Boton(11, Button11)
    End Sub
    Private Sub Button18_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button18.Click
        Vista.Boton(12, Button18)
    End Sub
    Private Sub Button14_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button14.Click
        Vista.Boton(13, Button14)
    End Sub
    Private Sub Button39_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button39.Click
        Vista.Eje(0)
    End Sub
    Private Sub Button40_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button40.Click
        Vista.Eje(1)
    End Sub
#End Region

#Region "Elementos X35"
    Private Sub Button25_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button25.Click
        Vista.Boton(22, Button25)
    End Sub
    Private Sub Button30_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button30.Click
        Vista.Boton(23, Button30)
    End Sub
    Private Sub Button24_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button24.Click
        Vista.Boton(24, Button24)
    End Sub
    Private Sub Button45_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button45.Click
        Vista.Boton(25, Button45)
    End Sub
    Private Sub Button19_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button19.Click
        Vista.Boton(26, Button19)
    End Sub
    Private Sub Button46_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button46.Click
        Vista.Boton(27, Button46)
    End Sub
    Private Sub Button26_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button26.Click
        Vista.Boton(28, Button26)
    End Sub
    Private Sub Button29_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button29.Click
        Vista.Boton(29, Button29)
    End Sub
    Private Sub Button27_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button27.Click
        Vista.Boton(30, Button27)
    End Sub
    Private Sub Button28_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button28.Click
        Vista.Boton(31, Button28)
    End Sub
    Private Sub Button31_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button31.Click
        Vista.Boton(32, Button31)
    End Sub
    Private Sub Button34_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button34.Click
        Vista.Boton(33, Button34)
    End Sub
    Private Sub Button32_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button32.Click
        Vista.Boton(34, Button32)
    End Sub
    Private Sub Button33_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button33.Click
        Vista.Boton(35, Button33)
    End Sub
    Private Sub Button37_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button37.Click
        Vista.Rotatorio(0)
    End Sub
    Private Sub Button36_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button36.Click
        Vista.Rotatorio(1)
    End Sub
    Private Sub Button35_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button35.Click
        Vista.Eje(2)
    End Sub
    Private Sub Button38_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button38.Click
        Vista.Eje(3)
    End Sub
#End Region

#Region "Eventos propiedades"

#Region "Eventos panel ejes"
    Private Sub RadioButton23_CheckedChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles RadioButton23.CheckedChanged
        If RadioButton23.Checked And eventos Then Vista.CambiarMapaEjes(0)
    End Sub
    Private Sub RadioButton22_CheckedChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles RadioButton22.CheckedChanged
        If RadioButton22.Checked And eventos Then Vista.CambiarMapaEjes(3)
    End Sub
    Private Sub RadioButton24_CheckedChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles RadioButton24.CheckedChanged
        If RadioButton24.Checked And eventos Then Vista.CambiarMapaEjes(1)
    End Sub
    Private Sub RadioButton21_CheckedChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles RadioButton21.CheckedChanged
        If RadioButton21.Checked And eventos Then Vista.CambiarMapaEjes(2)
    End Sub
#End Region

#Region "Eventos panel rotatorios"
    Private Sub RadioButton19_CheckedChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles RadioButton19.CheckedChanged
        If RadioButton19.Checked And eventos Then Vista.CambiarMapaRot(1)
    End Sub
    Private Sub RadioButton20_CheckedChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles RadioButton20.CheckedChanged
        If RadioButton20.Checked And eventos Then Vista.CambiarMapaRot(2)
    End Sub
    Private Sub CheckBox5_CheckedChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles CheckBox5.CheckedChanged
        If eventos Then Vista.CambiarMapaRot(0)
    End Sub
#End Region

#Region "Eventos panel ad"
    Private Sub RadioButton17_CheckedChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles RadioButton17.CheckedChanged
        If RadioButton17.Checked And eventos Then Vista.CambiarAD(True)
    End Sub
    Private Sub RadioButton18_CheckedChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles RadioButton18.CheckedChanged
        If RadioButton18.Checked And eventos Then Vista.CambiarAD(False)
    End Sub
#End Region

#Region "Eventos panel Modos"
    Private Sub CheckBox1_CheckedChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles CheckBox1.CheckedChanged
        If eventos Then Vista.CambiarPinkie(0, CheckBox1.Checked)
    End Sub
    Private Sub RadioButton3_CheckedChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles RadioButton3.CheckedChanged
        If RadioButton3.Checked And eventos Then Vista.CambiarPinkie(1, True)
    End Sub
    Private Sub RadioButton4_CheckedChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles RadioButton4.CheckedChanged
        If RadioButton4.Checked And eventos Then Vista.CambiarPinkie(1, False)
    End Sub
    Private Sub CheckBox2_CheckedChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles CheckBox2.CheckedChanged
        If eventos Then Vista.CambiarModo(0, CheckBox2.Checked)
    End Sub
    Private Sub RadioButton5_CheckedChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles RadioButton5.CheckedChanged
        If RadioButton5.Checked And eventos Then Vista.CambiarModo(1, 0)
    End Sub
    Private Sub RadioButton6_CheckedChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles RadioButton6.CheckedChanged
        If RadioButton6.Checked And eventos Then Vista.CambiarModo(1, 1)
    End Sub
    Private Sub RadioButton7_CheckedChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles RadioButton7.CheckedChanged
        If RadioButton7.Checked And eventos Then Vista.CambiarModo(1, 2)
    End Sub
    Private Sub CheckBox3_CheckedChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles CheckBox3.CheckedChanged
        If eventos Then Vista.CambiarModoAux(0, CheckBox3.Checked)
    End Sub
    Private Sub RadioButton9_CheckedChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles RadioButton9.CheckedChanged
        If RadioButton9.Checked And eventos Then Vista.CambiarModoAux(1, 0)
    End Sub
    Private Sub RadioButton8_CheckedChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles RadioButton8.CheckedChanged
        If RadioButton8.Checked And eventos Then Vista.CambiarModoAux(1, 1)
    End Sub
    Private Sub RadioButton10_CheckedChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles RadioButton10.CheckedChanged
        If RadioButton10.Checked And eventos Then Vista.CambiarModoAux(1, 2)
    End Sub
    Private Sub CheckBox4_CheckedChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles CheckBox4.CheckedChanged
        If eventos Then Vista.CambiarToggle(0, CheckBox4.Checked)
    End Sub
    Private Sub NumericUpDown1_ValueChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles NumericUpDown1.ValueChanged
        If eventos Then Vista.CambiarToggle(1, NumericUpDown1.Value)
    End Sub
    Private Sub RadioButton11_CheckedChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles RadioButton11.CheckedChanged
        If RadioButton11.Checked And eventos Then Vista.CambiarPresInc(True)
    End Sub
    Private Sub RadioButton12_CheckedChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles RadioButton12.CheckedChanged
        If RadioButton12.Checked And eventos Then Vista.CambiarPresInc(False)
    End Sub
#End Region

#Region "Eventos panel funcionamiento"
    Private Sub RadioButton1_CheckedChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles RadioButton1.CheckedChanged
        If RadioButton1.Checked And eventos Then Vista.CambiarFuncionamiento(False)
    End Sub
    Private Sub RadioButton2_CheckedChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles RadioButton2.CheckedChanged
        If RadioButton2.Checked And eventos Then Vista.CambiarFuncionamiento(True)
    End Sub
    Private Sub NumericUpDown2_ValueChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles NumericUpDown2.ValueChanged
        If eventos Then Vista.CambiarSensibilidad(NumericUpDown2.Value)
    End Sub
    Private Sub Button44_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button44.Click
        Dim b As New EditorBandas(Me)
        b.ShowDialog(Me)
    End Sub
#End Region

#Region "Eventos panel macros"
    Private Sub Button41_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button41.Click
        Dim m As New EditorMacros(0, Me)
        m.ShowDialog(Me)
    End Sub
    Private Sub Button42_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button42.Click
        If ComboBox1.SelectedIndex > 0 Then
            Dim m As New EditorMacros(ComboBox1.SelectedIndex, Me)
            m.ShowDialog(Me)
        End If
    End Sub
    Private Sub Button43_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button43.Click
        If ComboBox1.SelectedIndex > 0 Then
            datos.QuitarIndice(ComboBox1.SelectedIndex)
            ComboBox1.Items.RemoveAt(ComboBox1.SelectedIndex)
            ComboBox1.SelectedIndex = 0
        End If
    End Sub
    Private Sub ComboBox1_SelectedIndexChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ComboBox1.SelectedIndexChanged
        If ComboBox1.SelectedIndex <> -1 And eventos Then Vista.CambiarMacro(ComboBox1.SelectedIndex)
    End Sub
#End Region

#End Region

End Class
