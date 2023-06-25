Public Class EditorMacros
    Private padre As Principal
    Private indicep As Integer
    Private reps As Integer = 0

    Public Sub New(ByVal i As Integer, ByVal p As Principal)
        InitializeComponent()
        padre = p
        indicep = i
    End Sub

    Private Sub EditorMacros_Load(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.Load
        If indicep > 0 Then
            TextBox1.Text = padre.ComboBox1.Items.Item(indicep)
            RichTextBox1.Text = padre.datos.Macros(indicep - 1)
        End If
        Traducir()
        CargarPlantillas()
    End Sub

#Region "metodos"

    Private Sub Traducir()
        Me.Text = Traduce.Txt(113)
        GroupBox2.Text = Traduce.Txt(114)
        Label1.Text = Traduce.Txt(115)
        Button27.Text = Traduce.Txt(30)
        Button26.Text = Traduce.Txt(112)
        GroupBox1.Text = Traduce.Txt(116)
        Button32.Text = Traduce.Txt(117)
        Label5.Text = Traduce.Txt(118)
        Label6.Text = Traduce.Txt(119)
        Label7.Text = Traduce.Txt(120)
        Label2.Text = Traduce.Txt(121)
        Label3.Text = Traduce.Txt(122)
        Label4.Text = Traduce.Txt(123)
        Label18.Text = Traduce.Txt(124)
        Label9.Text = Traduce.Txt(125)
        Label14.Text = Traduce.Txt(126)
        Label8.Text = Traduce.Txt(127)
        Label12.Text = Traduce.Txt(128)
        Label13.Text = Traduce.Txt(129)
        Label15.Text = Traduce.Txt(130)
        Label16.Text = Traduce.Txt(131)
        Label17.Text = Traduce.Txt(132)
        Label10.Text = Traduce.Txt(133)
        Label11.Text = Traduce.Txt(134)
    End Sub

    Private Sub CargarPlantillas()
        Dim d As New System.IO.DirectoryInfo(".")
        For Each f As System.IO.FileInfo In d.GetFiles("*.kbp")
            vtSelPlantilla.Items.Add(Mid(f.Name, 1, f.Name.Length - 4))
        Next
        If vtSelPlantilla.Items.Count > 0 Then
            vtSelPlantilla.SelectedIndex = 0
            CargarPlantilla(0)
        Else
            Button1.Enabled = False
            Button2.Enabled = False
        End If
    End Sub

    Private Sub CargarPlantilla(ByVal p As Byte)
        ComboBox1.Items.Clear()
        Dim f As System.IO.StreamReader
        Try
            f = New System.IO.StreamReader(CType(vtSelPlantilla.Items.Item(vtSelPlantilla.SelectedIndex), String) & ".kbp")
        Catch
            Exit Sub
        End Try
        Try
            Do While f.Peek() >= 0
                ComboBox1.Items.Add(f.ReadLine())
            Loop
            ComboBox1.SelectedIndex = 0
        Catch
            Try
                f.Close()
            Catch
            End Try
            Exit Sub
        End Try
        Try
            f.Close()
        Catch
        End Try
    End Sub

    Private Sub Guardar()
        If TextBox1.Text.IndexOf(" ") <> -1 Or TextBox1.Text.IndexOf("=") <> -1 Or TextBox1.Text.IndexOf("/") <> -1 Then
            Traduce.Msg(165, 79, MsgBoxStyle.Information) ' Caracter no válido
            Exit Sub
        Else
            If reps > 0 Then
                Traduce.Msg(166, 79, MsgBoxStyle.Information)
                Exit Sub
            End If
            If indicep = 0 Then
                For i As Integer = 0 To padre.ComboBox1.Items.Count - 1
                    If TextBox1.Text.ToLower = padre.ComboBox1.Items.Item(i).ToString.ToLower Then
                        Traduce.Msg(167, 79, MsgBoxStyle.Information)
                        Exit Sub
                    End If
                Next
                padre.ComboBox1.Items.Add(TextBox1.Text)
                padre.datos.Macros.Add(Trim(RichTextBox1.Text))
            Else
                For i As Integer = 0 To padre.ComboBox1.Items.Count - 1
                    If TextBox1.Text.ToLower = padre.ComboBox1.Items.Item(i).ToString.ToLower And i <> indicep Then
                        Traduce.Msg(167, 79, MsgBoxStyle.Information)
                        Exit Sub
                    End If
                Next
                padre.ComboBox1.Items.Item(indicep) = TextBox1.Text
                padre.datos.Macros.Item(indicep - 1) = Trim(RichTextBox1.Text)
            End If
        End If

        Me.Close()
    End Sub
#End Region

    Private Sub Button27_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button27.Click
        Guardar()
    End Sub


    Private Sub Button26_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button26.Click
        Me.Close()
    End Sub

    Private Sub RichTextBox1_SelectionChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles RichTextBox1.SelectionChanged
        If RichTextBox1.SelectedText = "" Then
            Button32.Enabled = False
        Else
            Button32.Enabled = True
        End If
    End Sub
    Private Sub RichTextBox1_KeyDown(ByVal sender As System.Object, ByVal e As System.Windows.Forms.KeyEventArgs) Handles RichTextBox1.KeyDown
        e.SuppressKeyPress = True
    End Sub

    Private Sub Button32_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button32.Click
        Dim st As Integer = 0
        RichTextBox1.SelectedText = ""
        If RichTextBox1.Text.IndexOf("HOLD") <> -1 Then
            Button13.Enabled = False
        Else
            Button13.Enabled = True
        End If
        reps = 0
        Do
            st = RichTextBox1.Text.IndexOf("REPI", st)
            If st = -1 Then Exit Do
            reps = reps + 1
        Loop
        st = 0
        Do
            st = RichTextBox1.Text.IndexOf("REPN", st)
            If st = -1 Then Exit Do
            reps = reps + 1
        Loop
        If reps = 0 Then Button15.Enabled = False Else Button15.Enabled = True
    End Sub

#Region "raton"
    Private Sub Button3_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button3.Click
        RichTextBox1.SelectionColor = Color.Green
        RichTextBox1.SelectedText = "MS1U "
    End Sub
    Private Sub Button4_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button4.Click
        RichTextBox1.SelectionColor = Color.Green
        RichTextBox1.SelectedText = "MS2U "
    End Sub

    Private Sub Button5_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button5.Click
        RichTextBox1.SelectionColor = Color.Green
        RichTextBox1.SelectedText = "MS3U "
    End Sub

    Private Sub Button6_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button6.Click
        RichTextBox1.SelectionColor = Color.Green
        RichTextBox1.SelectedText = "MS1D "
    End Sub

    Private Sub Button7_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button7.Click
        RichTextBox1.SelectionColor = Color.Green
        RichTextBox1.SelectedText = "MS2D "
    End Sub

    Private Sub Button8_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button8.Click
        RichTextBox1.SelectionColor = Color.Green
        RichTextBox1.SelectedText = "MS3D "
    End Sub

    Private Sub Button9_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button9.Click
        RichTextBox1.SelectionColor = Color.Green
        RichTextBox1.SelectedText = "MSXL "
        RichTextBox1.SelectionColor = Color.Purple
        RichTextBox1.SelectedText = NumericUpDown5.Value.ToString & " "
    End Sub

    Private Sub Button10_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button10.Click
        RichTextBox1.SelectionColor = Color.Green
        RichTextBox1.SelectedText = "MSXR "
        RichTextBox1.SelectionColor = Color.Purple
        RichTextBox1.SelectedText = NumericUpDown5.Value.ToString & " "
    End Sub

    Private Sub Button11_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button11.Click
        RichTextBox1.SelectionColor = Color.Green
        RichTextBox1.SelectedText = "MSYU "
        RichTextBox1.SelectionColor = Color.Purple
        RichTextBox1.SelectedText = NumericUpDown5.Value.ToString & " "
    End Sub

    Private Sub Button12_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button12.Click
        RichTextBox1.SelectionColor = Color.Green
        RichTextBox1.SelectedText = "MSYD "
        RichTextBox1.SelectionColor = Color.Purple
        RichTextBox1.SelectedText = NumericUpDown5.Value.ToString & " "
    End Sub
#End Region
#Region "modos"
    Private Sub Button18_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button18.Click
        RichTextBox1.SelectionColor = Color.Green
        RichTextBox1.SelectedText = "SETK "
        RichTextBox1.SelectionColor = Color.Purple
        RichTextBox1.SelectedText = "0 "
    End Sub

    Private Sub Button19_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button19.Click
        RichTextBox1.SelectionColor = Color.Green
        RichTextBox1.SelectedText = "SETK "
        RichTextBox1.SelectionColor = Color.Purple
        RichTextBox1.SelectedText = "1 "
    End Sub

    Private Sub Button20_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button20.Click
        RichTextBox1.SelectionColor = Color.Green
        RichTextBox1.SelectedText = "SETM "
        RichTextBox1.SelectionColor = Color.Purple
        RichTextBox1.SelectedText = "1 "
    End Sub

    Private Sub Button21_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button21.Click
        RichTextBox1.SelectionColor = Color.Green
        RichTextBox1.SelectedText = "SETM "
        RichTextBox1.SelectionColor = Color.Purple
        RichTextBox1.SelectedText = "2 "
    End Sub

    Private Sub Button22_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button22.Click
        RichTextBox1.SelectionColor = Color.Green
        RichTextBox1.SelectedText = "SETM "
        RichTextBox1.SelectionColor = Color.Purple
        RichTextBox1.SelectedText = "3 "
    End Sub

    Private Sub Button23_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button23.Click
        RichTextBox1.SelectionColor = Color.Green
        RichTextBox1.SelectedText = "SETX "
        RichTextBox1.SelectionColor = Color.Purple
        RichTextBox1.SelectedText = "1 "
    End Sub

    Private Sub Button24_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button24.Click
        RichTextBox1.SelectionColor = Color.Green
        RichTextBox1.SelectedText = "SETX "
        RichTextBox1.SelectionColor = Color.Purple
        RichTextBox1.SelectedText = "2 "
    End Sub

    Private Sub Button25_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button25.Click
        RichTextBox1.SelectionColor = Color.Green
        RichTextBox1.SelectedText = "SETX "
        RichTextBox1.SelectionColor = Color.Purple
        RichTextBox1.SelectedText = "3 "
    End Sub
#End Region
    Private Sub Button17_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button17.Click
        RichTextBox1.SelectionColor = Color.Green
        RichTextBox1.SelectedText = "DLAY "
        RichTextBox1.SelectionColor = Color.Purple
        RichTextBox1.SelectedText = NumericUpDown6.Value.ToString() & " "
    End Sub
#Region "dx"
    Private Sub Button28_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button28.Click
        RichTextBox1.SelectionColor = Color.Green
        RichTextBox1.SelectedText = "DXBU "
        RichTextBox1.SelectionColor = Color.Purple
        RichTextBox1.SelectedText = NumericUpDown1.Value.ToString() & " "
    End Sub
    Private Sub Button29_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button29.Click
        RichTextBox1.SelectionColor = Color.Green
        RichTextBox1.SelectedText = "DXBD "
        RichTextBox1.SelectionColor = Color.Purple
        RichTextBox1.SelectedText = NumericUpDown1.Value.ToString() & " "
    End Sub
    Private Sub Button30_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button30.Click
        RichTextBox1.SelectionColor = Color.Green
        RichTextBox1.SelectedText = "DXPU "
        RichTextBox1.SelectionColor = Color.Purple
        RichTextBox1.SelectedText = NumericUpDown2.Value.ToString() & NumericUpDown3.Value.ToString() & " "
    End Sub
    Private Sub Button31_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button31.Click
        RichTextBox1.SelectionColor = Color.Green
        RichTextBox1.SelectedText = "DXPD "
        RichTextBox1.SelectionColor = Color.Purple
        RichTextBox1.SelectedText = NumericUpDown2.Value.ToString() & NumericUpDown3.Value.ToString() & " "
    End Sub
    Private Sub NumericUpDown2_ValueChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles NumericUpDown2.ValueChanged
        If NumericUpDown2.Value < 3 Then
            NumericUpDown3.Maximum = 8
        Else
            If NumericUpDown3.Value > 4 Then NumericUpDown3.Value = 4
            NumericUpDown3.Maximum = 4
        End If
    End Sub
#End Region
#Region "teclas"
    Private Sub Button1_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button1.Click
        RichTextBox1.SelectionColor = Color.Green
        RichTextBox1.SelectedText = "KEYD "
        RichTextBox1.SelectionColor = Color.Purple
        RichTextBox1.SelectedText = ComboBox1.SelectedIndex.ToString() & " "
    End Sub

    Private Sub Button2_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button2.Click
        RichTextBox1.SelectionColor = Color.Green
        RichTextBox1.SelectedText = "KEYU "
        RichTextBox1.SelectionColor = Color.Purple
        RichTextBox1.SelectedText = ComboBox1.SelectedIndex.ToString() & " "
    End Sub

    Private Sub vtSelPlantilla_SelectedItemChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles vtSelPlantilla.SelectedItemChanged
        CargarPlantilla(vtSelPlantilla.SelectedIndex)
    End Sub
#End Region
#Region "reps"
    Private Sub Button13_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button13.Click
        Button13.Enabled = False
        RichTextBox1.SelectionColor = Color.Green
        RichTextBox1.SelectedText = "HOLD "
    End Sub

    Private Sub Button14_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button14.Click
        If Button13.Enabled Then
            RichTextBox1.SelectionColor = Color.Green
            RichTextBox1.SelectedText = "REPI "
            reps = reps + 1
        End If
    End Sub

    Private Sub Button16_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button16.Click
        If Button13.Enabled Then
            Button15.Enabled = True
            RichTextBox1.SelectionColor = Color.Green
            RichTextBox1.SelectedText = "REPN "
            RichTextBox1.SelectionColor = Color.Purple
            RichTextBox1.SelectedText = NumericUpDown4.Value.ToString() & " "
            reps = reps + 1
        End If
    End Sub

    Private Sub Button15_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button15.Click
        If Button13.Enabled Then
            RichTextBox1.SelectionColor = Color.Green
            RichTextBox1.SelectedText = "REPF "
            reps = reps - 1
            If reps = 0 Then Button15.Enabled = False
        End If
    End Sub
#End Region

#Region "eventos hover"
    Private Sub Button1_MouseHover(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button1.MouseHover
        ToolStripStatusLabel1.Text = Traduce.Txt(135)
    End Sub
    Private Sub Button2_MouseHover(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button2.MouseHover
        ToolStripStatusLabel1.Text = Traduce.Txt(136)
    End Sub
    Private Sub Button3_MouseHover(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button3.MouseHover
        ToolStripStatusLabel1.Text = Traduce.Txt(137)
    End Sub
    Private Sub Button4_MouseHover(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button4.MouseHover
        ToolStripStatusLabel1.Text = Traduce.Txt(138)
    End Sub
    Private Sub Button5_MouseHover(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button5.MouseHover
        ToolStripStatusLabel1.Text = Traduce.Txt(139)
    End Sub
    Private Sub Button6_MouseHover(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button6.MouseHover
        ToolStripStatusLabel1.Text = Traduce.Txt(140)
    End Sub
    Private Sub Button7_MouseHover(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button7.MouseHover
        ToolStripStatusLabel1.Text = Traduce.Txt(141)
    End Sub
    Private Sub Button8_MouseHover(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button8.MouseHover
        ToolStripStatusLabel1.Text = Traduce.Txt(142)
    End Sub
    Private Sub Button9_MouseHover(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button9.MouseHover
        ToolStripStatusLabel1.Text = Traduce.Txt(143)
    End Sub
    Private Sub Button11_MouseHover(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button11.MouseHover
        ToolStripStatusLabel1.Text = Traduce.Txt(144)
    End Sub
    Private Sub NumericUpDown5_MouseHover(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles NumericUpDown5.MouseHover
        ToolStripStatusLabel1.Text = Traduce.Txt(145)
    End Sub
    Private Sub Button10_MouseHover(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button10.MouseHover
        ToolStripStatusLabel1.Text = Traduce.Txt(146)
    End Sub
    Private Sub Button12_MouseHover(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button12.MouseHover
        ToolStripStatusLabel1.Text = Traduce.Txt(147)
    End Sub
    Private Sub Button13_MouseHover(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button13.MouseHover
        ToolStripStatusLabel1.Text = Traduce.Txt(148)
    End Sub
    Private Sub Button14_MouseHover(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button14.MouseHover
        ToolStripStatusLabel1.Text = Traduce.Txt(149)
    End Sub
    Private Sub Button16_MouseHover(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button16.MouseHover
        ToolStripStatusLabel1.Text = Traduce.Txt(150)
    End Sub
    Private Sub Button15_MouseHover(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button15.MouseHover
        ToolStripStatusLabel1.Text = Traduce.Txt(151)
    End Sub
    Private Sub Button18_MouseHover(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button18.MouseHover
        ToolStripStatusLabel1.Text = Traduce.Txt(152)
    End Sub
    Private Sub Button19_MouseHover(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button19.MouseHover
        ToolStripStatusLabel1.Text = Traduce.Txt(153)
    End Sub
    Private Sub Button20_MouseHover(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button20.MouseHover
        ToolStripStatusLabel1.Text = Traduce.Txt(154)
    End Sub
    Private Sub Button21_MouseHover(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button21.MouseHover
        ToolStripStatusLabel1.Text = Traduce.Txt(155)
    End Sub
    Private Sub Button22_MouseHover(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button22.MouseHover
        ToolStripStatusLabel1.Text = Traduce.Txt(156)
    End Sub
    Private Sub Button23_MouseHover(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button23.MouseHover
        ToolStripStatusLabel1.Text = Traduce.Txt(157)
    End Sub
    Private Sub Button24_MouseHover(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button24.MouseHover
        ToolStripStatusLabel1.Text = Traduce.Txt(158)
    End Sub
    Private Sub Button25_MouseHover(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button25.MouseHover
        ToolStripStatusLabel1.Text = Traduce.Txt(159)
    End Sub
    Private Sub Button17_MouseHover(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button17.MouseHover
        ToolStripStatusLabel1.Text = Traduce.Txt(160)
    End Sub
    Private Sub Button30_MouseHover(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button30.MouseHover
        ToolStripStatusLabel1.Text = Traduce.Txt(161)
    End Sub
    Private Sub Button31_MouseHover(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button31.MouseHover
        ToolStripStatusLabel1.Text = Traduce.Txt(162)
    End Sub
    Private Sub Button28_MouseHover(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button28.MouseHover
        ToolStripStatusLabel1.Text = Traduce.Txt(163)
    End Sub
    Private Sub Button29_MouseHover(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button29.MouseHover
        ToolStripStatusLabel1.Text = Traduce.Txt(164)
    End Sub
#End Region
End Class