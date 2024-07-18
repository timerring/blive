<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()>
Partial Class MessageSettingDlg
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
        Dim resources As System.ComponentModel.ComponentResourceManager = New System.ComponentModel.ComponentResourceManager(GetType(MessageSettingDlg))
        Me.PictureBox1 = New System.Windows.Forms.PictureBox()
        Me.Label15 = New System.Windows.Forms.Label()
        Me.PosYNUD = New System.Windows.Forms.NumericUpDown()
        Me.PosXNUD = New System.Windows.Forms.NumericUpDown()
        Me.Label14 = New System.Windows.Forms.Label()
        Me.Label1 = New System.Windows.Forms.Label()
        Me.Label2 = New System.Windows.Forms.Label()
        Me.Label3 = New System.Windows.Forms.Label()
        Me.Label4 = New System.Windows.Forms.Label()
        Me.Label5 = New System.Windows.Forms.Label()
        Me.HeightNUD = New System.Windows.Forms.NumericUpDown()
        Me.WidthNUD = New System.Windows.Forms.NumericUpDown()
        Me.FontSizeLabel = New System.Windows.Forms.Label()
        Me.FontSizeNUD = New System.Windows.Forms.NumericUpDown()
        Me.DurationLabel = New System.Windows.Forms.Label()
        Me.DurationNUD = New System.Windows.Forms.NumericUpDown()
        Me.DurationUnitLabel = New System.Windows.Forms.Label()
        Me.DurationInfoLabel = New System.Windows.Forms.Label()
        Me.GiftMinPriceLabel = New System.Windows.Forms.Label()
        Me.GiftMinPriceNUD = New System.Windows.Forms.NumericUpDown()
        Me.GiftMinPriceUnitLabel = New System.Windows.Forms.Label()
        Me.GiftMergeToleranceLabel = New System.Windows.Forms.Label()
        Me.GiftMergeToleranceNUD = New System.Windows.Forms.NumericUpDown()
        Me.GiftMergeToleranceUnitLabel = New System.Windows.Forms.Label()
        Me.OkButton = New System.Windows.Forms.Button()
        CType(Me.PictureBox1, System.ComponentModel.ISupportInitialize).BeginInit()
        CType(Me.PosYNUD, System.ComponentModel.ISupportInitialize).BeginInit()
        CType(Me.PosXNUD, System.ComponentModel.ISupportInitialize).BeginInit()
        CType(Me.HeightNUD, System.ComponentModel.ISupportInitialize).BeginInit()
        CType(Me.WidthNUD, System.ComponentModel.ISupportInitialize).BeginInit()
        CType(Me.FontSizeNUD, System.ComponentModel.ISupportInitialize).BeginInit()
        CType(Me.DurationNUD, System.ComponentModel.ISupportInitialize).BeginInit()
        CType(Me.GiftMinPriceNUD, System.ComponentModel.ISupportInitialize).BeginInit()
        CType(Me.GiftMergeToleranceNUD, System.ComponentModel.ISupportInitialize).BeginInit()
        Me.SuspendLayout()
        '
        'PictureBox1
        '
        Me.PictureBox1.BackgroundImage = Global.DanmakuFactory.My.Resources.Resources.messageSetting
        resources.ApplyResources(Me.PictureBox1, "PictureBox1")
        Me.PictureBox1.Name = "PictureBox1"
        Me.PictureBox1.TabStop = False
        '
        'Label15
        '
        resources.ApplyResources(Me.Label15, "Label15")
        Me.Label15.Name = "Label15"
        '
        'PosYNUD
        '
        Me.PosYNUD.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle
        resources.ApplyResources(Me.PosYNUD, "PosYNUD")
        Me.PosYNUD.Maximum = New Decimal(New Integer() {2139062143, 0, 0, 0})
        Me.PosYNUD.Minimum = New Decimal(New Integer() {2139062143, 0, 0, -2147483648})
        Me.PosYNUD.Name = "PosYNUD"
        Me.PosYNUD.Value = New Decimal(New Integer() {1, 0, 0, 0})
        '
        'PosXNUD
        '
        Me.PosXNUD.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle
        resources.ApplyResources(Me.PosXNUD, "PosXNUD")
        Me.PosXNUD.Maximum = New Decimal(New Integer() {2139062143, 0, 0, 0})
        Me.PosXNUD.Minimum = New Decimal(New Integer() {2139062143, 0, 0, -2147483648})
        Me.PosXNUD.Name = "PosXNUD"
        Me.PosXNUD.Value = New Decimal(New Integer() {1, 0, 0, 0})
        '
        'Label14
        '
        resources.ApplyResources(Me.Label14, "Label14")
        Me.Label14.Name = "Label14"
        '
        'Label1
        '
        resources.ApplyResources(Me.Label1, "Label1")
        Me.Label1.Name = "Label1"
        '
        'Label2
        '
        resources.ApplyResources(Me.Label2, "Label2")
        Me.Label2.Name = "Label2"
        '
        'Label3
        '
        resources.ApplyResources(Me.Label3, "Label3")
        Me.Label3.Name = "Label3"
        '
        'Label4
        '
        resources.ApplyResources(Me.Label4, "Label4")
        Me.Label4.Name = "Label4"
        '
        'Label5
        '
        resources.ApplyResources(Me.Label5, "Label5")
        Me.Label5.Name = "Label5"
        '
        'HeightNUD
        '
        Me.HeightNUD.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle
        resources.ApplyResources(Me.HeightNUD, "HeightNUD")
        Me.HeightNUD.Maximum = New Decimal(New Integer() {2139062143, 0, 0, 0})
        Me.HeightNUD.Minimum = New Decimal(New Integer() {1, 0, 0, 0})
        Me.HeightNUD.Name = "HeightNUD"
        Me.HeightNUD.Value = New Decimal(New Integer() {1, 0, 0, 0})
        '
        'WidthNUD
        '
        Me.WidthNUD.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle
        resources.ApplyResources(Me.WidthNUD, "WidthNUD")
        Me.WidthNUD.Maximum = New Decimal(New Integer() {2139062143, 0, 0, 0})
        Me.WidthNUD.Minimum = New Decimal(New Integer() {1, 0, 0, 0})
        Me.WidthNUD.Name = "WidthNUD"
        Me.WidthNUD.Value = New Decimal(New Integer() {1, 0, 0, 0})
        '
        'FontSizeLabel
        '
        resources.ApplyResources(Me.FontSizeLabel, "FontSizeLabel")
        Me.FontSizeLabel.Name = "FontSizeLabel"
        '
        'FontSizeNUD
        '
        Me.FontSizeNUD.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle
        resources.ApplyResources(Me.FontSizeNUD, "FontSizeNUD")
        Me.FontSizeNUD.Maximum = New Decimal(New Integer() {2139062143, 0, 0, 0})
        Me.FontSizeNUD.Minimum = New Decimal(New Integer() {1, 0, 0, 0})
        Me.FontSizeNUD.Name = "FontSizeNUD"
        Me.FontSizeNUD.Value = New Decimal(New Integer() {1, 0, 0, 0})
        '
        'DurationLabel
        '
        resources.ApplyResources(Me.DurationLabel, "DurationLabel")
        Me.DurationLabel.Name = "DurationLabel"
        '
        'DurationNUD
        '
        Me.DurationNUD.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle
        Me.DurationNUD.DecimalPlaces = 2
        resources.ApplyResources(Me.DurationNUD, "DurationNUD")
        Me.DurationNUD.Maximum = New Decimal(New Integer() {2139062143, 0, 0, 0})
        Me.DurationNUD.Name = "DurationNUD"
        '
        'DurationUnitLabel
        '
        resources.ApplyResources(Me.DurationUnitLabel, "DurationUnitLabel")
        Me.DurationUnitLabel.Name = "DurationUnitLabel"
        '
        'DurationInfoLabel
        '
        Me.DurationInfoLabel.AutoEllipsis = True
        resources.ApplyResources(Me.DurationInfoLabel, "DurationInfoLabel")
        Me.DurationInfoLabel.Name = "DurationInfoLabel"
        '
        'GiftMinPriceLabel
        '
        resources.ApplyResources(Me.GiftMinPriceLabel, "GiftMinPriceLabel")
        Me.GiftMinPriceLabel.Name = "GiftMinPriceLabel"
        '
        'GiftMinPriceNUD
        '
        Me.GiftMinPriceNUD.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle
        Me.GiftMinPriceNUD.DecimalPlaces = 2
        resources.ApplyResources(Me.GiftMinPriceNUD, "GiftMinPriceNUD")
        Me.GiftMinPriceNUD.Maximum = New Decimal(New Integer() {2139062143, 0, 0, 0})
        Me.GiftMinPriceNUD.Name = "GiftMinPriceNUD"
        '
        'GiftMinPriceUnitLabel
        '
        resources.ApplyResources(Me.GiftMinPriceUnitLabel, "GiftMinPriceUnitLabel")
        Me.GiftMinPriceUnitLabel.Name = "GiftMinPriceUnitLabel"
        '
        'GiftMergeToleranceLabel
        '
        resources.ApplyResources(Me.GiftMergeToleranceLabel, "GiftMergeToleranceLabel")
        Me.GiftMergeToleranceLabel.Name = "GiftMergeToleranceLabel"
        '
        'GiftMergeToleranceNUD
        '
        Me.GiftMergeToleranceNUD.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle
        Me.GiftMergeToleranceNUD.DecimalPlaces = 2
        resources.ApplyResources(Me.GiftMergeToleranceNUD, "GiftMergeToleranceNUD")
        Me.GiftMergeToleranceNUD.Maximum = New Decimal(New Integer() {2139062143, 0, 0, 0})
        Me.GiftMergeToleranceNUD.Name = "GiftMergeToleranceNUD"
        '
        'GiftMergeToleranceUnitLabel
        '
        resources.ApplyResources(Me.GiftMergeToleranceUnitLabel, "GiftMergeToleranceUnitLabel")
        Me.GiftMergeToleranceUnitLabel.Name = "GiftMergeToleranceUnitLabel"
        '
        'OkButton
        '
        resources.ApplyResources(Me.OkButton, "OkButton")
        Me.OkButton.Name = "OkButton"
        Me.OkButton.UseVisualStyleBackColor = True
        '
        'MessageSettingDlg
        '
        resources.ApplyResources(Me, "$this")
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Dpi
        Me.Controls.Add(Me.OkButton)
        Me.Controls.Add(Me.GiftMergeToleranceUnitLabel)
        Me.Controls.Add(Me.GiftMergeToleranceNUD)
        Me.Controls.Add(Me.GiftMergeToleranceLabel)
        Me.Controls.Add(Me.GiftMinPriceUnitLabel)
        Me.Controls.Add(Me.GiftMinPriceNUD)
        Me.Controls.Add(Me.GiftMinPriceLabel)
        Me.Controls.Add(Me.DurationInfoLabel)
        Me.Controls.Add(Me.DurationUnitLabel)
        Me.Controls.Add(Me.DurationNUD)
        Me.Controls.Add(Me.DurationLabel)
        Me.Controls.Add(Me.FontSizeNUD)
        Me.Controls.Add(Me.FontSizeLabel)
        Me.Controls.Add(Me.Label4)
        Me.Controls.Add(Me.Label5)
        Me.Controls.Add(Me.HeightNUD)
        Me.Controls.Add(Me.WidthNUD)
        Me.Controls.Add(Me.Label3)
        Me.Controls.Add(Me.Label2)
        Me.Controls.Add(Me.Label1)
        Me.Controls.Add(Me.Label15)
        Me.Controls.Add(Me.PosYNUD)
        Me.Controls.Add(Me.PosXNUD)
        Me.Controls.Add(Me.Label14)
        Me.Controls.Add(Me.PictureBox1)
        Me.Cursor = System.Windows.Forms.Cursors.Arrow
        Me.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog
        Me.Name = "MessageSettingDlg"
        Me.ShowIcon = False
        CType(Me.PictureBox1, System.ComponentModel.ISupportInitialize).EndInit()
        CType(Me.PosYNUD, System.ComponentModel.ISupportInitialize).EndInit()
        CType(Me.PosXNUD, System.ComponentModel.ISupportInitialize).EndInit()
        CType(Me.HeightNUD, System.ComponentModel.ISupportInitialize).EndInit()
        CType(Me.WidthNUD, System.ComponentModel.ISupportInitialize).EndInit()
        CType(Me.FontSizeNUD, System.ComponentModel.ISupportInitialize).EndInit()
        CType(Me.DurationNUD, System.ComponentModel.ISupportInitialize).EndInit()
        CType(Me.GiftMinPriceNUD, System.ComponentModel.ISupportInitialize).EndInit()
        CType(Me.GiftMergeToleranceNUD, System.ComponentModel.ISupportInitialize).EndInit()
        Me.ResumeLayout(False)
        Me.PerformLayout()

    End Sub

    Friend WithEvents PictureBox1 As PictureBox
    Friend WithEvents Label15 As Label
    Friend WithEvents PosYNUD As NumericUpDown
    Friend WithEvents PosXNUD As NumericUpDown
    Friend WithEvents Label14 As Label
    Friend WithEvents Label1 As Label
    Friend WithEvents Label2 As Label
    Friend WithEvents Label3 As Label
    Friend WithEvents Label4 As Label
    Friend WithEvents Label5 As Label
    Friend WithEvents HeightNUD As NumericUpDown
    Friend WithEvents WidthNUD As NumericUpDown
    Friend WithEvents FontSizeLabel As Label
    Friend WithEvents FontSizeNUD As NumericUpDown
    Friend WithEvents DurationLabel As Label
    Friend WithEvents DurationNUD As NumericUpDown
    Friend WithEvents DurationUnitLabel As Label
    Friend WithEvents DurationInfoLabel As Label
    Friend WithEvents GiftMinPriceLabel As Label
    Friend WithEvents GiftMinPriceNUD As NumericUpDown
    Friend WithEvents GiftMinPriceUnitLabel As Label
    Friend WithEvents GiftMergeToleranceLabel As Label
    Friend WithEvents GiftMergeToleranceNUD As NumericUpDown
    Friend WithEvents GiftMergeToleranceUnitLabel As Label
    Friend WithEvents OkButton As Button
End Class
