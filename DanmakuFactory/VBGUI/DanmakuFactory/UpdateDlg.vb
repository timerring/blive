Imports System.Windows.Forms
Imports System.Threading

Public Class UpdateDlg
    Dim checkUpdate As CheckUpdate
    Public Sub SendUpdateStr(obj As CheckUpdate)
        checkUpdate = obj
    End Sub
    Private Sub OK_Button_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles OK_Button.Click
        System.Diagnostics.Process.Start(checkUpdate.GetUpdateUrl())
        Me.DialogResult = System.Windows.Forms.DialogResult.OK
        Me.Close()
    End Sub

    Private Sub Cancel_Button_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Cancel_Button.Click
        Me.DialogResult = System.Windows.Forms.DialogResult.Cancel
        Me.Close()
    End Sub

    Private Sub UpdateUIInfo()
        If checkUpdate.GetCheckStatus() = CheckUpdate.status_mustUpdate Then
            MessageBox.Show(checkUpdate.GetWarningMsg)
        End If
        OK_Button.Enabled = False
        Select Case checkUpdate.GetCheckStatus
            Case CheckUpdate.status_isLastest
                VersionLabel.Text = "DanmakuFactory"
                UpdateInfoLabel.Text = "当前为最新版本"
            Case CheckUpdate.status_internetErr
                VersionLabel.Text = "DanmakuFactory"
                UpdateInfoLabel.Text = "网络错误，请检查网络连接并重试"
            Case CheckUpdate.status_newVersion
                VersionLabel.Text = "NEW:DanmakuFactory V" + checkUpdate.GetNewVersion
                UpdateInfoLabel.Text = checkUpdate.GetUpdateMsg
                OK_Button.Enabled = True
            Case CheckUpdate.status_mustUpdate
                VersionLabel.Text = "紧急:DanmakuFactory V" + checkUpdate.GetNewVersion
                UpdateInfoLabel.Text = checkUpdate.GetUpdateMsg
                OK_Button.Enabled = True
            Case CheckUpdate.status_notChecked
                VersionLabel.Text = "DanmakuFactory"
                UpdateInfoLabel.Text = "检查未成功，请重试"
            Case CheckUpdate.status_parsingErr
                VersionLabel.Text = "DanmakuFactory"
                UpdateInfoLabel.Text = "返回信息解析错误，请反馈"
        End Select
    End Sub

    Private Sub UpdateDlg_Load(sender As Object, e As EventArgs) Handles MyBase.Load
        copyrightInfo.Text = "作者 HKM" + vbCrLf +
                             "版本 " + guiVersionText + vbCrLf +
                             "本软件仅在Github与吾爱论坛发布，其他网站均非本人发布，无法保证程序安全性" + vbCrLf +
                             "本程序开源并永久免费，请勿用于商业用途" + vbCrLf +
                             "https://github.com/hihkm/DanmakuFactory"
        UpdateUIInfo()
    End Sub

    Private Sub ReCheck_Botton_Click(sender As Object, e As EventArgs) Handles ReCheck_Botton.Click
        ReCheck_Botton.Enabled = False
        OK_Button.Enabled = False
        Cancel_Button.Enabled = False
        UpdateInfoLabel.Text = "检查更新中..."
        '重新检查更新
        Dim updateThread As Thread
        updateThread = New Thread(AddressOf checkUpdate.CheckUpdate)
        updateThread.Start()
        updateThread.IsBackground = True
        UpdateUIInfo()
        ReCheck_Botton.Enabled = True
        Cancel_Button.Enabled = True
    End Sub
End Class
