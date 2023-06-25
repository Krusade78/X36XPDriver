Public Class Traduce

    Public Shared Function Msg(ByVal id1 As Integer, ByVal id2 As Integer, ByVal tipo As Microsoft.VisualBasic.MsgBoxStyle) As Microsoft.VisualBasic.MsgBoxResult
        Dim texto1, texto2 As String
        Traducir(id1, texto1)
        Traducir(id2, texto2)
        Return MsgBox(texto1, tipo, texto2)
    End Function

    Public Shared Function Txt(ByVal id As Integer) As String
        Dim texto As String
        Traducir(id, texto)
        Txt = texto
    End Function

    Private Shared Sub Traducir(ByVal ID As Integer, ByRef texto As String)
        Dim nombre As String
        Dim ruta As String
        CogerNombreIdioma(nombre)
        If Not BuscarArchivo(nombre, ruta) Then
            texto = Nothing
            Exit Sub
        End If
        If Not ExtraerCadena(ruta, ID, texto) Then texto = Nothing
    End Sub

    Private Shared Sub CogerNombreIdioma(ByRef nombre As String)
        Select Case (System.Globalization.CultureInfo.CurrentCulture.LCID And &HFF)
            Case &HC
                nombre = "french.h"
            Case &H10
                nombre = "italian.h"
            Case &HA
                nombre = "spanish.h"
            Case &H7
                nombre = "german.h"
            Case Else
                nombre = "english.h"
        End Select
    End Sub

    Private Shared Function BuscarArchivo(ByVal idioma As String, ByRef ruta As String) As Boolean
        Dim arch As System.IO.FileInfo
        Dim pordefecto As Boolean = False

da:
        Try
            arch = New System.IO.FileInfo(idioma)
        Catch
            arch = Nothing
            BuscarArchivo = False
            Exit Function
        End Try
        If arch.Exists Then
            ruta = arch.FullName
            BuscarArchivo = True
        Else
            If Not pordefecto Then
                idioma = "english.h"
                pordefecto = True
                arch = Nothing
                GoTo da
            Else
                BuscarArchivo = False
            End If
        End If
        arch = Nothing
    End Function

    Private Shared Function ExtraerCadena(ByVal ruta As String, ByVal ID As Integer, ByRef texto As String) As Boolean
        Dim arch As System.IO.StreamReader
        Dim linea As String
        Dim lID As Integer
        Dim dl As Char() = {"#", "="}

        Try
            arch = New System.IO.StreamReader(ruta)
        Catch
            ExtraerCadena = False
            Exit Function
        End Try

        While True
            Try
                linea = arch.ReadLine()
            Catch
                GoTo mal
            End Try
            If linea = Nothing Then
                GoTo mal
            Else
                If linea.Chars(0) = "#" Then
                    texto = ""
                    linea = linea.Remove(0, linea.IndexOf("#") + 1)
                    lID = Val(Left(linea, linea.IndexOf("=")))
                    If lID = ID Then
                        linea = linea.Remove(0, linea.IndexOf("=") + 1)
                        linea = linea.Replace("\n", Chr(13))
                        texto = linea
                        Exit While
                    End If
                End If
            End If
        End While

        arch.Close()
        ExtraerCadena = True
        Exit Function

mal:
        arch.Close()
        ExtraerCadena = False
    End Function
End Class
