Public Class Launcher

    Private Declare Ansi Function CompileAndLaunch Lib "x36_svc.dll" (ByVal mdf As String, ByVal jdf As String) As Byte

    Private Sub Form1_Load(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.Load
        Me.Text = Traduce.Txt(0)
        Label1.Text = Traduce.Txt(1)
        Button1.Text = Traduce.Txt(2)
        Button2.Text = Traduce.Txt(3)
        Button3.Text = Traduce.Txt(5)
        Button4.Text = Traduce.Txt(4)
        Button5.Text = Traduce.Txt(6)
        ActualizarLista()
    End Sub

    Private Sub Button4_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button4.Click
        ActualizarLista()
    End Sub

    Private Sub Button3_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button3.Click
        If Not ListBox1.SelectedItem Is Nothing Then
            Shell("editor.exe " & """" & System.Environment.CurrentDirectory & "\" & ListBox1.GetItemText(ListBox1.SelectedItem) & """", AppWinStyle.NormalFocus)
        End If
    End Sub

    Private Sub Button2_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button2.Click
        If Not ListBox1.SelectedItem Is Nothing Then
            Cargar(ListBox1.GetItemText(ListBox1.SelectedItem))
        End If
    End Sub

    Private Sub Button1_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button1.Click
        CargarPredeterminado()
    End Sub

    Private Sub ActualizarLista()
        ListBox1.Items.Clear()

        Dim di As New System.IO.DirectoryInfo(".")
        Dim fi As System.IO.FileInfo() = di.GetFiles("*.jdf")
        For i As Integer = 0 To fi.Length - 1
            ListBox1.Items.Add(Mid(fi(i).Name, 1, (fi(i).Name.Length - 4)))
        Next
    End Sub

    Private Sub CargarPredeterminado()
        Dim f As System.IO.StreamWriter

        Try
            f = New System.IO.StreamWriter("defaultTemp.tmp.mdf", False)
        Catch
            Traduce.Msg(11, 0, MsgBoxStyle.Exclamation)
            Exit Sub
        End Try
        Try
            f.WriteLine("DXB1=DXBD 1 HOLD DXBU 1")
            f.WriteLine("DXB2=DXBD 2 HOLD DXBU 2")
            f.WriteLine("DXB3=DXBD 3 HOLD DXBU 3")
            f.WriteLine("DXB4=DXBD 4 HOLD DXBU 4")
            f.WriteLine("DXB5=DXBD 5 HOLD DXBU 5")
            f.WriteLine("DXB6=DXBD 6 HOLD DXBU 6")
            f.WriteLine("DXP11=DXPD 11 HOLD DXPU 11")
            f.WriteLine("DXP15=DXPD 15 HOLD DXPU 15")
            f.WriteLine("DXP17=DXPD 17 HOLD DXPU 17")
            f.WriteLine("DXP13=DXPD 13 HOLD DXPU 13")
            f.WriteLine("DXP18=DXPD 18 HOLD DXPU 18")
            f.WriteLine("DXP12=DXPD 12 HOLD DXPU 12")
            f.WriteLine("DXP16=DXPD 16 HOLD DXPU 16")
            f.WriteLine("DXP14=DXPD 14 HOLD DXPU 14")
            f.WriteLine("DXP21=DXPD 21 HOLD DXPU 21")
            f.WriteLine("DXP25=DXPD 25 HOLD DXPU 25")
            f.WriteLine("DXP27=DXPD 27 HOLD DXPU 27")
            f.WriteLine("DXP23=DXPD 23 HOLD DXPU 23")
            f.WriteLine("DXP28=DXPD 28 HOLD DXPU 28")
            f.WriteLine("DXP22=DXPD 22 HOLD DXPU 22")
            f.WriteLine("DXP26=DXPD 26 HOLD DXPU 26")
            f.WriteLine("DXP24=DXPD 24 HOLD DXPU 24")
            f.WriteLine("DXB7=DXBD 7 HOLD DXBU 7")
            f.WriteLine("DXB8=DXBD 8 HOLD DXBU 8")
            f.WriteLine("DXP31=DXPD 31 HOLD DXPU 31")
            f.WriteLine("DXP32=DXPD 32 HOLD DXPU 32")
            f.WriteLine("DXP33=DXPD 33 HOLD DXPU 33")
            f.WriteLine("DXP34=DXPD 34 HOLD DXPU 34")
            f.WriteLine("DXP41=DXPD 41 HOLD DXPU 41")
            f.WriteLine("DXP42=DXPD 42 HOLD DXPU 42")
            f.WriteLine("DXP43=DXPD 43 HOLD DXPU 43")
            f.WriteLine("DXP44=DXPD 44 HOLD DXPU 44")
        Catch
            Traduce.Msg(11, 0, MsgBoxStyle.Exclamation)
            Try
                f.Close()
                System.IO.File.Delete("defaultTemp.tmp.mdf")
            Catch
            End Try
            Exit Sub
        End Try
        Try
            f.Close()
        Catch
            Traduce.Msg(11, 0, MsgBoxStyle.Exclamation)
            Exit Sub
        End Try

        Try
            f = New System.IO.StreamWriter("defaultTemp.tmp.jdf", False)
        Catch
            Traduce.Msg(12, 0, MsgBoxStyle.Exclamation)
            Try
                System.IO.File.Delete("defaultTemp.tmp.mdf")
            Catch
            End Try
            Exit Sub
        End Try

        Try
            f.WriteLine("BTN1 /P DXB1")
            f.WriteLine("BTN2 /P DXB2")
            f.WriteLine("BTN3 /P DXB3")
            f.WriteLine("BTN4 /P DXB4")
            f.WriteLine("BTN5 /P DXB5")
            f.WriteLine("BTN6 /P DXB6")
            f.WriteLine("BTN7 /P DXP11")
            f.WriteLine("BTN8 /P DXP15")
            f.WriteLine("BTN9 /P DXP17")
            f.WriteLine("BTN10 /P DXP13")
            f.WriteLine("BTN11 /P DXP18")
            f.WriteLine("BTN12 /P DXP12")
            f.WriteLine("BTN13 /P DXP16")
            f.WriteLine("BTN14 /P DXP14")
            f.WriteLine("BTN15 /P DXP21")
            f.WriteLine("BTN16 /P DXP25")
            f.WriteLine("BTN17 /P DXP27")
            f.WriteLine("BTN18 /P DXP23")
            f.WriteLine("BTN19 /P DXP28")
            f.WriteLine("BTN20 /P DXP22")
            f.WriteLine("BTN21 /P DXP26")
            f.WriteLine("BTN22 /P DXP24")
            f.WriteLine("BTN23 /P DXB7")
            f.WriteLine("BTN24 /P DXB8")
            f.WriteLine("BTN29 /P DXP31")
            f.WriteLine("BTN30 /P DXP33")
            f.WriteLine("BTN31 /P DXP34")
            f.WriteLine("BTN32 /P DXP32")
            f.WriteLine("BTN33 /P DXP41")
            f.WriteLine("BTN34 /P DXP43")
            f.WriteLine("BTN35 /P DXP44")
            f.WriteLine("BTN36 /P DXP42")
            f.WriteLine("AXIS1 /ASX")
            f.WriteLine("AXIS2 /ASY")
            f.WriteLine("AXIS3 /ASZ")
            f.WriteLine("AXIS4 /ASR")
            f.WriteLine("ROT1 /AS1")
            f.WriteLine("ROT2 /AS2")
        Catch
            Traduce.Msg(12, 0, MsgBoxStyle.Exclamation)
            Try
                System.IO.File.Delete("defaultTemp.tmp.mdf")
                f.Close()
                System.IO.File.Delete("defaultTemp.tmp.jdf")
            Catch
            End Try
            Exit Sub
        End Try

        Try
            f.Close()
        Catch
            Traduce.Msg(12, 0, MsgBoxStyle.Exclamation)
            Try
                System.IO.File.Delete("defaultTemp.tmp.mdf")
            Catch
            End Try
            Exit Sub
        End Try

        Cargar("defaultTemp.tmp")

        Try
            System.IO.File.Delete("defaultTemp.tmp.jdf")
            System.IO.File.Delete("defaultTemp.tmp.mdf")
        Catch
        End Try
    End Sub

    Private Sub Cargar(ByVal nombre As String)
        Try
            System.IO.File.OpenRead(nombre & ".jdf").Close()
        Catch
            Traduce.Msg(7, 0, MsgBoxStyle.Exclamation)
            Exit Sub
        End Try
        Try
            System.IO.File.OpenRead(nombre & ".mdf").Close()
        Catch
            Traduce.Msg(8, 0, MsgBoxStyle.Exclamation)
            Exit Sub
        End Try
        nombre = New System.IO.DirectoryInfo(".").FullName & "\" & nombre
        Try
            If CompileAndLaunch(nombre & ".mdf", nombre & ".jdf") = 1 Then
                Traduce.Msg(10, 0, MsgBoxStyle.Information)
            End If
        Catch
            Traduce.Msg(9, 0, MsgBoxStyle.Exclamation)
        End Try
    End Sub

    Private Sub Button5_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button5.Click
        Me.Close()
    End Sub
End Class