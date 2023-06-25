Module Vista

    Private modoP As Byte = 0
    Private modoM As Byte = 0
    Private modoA As Byte = 0
    Private modoT As Byte = 1
    Private PrIn As Byte = 0
    Public padre As Principal

    Public Sub GetModos(ByRef p As Byte, ByRef m As Byte, ByRef a As Byte)
        p = modoP
        m = modoM
        a = modoA
    End Sub

#Region "Cargar inicial"
    Public Sub Boton(ByVal b As Integer, ByVal bt As Button)
        padre.actual = b
        If bt.Text = "" Then padre.ToolStripLabel1.Text = bt.Tag() & "    " Else padre.ToolStripLabel1.Text = bt.Text & "    "
        Reset()

        padre.MapaEjes.Enabled = False
        padre.MapaRot.Enabled = False
        padre.ModoAD.Enabled = False
        padre.Modofun.Enabled = False
        padre.Modos.Enabled = True
        padre.Macros.Enabled = True
        padre.RadioButton11.Text = Traduce.Txt(70)
        padre.RadioButton12.Text = Traduce.Txt(71)
        PrIn = 1

        Cargar()
    End Sub
    Public Sub Eje(ByVal e As Integer)
        padre.actual = e + 38
        If e < 2 Then padre.ToolStripLabel1.Text = Traduce.Txt(e + 42) & "    " Else padre.ToolStripLabel1.Text = Traduce.Txt(e + 50) & "    "
        Reset()

        padre.MapaRot.Enabled = False
        padre.RadioButton11.Text = Traduce.Txt(72)
        padre.RadioButton12.Text = Traduce.Txt(73)
        If e = 0 Or e = 1 Then
            padre.MapaEjes.Enabled = True
            padre.ModoAD.Enabled = False
            padre.Modofun.Enabled = False
            padre.Modos.Enabled = False
            padre.Macros.Enabled = False
        Else
            padre.ModoAD.Enabled = True
            If padre.datos.GetMapaAD(e - 2) Then
                padre.MapaEjes.Enabled = True
                padre.Modofun.Enabled = False
                padre.Modos.Enabled = False
                padre.Macros.Enabled = False
            Else
                padre.MapaEjes.Enabled = False
                padre.Modofun.Enabled = True
                padre.Modos.Enabled = True
                padre.Macros.Enabled = True
            End If
        End If
        Cargar()
    End Sub
    Public Sub Rotatorio(ByVal r As Integer)
        padre.actual = r + 36
        padre.ToolStripLabel1.Text = Traduce.Txt(r + 54) & "    "
        Reset()

        padre.MapaEjes.Enabled = False
        padre.MapaRot.Enabled = True
        padre.ModoAD.Enabled = False
        padre.Modofun.Enabled = True
        padre.Modos.Enabled = True
        padre.Macros.Enabled = True
        padre.RadioButton11.Text = Traduce.Txt(72)
        padre.RadioButton12.Text = Traduce.Txt(73)

        Cargar()
    End Sub
    Private Sub Reset()
        padre.eventos = False

        ' Ejes
        padre.RadioButton23.Checked = True
        padre.RadioButton23.Checked = False
        ' Rots
        padre.CheckBox5.Checked = False
        padre.RadioButton19.Checked = True
        padre.RadioButton19.Checked = False
        ' AD
        padre.RadioButton17.Checked = True
        padre.RadioButton17.Checked = False
        ' funcionamiento
        padre.RadioButton1.Checked = True
        padre.RadioButton1.Checked = False
        padre.Button44.Visible = False
        padre.Label11.Visible = True
        padre.NumericUpDown2.Visible = True
        padre.NumericUpDown2.Value = 0
        ' modos
        padre.RadioButton3.Checked = True
        padre.RadioButton3.Checked = False
        padre.RadioButton5.Checked = True
        padre.RadioButton5.Checked = False
        padre.RadioButton9.Checked = True
        padre.RadioButton9.Checked = False
        padre.NumericUpDown1.Value = 1
        padre.RadioButton11.Checked = True
        padre.RadioButton11.Checked = False
        padre.CheckBox1.Checked = False
        padre.CheckBox2.Checked = False
        padre.CheckBox3.Checked = False
        padre.CheckBox4.Checked = False
        padre.Panel2.Enabled = True
        padre.Panel5.Enabled = True

        modoA = 0
        modoP = 0
        modoM = 0
        modoT = 0
        PrIn = 0

        padre.eventos = True
    End Sub

