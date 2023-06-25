Module Vista

    Private modificando As Boolean = True
    Private modoP As Byte = 0
    Private modoM As Byte = 0
    Private modoA As Byte = 0
    Private modoT As Byte = 1
    Private PrIn As Byte = 0
    Public padre As Principal


#Region "Cargar inicial"
    Public Sub Boton(ByVal b As Integer, ByVal bt As Button)
        modificando = True
        If bt.Text = "" Then padre.ToolStripLabel1.Text = bt.Tag() & "    " Else padre.ToolStripLabel1.Text = bt.Text & "    "
        Reset()
        padre.MapaEjes.Enabled = False
        padre.MapaRot.Enabled = False
        padre.ModoAD.Enabled = False
        padre.Modofun.Enabled = False
        padre.Modos.Enabled = True
        padre.Macros.Enabled = True

        CambiarCuadroModos(b)
        padre.RadioButton11.Text = Traduce.Txt(70)
        padre.RadioButton12.Text = Traduce.Txt(71)

        padre.actual = b
        modificando = False

        padre.RadioButton11.Checked = True
    End Sub
    Public Sub Eje(ByVal e As Integer)
        modificando = True
        If e < 2 Then padre.ToolStripLabel1.Text = Traduce.Txt(e + 42) & "    " Else padre.ToolStripLabel1.Text = Traduce.Txt(e + 50) & "    "
        Reset()
        padre.MapaRot.Enabled = False
        If e = 0 Or e = 1 Then
            padre.MapaEjes.Enabled = True
            padre.ModoAD.Enabled = False
            padre.Modofun.Enabled = False
            padre.Modos.Enabled = False
            padre.Macros.Enabled = False
            Select Case padre.datos.MapaEjesDX(e)
                Case 0
                    padre.RadioButton23.Checked = True
                Case 1
                    padre.RadioButton24.Checked = True
                Case 2
                    padre.RadioButton21.Checked = True
                Case 3
                    padre.RadioButton22.Checked = True
            End Select
        Else
            padre.ModoAD.Enabled = True
            padre.CheckBox4.Checked = True
            padre.NumericUpDown1.Enabled = True
            If padre.datos.MapaAD(e - 2) Then
                padre.MapaEjes.Enabled = True
                padre.Modofun.Enabled = False
                padre.Modos.Enabled = False
                padre.Macros.Enabled = False
                Select Case padre.datos.MapaEjesDX(e)
                    Case 0
                        padre.RadioButton23.Checked = True
                    Case 1
                        padre.RadioButton24.Checked = True
                    Case 2
                        padre.RadioButton21.Checked = True
                    Case 3
                        padre.RadioButton22.Checked = True
                End Select
                padre.RadioButton17.Checked = True
            Else
                padre.MapaEjes.Enabled = False
                padre.Modofun.Enabled = True
                padre.Modos.Enabled = True
                padre.Macros.Enabled = True
                padre.RadioButton18.Checked = True
                CambiarCuadroModos(e + 38)
            End If
        End If
        padre.RadioButton11.Text = Traduce.Txt(72)
        padre.RadioButton12.Text = Traduce.Txt(73)

        padre.actual = e + 38
        If e = 2 Or e = 3 Then
            If Not padre.datos.MapaAD(e - 2) Then
                Precargar()
            End If
        End If
        modificando = False
    End Sub
    Public Sub Rotatorio(ByVal r As Integer)
        modificando = True
        padre.ToolStripLabel1.Text = Traduce.Txt(r + 54) & "    "
        Reset()
        padre.MapaEjes.Enabled = False
        padre.MapaRot.Enabled = True
        padre.ModoAD.Enabled = False
        padre.Modofun.Enabled = True
        padre.Modos.Enabled = True
        padre.Macros.Enabled = True

        Select Case padre.datos.MapaRotDX(r)
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

        CambiarCuadroModos(r + 36)
        padre.RadioButton11.Text = Traduce.Txt(72)
        padre.RadioButton12.Text = Traduce.Txt(73)

        padre.actual = r + 36
        Precargar()
        modificando = False
    End Sub

    Private Sub Reset()
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
    End Sub
    Private Sub CambiarCuadroModos(ByVal b As Byte)
        CuadroModosPinkie(padre.datos.PinkieOn(b))
        CuadroModosModo(padre.datos.ModosOn(b))
        CuadroModosAuxModo(padre.datos.AuxModosOn(b))
    End Sub
    Public Sub Precargar()
        Dim sel As Byte
        If padre.actual = 40 Or padre.actual = 41 Then
            sel = padre.actual - 40
        Else
            sel = padre.actual - 34
        End If
        padre.CheckBox4.Checked = True
        If padre.datos.MapaEjes(0, 0, 0, sel).Incremental Then
            padre.Panel5.Enabled = False
            padre.RadioButton12.Checked = True
            padre.RadioButton2.Checked = True
            padre.NumericUpDown2.Value = padre.datos.MapaEjes(0, 0, 0, sel).Indices(2)
            padre.ComboBox1.SelectedIndex = padre.datos.MapaEjes(0, 0, 0, sel).Indices(0)
        Else
            padre.NumericUpDown1.Enabled = True
            padre.Label11.Visible = False
            padre.NumericUpDown2.Visible = False
            padre.Button44.Visible = True
            padre.Panel2.Enabled = False
            padre.RadioButton1.Checked = True
            padre.NumericUpDown1.Value = 1
            padre.ComboBox1.SelectedIndex = padre.datos.MapaEjes(0, 0, 0, sel).Indices(0)
        End If
    End Sub

    Public Sub CuadroModosPinkie(ByVal b As Boolean)
        If Not modificando Then
            padre.datos.SetPinkieOn(padre.actual, b)
        End If
        If b Then
            padre.RadioButton3.Enabled = True
            padre.RadioButton4.Enabled = True
            padre.RadioButton4.Checked = True
            padre.CheckBox1.Checked = True
        Else
            padre.RadioButton3.Enabled = False
            padre.RadioButton4.Enabled = False
            If Not modificando Then
                modificando = True
                padre.RadioButton4.Checked = True
                padre.RadioButton4.Checked = False
                modificando = False
            Else
                padre.RadioButton4.Checked = True
                padre.RadioButton4.Checked = False
            End If
            padre.CheckBox1.Checked = False
        End If
    End Sub
    Public Sub CuadroModosModo(ByVal b As Boolean)
        If Not modificando Then
            padre.datos.SetModosOn(padre.actual, b)
        End If
        If b Then
            padre.RadioButton5.Enabled = True
            padre.RadioButton6.Enabled = True
            padre.RadioButton7.Enabled = True
            padre.RadioButton5.Checked = True
            padre.CheckBox2.Checked = True
        Else
            padre.RadioButton5.Enabled = False
            padre.RadioButton6.Enabled = False
            padre.RadioButton7.Enabled = False
            If Not modificando Then
                modificando = True
                padre.RadioButton5.Checked = True
                padre.RadioButton5.Checked = False
                modificando = False
            Else
                padre.RadioButton5.Checked = True
                padre.RadioButton5.Checked = False
            End If
            padre.CheckBox2.Checked = False
        End If
    End Sub
    Public Sub CuadroModosAuxModo(ByVal b As Boolean)
        If Not modificando Then
            padre.datos.SetAuxModosOn(padre.actual, b)
        End If
        If b Then
            padre.RadioButton8.Enabled = True
            padre.RadioButton9.Enabled = True
            padre.RadioButton10.Enabled = True
            padre.RadioButton9.Checked = True
            padre.CheckBox3.Checked = True
        Else
            padre.RadioButton8.Enabled = False
            padre.RadioButton9.Enabled = False
            padre.RadioButton10.Enabled = False
            If Not modificando Then
                modificando = True
                padre.RadioButton9.Checked = True
                padre.RadioButton9.Checked = False
                modificando = False
            Else
                padre.RadioButton9.Checked = True
                padre.RadioButton9.Checked = False
            End If
            padre.CheckBox3.Checked = False
        End If
    End Sub

