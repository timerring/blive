Imports System.IO
Imports System.Windows.Forms

Public Class FilesDialog
    Private fileList As FileList
    Public Sub SendFileList(obj As FileList)
        fileList = obj
    End Sub
    Private Sub OK_Button_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles OK_Button.Click
        Me.DialogResult = System.Windows.Forms.DialogResult.OK
        Me.Close()
    End Sub

    Private Sub Cancel_Button_Click(ByVal sender As System.Object, ByVal e As System.EventArgs)
        Me.DialogResult = System.Windows.Forms.DialogResult.Cancel
        Me.Close()
    End Sub

    Private Sub FilesDialog_Load(sender As Object, e As EventArgs) Handles MyBase.Load
        For cnt As Integer = 0 To fileList.GetFileNum - 1
            FileListBox.Items.Add(fileList.GetFileName(cnt))
        Next
    End Sub

    Private Sub Button1_Click(sender As Object, e As EventArgs) Handles Button1.Click
        fileList.Clear()
        FileListBox.Items.Clear()
    End Sub
End Class
