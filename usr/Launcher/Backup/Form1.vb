Public Class Form1
    Inherits System.Windows.Forms.Form

#Region " Código generado por el Diseñador de Windows Forms "

    Public Sub New()
        MyBase.New()

        'El Diseñador de Windows Forms requiere esta llamada.
        InitializeComponent()

        'Agregar cualquier inicialización después de la llamada a InitializeComponent()

    End Sub

    'Form reemplaza a Dispose para limpiar la lista de componentes.
    Protected Overloads Overrides Sub Dispose(ByVal disposing As Boolean)
        If disposing Then
            If Not (components Is Nothing) Then
                components.Dispose()
            End If
        End If
        MyBase.Dispose(disposing)
    End Sub

    'Requerido por el Diseñador de Windows Forms
    Private components As System.ComponentModel.IContainer

    'NOTA: el Diseñador de Windows Forms requiere el siguiente procedimiento
    'Puede modificarse utilizando el Diseñador de Windows Forms. 
    'No lo modifique con el editor de código.
    Friend WithEvents Label1 As System.Windows.Forms.Label
    Friend WithEvents ListBox1 As System.Windows.Forms.ListBox
    Friend WithEvents Button1 As System.Windows.Forms.Button
    Friend WithEvents Button2 As System.Windows.Forms.Button
    Friend WithEvents Button3 As System.Windows.Forms.Button
    Friend WithEvents Button4 As System.Windows.Forms.Button
    Friend WithEvents Button5 As System.Windows.Forms.Button
    <System.Diagnostics.DebuggerStepThrough()> Private Sub InitializeComponent()
        Dim resources As System.ComponentModel.ComponentResourceManager = New System.ComponentModel.ComponentResourceManager(GetType(Form1))
        Me.Label1 = New System.Windows.Forms.Label
        Me.ListBox1 = New System.Windows.Forms.ListBox
        Me.Button1 = New System.Windows.Forms.Button
        Me.Button2 = New System.Windows.Forms.Button
        Me.Button3 = New System.Windows.Forms.Button
        Me.Button4 = New System.Windows.Forms.Button
        Me.Button5 = New System.Windows.Forms.Button
        Me.SuspendLayout()
        '
        'Label1
        '
        Me.Label1.ForeColor = System.Drawing.SystemColors.ControlText
        Me.Label1.Location = New System.Drawing.Point(8, 8)
        Me.Label1.Name = "Label1"
        Me.Label1.Size = New System.Drawing.Size(192, 24)
        Me.Label1.TabIndex = 0
        Me.Label1.Text = "Label1"
        '
        'ListBox1
        '
        Me.ListBox1.BackColor = System.Drawing.Color.AliceBlue
        Me.ListBox1.ForeColor = System.Drawing.SystemColors.ControlText
        Me.ListBox1.Location = New System.Drawing.Point(8, 32)
        Me.ListBox1.Name = "ListBox1"
        Me.ListBox1.Size = New System.Drawing.Size(208, 329)
        Me.ListBox1.TabIndex = 1
        '
        'Button1
        '
        Me.Button1.BackColor = System.Drawing.Color.Black
        Me.Button1.FlatStyle = System.Windows.Forms.FlatStyle.System
        Me.Button1.Font = New System.Drawing.Font("Arial Narrow", 9.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Button1.Location = New System.Drawing.Point(224, 32)
        Me.Button1.Name = "Button1"
        Me.Button1.Size = New System.Drawing.Size(80, 24)
        Me.Button1.TabIndex = 2
        Me.Button1.Text = "Button1"
        Me.Button1.UseVisualStyleBackColor = False
        '
        'Button2
        '
        Me.Button2.FlatStyle = System.Windows.Forms.FlatStyle.System
        Me.Button2.Font = New System.Drawing.Font("Arial Narrow", 9.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Button2.Location = New System.Drawing.Point(224, 64)
        Me.Button2.Name = "Button2"
        Me.Button2.Size = New System.Drawing.Size(80, 23)
        Me.Button2.TabIndex = 3
        Me.Button2.Text = "Button2"
        '
        'Button3
        '
        Me.Button3.FlatStyle = System.Windows.Forms.FlatStyle.System
        Me.Button3.Font = New System.Drawing.Font("Arial Narrow", 9.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Button3.Location = New System.Drawing.Point(224, 96)
        Me.Button3.Name = "Button3"
        Me.Button3.Size = New System.Drawing.Size(80, 23)
        Me.Button3.TabIndex = 4
        Me.Button3.Text = "Button3"
        '
        'Button4
        '
        Me.Button4.FlatStyle = System.Windows.Forms.FlatStyle.System
        Me.Button4.Font = New System.Drawing.Font("Microsoft Sans Serif", 8.25!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Button4.Location = New System.Drawing.Point(224, 304)
        Me.Button4.Name = "Button4"
        Me.Button4.Size = New System.Drawing.Size(80, 23)
        Me.Button4.TabIndex = 5
        Me.Button4.Text = "Button4"
        '
        'Button5
        '
        Me.Button5.FlatStyle = System.Windows.Forms.FlatStyle.System
        Me.Button5.Font = New System.Drawing.Font("Microsoft Sans Serif", 8.25!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Button5.Location = New System.Drawing.Point(224, 336)
        Me.Button5.Name = "Button5"
        Me.Button5.Size = New System.Drawing.Size(80, 23)
        Me.Button5.TabIndex = 6
        Me.Button5.Text = "Button5"
        '
        'Form1
        '
        Me.AutoScaleBaseSize = New System.Drawing.Size(5, 13)
        Me.BackColor = System.Drawing.SystemColors.Control
        Me.ClientSize = New System.Drawing.Size(312, 365)
        Me.Controls.Add(Me.Button5)
        Me.Controls.Add(Me.Button4)
        Me.Controls.Add(Me.Button3)
        Me.Controls.Add(Me.Button2)
        Me.Controls.Add(Me.Button1)
        Me.Controls.Add(Me.ListBox1)
        Me.Controls.Add(Me.Label1)
        Me.Icon = CType(resources.GetObject("$this.Icon"), System.Drawing.Icon)
        Me.MaximizeBox = False
        Me.Name = "Form1"
        Me.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen
        Me.Text = "Form1"
        Me.ResumeLayout(False)

    End Sub

#End Region

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

    Private Sub Button5_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button5.Click
        Me.Close()
    End Sub

    Private Sub Button4_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button4.Click
        ActualizarLista()
    End Sub

    Private Sub Button3_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button3.Click
        If Not ListBox1.SelectedItem Is Nothing Then
            Shell("editor.exe " & """" & ListBox1.GetItemText(ListBox1.SelectedItem) & """", AppWinStyle.NormalFocus)
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
            f.WriteLine("// Null mdf")
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
            f.WriteLine("BTN1 /DXB1")
            f.WriteLine("BTN2 /DXB2")
            f.WriteLine("BTN3 /DXB3")
            f.WriteLine("BTN4 /DXB4")
            f.WriteLine("BTN5 /DXB5")
            f.WriteLine("BTN6 /DXB6")
            f.WriteLine("BTN7 /DXP11")
            f.WriteLine("BTN8 /DXP15")
            f.WriteLine("BTN9 /DXP17")
            f.WriteLine("BTN10 /DXP13")
            f.WriteLine("BTN11 /DXP18")
            f.WriteLine("BTN12 /DXP12")
            f.WriteLine("BTN13 /DXP16")
            f.WriteLine("BTN14 /DXP14")
            f.WriteLine("BTN15 /DXP21")
            f.WriteLine("BTN16 /DXP25")
            f.WriteLine("BTN17 /DXP27")
            f.WriteLine("BTN18 /DXP23")
            f.WriteLine("BTN19 /DXP28")
            f.WriteLine("BTN20 /DXP22")
            f.WriteLine("BTN21 /DXP26")
            f.WriteLine("BTN22 /DXP24")
            f.WriteLine("BTN23 /DXB7")
            f.WriteLine("BTN24 /DXB8")
            f.WriteLine("BTN29 /DXP31")
            f.WriteLine("BTN30 /DXP33")
            f.WriteLine("BTN31 /DXP34")
            f.WriteLine("BTN32 /DXP32")
            f.WriteLine("BTN33 /DXP41")
            f.WriteLine("BTN34 /DXP43")
            f.WriteLine("BTN35 /DXP44")
            f.WriteLine("BTN36 /DXP42")
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

End Class
