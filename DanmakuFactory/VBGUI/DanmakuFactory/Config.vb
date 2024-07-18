Public Class Config
    'TODO: 改用与cli版本一致的json配置文件或ini

    '不保存的配置
    Public timeShift As Double = 0.00

    '保存的配置
    Public resolution() As Integer = {1920, 1080}
    Public displayarea As Double = 1.0
    Public scrollarea As Double = 1.0

    Public scrolltime As Double = 12.0
    Public fixtime As Double = 5.0
    Public density As Integer = 0
    Public fontname As String = "Microsoft YaHei"
    Public fontsize As Integer = 38
    Public opacity As Integer = 200
    Public outline As Integer = 0
    Public shadow As Integer = 1
    Public bold As Boolean = False
    Public showUsernames As Boolean = False

    Public blockR2L As Boolean = False
    Public blockL2R As Boolean = False
    Public blockTop As Boolean = False
    Public blockBtm As Boolean = False
    Public blockSpecial As Boolean = False
    Public blockColor As Boolean = False
    Public blockRepeat As Boolean = False

    Public showMsgbox As Boolean = True
    Public msgboxPos() As Integer = {10, 0}
    Public msgboxSize() As Integer = {500, 1080}
    Public msgboxFontsize As Integer = 35
    Public msgboxDuration As Double = 0.00
    Public giftMinPrice As Double = 0.00
    Public giftMergeTolerance As Double = 0.00

    Public statTable As Boolean = False
    Public statHistogram As Boolean = False

    Private configFileName As String = Application.StartupPath + "\" + "DanmakuFactoryGUIConfig.txt"

    Public Sub WriteConfig()
        WriteConfig(configFileName)
    End Sub

    Public Sub WriteConfig(fileName As String)
        Dim outStr As String = ToString()
        System.IO.File.WriteAllText(fileName, outStr)
    End Sub

    Public Sub ReadConfig()
        ReadConfig(configFileName)
    End Sub

    Public Sub ReadConfig(fileName As String)
        Dim inStr As String
        inStr = System.IO.File.ReadAllText(fileName)
        Dim Settings() As String = inStr.Split(";")
        For Each setStr As String In Settings
            Dim str As String() = setStr.Split(":")
            If str.Length > 1 Then
                Dim key As String = str(0).Trim().ToLower()
                Dim value As String = str(1).Trim()
                If key = "resx" Then
                    resolution(0) = Convert.ToInt32(value)
                ElseIf key = "resy" Then
                    resolution(1) = Convert.ToInt32(value)
                ElseIf key = "displayarea" Then
                    displayarea = Convert.ToDouble(value)
                ElseIf key = "scrollarea" Then
                    scrollarea = Convert.ToDouble(value)
                ElseIf key = "scrolltime" Then
                    scrolltime = Convert.ToDouble(value)
                ElseIf key = "fixtime" Then
                    fixtime = Convert.ToDouble(value)
                ElseIf key = "density" Then
                    density = Convert.ToInt32(value)
                ElseIf key = "fontsize" Then
                    fontsize = Convert.ToInt32(value)
                ElseIf key = "opacity" Then
                    opacity = Convert.ToInt32(value)
                ElseIf key = "fontname" Then
                    fontname = str(1).Trim()
                ElseIf key = "outline" Then
                    outline = Convert.ToInt32(value)
                ElseIf key = "shadow" Then
                    shadow = Convert.ToInt32(value)
                ElseIf key = "bold" Then
                    bold = Convert.ToBoolean(value)
                ElseIf key = "blockr2l" Then
                    blockR2L = Convert.ToBoolean(value)
                ElseIf key = "blockl2r" Then
                    blockL2R = Convert.ToBoolean(value)
                ElseIf key = "blocktop" Then
                    blockTop = Convert.ToBoolean(value)
                ElseIf key = "blockbtm" Then
                    blockBtm = Convert.ToBoolean(value)
                ElseIf key = "blockspecial" Then
                    blockSpecial = Convert.ToBoolean(value)
                ElseIf key = "blockcolor" Then
                    blockColor = Convert.ToBoolean(value)
                ElseIf key = "blockrepeat" Then
                    blockRepeat = Convert.ToBoolean(value)
                ElseIf key = "stattable" Then
                    statTable = Convert.ToBoolean(value)
                ElseIf key = "stathistogram" Then
                    statHistogram = Convert.ToBoolean(value)
                ElseIf key = "showusernames" Then
                    showUsernames = Convert.ToBoolean(value)
                ElseIf key = "showmsgbox" Then
                    showMsgbox = Convert.ToBoolean(value)
                ElseIf key = "msgboxposx" Then
                    msgboxPos(0) = Convert.ToInt32(value)
                ElseIf key = "msgboxposy" Then
                    msgboxPos(1) = Convert.ToInt32(value)
                ElseIf key = "msgboxsizewidth" Then
                    msgboxSize(0) = Convert.ToInt32(value)
                ElseIf key = "msgboxsizeheight" Then
                    msgboxSize(1) = Convert.ToInt32(value)
                ElseIf key = "msgboxfontsize" Then
                    msgboxFontsize = Convert.ToInt32(value)
                ElseIf key = "msgboxduration" Then
                    msgboxDuration = Convert.ToDouble(value)
                ElseIf key = "giftminprice" Then
                    giftMinPrice = Convert.ToDouble(value)
                ElseIf key = "giftmergetolerance" Then
                    giftMergeTolerance = Convert.ToDouble(value)
                End If
            End If
        Next
    End Sub

    Public Overrides Function ToString() As String
        Dim retStr As String
        retStr = "DanmakuFactoryConfigVer:1.0;" + vbCrLf +
                 "resX:" + resolution(0).ToString() + ";" + "resY:" + resolution(1).ToString() + ";" + vbCrLf +
                 "displayarea:" + displayarea.ToString() + ";" + vbCrLf +
                 "scrollarea:" + scrollarea.ToString() + ";" + vbCrLf +
                 "scrolltime:" + scrolltime.ToString() + ";" + vbCrLf +
                 "fixtime:" + fixtime.ToString() + ";" + vbCrLf +
                 "density:" + density.ToString() + ";" + vbCrLf +
                 "fontname:" + fontname + ";" + vbCrLf +
                 "fontsize:" + fontsize.ToString() + ";" + vbCrLf +
                 "opacity:" + opacity.ToString() + ";" + vbCrLf +
                 "outline:" + outline.ToString() + ";" + vbCrLf +
                 "shadow:" + shadow.ToString() + ";" + vbCrLf +
                 "bold:" + bold.ToString() + ";" + vbCrLf +
                 "blockR2L:" + blockR2L.ToString() + ";" +
                 "blockL2R:" + blockL2R.ToString() + ";" +
                 "blockTop:" + blockTop.ToString() + ";" +
                 "blockBtm:" + blockBtm.ToString() + ";" +
                 "blockSpecial:" + blockSpecial.ToString() + ";" +
                 "blockColor:" + blockColor.ToString() + ";" +
                 "blockRepeat:" + blockRepeat.ToString() + ";" + vbCrLf +
                 "statTable:" + statTable.ToString() + ";" +
                 "statHistogram:" + statHistogram.ToString() + ";" + vbCrLf +
                 "showUsernames:" + showUsernames.ToString() + ";" + vbCrLf +
                 "showMsgbox:" + showMsgbox.ToString() + ";" + vbCrLf +
                 "msgboxPosX:" + msgboxPos(0).ToString() + ";" + "msgboxPosY:" + msgboxPos(1).ToString() + ";" + vbCrLf +
                 "msgboxSizeWidth:" + msgboxSize(0).ToString() + ";" +
                 "msgboxSizeHeight:" + msgboxSize(1).ToString() + ";" + vbCrLf +
                 "msgboxFontsize:" + msgboxFontsize.ToString() + ";" + vbCrLf +
                 "msgboxDuration:" + msgboxDuration.ToString() + ";" + vbCrLf +
                 "giftMinPrice:" + giftMinPrice.ToString() + ";" + vbCrLf +
                 "giftMergeTolerance:" + giftMergeTolerance.ToString() + ";" + vbCrLf
        Return retStr
    End Function

    '按照 TOP-BOTTOM 的格式输出屏蔽字符串
    Public Function ToBlockString() As String
        Dim blockStr As String = ""

        If blockR2L Then
            blockStr += "r2l"
        End If
        If blockL2R Then
            If Not blockStr.Equals("") Then
                blockStr += "-"
            End If
            blockStr += "l2r"
        End If
        If blockTop Then
            If Not blockStr.Equals("") Then
                blockStr += "-"
            End If
            blockStr += "top"
        End If
        If blockBtm Then
            If Not blockStr.Equals("") Then
                blockStr += "-"
            End If
            blockStr += "bottom"
        End If
        If blockColor Then
            If Not blockStr.Equals("") Then
                blockStr += "-"
            End If
            blockStr += "color"
        End If
        If blockSpecial Then
            If Not blockStr.Equals("") Then
                blockStr += "-"
            End If
            blockStr += "special"
        End If
        If blockRepeat Then
            If Not blockStr.Equals("") Then
                blockStr += "-"
            End If
            blockStr += "repeat"
        End If

        Return blockStr
    End Function

    '按照 HISTOGRAM-TABLE 的格式输出统计字符串
    Public Function ToStatString() As String
        Dim statStr As String = ""

        If statHistogram Then
            statStr += "histogram"
        End If
        If statTable Then
            If Not statStr.Equals("") Then
                statStr += "-"
            End If
            statStr += "table"
        End If

        Return statStr
    End Function

    '非法设置修正
    Public Sub Check()
        Dim defaultConfig As Config = New Config

        If resolution(0) <= 0 Or resolution(1) <= 0 Then
            resolution = defaultConfig.resolution
        End If
        If displayarea <= 0 Or displayarea > 1 Then
            displayarea = defaultConfig.displayarea
        End If
        If scrollarea <= 0 Or scrollarea > displayarea Then
            scrollarea = displayarea
        End If
        If scrolltime <= 0 Then
            scrolltime = defaultConfig.scrolltime
        End If
        If fixtime <= 0 Then
            fixtime = defaultConfig.fixtime
        End If
        If density < -1 Then
            density = defaultConfig.density
        End If
        If fontsize <= 0 Then
            fontsize = defaultConfig.fontsize
        End If
        If opacity <= 0 Or opacity > 255 Then
            opacity = defaultConfig.opacity
        End If
        If outline < 0 Or outline > 4 Then
            outline = defaultConfig.outline
        End If
        If shadow < 0 Or shadow > 4 Then
            shadow = defaultConfig.shadow
        End If
        If giftMinPrice < 0 Then
            giftMinPrice = defaultConfig.giftMinPrice
        End If
    End Sub
End Class
