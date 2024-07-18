Imports System.IO
Imports System.Reflection
Imports System.Threading

Delegate Sub changeUI(str As String)

Public Class MainWindow
    Dim config As New Config
    Dim checkUpdate As New CheckUpdate
    Dim fileList As New FileList

    Private Sub SettingButton_Click(sender As Object, e As EventArgs) Handles SettingButton.Click
        DanmakuSettingDlg.SendConfig(config)
        DanmakuSettingDlg.ShowDialog()
    End Sub

    Private Sub FileOpenButton_Click(sender As Object, e As EventArgs) Handles FileOpenButton.Click
        Dim dlg As New OpenFileDialog
        dlg.Filter = "弹幕文件|*.ass;*.xml;*.json|全部文件|*.*"
        dlg.Title = "选择一个或多个输入文件"
        dlg.Multiselect = True

        FileListButton.Enabled = False
        FileListButton.Text = "更新中..."
        InfoLabel.Text = "更新文件列表中"

        If dlg.ShowDialog() = DialogResult.Cancel Then
            MessageBox.Show("没有添加任何文件")
        Else
            fileList.AddFiles(dlg.FileNames)
        End If

        If fileList.GetFileNum() > 0 Then
            FileListButton.Enabled = True
            FileListButton.Text = "已打开(" + fileList.GetFileNum().ToString + ")"
        Else
            FileListButton.Enabled = False
            FileListButton.Text = "未打开任何文件"
        End If
        dlg.Dispose()
        InfoLabel.Text = "就绪"
    End Sub

    Private Sub MainWindow_Load(sender As Object, e As EventArgs) Handles MyBase.Load
        Me.Text = "DanmakuFactory " + Define.guiVersionText
        FileListButton.Enabled = False
        FileListButton.Text = "未打开任何文件"
        Try
            config.ReadConfig()
        Catch ex As Exception

        End Try

        OutputTemplate.Text = OutputTemplate.Items(0)

        If My.Computer.FileSystem.FileExists(Application.StartupPath + "\DanamkuFactoryGUIConfig.txt") Then
            MessageBox.Show("抱歉，因为拼写疏忽，旧配置文件DanamkuFactoryGUIConfig.txt现不可用，请更名为DanmakuFactoryGUIConfig.txt")
        End If

        '检查更新
        Dim updateThread As Thread
        updateThread = New Thread(AddressOf GetUpdate)
        updateThread.Start()
        updateThread.IsBackground = True
    End Sub

    Private Sub GetUpdate()
        If checkUpdate.CheckUpdate() = CheckUpdate.status_newVersion Then
            Me.Invoke(New changeUI(AddressOf ChangeINFOLabel), "发现新版本，点击更新")
        ElseIf checkUpdate.GetCheckStatus() = CheckUpdate.status_mustUpdate Then
            '紧急更新
            UpdateDlg.SendUpdateStr(checkUpdate)
            UpdateDlg.ShowDialog()
        End If
    End Sub

    '文件管理窗口
    Private Sub FileListButton_Click(sender As Object, e As EventArgs) Handles FileListButton.Click
        Dim FileDlg As New FilesDialog
        FileDlg.SendFileList(fileList)
        FileDlg.ShowDialog()
        If fileList.GetFileNum() > 0 Then
            FileListButton.Enabled = True
            FileListButton.Text = "已打开(" + fileList.GetFileNum().ToString + ")"
        Else
            FileListButton.Enabled = False
            FileListButton.Text = "未打开任何文件"
        End If
    End Sub

    Private Sub Button2_Click(sender As Object, e As EventArgs) Handles Button2.Click
        Dim dlg As New FolderBrowserDialog
        If dlg.ShowDialog() = DialogResult.OK Then
            OutputDir.Text = dlg.SelectedPath
        End If
        dlg.Dispose()
    End Sub

    Sub ChangeINFOLabel(str As String)
        InfoLabel.Text = str
    End Sub

    Private Sub InfoLabel_Click(sender As Object, e As EventArgs) Handles InfoLabel.Click
        UpdateDlg.SendUpdateStr(checkUpdate)
        UpdateDlg.ShowDialog()
    End Sub

    Private Sub StartButton_Click(sender As Object, e As EventArgs) Handles StartButton.Click
        '检查是否有紧急更新
        If checkUpdate.GetCheckStatus() = CheckUpdate.status_mustUpdate Then
            UpdateDlg.SendUpdateStr(checkUpdate)
            UpdateDlg.ShowDialog()
            Exit Sub
        End If

        '检查程序是否存在
        If Not My.Computer.FileSystem.FileExists(Application.StartupPath + "\" + Define.cliProgramFileName) Then
            MessageBox.Show("找不到依赖程序 " + Define.cliProgramFileName)
            Exit Sub
        End If

        '检查设定合法性
        If fileList.GetFileNum = 0 Then
            MessageBox.Show("当前未打开任何文件", "错误 - DanmakuFactory")
            Exit Sub
        End If
        If OutputTemplate.Text Is Nothing Then
            MessageBox.Show("输出模板不能为空", "错误 - DanmakuFactory")
            Exit Sub
        End If
        If (Not OutputDir.Text.Equals("")) AndAlso Not My.Computer.FileSystem.DirectoryExists(OutputDir.Text) Then
            MessageBox.Show("输出目录 " + OutputDir.Text + " 不存在", "错误 - DanmakuFactory")
            Exit Sub
        End If

        '调出处理窗口
        Dim dlg As New ProcessingDlg
        dlg.SendData(config, fileList, OutputDir.Text, OutputTemplate.Text)
        dlg.ShowDialog()
        dlg.Dispose()

        '更新UI
        If fileList.GetFileNum() > 0 Then
            FileListButton.Enabled = True
            FileListButton.Text = "已打开(" + fileList.GetFileNum().ToString + ")"
        Else
            FileListButton.Enabled = False
            FileListButton.Text = "未打开任何文件"
        End If
    End Sub

    Private Sub OutputTemplate_SelectedIndexChanged(sender As Object, e As EventArgs) Handles OutputTemplate.SelectedIndexChanged
        If OutputTemplate.Text = OutputTemplate.Items(0).ToString Then
            SettingButton.Enabled = True
        Else
            SettingButton.Enabled = False
        End If
    End Sub
End Class
