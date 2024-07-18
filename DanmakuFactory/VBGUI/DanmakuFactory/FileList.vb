Public Class FileList
    Private fileNames() As String
    Private fileListNum As Integer

    Sub AddFile(path As String)
        ReDim Preserve fileNames(fileListNum + 1)
        fileNames(fileListNum) = path
        fileListNum += 1
    End Sub

    Sub AddFiles(paths As String())
        ReDim Preserve fileNames(fileListNum + paths.Length)
        For cnt As Integer = 0 To paths.Length - 1
            fileNames(cnt + fileListNum) = paths(cnt)
        Next
        fileListNum += paths.Length
    End Sub

    '清空文件列表
    Sub Clear()
        ReDim fileNames(0)
        fileListNum = 0
    End Sub

    '获取指定下标的文件名
    Public Function GetFileName(index As Integer) As String
        Return fileNames(index)
    End Function

    '检查文件是否存在 存在返回True
    Public Function FileIsExist(fileName As String) As Boolean
        If My.Computer.FileSystem.FileExists(fileName) Then
            Return True
        Else
            Return False
        End If
    End Function

    Public Function FileIsExist(index As Integer) As Boolean
        Return FileIsExist(GetFileName(index))
    End Function

    Public Function GetFileNum() As Integer
        Return fileListNum
    End Function
End Class
