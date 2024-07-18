<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()>
Partial Class MainWindow
    Inherits System.Windows.Forms.Form

    'Form 重写 Dispose，以清理组件列表。
    <System.Diagnostics.DebuggerNonUserCode()>
    Protected Overrides Sub Dispose(ByVal disposing As Boolean)
        Try
            If disposing AndAlso components IsNot Nothing Then
                components.Dispose()
            End If
        Finally
            MyBase.Dispose(disposing)
        End Try
    End Sub

    'Windows 窗体设计器所必需的
    Private components As System.ComponentModel.IContainer

    '注意: 以下过程是 Windows 窗体设计器所必需的
    '可以使用 Windows 窗体设计器修改它。  
    '不要使用代码编辑器修改它。
    <System.Diagnostics.DebuggerStepThrough()>
    Private Sub InitializeComponent()
        Dim resources As System.ComponentModel.ComponentResourceManager = New System.ComponentModel.ComponentResourceManager(GetType(MainWindow))
        Me.StartButton = New System.Windows.Forms.Button()
        Me.InputGroupBox = New System.Windows.Forms.GroupBox()
        Me.FileListButton = New System.Windows.Forms.Button()
        Me.FileOpenButton = New System.Windows.Forms.Button()
        Me.Label1 = New System.Windows.Forms.Label()
        Me.Label3 = New System.Windows.Forms.Label()
        Me.OutputDir = New System.Windows.Forms.TextBox()
        Me.Button2 = New System.Windows.Forms.Button()
        Me.Label4 = New System.Windows.Forms.Label()
        Me.OutputTemplate = New System.Windows.Forms.ComboBox()
        Me.SettingButton = New System.Windows.Forms.Button()
        Me.OutputGroupBox = New System.Windows.Forms.GroupBox()
        Me.InfoLabel = New System.Windows.Forms.Label()
        Me.InputGroupBox.SuspendLayout()
        Me.OutputGroupBox.SuspendLayout()
        Me.SuspendLayout()
        '
        'StartButton
        '
        resources.ApplyResources(Me.StartButton, "StartButton")
        Me.StartButton.Name = "StartButton"
        Me.StartButton.UseVisualStyleBackColor = True
        '
        'InputGroupBox
        '
        Me.InputGroupBox.Controls.Add(Me.FileListButton)
        Me.InputGroupBox.Controls.Add(Me.FileOpenButton)
        Me.InputGroupBox.Controls.Add(Me.Label1)
        resources.ApplyResources(Me.InputGroupBox, "InputGroupBox")
        Me.InputGroupBox.Name = "InputGroupBox"
        Me.InputGroupBox.TabStop = False
        '
        'FileListButton
        '
        resources.ApplyResources(Me.FileListButton, "FileListButton")
        Me.FileListButton.Name = "FileListButton"
        Me.FileListButton.UseVisualStyleBackColor = True
        '
        'FileOpenButton
        '
        resources.ApplyResources(Me.FileOpenButton, "FileOpenButton")
        Me.FileOpenButton.Name = "FileOpenButton"
        Me.FileOpenButton.UseVisualStyleBackColor = True
        '
        'Label1
        '
        resources.ApplyResources(Me.Label1, "Label1")
        Me.Label1.Name = "Label1"
        '
        'Label3
        '
        resources.ApplyResources(Me.Label3, "Label3")
        Me.Label3.Name = "Label3"
        '
        'OutputDir
        '
        Me.OutputDir.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle
        resources.ApplyResources(Me.OutputDir, "OutputDir")
        Me.OutputDir.Name = "OutputDir"
        '
        'Button2
        '
        resources.ApplyResources(Me.Button2, "Button2")
        Me.Button2.Name = "Button2"
        Me.Button2.UseVisualStyleBackColor = True
        '
        'Label4
        '
        resources.ApplyResources(Me.Label4, "Label4")
        Me.Label4.Name = "Label4"
        '
        'OutputTemplate
        '
        Me.OutputTemplate.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList
        resources.ApplyResources(Me.OutputTemplate, "OutputTemplate")
        Me.OutputTemplate.FormattingEnabled = True
        Me.OutputTemplate.Items.AddRange(New Object() {resources.GetString("OutputTemplate.Items"), resources.GetString("OutputTemplate.Items1"), resources.GetString("OutputTemplate.Items2")})
        Me.OutputTemplate.Name = "OutputTemplate"
        '
        'SettingButton
        '
        resources.ApplyResources(Me.SettingButton, "SettingButton")
        Me.SettingButton.Name = "SettingButton"
        Me.SettingButton.UseVisualStyleBackColor = True
        '
        'OutputGroupBox
        '
        Me.OutputGroupBox.Controls.Add(Me.SettingButton)
        Me.OutputGroupBox.Controls.Add(Me.OutputTemplate)
        Me.OutputGroupBox.Controls.Add(Me.Label4)
        Me.OutputGroupBox.Controls.Add(Me.Button2)
        Me.OutputGroupBox.Controls.Add(Me.OutputDir)
        Me.OutputGroupBox.Controls.Add(Me.Label3)
        resources.ApplyResources(Me.OutputGroupBox, "OutputGroupBox")
        Me.OutputGroupBox.ForeColor = System.Drawing.SystemColors.ControlText
        Me.OutputGroupBox.Name = "OutputGroupBox"
        Me.OutputGroupBox.TabStop = False
        '
        'InfoLabel
        '
        resources.ApplyResources(Me.InfoLabel, "InfoLabel")
        Me.InfoLabel.Name = "InfoLabel"
        '
        'MainWindow
        '
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.None
        Me.BackColor = System.Drawing.SystemColors.Control
        resources.ApplyResources(Me, "$this")
        Me.Controls.Add(Me.InfoLabel)
        Me.Controls.Add(Me.OutputGroupBox)
        Me.Controls.Add(Me.InputGroupBox)
        Me.Controls.Add(Me.StartButton)
        Me.Name = "MainWindow"
        Me.InputGroupBox.ResumeLayout(False)
        Me.InputGroupBox.PerformLayout()
        Me.OutputGroupBox.ResumeLayout(False)
        Me.OutputGroupBox.PerformLayout()
        Me.ResumeLayout(False)
        Me.PerformLayout()

    End Sub

    Friend WithEvents StartButton As Button
    Friend WithEvents InputGroupBox As GroupBox
    Friend WithEvents FileOpenButton As Button
    Friend WithEvents Label1 As Label
    Friend WithEvents Label3 As Label
    Friend WithEvents OutputDir As TextBox
    Friend WithEvents Button2 As Button
    Friend WithEvents Label4 As Label
    Friend WithEvents SettingButton As Button
    Friend WithEvents OutputGroupBox As GroupBox
    Friend WithEvents FileListButton As Button
    Friend WithEvents InfoLabel As Label
    Friend WithEvents OutputTemplate As ComboBox
End Class