#End Region

#Region "Eventos raton"
    Public Sub CambiarMapaEjes(ByVal e As Byte)
        If Not modificando Then
            padre.datos.MapaEjesDX(padre.actual - 38) = e
        End If
    End Sub
    Public Sub CambiarMapaRot(ByVal r As Byte)
        If Not modificando Then
            padre.datos.MapaRotDX(padre.actual - 36) = r
        End If
    End Sub
    Public Sub CambiarMapaRot2()
        If Not modificando Then
            If padre.CheckBox5.Checked Then
                padre.RadioButton19.Enabled = True
                padre.RadioButton20.Enabled = True
            Else
                padre.RadioButton19.Enabled = False
                padre.RadioButton20.Enabled = False
                padre.RadioButton19.Checked = True
                padre.RadioButton19.Checked = False
                padre.datos.MapaRotDX(padre.actual - 36) = 0
            End If
        End If
    End Sub
    Public Sub CambiarAD(ByVal a As Boolean)
        If Not modificando Then
            padre.datos.MapaAD(padre.actual - 40) = a
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
        End If
    End Sub

    Public Sub CambiarFuncionamiento(ByVal inc As Boolean)
        If Not modificando Then
            Dim sel As Byte
            If padre.actual = 40 Or padre.actual = 41 Then
                sel = padre.actual - 40
            Else
                sel = padre.actual - 34
            End If
            If inc Then
                For i As Byte = 0 To 14
                    padre.datos.MapaEjes(modoP, modoM, modoA, sel).Bandas(i) = 0
                    padre.datos.MapaEjes(modoP, modoM, modoA, sel).Indices(i) = 0
                Next
                padre.datos.MapaEjes(modoP, modoM, modoA, sel).Indices(15) = 0
                modificando = True
                padre.RadioButton11.Checked = True
                modificando = False
            Else
                For i As Byte = 0 To 3
                    padre.datos.MapaEjes(modoP, modoM, modoA, sel).Indices(i) = 0
                Next
                modificando = True
                padre.NumericUpDown1.Value = 1
                modificando = False
            End If
            padre.datos.MapaEjes(modoP, modoM, modoA, sel).Incremental = inc
            Cargar()
        End If
    End Sub
    Public Sub CambiarSensibilidad(ByVal v As Integer)
        If Not modificando Then
            If padre.actual = 40 Or padre.actual = 41 Then
                padre.datos.MapaEjes(modoP, modoM, modoA, padre.actual - 40).Indices(PrIn) = v
            Else
                padre.datos.MapaEjes(modoP, modoM, modoA, padre.actual - 34).Indices(PrIn) = v
            End If
        End If
    End Sub

    Public Sub CambiarPinkie(ByVal b As Boolean)
        If Not modificando Then
            If b Then
                modoP = 1
            Else
                modoP = 0
            End If
            Cargar()
        End If
    End Sub
    Public Sub CambiarModo(ByVal m As Byte)
        If Not modificando Then
            modoM = m
            Cargar()
        End If
    End Sub
    Public Sub CambiarModoAux(ByVal m As Byte)
        If Not modificando Then
            modoA = m
            Cargar()
        End If
    End Sub
    Public Sub CambiarToggle(ByVal t As Byte)
        If Not modificando Then
            modoT = t - 1
            Cargar()
        End If
    End Sub
    Public Sub CuadroModosToggle(ByVal b As Boolean)
        If Not modificando Then
            If padre.actual < 36 Then
                padre.datos.mapaBotones(modoP, modoM, modoA, PrIn, padre.actual).Estado = b
                padre.NumericUpDown1.Enabled = b
            Else
                padre.CheckBox4.Checked = True
                padre.NumericUpDown1.Enabled = True
            End If
        End If
    End Sub
    Public Sub CambiarPresInc(ByVal p As Boolean)
        If Not modificando Then
            If p Then
                PrIn = 1
            Else
                PrIn = 0
            End If
            Cargar()
        End If
    End Sub

    Public Sub CambiarMacro(ByVal cs As Integer)
        If Not modificando Then
            If padre.actual < 36 Then
                padre.datos.mapaBotones(modoP, modoM, modoA, PrIn, padre.actual).Indices(modoT) = cs
            Else
                Dim sel As Byte
                If padre.actual = 40 Or padre.actual = 41 Then
                    sel = padre.actual - 40
                Else
                    sel = padre.actual - 34
                End If
                If padre.datos.MapaEjes(modoP, modoM, modoA, sel).Incremental Then
                    padre.datos.MapaEjes(modoP, modoM, modoA, sel).Indices(PrIn) = cs
                Else
                    padre.datos.MapaEjes(modoP, modoM, modoA, sel).Indices(modoT) = cs
                End If
            End If
        End If
    End Sub
