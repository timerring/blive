Imports System.Windows.Forms

Public Class TextView
    Private defaultText As String
    Public Sub sendText(str As String)
        defaultText = str
    End Sub

    Private Sub OK_Button_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles OK_Button.Click
        Me.DialogResult = System.Windows.Forms.DialogResult.OK
        Me.Close()
    End Sub

    Private Sub Cancel_Button_Click(ByVal sender As System.Object, ByVal e As System.EventArgs)
        Me.DialogResult = System.Windows.Forms.DialogResult.Cancel
        Me.Close()
    End Sub

    Private Sub TextView_Load(sender As Object, e As EventArgs) Handles MyBase.Load

        If defaultText = "" Then
            RichTextBox1.Text = "暂无记录"
        Else
            RichTextBox1.Text = defaultText
        End If
    End Sub
End Class
