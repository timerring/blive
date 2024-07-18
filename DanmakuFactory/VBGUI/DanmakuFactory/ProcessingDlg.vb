Imports System.IO
Imports System.Windows.Forms
Imports System.Windows.Forms.DataVisualization.Charting
Imports System.Threading

Public Delegate Sub UpdateProcUI(statusLabelStr As String, doneLabelStr As String,
                                 totalLabelStr As String, failedLabelStr As String,
                                 progressBar1Int As Integer, loadingLabelStr As String,
                                 utilBottonStr As String)

Public Class ProcessingDlg
    Private config As Config
    Private fileList As FileList
    Private outputPath As String
    Private outputTemplate As String
    Private errorMsg As String = ""

    Private totalNum As Integer
    Private failedNum As Integer
    Private doneNum As Integer

    Private stopFlag As Boolean = False
    Public Sub SendData(con As Config, fl As FileList, opPath As String, opTemp As String)
        config = con
        fileList = fl
        outputPath = opPath
        outputTemplate = opTemp
    End Sub

    Private Sub ProcessingDlg_Load(sender As Object, e As EventArgs) Handles MyBase.Load
        UtilButton.Text = "终止"

        Dim procThread As Thread
        procThread = New Thread(AddressOf StartProcess)
        procThread.Start()
        procThread.IsBackground = True
    End Sub

    Private Sub StartProcess()
        stopFlag = False
        totalNum = fileList.GetFileNum()

        For cnt As Integer = 0 To fileList.GetFileNum() - 1
            Me.Invoke(New UpdateProcUI(AddressOf UpdateUI),
                      (CInt(cnt / totalNum * 100)).ToString() + "%",'状态大标题
                      "已完成 " + doneNum.ToString,'已完成标签
                      "总计 " + totalNum.ToString,'总计标签
                      "失败 " + failedNum.ToString,'失败标签
                      CInt(cnt / totalNum * 100.0),'进度条比例
                      "正在转换 " + fileList.GetFileName(cnt),'当前执行标签
                      vbNullString'按钮
                      )
            '输出文件名
            Dim outputFileName As String

            If outputPath = "" Then
                '没有输入路径
                Dim indexOfDot As Integer = fileList.GetFileName(cnt).LastIndexOf(".")
                If (indexOfDot = -1) Then
                    '文件没有后缀
                    outputFileName = fileList.GetFileName(cnt) + "." +
                                     outputTemplate.ToLower()
                Else
                    '文件有后缀
                    outputFileName = fileList.GetFileName(cnt).Substring(0, indexOfDot) + "." +
                                     outputTemplate.ToLower()
                End If
            Else
                '输入了路径
                If (outputPath.LastIndexOf("\") = outputPath.Length - 1) Or
                   (outputPath.LastIndexOf("/") = outputPath.Length - 1) Then
                    '后面带斜杠
                    outputFileName = outputPath +
                        Path.GetFileNameWithoutExtension(fileList.GetFileName(cnt)) + "." +
                        outputTemplate.ToLower
                Else
                    '后面不带斜杠
                    outputFileName = outputPath + "\" +
                                    Path.GetFileNameWithoutExtension(fileList.GetFileName(cnt)) + "." +
                                    outputTemplate.ToLower()
                End If
            End If

            '检查输出文件是否存在
            If My.Computer.FileSystem.FileExists(outputFileName) Then
                If MessageBox.Show("以下目标文件已存在，点击确定(OK)将会覆盖，否则请点击取消(Cancel)来跳过这个文件" +
                                   vbCrLf + outputFileName,
                                   "警告 - DanmakuFactory", MessageBoxButtons.OKCancel,
                                   MessageBoxIcon.Information, MessageBoxDefaultButton.Button1) =
                                   Windows.Forms.DialogResult.Cancel Then
                    errorMsg += cnt.ToString + "---------" + vbCrLf +
                                "输入文件:" + fileList.GetFileName(cnt) + vbCrLf +
                                "输出文件:" + outputFileName + vbCrLf +
                                "信息:已跳过当前文件" + vbCrLf + vbCrLf
                    failedNum += 1
                    Continue For
                End If
            End If

            '执行转换
            If stopFlag Then
                '检查终止指令
                Exit For
            End If

            Dim Proc As New Process
            Dim arg As String

            arg = "--check-version-" + Define.cliVersion + " --ignore-warnings -i """ + fileList.GetFileName(cnt) +
                            """ -o """ + outputFileName +
                            """ -N """ + config.fontname +
                            """ -r " + config.resolution(0).ToString + " " + config.resolution(1).ToString +
                            " -t " + config.timeShift.ToString +
                            " -s " + config.scrolltime.ToString +
                            " -f " + config.fixtime.ToString +
                            " -d " + config.density.ToString +
                            " -S " + config.fontsize.ToString +
                            " -O " + config.opacity.ToString +
                            " -L " + config.outline.ToString +
                            " -D " + config.shadow.ToString +
                            " -B " + config.bold.ToString +
                            " --displayarea " + config.displayarea.ToString +
                            " --scrollarea " + config.scrollarea.ToString +
                            " --showusernames " + config.showUsernames.ToString +
                            " --showmsgbox " + config.showMsgbox.ToString +
                            " --msgboxsize " + config.msgboxSize(0).ToString + " " + config.msgboxSize(1).ToString +
                            " --msgboxpos " + config.msgboxPos(0).ToString + " " + config.msgboxPos(1).ToString +
                            " --msgboxfontsize " + config.msgboxFontsize.ToString +
                            " --msgboxduration " + config.msgboxDuration.ToString +
                            " --giftminprice " + config.giftMinPrice.ToString +
                            " --giftmergetolerance " + config.giftMergeTolerance.ToString
            If Not config.ToBlockString.Equals("") Then
                arg += " -b " + config.ToBlockString
            End If
            If Not config.ToStatString.Equals("") Then
                arg += " --statmode " + config.ToStatString
            End If

            Proc.StartInfo.FileName = Application.StartupPath + "\" + Define.cliProgramFileName
            Proc.StartInfo.RedirectStandardError = True
            Proc.StartInfo.UseShellExecute = False
            Proc.StartInfo.CreateNoWindow = True
            Proc.StartInfo.Arguments = arg

            '异常处理
            Try
                If Proc.Start() Then
                    Dim errorOp As String = Proc.StandardError.ReadToEnd()
                    If errorOp.Equals("") Then
                        '成功
                        doneNum += 1
                    Else
                        '命令行错误
                        failedNum += 1
                        errorMsg += cnt.ToString + "---------" + vbCrLf +
                                "输入文件:" + fileList.GetFileName(cnt) + vbCrLf +
                                "输出文件:" + outputFileName + vbCrLf +
                                "信息:命令行程序执行时发生错误" + vbCrLf + errorOp + vbCrLf + vbCrLf
                    End If
                Else
                    '调用错误
                    failedNum += 1
                    errorMsg += cnt.ToString + "---------" + vbCrLf +
                                "输入文件:" + fileList.GetFileName(cnt) + vbCrLf +
                                "输出文件:" + outputFileName + vbCrLf +
                                "信息:无法执行此命令行程序" + vbCrLf + +vbCrLf
                End If
                Proc.WaitForExit()
            Catch ex As Exception
                '调用错误
                failedNum += 1
                errorMsg += cnt.ToString + "---------" + vbCrLf +
                                "输入文件:" + fileList.GetFileName(cnt) + vbCrLf +
                                "输出文件:" + outputFileName + vbCrLf +
                                "信息:发生异常" + vbCrLf + ex.Message + vbCrLf + +vbCrLf
            End Try

            '刷新状态
            Me.Invoke(New UpdateProcUI(AddressOf UpdateUI),
                      (CInt((cnt + 1) / totalNum * 100)).ToString() + "%",'状态大标题
                      "已完成 " + doneNum.ToString,'已完成标签
                      "总计 " + totalNum.ToString,'总计标签
                      "失败 " + failedNum.ToString,'失败标签
                      CInt((cnt + 1) / totalNum * 100.0),'进度条比例
                      "正在转换 " + fileList.GetFileName(cnt),'当前执行标签
                      vbNullString'按钮
                      )
        Next

        '完成
        Dim title As String
        If failedNum = 0 Then
            title = "已完成"
        Else
            title = "已完成 - 有错误"
        End If
        Me.Invoke(New UpdateProcUI(AddressOf UpdateUI),
                title,'状态大标题
                "已完成 " + doneNum.ToString,'已完成标签
                "总计 " + totalNum.ToString,'总计标签
                "失败 " + failedNum.ToString,'失败标签
                100,'进度条比例
                "任务结束",'当前执行标签
                "退出"'按钮
                )
    End Sub

    Sub UpdateUI(statusLabelStr As String, doneLabelStr As String, totalLabelStr As String,
                 failedLabelStr As String, progressBar1Int As Integer,
                 loadingLabelStr As String, utilBottonStr As String)
        If doneLabelStr IsNot vbNullString Then
            DoneLabel.Text = doneLabelStr
        End If
        If totalLabelStr IsNot vbNullString Then
            TotalLabel.Text = totalLabelStr
        End If
        If failedLabelStr IsNot vbNullString Then
            FailedLabel.Text = failedLabelStr
        End If
        If statusLabelStr IsNot vbNullString Then
            StatusLabel.Text = statusLabelStr
        End If
        If loadingLabelStr IsNot vbNullString Then
            LoadingLabel.Text = loadingLabelStr
        End If
        If utilBottonStr IsNot vbNullString Then
            UtilButton.Text = utilBottonStr
        End If
        If progressBar1Int >= 0 Then
            ProgressBar1.Value = progressBar1Int
        End If
    End Sub

    Private Sub UtilButton_Click(sender As Object, e As EventArgs) Handles UtilButton.Click
        If UtilButton.Text = "退出" Then
            '清空文件列表
            If ClearFileListCheckBox.Checked Then
                fileList.Clear()
            End If
            Me.Close()
        ElseIf UtilButton.Text = "终止" Then
            stopFlag = True
            StatusLabel.Text = "正在终止..."
        End If
    End Sub

    Private Sub ViewErrMsgButton_Click(sender As Object, e As EventArgs) Handles ViewErrMsgButton.Click
        TextView.sendText(errorMsg)
        TextView.ShowDialog()
    End Sub
End Class
