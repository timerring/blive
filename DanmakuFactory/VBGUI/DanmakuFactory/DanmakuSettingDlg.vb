Imports System.Windows.Forms

Public Class DanmakuSettingDlg
    Private config As Config
    Public Sub SendConfig(config As Config)
        Me.config = config
    End Sub
    Private Sub DisplayAreaNUD_ValueChanged(sender As Object, e As EventArgs) Handles DisplayAreaNUD.ValueChanged
        ScrollAreaNUD.Maximum = DisplayAreaNUD.Value
    End Sub

    Private Sub CancelButton_Click(sender As Object, e As EventArgs) Handles CancelButton.Click
        Me.DialogResult = System.Windows.Forms.DialogResult.Cancel
        Me.Close()
    End Sub

    '保存按钮
    Private Sub OkButton_Click(sender As Object, e As EventArgs) Handles OkButton.Click
        '数值部分
        config.fontsize = FontSizeNUD.Value
        config.shadow = ShadowNUD.Value
        config.outline = OutlineNUD.Value
        config.opacity = OpacityNUD.Value * 255.0 / 100
        config.timeShift = TimeshiftUpDown.Value
        config.scrolltime = ScrollTimeNUD.Value
        config.fixtime = FixTimeNUD.Value
        config.scrollarea = ScrollAreaNUD.Value / 100.0
        config.displayarea = DisplayAreaNUD.Value / 100.0
        config.resolution(0) = ResXNUD.Value
        config.resolution(1) = ResYNUD.Value

        '复选框部分
        config.bold = BoldCheckBox.Checked

        '密度部分
        If DensityRadioButton1.Checked Then
            config.density = 0
        ElseIf DensityRadioButton2.Checked Then
            config.density = -1
        Else
            config.density = DensityNUD.Value
        End If

        '按类型屏蔽部分
        config.blockR2L = BLK_R2L_CheckBox.Checked
        config.blockL2R = BLK_L2R_CheckBox.Checked
        config.blockTop = BLK_Top_CheckBox.Checked
        config.blockBtm = BLK_Btm_CheckBox.Checked
        config.blockColor = BLK_Color_CheckBox.Checked
        config.blockSpecial = BLK_Special_CheckBox.Checked
        config.blockRepeat = BLK_Rep_CheckBox.Checked

        '调试部分
        config.statHistogram = StatHistogramCheckBox.Checked
        config.statTable = StatTableCheckBox.Checked

        '其他设置部分
        config.showUsernames = ShowUsernamesCheckBox.Checked
        config.showMsgbox = ShowMsgboxCheckBox.Checked

        '保存到文件
        If SaveCheckBox.Checked Then
            config.WriteConfig()
        End If

        Me.DialogResult = System.Windows.Forms.DialogResult.OK
        Me.Close()
    End Sub

    Private Sub DensityRadioButton3_CheckedChanged(sender As Object, e As EventArgs) Handles DensityRadioButton3.CheckedChanged
        If (DensityRadioButton3.Checked = True) Then
            DensityNUD.Enabled = True
        Else
            DensityNUD.Enabled = False
        End If
    End Sub

    Private Sub DanmakuSettingDlg_Load(sender As Object, e As EventArgs) Handles MyBase.Load
        '合法性检验
        config.Check()

        '数值部分
        FontSizeNUD.Value = config.fontsize
        ShadowNUD.Value = config.shadow
        OutlineNUD.Value = config.outline
        OpacityNUD.Value = config.opacity / 255.0 * 100
        ScrollTimeNUD.Value = config.scrolltime
        FixTimeNUD.Value = config.fixtime
        ScrollAreaNUD.Value = config.scrollarea * 100.0
        DisplayAreaNUD.Value = config.displayarea * 100.0
        ResXNUD.Value = config.resolution(0)
        ResYNUD.Value = config.resolution(1)

        '复选框部分
        BoldCheckBox.Checked = config.bold

        '密度部分
        If config.density = 0 Then
            DensityRadioButton1.Select()
            DensityNUD.Enabled = False
        ElseIf config.density = -1 Then
            DensityRadioButton2.Select()
            DensityNUD.Enabled = False
        Else
            DensityRadioButton3.Select()
            DensityNUD.Enabled = True
            DensityNUD.Value = config.density
        End If

        '按类型屏蔽部分
        BLK_R2L_CheckBox.Checked = config.blockR2L
        BLK_L2R_CheckBox.Checked = config.blockL2R
        BLK_Top_CheckBox.Checked = config.blockTop
        BLK_Btm_CheckBox.Checked = config.blockBtm
        BLK_Color_CheckBox.Checked = config.blockColor
        BLK_Special_CheckBox.Checked = config.blockSpecial
        BLK_Rep_CheckBox.Checked = config.blockRepeat

        '调试部分
        StatHistogramCheckBox.Checked = config.statHistogram
        StatTableCheckBox.Checked = config.statTable

        '其他设置部分
        ShowUsernamesCheckBox.Checked = config.showUsernames
        ShowMsgboxCheckBox.Checked = config.showMsgbox
    End Sub

    Private Sub Button1_Click(sender As Object, e As EventArgs) Handles Button1.Click
        MessageSettingDlg.SendConfig(config)
        MessageSettingDlg.ShowDialog()
    End Sub

    Private Sub SaveCheckBox_CheckedChanged(sender As Object, e As EventArgs) Handles SaveCheckBox.CheckedChanged

    End Sub
End Class
