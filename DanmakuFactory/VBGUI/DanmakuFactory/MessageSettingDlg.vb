Public Class MessageSettingDlg
    Private config As Config
    Public Sub SendConfig(config As Config)
        Me.config = config
    End Sub
    Private Sub MessageSettingDlg_Load(sender As Object, e As EventArgs) Handles MyBase.Load
        HeightNUD.Minimum = config.msgboxFontsize
        WidthNUD.Minimum = config.msgboxFontsize

        WidthNUD.Value = config.msgboxSize(0)
        HeightNUD.Value = config.msgboxSize(1)
        PosXNUD.Value = config.msgboxPos(0)
        PosYNUD.Value = config.msgboxPos(1)
        FontSizeNUD.Value = config.msgboxFontsize
        DurationNUD.Value = config.msgboxDuration
        GiftMinPriceNUD.Value = config.giftMinPrice
        GiftMergeToleranceNUD.Value = config.giftMergeTolerance
    End Sub

    Private Sub FontSizeNUD_ValueChanged(sender As Object, e As EventArgs) Handles FontSizeNUD.ValueChanged
        HeightNUD.Minimum = FontSizeNUD.Value
        WidthNUD.Minimum = FontSizeNUD.Value
    End Sub

    Private Sub OkButton_Click(sender As Object, e As EventArgs) Handles OkButton.Click
        '配置项赋值
        config.msgboxSize(0) = WidthNUD.Value
        config.msgboxSize(1) = HeightNUD.Value
        config.msgboxPos(0) = PosXNUD.Value
        config.msgboxPos(1) = PosYNUD.Value
        config.msgboxFontsize = FontSizeNUD.Value
        config.msgboxDuration = DurationNUD.Value
        config.giftMinPrice = GiftMinPriceNUD.Value
        config.giftMergeTolerance = GiftMergeToleranceNUD.Value

        '返回
        Me.Close()
    End Sub
End Class