#End Region

    Public Sub Cargar()
        If padre.actual < 36 Then
            padre.ComboBox1.SelectedIndex = padre.datos.mapaBotones(modoP, modoM, modoA, PrIn, padre.actual).Indices(modoT)
            padre.CheckBox4.Checked = padre.datos.mapaBotones(modoP, modoM, modoA, PrIn, padre.actual).Estado
            padre.NumericUpDown1.Enabled = padre.CheckBox4.Checked
        Else
            Dim sel As Byte
            If padre.actual = 40 Or padre.actual = 41 Then
                sel = padre.actual - 40
            Else
                sel = padre.actual - 34
            End If
            If padre.datos.MapaEjes(modoP, modoM, modoA, sel).Incremental Then
                padre.Label11.Visible = True
                padre.NumericUpDown2.Visible = True
                padre.Button44.Visible = False
                padre.Panel5.Enabled = False
                padre.Panel2.Enabled = True
                modificando = True
                padre.RadioButton12.Checked = True
                padre.RadioButton2.Checked = True
                modificando = False
                padre.NumericUpDown2.Value = padre.datos.MapaEjes(modoP, modoM, modoA, sel).Indices(PrIn + 2)
                padre.ComboBox1.SelectedIndex = padre.datos.MapaEjes(modoP, modoM, modoA, sel).Indices(PrIn)
            Else
                padre.Label11.Visible = False
                padre.NumericUpDown2.Visible = False
                padre.Button44.Visible = True
                padre.Panel2.Enabled = False
                padre.Panel5.Enabled = True
                modificando = True
                padre.RadioButton1.Checked = True
                modificando = False
                padre.ComboBox1.SelectedIndex = padre.datos.MapaEjes(modoP, modoM, modoA, sel).Indices(modoT)
            End If
        End If
    End Sub

    Public Sub GetModos(ByRef p As Byte, ByRef m As Byte, ByRef a As Byte)
        p = modoP
        m = modoM
        a = modoA
    End Sub
End Module