#End Region

#Region "Eventos raton"
    Public Sub CambiarMapaEjes(ByVal e As Byte)
        padre.datos.SetMapaEjesDX(padre.actual - 38, e)
        Cargar()
    End Sub
    Public Sub CambiarMapaRot(ByVal r As Byte)
        If r > 0 Then
            padre.datos.SetMapaRotDX(padre.actual - 36, r)
            Cargar()
        Else
            If padre.CheckBox5.Checked Then
                padre.RadioButton19.Enabled = True
                padre.RadioButton20.Enabled = True
            Else
                padre.RadioButton19.Enabled = False
                padre.RadioButton20.Enabled = False
                padre.eventos = False
                padre.RadioButton19.Checked = True
                padre.RadioButton19.Checked = False
                padre.eventos = True
                padre.datos.SetMapaRotDX(padre.actual - 36, 0)
            End If
        End If
    End Sub
    Public Sub CambiarAD(ByVal a As Boolean)
        padre.datos.SetMapaAD(padre.actual - 40, a)
        If a Then
            padre.MapaEjes.Enabled = True
            padre.Modofun.Enabled = False
            padre.Modos.Enabled = False
            padre.Macros.Enabled = False
        Else
            padre.MapaEjes.Enabled = False
            padre.Modofun.Enabled = True
            padre.Modos.Enabled = True
            padre.Macros.Enabled = True
        End If
    End Sub

    Public Sub CambiarFuncionamiento(ByVal inc As Boolean)
        Dim sel As Byte
        If padre.actual = 40 Or padre.actual = 41 Then
            sel = padre.actual - 40
        Else
            sel = padre.actual - 34
        End If

        padre.datos.SetMapaEjes_Incremental(modoP, modoM, modoA, sel, inc)
        If inc Then
            For i As Byte = 0 To 14
                padre.datos.SetMapaEjes_Bandas(modoP, modoM, modoA, sel, i, 0)
                padre.datos.SetMapaEjes_Indices(modoP, modoM, modoA, sel, i, 0)
            Next
            padre.datos.SetMapaEjes_Indices(modoP, modoM, modoA, sel, 15, 0)
        Else
            For i As Byte = 0 To 3
                padre.datos.SetMapaEjes_Indices(modoP, modoM, modoA, sel, i, 0)
            Next
            padre.eventos = False
            padre.NumericUpDown1.Value = 1
            padre.eventos = True
        End If
        Cargar()
    End Sub
    Public Sub CambiarSensibilidad(ByVal v As Integer)
        If padre.actual = 40 Or padre.actual = 41 Then
            padre.datos.SetMapaEjes_Indices(modoP, modoM, modoA, padre.actual - 40, PrIn + 2, v)
        Else
            padre.datos.SetMapaEjes_Indices(modoP, modoM, modoA, padre.actual - 34, PrIn + 2, v)
        End If
    End Sub

    Public Sub CambiarPinkie(ByVal c As Byte, ByVal b As Boolean)
        If c = 0 Then
            padre.datos.SetPinkieOn(padre.actual, b)
        Else
            If b Then modoP = 1 Else modoP = 0
        End If
        Cargar()
    End Sub
    Public Sub CambiarModo(ByVal c As Byte, ByVal m As Byte)
        If c = 0 Then
            padre.datos.SetModosOn(padre.actual, m)
        Else
            modoM = m
        End If
        Cargar()
    End Sub
    Public Sub CambiarModoAux(ByVal c As Byte, ByVal m As Byte)
        If c = 0 Then
            padre.datos.SetAuxModosOn(padre.actual, m)
        Else
            modoA = m
        End If
        Cargar()
    End Sub
    Public Sub CambiarToggle(ByVal c As Byte, ByVal t As Byte)
        If c = 0 Then
            If padre.actual < 36 Then
                padre.datos.SetMapaBotones_Estado(modoP, modoM, modoA, PrIn, padre.actual, t)
            End If
        Else
            modoT = t - 1
        End If
        Cargar()
    End Sub
    Public Sub CambiarPresInc(ByVal p As Boolean)
        If p Then
            PrIn = 1
        Else
            PrIn = 0
        End If
        Cargar()
    End Sub

    Public Sub CambiarMacro(ByVal cs As Integer)
        If padre.actual < 36 Then
            padre.datos.SetMapaBotones_Indices(modoP, modoM, modoA, PrIn, padre.actual, modoT, cs)
        Else
            Dim sel As Byte
            Dim mapa As Datos.MEj = Nothing
            If padre.actual = 40 Or padre.actual = 41 Then
                sel = padre.actual - 40
            Else
                sel = padre.actual - 34
            End If
            If padre.datos.GetMapaEjes(modoP, modoM, modoA, sel).Incremental Then
                padre.datos.SetMapaEjes_Indices(modoP, modoM, modoA, sel, PrIn, cs)
            Else
                padre.datos.SetMapaEjes_Indices(modoP, modoM, modoA, sel, modoT, cs)
            End If
        End If
    End Sub
