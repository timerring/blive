<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()>
Partial Class ProcessingDlg
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
        Me.UtilButton = New System.Windows.Forms.Button()
        Me.ViewErrMsgButton = New System.Windows.Forms.Button()
        Me.ProgressBar1 = New System.Windows.Forms.ProgressBar()
        Me.StatusLabel = New System.Windows.Forms.Label()
        Me.LoadingLabel = New System.Windows.Forms.Label()
        Me.DoneLabel = New System.Windows.Forms.Label()
        Me.TotalLabel = New System.Windows.Forms.Label()
        Me.FailedLabel = New System.Windows.Forms.Label()
        Me.ClearFileListCheckBox = New System.Windows.Forms.CheckBox()
        Me.SuspendLayout()
        '
        'UtilButton
        '
        Me.UtilButton.FlatStyle = System.Windows.Forms.FlatStyle.Flat
        Me.UtilButton.Location = New System.Drawing.Point(400, 276)
        Me.UtilButton.Margin = New System.Windows.Forms.Padding(4)
        Me.UtilButton.Name = "UtilButton"
        Me.UtilButton.Size = New System.Drawing.Size(94, 30)
        Me.UtilButton.TabIndex = 0
        Me.UtilButton.Text = "确定"
        '
        'ViewErrMsgButton
        '
        Me.ViewErrMsgButton.FlatStyle = System.Windows.Forms.FlatStyle.Flat
        Me.ViewErrMsgButton.Location = New System.Drawing.Point(284, 276)
        Me.ViewErrMsgButton.Margin = New System.Windows.Forms.Padding(4)
        Me.ViewErrMsgButton.Name = "ViewErrMsgButton"
        Me.ViewErrMsgButton.Size = New System.Drawing.Size(94, 30)
        Me.ViewErrMsgButton.TabIndex = 1
        Me.ViewErrMsgButton.Text = "查看报错"
        Me.ViewErrMsgButton.UseVisualStyleBackColor = True
        '
        'ProgressBar1
        '
        Me.ProgressBar1.Location = New System.Drawing.Point(35, 232)
        Me.ProgressBar1.Margin = New System.Windows.Forms.Padding(4)
        Me.ProgressBar1.Name = "ProgressBar1"
        Me.ProgressBar1.Size = New System.Drawing.Size(459, 26)
        Me.ProgressBar1.Step = 1
        Me.ProgressBar1.Style = System.Windows.Forms.ProgressBarStyle.Continuous
        Me.ProgressBar1.TabIndex = 1
        '
        'StatusLabel
        '
        Me.StatusLabel.AutoSize = True
        Me.StatusLabel.Font = New System.Drawing.Font("微软雅黑 Light", 18.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(134, Byte))
        Me.StatusLabel.Location = New System.Drawing.Point(28, 24)
        Me.StatusLabel.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.StatusLabel.MinimumSize = New System.Drawing.Size(0, 20)
        Me.StatusLabel.Name = "StatusLabel"
        Me.StatusLabel.Size = New System.Drawing.Size(107, 39)
        Me.StatusLabel.TabIndex = 2
        Me.StatusLabel.Text = "未开始"
        '
        'LoadingLabel
        '
        Me.LoadingLabel.AutoSize = True
        Me.LoadingLabel.Font = New System.Drawing.Font("微软雅黑", 9.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(134, Byte))
        Me.LoadingLabel.Location = New System.Drawing.Point(31, 167)
        Me.LoadingLabel.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.LoadingLabel.MaximumSize = New System.Drawing.Size(460, 0)
        Me.LoadingLabel.MinimumSize = New System.Drawing.Size(0, 10)
        Me.LoadingLabel.Name = "LoadingLabel"
        Me.LoadingLabel.Size = New System.Drawing.Size(147, 20)
        Me.LoadingLabel.TabIndex = 3
        Me.LoadingLabel.Text = "正在转换 FILENAME"
        '
        'DoneLabel
        '
        Me.DoneLabel.AutoSize = True
        Me.DoneLabel.Font = New System.Drawing.Font("微软雅黑", 9.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(134, Byte))
        Me.DoneLabel.Location = New System.Drawing.Point(31, 73)
        Me.DoneLabel.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.DoneLabel.MinimumSize = New System.Drawing.Size(0, 10)
        Me.DoneLabel.Name = "DoneLabel"
        Me.DoneLabel.Size = New System.Drawing.Size(67, 20)
        Me.DoneLabel.TabIndex = 4
        Me.DoneLabel.Text = "已完成 0"
        '
        'TotalLabel
        '
        Me.TotalLabel.AutoSize = True
        Me.TotalLabel.Font = New System.Drawing.Font("微软雅黑", 9.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(134, Byte))
        Me.TotalLabel.Location = New System.Drawing.Point(31, 105)
        Me.TotalLabel.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.TotalLabel.MinimumSize = New System.Drawing.Size(0, 10)
        Me.TotalLabel.Name = "TotalLabel"
        Me.TotalLabel.Size = New System.Drawing.Size(52, 20)
        Me.TotalLabel.TabIndex = 5
        Me.TotalLabel.Text = "总计 0"
        '
        'FailedLabel
        '
        Me.FailedLabel.AutoSize = True
        Me.FailedLabel.Font = New System.Drawing.Font("微软雅黑", 9.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(134, Byte))
        Me.FailedLabel.Location = New System.Drawing.Point(31, 137)
        Me.FailedLabel.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.FailedLabel.MinimumSize = New System.Drawing.Size(0, 10)
        Me.FailedLabel.Name = "FailedLabel"
        Me.FailedLabel.Size = New System.Drawing.Size(52, 20)
        Me.FailedLabel.TabIndex = 6
        Me.FailedLabel.Text = "失败 0"
        '
        'ClearFileListCheckBox
        '
        Me.ClearFileListCheckBox.AutoSize = True
        Me.ClearFileListCheckBox.Checked = True
        Me.ClearFileListCheckBox.CheckState = System.Windows.Forms.CheckState.Checked
        Me.ClearFileListCheckBox.FlatStyle = System.Windows.Forms.FlatStyle.Flat
        Me.ClearFileListCheckBox.Font = New System.Drawing.Font("微软雅黑", 10.8!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(134, Byte))
        Me.ClearFileListCheckBox.Location = New System.Drawing.Point(35, 276)
        Me.ClearFileListCheckBox.Margin = New System.Windows.Forms.Padding(4)
        Me.ClearFileListCheckBox.MinimumSize = New System.Drawing.Size(0, 12)
        Me.ClearFileListCheckBox.Name = "ClearFileListCheckBox"
        Me.ClearFileListCheckBox.Size = New System.Drawing.Size(201, 29)
        Me.ClearFileListCheckBox.TabIndex = 7
        Me.ClearFileListCheckBox.Text = "结束后清空文件列表" & Global.Microsoft.VisualBasic.ChrW(13) & Global.Microsoft.VisualBasic.ChrW(10)
        Me.ClearFileListCheckBox.UseVisualStyleBackColor = True
        '
        'ProcessingDlg
        '
        Me.AcceptButton = Me.UtilButton
        Me.AutoScaleDimensions = New System.Drawing.SizeF(8.0!, 15.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.AutoSize = True
        Me.ClientSize = New System.Drawing.Size(529, 362)
        Me.ControlBox = False
        Me.Controls.Add(Me.UtilButton)
        Me.Controls.Add(Me.DoneLabel)
        Me.Controls.Add(Me.ViewErrMsgButton)
        Me.Controls.Add(Me.ClearFileListCheckBox)
        Me.Controls.Add(Me.ProgressBar1)
        Me.Controls.Add(Me.StatusLabel)
        Me.Controls.Add(Me.LoadingLabel)
        Me.Controls.Add(Me.TotalLabel)
        Me.Controls.Add(Me.FailedLabel)
        Me.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle
        Me.Margin = New System.Windows.Forms.Padding(4)
        Me.MaximizeBox = False
        Me.MinimizeBox = False
        Me.Name = "ProcessingDlg"
        Me.ShowInTaskbar = False
        Me.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent
        Me.Text = "转换执行 - DanmakuFactory"
        Me.ResumeLayout(False)
        Me.PerformLayout()

    End Sub
    Friend WithEvents UtilButton As Button
    Friend WithEvents ViewErrMsgButton As Button
    Friend WithEvents ProgressBar1 As ProgressBar
    Friend WithEvents StatusLabel As Label
    Friend WithEvents LoadingLabel As Label
    Friend WithEvents DoneLabel As Label
    Friend WithEvents TotalLabel As Label
    Friend WithEvents FailedLabel As Label
    Friend WithEvents ClearFileListCheckBox As CheckBox
End Class
