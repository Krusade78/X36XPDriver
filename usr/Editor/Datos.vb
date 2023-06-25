Public Class Datos

    Private Declare Ansi Function CargarDatos Lib "x36_svc.dll" (ByVal mdf As String, ByVal jdf As String, ByVal mapab As Byte(,,,,,), ByVal mapae As Byte(,,,,), ByVal confd As Byte(), ByVal mapaad() As Byte) As Byte

    Public Structure MB
        Public Estado As Byte  ' 4 bit idc 4 bit npos
        Public Indices() As uInt16
    End Structure
    Public Structure MEj
        Public Incremental As Boolean
        Public Bandas() As Byte
        Public Indices() As UInt16
    End Structure
    Private Structure MMod
        Public Modos As Boolean
        Public AuxModos As Boolean
        Public Pinkie As Boolean
        'Public Toggle As Boolean
    End Structure

    Public mapaBotones(1, 2, 2, 1, 35) As MB
    Public MapaEjes(1, 2, 2, 3) As MEj
    Private MapaModos(41) As MMod
    Public MapaEjesDX() As Byte = {0, 1, 2, 3}
    Public MapaRotDX() As Byte = {0, 0}
    Public MapaAD() As Boolean = {True, True}
    Public Macros As New ArrayList

    Public Sub New()
        For i1 As Byte = 0 To 1
            For i2 As Byte = 0 To 2
                For i3 As Byte = 0 To 2
                    For i4 As Byte = 0 To 1
                        For i5 As Byte = 0 To 35
                            ReDim mapaBotones(i1, i2, i3, i4, i5).Indices(15)
                        Next
                    Next
                Next
            Next
        Next
        For i1 As Byte = 0 To 1
            For i2 As Byte = 0 To 2
                For i3 As Byte = 0 To 2
                    For i4 As Byte = 0 To 3
                        ReDim MapaEjes(i1, i2, i3, i4).Indices(15)
                        ReDim MapaEjes(i1, i2, i3, i4).Bandas(14)
                    Next
                Next
            Next
        Next
    End Sub

    Public Function ModosOn(ByVal b As Byte) As Boolean
        Return MapaModos(b).Modos
    End Function
    Public Function AuxModosOn(ByVal b As Byte) As Boolean
        Return MapaModos(b).AuxModos
    End Function
    Public Function PinkieOn(ByVal b As Byte) As Boolean
        Return MapaModos(b).Pinkie
    End Function
    'Public Function ToggleOn(ByVal b As Byte) As Boolean
    '    Return MapaModos(b).Toggle
    'End Function
    Public Sub SetModosOn(ByVal b As Byte, ByVal st As Boolean)
        MapaModos(b).Modos = st
    End Sub
    Public Sub SetAuxModosOn(ByVal b As Byte, ByVal st As Boolean)
        MapaModos(b).AuxModos = st
    End Sub
    Public Sub SetPinkieOn(ByVal b As Byte, ByVal st As Boolean)
        MapaModos(b).Pinkie = st
    End Sub
    'Public Sub SetToggleOn(ByVal b As Byte, ByVal st As Boolean)
    '    MapaModos(b).Toggle = st
    'End Sub

    Public Sub QuitarIndice(ByVal i As Integer)
        For i1 As Byte = 0 To 1
            For i2 As Byte = 0 To 2
                For i3 As Byte = 0 To 2
                    For i4 As Byte = 0 To 1
                        For i5 As Byte = 0 To 35
                            For i6 As Byte = 0 To 15
                                If mapaBotones(i1, i2, i3, i4, i5).Indices(i6) > i Then
                                    mapaBotones(i1, i2, i3, i4, i5).Indices(i6) = mapaBotones(i1, i2, i3, i4, i5).Indices(i6) - 1
                                ElseIf mapaBotones(i1, i2, i3, i4, i5).Indices(i6) = i Then
                                    mapaBotones(i1, i2, i3, i4, i5).Indices(i6) = 0
                                End If
                            Next
                        Next
                    Next
                Next
            Next
        Next
        For i1 As Byte = 0 To 1
            For i2 As Byte = 0 To 2
                For i3 As Byte = 0 To 2
                    For i4 As Byte = 0 To 3
                        For i5 As Byte = 0 To 15
                            If MapaEjes(i1, i2, i3, i4).Indices(i5) > i Then
                                MapaEjes(i1, i2, i3, i4).Indices(i5) = MapaEjes(i1, i2, i3, i4).Indices(i5) - 1
                            ElseIf MapaEjes(i1, i2, i3, i4).Indices(i5) = i Then
                                MapaEjes(i1, i2, i3, i4).Indices(i5) = 0
                            End If
                        Next
                    Next
                Next
            Next
        Next
        Macros.RemoveAt(i - 1)
    End Sub

    Private rutaPerfil As String
    Public Function GetRutaPerfil() As String
        Return rutaPerfil
    End Function

    Public Function Guardar(ByVal combo As ComboBox) As Boolean
        Return Guardar(rutaPerfil & ".jdf", combo)
    End Function
    Public Function Guardar(ByVal archivo As String, ByVal combo As ComboBox) As Boolean
        Dim f As System.IO.StreamWriter

        ' MDF
        Try
            f = New System.IO.StreamWriter(Mid(archivo, 1, archivo.Length - 4) & ".mdf", False, System.Text.Encoding.ASCII)
        Catch
            Return False
        End Try
        Try
            For i As Integer = 1 To combo.Items.Count - 1
                f.WriteLine(combo.Items.Item(i) & "=" & Trim(Macros.Item(i - 1)))
            Next
        Catch
            Try
                f.Close()
            Catch
            End Try
            Return False
        End Try
        Try
            f.Close()
        Catch
            Return False
        End Try

        ' JDF
        Try
            f = New System.IO.StreamWriter(archivo, False, System.Text.Encoding.ASCII)
        Catch
            Return False
        End Try
        Try
            For b As Byte = 0 To 35
                f.WriteLine("BTN" & (b + 1).ToString())
                For i1 As Byte = 0 To 1
                    f.WriteLine(Chr(9) & "/K" & i1.ToString())
                    For i2 As Byte = 0 To 2
                        f.WriteLine(Chr(9) & Chr(9) & "/M" & (i2 + 1).ToString())
                        For i3 As Byte = 0 To 2
                            f.WriteLine(Chr(9) & Chr(9) & Chr(9) & "/X" & (i3 + 1).ToString())
                            For i4 As Byte = 0 To 1
                                If i4 = 0 Then
                                    f.Write(Chr(9) & Chr(9) & Chr(9) & Chr(9) & "/R" & Chr(9))
                                Else
                                    f.Write(Chr(9) & Chr(9) & Chr(9) & Chr(9) & "/P" & Chr(9))
                                End If
                                If mapaBotones(i1, i2, i3, i4, b).Estado = 0 Then
                                    If mapaBotones(i1, i2, i3, i4, b).Indices(0) <> 0 Then
                                        f.Write(combo.Items.Item(mapaBotones(i1, i2, i3, i4, b).Indices(0)))
                                    End If
                                Else
                                    For i5 As Byte = 1 To 16
                                        If mapaBotones(i1, i2, i3, i4, b).Indices(i5 - 1) <> 0 Then
                                            f.Write("/T" & (i5).ToString() & " " & combo.Items.Item(mapaBotones(i1, i2, i3, i4, b).Indices(i5 - 1)) & "  ")
                                        End If
                                    Next
                                End If
                                f.WriteLine()
                            Next
                        Next
                    Next
                Next
            Next
            For i As Byte = 0 To 3
                f.Write("AXIS" & (i + 1).ToString() & " ")
                If i < 2 Then
                    Select Case MapaEjesDX(i)
                        Case 0
                            f.WriteLine("/ASx")
                        Case 1
                            f.WriteLine("/ASy")
                        Case 2
                            f.WriteLine("/ASz")
                        Case 3
                            f.WriteLine("/ASr")
                    End Select
                ElseIf MapaAD(i - 2) Then
                    Select Case MapaEjesDX(i)
                        Case 0
                            f.WriteLine("/ASx")
                        Case 1
                            f.WriteLine("/ASy")
                        Case 2
                            f.WriteLine("/ASz")
                        Case 3
                            f.WriteLine("/ASr")
                    End Select
                Else
                    f.WriteLine("/ASD")
                    For i1 As Byte = 0 To 1
                        f.WriteLine(Chr(19) & "/K" & i1.ToString())
                        For i2 As Byte = 0 To 2
                            f.WriteLine(Chr(19) & Chr(19) & "/M" & (i2 + 1).ToString())
                            For i3 As Byte = 0 To 2
                                f.WriteLine(Chr(19) & Chr(19) & Chr(19) & "/X" & (i3 + 1).ToString())

                                If MapaEjes(i1, i2, i3, i - 2).Incremental Then
                                    If MapaEjes(i1, i2, i3, i - 2).Indices(0) <> 0 Then
                                        f.Write(Chr(19) & Chr(19) & Chr(19) & Chr(19) & "/DEC" & MapaEjes(i1, i2, i3, i - 2).Indices(2).ToString() & " " & combo.Items.Item(MapaEjes(i1, i2, i3, i - 2).Indices(0)))
                                        If MapaEjes(i1, i2, i3, i - 2).Indices(1) <> 0 Then
                                            f.WriteLine("   " & "/INC" & MapaEjes(i1, i2, i3, i - 2).Indices(3).ToString() & " " & combo.Items.Item(MapaEjes(i1, i2, i3, i - 2).Indices(1)))
                                        Else
                                            f.WriteLine()
                                        End If
                                    ElseIf MapaEjes(i1, i2, i3, i - 2).Indices(1) <> 0 Then
                                        f.WriteLine(Chr(19) & Chr(19) & Chr(19) & Chr(19) & "/INC" & MapaEjes(i1, i2, i3, i - 2).Indices(3).ToString() & " " & combo.Items.Item(MapaEjes(i1, i2, i3, i - 2).Indices(1)))
                                    End If
                                Else
                                    Dim tab As Boolean = False
                                    f.Write(Chr(19) & Chr(19) & Chr(19) & Chr(19))
                                    For b As Byte = 0 To 15
                                        If MapaEjes(i1, i2, i3, i - 2).Indices(b) <> 0 Then
                                            If Not tab Then
                                                f.Write(Chr(9) & Chr(9) & Chr(9) & Chr(9))
                                                tab = True
                                            End If
                                            f.Write("/T" & (b + 1).ToString() & " " & combo.Items.Item(MapaEjes(i1, i2, i3, i - 2).Indices(b)) & "   ")
                                        End If
                                    Next
                                    If tab Then f.WriteLine()
                                    tab = False
                                    For b As Byte = 0 To 14
                                        If MapaEjes(i1, i2, i3, i - 2).Indices(b) <> 0 Then
                                            If Not tab Then
                                                f.Write(Chr(9) & Chr(9) & Chr(9) & Chr(9))
                                                tab = True
                                            End If
                                            f.Write("/B" & (b + 1).ToString() & " #" & MapaEjes(i1, i2, i3, i - 2).Bandas(b).ToString() & "   ")
                                        End If
                                    Next
                                    If tab Then f.WriteLine()
                                End If
                            Next
                        Next
                    Next
                End If
            Next
            For rt As Byte = 0 To 1
                If MapaRotDX(rt) = 1 Then
                    f.WriteLine("ROT" & (rt + 1).ToString() & " /AS1")
                ElseIf MapaRotDX(rt) = 2 Then
                    f.WriteLine("ROT" & (rt + 1).ToString() & " /AS2")
                Else
                    f.WriteLine("ROT" & (rt + 1).ToString())
                End If
                For i1 As Byte = 0 To 1
                    f.WriteLine(Chr(9) & "/K" & i1.ToString())
                    For i2 As Byte = 0 To 2
                        f.WriteLine(Chr(9) & Chr(9) & "/M" & (i2 + 1).ToString())
                        For i3 As Byte = 0 To 2
                            f.WriteLine(Chr(9) & Chr(9) & Chr(9) & "/X" & (i3 + 1).ToString())

                            If MapaEjes(i1, i2, i3, rt + 2).Incremental Then
                                If MapaEjes(i1, i2, i3, rt + 2).Indices(0) <> 0 Then
                                    f.Write(Chr(9) & Chr(9) & Chr(9) & Chr(9) & "/DEC" & MapaEjes(i1, i2, i3, rt + 2).Indices(2).ToString() & " " & combo.Items.Item(MapaEjes(i1, i2, i3, rt + 2).Indices(0)))
                                    If MapaEjes(i1, i2, i3, rt + 2).Indices(1) <> 0 Then
                                        f.WriteLine("   " & "/INC" & MapaEjes(i1, i2, i3, rt + 2).Indices(3).ToString() & " " & combo.Items.Item(MapaEjes(i1, i2, i3, rt + 2).Indices(1)))
                                    Else
                                        f.WriteLine()
                                    End If
                                ElseIf MapaEjes(i1, i2, i3, rt + 2).Indices(1) <> 0 Then
                                    f.WriteLine(Chr(9) & Chr(9) & Chr(9) & Chr(9) & "/INC" & MapaEjes(i1, i2, i3, rt + 2).Indices(3).ToString() & " " & combo.Items.Item(MapaEjes(i1, i2, i3, rt + 2).Indices(1)))
                                End If
                            Else
                                Dim tab As Boolean = False
                                For b As Byte = 0 To 15
                                    If MapaEjes(i1, i2, i3, rt + 2).Indices(b) <> 0 Then
                                        If Not tab Then
                                            f.Write(Chr(9) & Chr(9) & Chr(9) & Chr(9))
                                            tab = True
                                        End If
                                        f.Write("/T" & (b + 1).ToString() & " " & combo.Items.Item(MapaEjes(i1, i2, i3, rt + 2).Indices(b)) & "   ")
                                    End If
                                Next
                                If tab Then f.WriteLine()
                                tab = False
                                For b As Byte = 0 To 14
                                    If MapaEjes(i1, i2, i3, rt + 2).Indices(b) <> 0 Then
                                        If Not tab Then
                                            f.Write(Chr(9) & Chr(9) & Chr(9) & Chr(9))
                                            tab = True
                                        End If
                                        f.Write("/B" & (b + 1).ToString() & " #" & MapaEjes(i1, i2, i3, rt + 2).Bandas(b).ToString() & "   ")
                                    End If
                                Next
                                If tab Then f.WriteLine()
                            End If
                        Next
                    Next
                Next
            Next
        Catch
            Try
                f.Close()
            Catch
            End Try
            Return False
        End Try
        Try
            f.Close()
        Catch
            Return False
        End Try

        rutaPerfil = Mid(archivo, 1, archivo.Length - 4)
        Return True
    End Function


    Public Function CargarArchivo(ByVal archivo As String, ByVal combo As ComboBox) As Boolean
        Dim bufferB(1, 2, 2, 1, 35, 33) As Byte
        Dim bufferE(1, 2, 2, 3, 47) As Byte
        Dim confD(5) As Byte
        Dim bufferAD(1) As Byte
        archivo = Mid(archivo, 1, archivo.Length - 4)
        Dim b As Byte = CargarDatos(archivo & ".mdf", archivo & ".jdf", bufferB, bufferE, confD, bufferAD)
        If b = 1 Then
            If CargarMacros(archivo & ".mdf", combo) Then
                CargarMapa(bufferB, bufferE, confD, bufferAD)
                rutaPerfil = archivo
            Else
                Return False
            End If
        Else
            Return False
        End If
        Return True
    End Function
    Private Function CargarMacros(ByVal archivo As String, ByVal combo As ComboBox) As Boolean
        Dim f As System.IO.StreamReader
        Dim linea As String
        Dim r As String = Chr(9)
        Try
            f = New System.IO.StreamReader(archivo)
        Catch
            Return False
        End Try
        Try
            Do While f.Peek() >= 0
                linea = f.ReadLine()
                linea = linea.Replace(Chr(9), " ")
                linea = linea.Trim()
                If linea <> "" Then
                    If Left(linea, 2) <> "//" Then
                        combo.Items.Add(Left(linea, linea.IndexOf("=")))
                        linea = linea.Remove(0, linea.IndexOf("=") + 1)
                        If linea.IndexOf("//") <> -1 Then linea = Left(linea, linea.IndexOf("//"))
                        Macros.Add(linea)
                    End If
                End If
            Loop
        Catch
            Try
                f.Close()
            Catch
            End Try
            Return False
        End Try

        Try
            f.Close()
        Catch
        End Try
        Return True
    End Function
    Private Sub CargarMapa(ByRef bfB As Byte(,,,,,), ByRef bfE As Byte(,,,,), ByRef confD As Byte(), ByRef AD As Byte())
        For i1 As Byte = 0 To 1
            For i2 As Byte = 0 To 2
                For i3 As Byte = 0 To 2
                    For i4 As Byte = 0 To 1
                        For i5 As Byte = 0 To 35
                            If bfB(i1, i2, i3, i4, i5, 0) = 0 Then
                                mapaBotones(i1, i2, i3, i4, i5).Estado = 0
                            Else
                                mapaBotones(i1, i2, i3, i4, i5).Estado = 1
                            End If
                            For idx As Byte = 0 To 15
                                mapaBotones(i1, i2, i3, i4, i5).Indices(idx) = bfB(i1, i2, i3, i4, i5, 2 + (idx * 2)) + (bfB(i1, i2, i3, i4, i5, 3 + (idx * 2)) * 256)
                                If mapaBotones(i1, i2, i3, i4, i5).Indices(idx) <> 0 Then
                                    If i1 = 1 Then MapaModos(i5).Pinkie = True
                                    If i2 > 0 Then MapaModos(i5).Modos = True
                                    If i3 > 0 Then MapaModos(i5).AuxModos = True
                                    'If idx > 0 Then MapaModos(i5).Toggle = True
                                End If
                            Next
                        Next
                    Next
                Next
            Next
        Next
        For i1 As Byte = 0 To 1
            For i2 As Byte = 0 To 2
                For i3 As Byte = 0 To 2
                    For i4 As Byte = 0 To 1
                        MapaEjes(i1, i2, i3, i4).Incremental = bfE(i1, i2, i3, i4, 0)
                        For idx As Byte = 0 To 14
                            MapaEjes(i1, i2, i3, i4).Bandas(idx) = bfE(i1, i2, i3, i4, 1 + idx)
                            MapaEjes(i1, i2, i3, i4).Indices(idx) = bfE(i1, i2, i3, i4, 16 + (idx * 2)) + (bfE(i1, i2, i3, i4, 17 + (idx * 2)) * 256)
                            If MapaEjes(i1, i2, i3, i4).Indices(idx) <> 0 Or MapaEjes(i1, i2, i3, i4).Bandas(idx) Then
                                If i1 = 1 Then MapaModos(38 + i4).Pinkie = True
                                If i2 > 0 Then MapaModos(38 + i4).Modos = True
                                If i3 > 0 Then MapaModos(38 + i4).AuxModos = True
                            End If
                        Next
                        MapaEjes(i1, i2, i3, i4).Indices(15) = bfE(i1, i2, i3, i4, 46) + (bfE(i1, i2, i3, i4, 47) * 256)
                        If MapaEjes(i1, i2, i3, i4).Indices(15) Then
                            If i1 = 1 Then MapaModos(38 + i4).Pinkie = True
                            If i2 > 0 Then MapaModos(38 + i4).Modos = True
                            If i3 > 0 Then MapaModos(38 + i4).AuxModos = True
                        End If
                    Next
                    For i4 As Byte = 2 To 3
                        MapaEjes(i1, i2, i3, i4).Incremental = bfE(i1, i2, i3, i4, 0)
                        For idx As Byte = 0 To 14
                            MapaEjes(i1, i2, i3, i4).Bandas(idx) = bfE(i1, i2, i3, i4, 1 + idx)
                            MapaEjes(i1, i2, i3, i4).Indices(idx) = bfE(i1, i2, i3, i4, 16 + (idx * 2)) + (bfE(i1, i2, i3, i4, 17 + (idx * 2)) * 256)
                            If MapaEjes(i1, i2, i3, i4).Indices(idx) <> 0 Or MapaEjes(i1, i2, i3, i4).Bandas(idx) Then
                                If i1 = 1 Then MapaModos(34 + i4).Pinkie = True
                                If i2 > 0 Then MapaModos(34 + i4).Modos = True
                                If i3 > 0 Then MapaModos(34 + i4).AuxModos = True
                            End If
                        Next
                        MapaEjes(i1, i2, i3, i4).Indices(15) = bfE(i1, i2, i3, i4, 46) + (bfE(i1, i2, i3, i4, 47) * 256)
                        If MapaEjes(i1, i2, i3, i4).Indices(15) Then
                            If i1 = 1 Then MapaModos(34 + i4).Pinkie = True
                            If i2 > 0 Then MapaModos(34 + i4).Modos = True
                            If i3 > 0 Then MapaModos(34 + i4).AuxModos = True
                        End If
                    Next
                Next
            Next
        Next
        MapaEjesDX(0) = confD(0)
        MapaEjesDX(1) = confD(1)
        MapaEjesDX(2) = confD(2)
        MapaEjesDX(3) = confD(3)
        MapaRotDX(0) = confD(4) And 3
        MapaRotDX(1) = confD(4) >> 2
        If AD(0) = 1 Then MapaAD(0) = True Else MapaAD(0) = False
        If AD(1) = 1 Then MapaAD(1) = True Else MapaAD(1) = False
    End Sub
End Class