#End Region

#Region "Carga datos"
    Private Sub Cargar()
        padre.eventos = False
        CambiarCuadroModos()
        If padre.actual < 36 Then
            CargarBoton()
        ElseIf padre.actual > 37 Then
            CargarEje()
        Else
            CargarRotatorio()
        End If
        padre.eventos = True
    End Sub

    Private Sub CambiarCuadroModos()
        CuadroModosPinkie(padre.datos.PinkieOn(padre.actual))
        CuadroModosModo(padre.datos.ModosOn(padre.actual))
        CuadroModosAuxModo(padre.datos.AuxModosOn(padre.actual))
    End Sub
    Private Sub CuadroModosPinkie(ByVal b As Boolean)
        If b Then
            padre.RadioButton3.Enabled = True
            padre.RadioButton4.Enabled = True
            If modoP = 1 Then padre.RadioButton3.Checked = True Else padre.RadioButton4.Checked = True
            padre.CheckBox1.Checked = True
        Else
            padre.RadioButton3.Enabled = False
            padre.RadioButton4.Enabled = False
            padre.RadioButton4.Checked = True
            padre.RadioButton4.Checked = False
            padre.CheckBox1.Checked = False
        End If
    End Sub
    Private Sub CuadroModosModo(ByVal b As Boolean)
        If b Then
            Dim r() As RadioButton = {padre.RadioButton5, padre.RadioButton6, padre.RadioButton7}
            padre.RadioButton5.Enabled = True
            padre.RadioButton6.Enabled = True
            padre.RadioButton7.Enabled = True
            r(modoM).Checked = True
            padre.CheckBox2.Checked = True
        Else
            padre.RadioButton5.Enabled = False
            padre.RadioButton6.Enabled = False
            padre.RadioButton7.Enabled = False
            padre.RadioButton5.Checked = True
            padre.RadioButton5.Checked = False
            padre.CheckBox2.Checked = False
        End If
    End Sub
    Private Sub CuadroModosAuxModo(ByVal b As Boolean)
        If b Then
            Dim r() As RadioButton = {padre.RadioButton9, padre.RadioButton8, padre.RadioButton10}
            padre.RadioButton8.Enabled = True
            padre.RadioButton9.Enabled = True
            padre.RadioButton10.Enabled = True
            r(modoA).Checked = True
            padre.CheckBox3.Checked = True
        Else
            padre.RadioButton8.Enabled = False
            padre.RadioButton9.Enabled = False
            padre.RadioButton10.Enabled = False

            padre.RadioButton9.Checked = True
            padre.RadioButton9.Checked = False
            padre.CheckBox3.Checked = False
        End If
    End Sub

    Private Sub CargarBoton()
        padre.ComboBox1.SelectedIndex = padre.datos.GetMapaBotones(modoP, modoM, modoA, PrIn, padre.actual).Indices(modoT)
        padre.CheckBox4.Checked = padre.datos.GetMapaBotones(modoP, modoM, modoA, PrIn, padre.actual).Estado
        padre.NumericUpDown1.Enabled = padre.CheckBox4.Checked
        padre.NumericUpDown1.Value = modoT + 1
        If PrIn = 0 Then padre.RadioButton12.Checked = True Else padre.RadioButton11.Checked = True
    End Sub
    Private Sub CargarEje()
        Dim e As Byte = padre.actual - 38
        Select Case padre.datos.GetMapaEjesDX(e)
            Case 0
                padre.RadioButton23.Checked = True
            Case 1
                padre.RadioButton24.Checked = True
            Case 2
                padre.RadioButton21.Checked = True
            Case 3
                padre.RadioButton22.Checked = True
        End Select
        If e > 1 Then
            If padre.datos.GetMapaAD(e - 2) Then
                padre.RadioButton17.Checked = True
            Else
                padre.RadioButton18.Checked = True
                CargarEjRot()
            End If
        End If
    End Sub
    Private Sub CargarRotatorio()
        Select Case padre.datos.GetMapaRotDX(padre.actual - 36)
            Case 0
                padre.RadioButton19.Enabled = False
                padre.RadioButton20.Enabled = False
            Case 1
                padre.CheckBox5.Checked = True
                padre.RadioButton19.Enabled = True
                padre.RadioButton20.Enabled = True
                padre.RadioButton19.Checked = True
            Case 2
                padre.CheckBox5.Checked = True
                padre.RadioButton19.Enabled = True
                padre.RadioButton20.Enabled = True
                padre.RadioButton20.Checked = True
        End Select
        CargarEjRot()
    End Sub
    Private Sub CargarEjRot()
        Dim sel As Byte
        If padre.actual = 40 Or padre.actual = 41 Then
            sel = padre.actual - 40
        Else
            sel = padre.actual - 34
        End If

        If padre.datos.GetMapaEjes(modoP, modoM, modoA, sel).Incremental Then
            padre.Label11.Visible = True
            padre.NumericUpDown2.Visible = True
            padre.Button44.Visible = False
            padre.Panel5.Enabled = False
            padre.Panel2.Enabled = True
            If PrIn = 0 Then padre.RadioButton12.Checked = True Else padre.RadioButton11.Checked = True
            padre.RadioButton2.Checked = True
            padre.NumericUpDown2.Value = padre.datos.GetMapaEjes(modoP, modoM, modoA, sel).Indices(PrIn + 2)
            padre.ComboBox1.SelectedIndex = padre.datos.GetMapaEjes(modoP, modoM, modoA, sel).Indices(PrIn)
        Else
            padre.Label11.Visible = False
            padre.NumericUpDown2.Visible = False
            padre.Button44.Visible = True
            padre.Panel2.Enabled = False
            padre.Panel5.Enabled = True
            padre.CheckBox4.Checked = True
            padre.NumericUpDown1.Enabled = True
            padre.RadioButton1.Checked = True
            padre.ComboBox1.SelectedIndex = padre.datos.GetMapaEjes(modoP, modoM, modoA, sel).Indices(modoT)
        End If
    End Sub
#End Region

End Module
