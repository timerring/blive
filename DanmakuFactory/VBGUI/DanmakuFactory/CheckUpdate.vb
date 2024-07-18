Public Class CheckUpdate
    '0:未检查 1:当前新版本 2:有更新 3:重大问题必须更新 -1:网络错误 -2:返回值解析错误
    Public Const status_notChecked As Integer = 0
    Public Const status_isLastest As Integer = 1
    Public Const status_newVersion As Integer = 2
    Public Const status_mustUpdate As Integer = 3
    Public Const status_internetErr As Integer = -1
    Public Const status_parsingErr As Integer = -2
    Private checkStatus As Integer = status_notChecked

    Private newVersion As String = ""
    Private updateMsg As String = ""
    Private warningMsg As String = ""
    Private updateUrl As String = "https://github.com/hihkm/DanmakuFactory"

    '检查更新
    Public Function CheckUpdate() As Integer
        Dim getStr As String
        Dim client As New System.Net.WebClient()
        Try
            Dim res As Byte() = client.DownloadData("http://1.api.tikm.org/checkupdate/?ver=" + Define.guiVersion)
            getStr = System.Text.Encoding.UTF8.GetString(res)

            If Not getStr.Equals("Lastest") Then
                checkStatus = status_newVersion '有更新
                Dim pathOfGetStr As String() = getStr.Split(";")
                If pathOfGetStr.Length >= 5 Then
                    If pathOfGetStr(3) = "Force" Then
                        newVersion = pathOfGetStr(0)
                        updateMsg = pathOfGetStr(1)
                        updateUrl = pathOfGetStr(2)
                        warningMsg = pathOfGetStr(4)
                        checkStatus = status_mustUpdate '紧急更新
                    Else
                        checkStatus = status_parsingErr '解析错误
                    End If
                ElseIf pathOfGetStr.Length >= 3 Then
                    newVersion = pathOfGetStr(0)
                    updateMsg = pathOfGetStr(1)
                    updateUrl = pathOfGetStr(2)
                Else
                    checkStatus = status_parsingErr '解析错误
                End If
                If newVersion <> "" And Convert.ToDouble(newVersion) < Convert.ToDouble(Define.guiVersion) Then
                    checkStatus = status_isLastest
                End If
            Else
                checkStatus = status_isLastest '当前为最新
            End If
        Catch ex As Exception
            checkStatus = status_internetErr '网络错误
        End Try

        Return checkStatus
    End Function

    Public Function GetNewVersion() As String
        Return newVersion
    End Function

    Public Function GetUpdateMsg() As String
        Return updateMsg
    End Function

    Public Function GetUpdateUrl() As String
        Return updateUrl
    End Function

    Public Function GetCheckStatus() As Integer
        Return checkStatus
    End Function

    Public Function GetWarningMsg() As String
        Return warningMsg
    End Function
End Class