/* MIT License
 * 
 * Copyright (c) 2022 hkm
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "TemplateFile.h"

/*
 * 读取用户自定义模板文件
 * 参数：模板文件名/输出弹幕头/模式/错误信息文本输出容器/容器最大长度
 * 返回值：成功返回0/失败返回1
 */
int readTemplateFile(const char *const ipFile, const char *const templateFile, 
                     DANMAKU **danmakuHead, const char *mode, const float timeShift, 
                     STATUS *const status, char *errMsg, int msgLen)
{
    FILE *tempfptr;
    DANMAKU *last = NULL;
    int cnt;
    BOOL isError = FALSE;
    
    /* 刷新status */
    if (status != NULL)
    {
        status -> function = (void *)readTemplateFile;
        (status -> completedNum) = 0;
    }
    
    /* 打开文件 */
    if((tempfptr = fopen(templateFile, "r")) == NULL)
    {
        return 1;
    }
    
    /* 判断读入方式 */
    if(*danmakuHead == NULL || *mode == 'n')
    {/* 新建模式 */
//      freeList(*danmakuHead);
        *danmakuHead = NULL;
    }
    else if(*mode == 'a')
    {/* 追加模式 */
        last = *danmakuHead;
        while(last -> next != NULL)
        {
            last = last -> next;
        }
    }

    errMsg[0] = '\0';

    /* 模板属性 */
    float version = 0.0;  /* 版本号 */

    FORMAT format;
    SET_TIME setTime = {
        1.0 /* 时间比例 */
    };
    SET_TYPE setType = {
        0, /* 默认右左 */
        0, /* 默认左右 */
        0, /* 默认顶端固定 */
        0, /* 默认底部固定 */
        R2L  /* 默认弹幕类型 */
    };
    SET_COLOR setColor = {
        16777215 /* 默认颜色 */
    };
    SET_FONTSIZE setFontsize = {
        25 /* 默认字号 */
    };

    /* 解析模板 */
    int lineCnt = 0;
    int tar = TAR_NULL;
    char lineStr[LINE_MAX_LEN], tempStr[LINE_MAX_LEN], tempMsgStr[LINE_MAX_LEN];
    char *linePtr, *tempPtr;
    int formatTableIndex = 0;
    int strTableIndex = 0;
    
    /* 读取版本信息行 */
    fgets(lineStr, LINE_MAX_LEN, tempfptr);
    linePtr = lineStr;
    getNextWord(&linePtr, tempStr, LINE_MAX_LEN, ':', TRUE);
    if (!strcmp(tempStr, "DanmakuFactory_Template_Version"))
    {
        strGetLeftPart(NULL, &linePtr, ':', LINE_MAX_LEN);
        getNextWord(&linePtr, tempStr, LINE_MAX_LEN, '\n', TRUE);
        version = atof(tempStr);
        if (version != 1.0)
        {
            // sprintf_s(errMsg, msgLen, "[err]Line 1(version): template file version %s is not supported.\n", tempStr);

            strSafeCat(errMsg, msgLen, "[err]Line 1(version): template file version ");
            strSafeCat(errMsg, msgLen, tempStr);
            strSafeCat(errMsg, msgLen, " is not supported.\n");
            return -1;
        }
    }
    else
    {
        strSafeCat(errMsg, msgLen, "[err]Line 1(version): You must specify the template file version on the first line.\n"
                                 "     example: DanmakuFactory_Template_Version: 1.0\n"
                  );
        return -1;
    }
    
    lineCnt++;

    while (!feof(tempfptr))
    {
        lineCnt++;
        fgets(lineStr, LINE_MAX_LEN, tempfptr);
        linePtr = lineStr;

        /* TODO:修改注释解析 */
        /* 注释截断 */
        if (strchr(lineStr, '#') != NULL)
        {
            *(strchr(lineStr, '#')) = '\0';
        }
        
        while (*linePtr != '\0')
        {
            /* 空白行判断 */
            if (STR_IS_BLANK(linePtr))
            {
                break;
            }

            char *startPtr = linePtr;
            getNextWord(&linePtr, tempStr, LINE_MAX_LEN, ':', TRUE);

            /* 解析标签 */
            if (!strcmp(tempStr, "set"))
            {
                getNextWord(&linePtr, tempStr, LINE_MAX_LEN, ':', TRUE);
                if (!strcmp(tempStr, "$time$"))
                {
                    tar = TAR_SET_TIME;
                }
                else if (!strcmp(tempStr, "$type$"))
                {
                    tar = TAR_SET_TYPE;
                }
                else if (!strcmp(tempStr, "$color$"))
                {
                    tar = TAR_SET_COLOR;
                }
                else if (!strcmp(tempStr, "$fontsize$"))
                {
                    tar = TAR_SET_FONTSIZE;
                }
                else
                {
                    sprintf(tempMsgStr, "[err]Line %d(set): Unknown Words %s.\n", lineCnt, tempStr);
                    strSafeCat(errMsg, msgLen, tempMsgStr);
                    return -1;
                }
                strGetLeftPart(NULL, &linePtr, ':', LINE_MAX_LEN); /* 移动指针到冒号之后 */
            }
            else if (!strcmp(tempStr, "format"))
            {
                strGetLeftPart(NULL, &linePtr, ':', LINE_MAX_LEN); /* 移动指针到冒号之后 */
                tar = TAR_FORMAT;
            }
            else
            {/* 根据当前标签作用域读取信息 */
                linePtr = startPtr;
                switch (tar)
                {
                case TAR_FORMAT:
                    /* 循环直到读完整行格式行 */
                    tempPtr = tempStr;
                    int verType;
                    while (*linePtr != '\0')
                    {
                        verType = 0;
                        if (isStartWith(linePtr, "$time$"))
                        {
                            verType = FORMAT_TIME;
                            STR_PTR_JUMP(linePtr, "$time$");
                        }
                        else if (isStartWith(linePtr, "$type$"))
                        {
                            verType = FORMAT_TYPE;
                            STR_PTR_JUMP(linePtr, "$type$");
                        }
                        else if (isStartWith(linePtr, "$color$"))
                        {
                            verType = FORMAT_COLOR;
                            STR_PTR_JUMP(linePtr, "$color$");
                        }
                        else if (isStartWith(linePtr, "$fontsize$"))
                        {
                            verType = FORMAT_FONTSIZE;
                            STR_PTR_JUMP(linePtr, "$fontsize$");
                        }
                        else if (isStartWith(linePtr, "$text$"))
                        {
                            verType = FORMAT_TEXT;
                            STR_PTR_JUMP(linePtr, "$text$");
                        }
                        else if (isStartWith(linePtr, "$anynumber$"))
                        {
                            verType = FORMAT_ANYNUM;
                            STR_PTR_JUMP(linePtr, "$anynumber$");
                        }
                        else if (isStartWith(linePtr, "$anystring$"))
                        {
                            verType = FORMAT_ANYSTR;
                            STR_PTR_JUMP(linePtr, "$anystring$");
                        }
                        else
                        {/* 字符串 */
                            *tempPtr = *linePtr;
                            tempPtr++;
                            linePtr++;
                        }

                        if ((verType != 0 && tempPtr != tempStr) || *linePtr == '\0')
                        {/* 存储字符串常量 */
                            *tempPtr = '\0';
                            if ((format.strTable[strTableIndex] = (char *)malloc(strlen(tempStr) * sizeof(char))) == NULL)
                            {
                                return 2;
                            }
                            strcpy(format.strTable[strTableIndex], tempStr);
                            tempPtr = tempStr;
                            format.formatTable[formatTableIndex] = strTableIndex;

                            strTableIndex++;
                            formatTableIndex++;

                        }

                        if (verType != 0)
                        {
                            format.formatTable[formatTableIndex] = verType;
                            formatTableIndex++;
                        }
                    }
                    break;
                case TAR_SET_TIME:
                    getNextWord(&linePtr, tempStr, LINE_MAX_LEN, '=', TRUE);
                    if (!strcmp(tempStr, "Ratio_to_seconds"))
                    {
                        strGetLeftPart(NULL, &linePtr, '=', LINE_MAX_LEN);
                        getNextWord(&linePtr, tempStr, LINE_MAX_LEN, '\0', TRUE);
                        setTime.ratioToSeconds = atof(tempStr);
                    }
                    else
                    {
                        sprintf(tempMsgStr, "[err]Line %d: Unknown Words %s.\n", lineCnt, tempStr);
                        strSafeCat(errMsg, msgLen, tempMsgStr);
                        return -1;
                    }
                    
                    break;
                case TAR_SET_TYPE:
                    getNextWord(&linePtr, tempStr, LINE_MAX_LEN, '=', TRUE);
                    if (!strcmp(tempStr, "right_to_left"))
                    {
                        strGetLeftPart(NULL, &linePtr, '=', LINE_MAX_LEN);
                        getNextWord(&linePtr, tempStr, LINE_MAX_LEN, '\0', TRUE);
                        setType.rightToLeft = atoi(tempStr);
                    }
                    else if (!strcmp(tempStr, "left_to_right"))
                    {
                        strGetLeftPart(NULL, &linePtr, '=', LINE_MAX_LEN);
                        getNextWord(&linePtr, tempStr, LINE_MAX_LEN, '\0', TRUE);
                        setType.leftToRight = atoi(tempStr);
                    }
                    else if (!strcmp(tempStr, "top_fix"))
                    {
                        strGetLeftPart(NULL, &linePtr, '=', LINE_MAX_LEN);
                        getNextWord(&linePtr, tempStr, LINE_MAX_LEN, '\0', TRUE);
                        setType.topFix = atoi(tempStr);
                    }
                    else if (!strcmp(tempStr, "bottom_fix"))
                    {
                        strGetLeftPart(NULL, &linePtr, '=', LINE_MAX_LEN);
                        getNextWord(&linePtr, tempStr, LINE_MAX_LEN, '\0', TRUE);
                        setType.bottomFix = atoi(tempStr);
                    }
                    else if (!strcmp(tempStr, "default_type"))
                    {
                        strGetLeftPart(NULL, &linePtr, '=', LINE_MAX_LEN);
                        getNextWord(&linePtr, tempStr, LINE_MAX_LEN, '\0', TRUE);
                        /* 解析默认类型 */
                        if (!strcmp(tempStr, "right_to_left"))
                        {
                            setType.defaultType = R2L;
                        }
                        else if (!strcmp(tempStr, "left_to_right"))
                        {
                            setType.defaultType = L2R;
                        }
                        else if (!strcmp(tempStr, "top_fix"))
                        {
                            setType.defaultType = TOP;
                        }
                        else if (!strcmp(tempStr, "bottom_fix"))
                        {
                            setType.defaultType = BOTTOM;
                        }
                        else
                        {
                            sprintf(tempMsgStr, "[err]Line %d: Unknown Words %s.\n", lineCnt, tempStr);
                            strSafeCat(errMsg, msgLen, tempMsgStr);
                            return -1;
                        }
                    }
                    else
                    {
                        sprintf(tempMsgStr, "[err]Line %d: Unknown Words %s.\n", lineCnt, tempStr);
                        strSafeCat(errMsg, msgLen, tempMsgStr);
                        return -1;
                    }
                    break;
                case TAR_SET_COLOR:
                    getNextWord(&linePtr, tempStr, LINE_MAX_LEN, '=', TRUE);
                    if (!strcmp(tempStr, "default_color"))
                    {
                        strGetLeftPart(NULL, &linePtr, '=', LINE_MAX_LEN);
                        getNextWord(&linePtr, tempStr, LINE_MAX_LEN, '\0', TRUE);
                        setColor.defaultColor = atoi(tempStr);
                    }
                    else
                    {
                        sprintf(tempMsgStr, "[err]Line %d: Unknown Words %s.\n", lineCnt, tempStr);
                        strSafeCat(errMsg, msgLen, tempMsgStr);
                        return -1;
                    }
                    break;
                case TAR_SET_FONTSIZE:
                    getNextWord(&linePtr, tempStr, LINE_MAX_LEN, '=', TRUE);
                    if (!strcmp(tempStr, "default_fontsize"))
                    {
                        strGetLeftPart(NULL, &linePtr, '=', LINE_MAX_LEN);
                        getNextWord(&linePtr, tempStr, LINE_MAX_LEN, '\0', TRUE);
                        setFontsize.defaultFontsize = atoi(tempStr);
                    }
                    else
                    {
                        sprintf(tempMsgStr, "[err]Line %d: Unknown Words %s.\n", lineCnt, tempStr);
                        strSafeCat(errMsg, msgLen, tempMsgStr);
                        return -1;
                    }
                    break;
                default:
                    sprintf(tempMsgStr, "[err]Line %d: Unknown Words %s.\n", lineCnt, tempStr);
                    strSafeCat(errMsg, msgLen, tempMsgStr);
                    return -1;
                    break;
                }
            }
        } /* 结束单行解析 */
        
    } /* 文件解析结束 */
    
    format.formatTableLen = formatTableIndex;
    format.strTableLen = strTableIndex;

    /* 去除格式字符串前后空白字符 */
    if (format.formatTableLen >= 2)
    {
        char *formatStr = format.strTable[format.formatTable[0]];
        char *leftPtr, *rightPtr;
        /* 去除打头字符串空白字符 */
        if (!FORMAT_IS_VAR(format.formatTable[0]))
        {
            leftPtr = rightPtr = formatStr;
            while (*rightPtr != '\0' && CHAR_IS_BLANK(*rightPtr))
            {
                rightPtr++;
            }
            while (*rightPtr != '\0')
            {
                *leftPtr = *rightPtr;
                leftPtr++;
                rightPtr++;
            }
            *leftPtr = '\0';
        }

        /* 去除结束字符串空白字符 */
        if (!FORMAT_IS_VAR(format.formatTable[format.formatTableLen - 1]))
        {
            formatStr = format.strTable[format.formatTable[format.formatTableLen - 1]];
            leftPtr = formatStr;
            while (!STR_IS_BLANK(leftPtr) && *leftPtr != '\0')
            {
                leftPtr++;
            }
            *leftPtr = '\0';
        }
    }

#ifdef DEBUG    
    for (cnt = 0; cnt <format.formatTableLen; cnt++)
    {
        if (FORMAT_IS_VAR(format.formatTable[cnt]))
        {
            printf("%d|", format.formatTable[cnt]);
        }
        else
        {
            printf("%s[%d]|", format.strTable[format.formatTable[cnt]], format.formatTable[cnt]);
        }
        
    }
#endif

    fclose(tempfptr);

    /* 格式字符串合法性检查 */
    {
        /* 格式字符串空检测 */
        if (format.formatTableLen < 2)
        {
            strSafeCat(errMsg, msgLen, "[err]Line ?(Format string): Format string must be specified.\n");
            return -1;
        }

        /* 检测是否是变量开头 */
        if (FORMAT_IS_VAR(format.formatTable[0]))
        {
            strSafeCat(errMsg, msgLen, "[err]Line ?(Format string): Format string cannot begin with a variable.\n");
            return -1;
        }

        int timeDefineTimes = 0;
        int typeDefineTimes = 0;
        int colorDefineTimes = 0;
        int fontsizeDefineTimes = 0;
        int textDefineTimes = 0;
        
        for (cnt = 1; cnt < format.formatTableLen; cnt++)
        {
            /* 检测两个变量是否连用 */
            if (FORMAT_IS_VAR(format.formatTable[cnt]))
            {
                if (FORMAT_IS_VAR(format.formatTable[cnt-1]))
                {
                    strSafeCat(errMsg, msgLen, 
                               "[err]Line ?(Format string): The two variables must be separated by string constants.\n");
                    return -1;
                }
                switch (format.formatTable[cnt])
                {
                case FORMAT_TIME:
                    timeDefineTimes++;
                    break;
                case FORMAT_TYPE:
                    typeDefineTimes++;
                    break;
                case FORMAT_FONTSIZE:
                    fontsizeDefineTimes++;
                    break;
                case FORMAT_COLOR:
                    colorDefineTimes++;
                    break;
                case FORMAT_TEXT:
                    textDefineTimes++;
                    break;
                default:
                    break;
                }
            }
        }

        /* 检查结尾是不是空白变量 */
        if (STR_IS_EMPTY(format.strTable[format.formatTable[format.formatTableLen-1]]) && 
            (format.formatTable[format.formatTableLen-2] == FORMAT_TEXT || 
             format.formatTable[format.formatTableLen-2] == FORMAT_ANYSTR)
           )
        {
            strSafeCat(errMsg, msgLen, 
                       "[err]Line ?(Format string): Format string cannot ending with variable $text$ or $anystring$.\n");
            isError = TRUE;
        }

        /* 检测时间定义次数 */
        if (timeDefineTimes == 0)
        {
            strSafeCat(errMsg, msgLen, 
                       "[err]Line ?(Format string): Variable $time$ must be specified.\n");
            isError = TRUE;
        }
        else if (timeDefineTimes > 1)
        {
            strSafeCat(errMsg, msgLen, 
                       "[err]Line ?(Format string): Variable $time$ is defined repeatedly.\n");
            isError = TRUE;
        }
        /* 检测类型定义次数 */
        if (typeDefineTimes > 1)
        {
            strSafeCat(errMsg, msgLen, 
                       "[err]Line ?(Format string): Variable $type$ is defined repeatedly.\n");
            isError = TRUE;
        }
        /* 检查文字大小定义次数 */
        if (fontsizeDefineTimes > 1)
        {
            strSafeCat(errMsg, msgLen, 
                       "[err]Line ?(Format string): Variable $fontsize$ is defined repeatedly.\n");
            isError = TRUE;
        }
        /* 检查颜色定义次数 */
        if (colorDefineTimes > 1)
        {
            strSafeCat(errMsg, msgLen, 
                       "[err]Line ?(Format string): Variable $color$ is defined repeatedly.\n");
            isError = TRUE;
        }
        /* 检查文本定义次数 */
        if (textDefineTimes == 0)
        {
            strSafeCat(errMsg, msgLen, 
                       "[err]Line ?(Format string): Variable $text$ must be specified.\n");
            isError = TRUE;
        }
        else if (textDefineTimes > 1)
        {
            strSafeCat(errMsg, msgLen, 
                       "[err]Line ?(Format string): Variable $text$ is defined repeatedly.\n");
            isError = TRUE;
        }

        /* 报错并返回 */
        if (isError == TRUE)
        {
            return -1;
        }
    }/* 合法性检测结束 */

    /* 读取弹幕文件 */
    FILE *ipfptr;
    char readBuffer[READ_BUFFER_SIZE];
    char *bufferPtr = readBuffer;
    char *startPtr;
    size_t readSize = 0;

    if ((ipfptr = fopen(ipFile, "r")) == NULL)
    {
        return 3;
    }

    while (TRUE)
    {
        /* 按块读取文件 */
        if (((bufferPtr - readBuffer) > READ_BUFFER_SIZE / 2) || (bufferPtr - readBuffer) == readSize)
        {
            if (readSize != 0)
            {
                fseek(ipfptr, -(long)(readBuffer + readSize - bufferPtr), SEEK_CUR);
            }
            
            readSize = fread(readBuffer, 1, READ_BUFFER_SIZE, ipfptr);
            
            if (readSize == 0)
            {
                break;
            }
            bufferPtr = readBuffer;
        }

        /* 读取一条 */
        startPtr = bufferPtr;
        DANMAKU *readDanmaku;
        BOOL isSuccess = TRUE;

        if ((readDanmaku = (DANMAKU *)malloc(sizeof(DANMAKU))) == NULL)
        {
            /* 释放内存 */
//          freeList(*danmakuHead);
            return 4;
        }
        readDanmaku -> type = HAVE_NOT_BEEN_SET;
        readDanmaku -> fontSize = HAVE_NOT_BEEN_SET;
        readDanmaku -> color = HAVE_NOT_BEEN_SET;
        readDanmaku -> text = NULL;

        for (cnt = 0; cnt < format.formatTableLen; cnt++)
        {
            if (format.formatTable[cnt] == FORMAT_TIME)
            {
                tempPtr = tempStr;
                while (isDesignatedChar(*bufferPtr, "0123456789."))
                {
                    *tempPtr = *bufferPtr;
                    tempPtr++;
                    bufferPtr++;
                    if ((size_t)(bufferPtr - readBuffer) >= readSize)
                    {
                        isSuccess = FALSE;
                        goto LINE_END;
                    }
                }
                *tempPtr = '\0';
                float tempTime = atof(tempStr) / setTime.ratioToSeconds + timeShift;
                if (tempTime < EPS) {
                    tempTime = 0.0f;
                }
                readDanmaku->time = GET_MS_FLT(tempTime);
            }
            else if (format.formatTable[cnt] == FORMAT_TYPE)
            {
                tempPtr = tempStr;
                while (isDesignatedChar(*bufferPtr, "0123456789"))
                {
                    *tempPtr = *bufferPtr;
                    tempPtr++;
                    bufferPtr++;
                    if ((size_t)(bufferPtr - readBuffer) >= readSize)
                    {
                        isSuccess = FALSE;
                        goto LINE_END;
                    }
                }
                *tempPtr = '\0';
                
                int typeNum = atoi(tempStr);
                if (typeNum == setType.rightToLeft)
                {
                    readDanmaku -> type = R2L;
                }
                else if (typeNum == setType.leftToRight)
                {
                    readDanmaku -> type = L2R;
                }
                else if (typeNum == setType.topFix)
                {
                    readDanmaku -> type = TOP;
                }
                else if (typeNum == setType.bottomFix)
                {
                    readDanmaku -> type = BOTTOM;
                }
                else
                {
                    readDanmaku -> type = setType.defaultType;
                }
                
            }
            else if (format.formatTable[cnt] == FORMAT_COLOR)
            {
                tempPtr = tempStr;
                while (isDesignatedChar(*bufferPtr, "0123456789"))
                {
                    *tempPtr = *bufferPtr;
                    tempPtr++;
                    bufferPtr++;
                    if ((size_t)(bufferPtr - readBuffer) >= readSize)
                    {
                        isSuccess = FALSE;
                        goto LINE_END;
                    }
                }
                *tempPtr = '\0';
                readDanmaku -> color = atoi(tempStr);
            }
            else if (format.formatTable[cnt] == FORMAT_FONTSIZE)
            {
                tempPtr = tempStr;
                while (isDesignatedChar(*bufferPtr, "0123456789"))
                {
                    *tempPtr = *bufferPtr;
                    tempPtr++;
                    bufferPtr++;
                    if ((size_t)(bufferPtr - readBuffer) >= readSize)
                    {
                        isSuccess = FALSE;
                        goto LINE_END;
                    }
                }
                *tempPtr = '\0';
                readDanmaku -> fontSize = atoi(tempStr);
            }
            else if (format.formatTable[cnt] == FORMAT_TEXT)
            {
                tempPtr = tempStr;
                char *formatStr = format.strTable[format.formatTable[cnt+1]];
                while (!isStartWith(bufferPtr, formatStr))
                {
                    *tempPtr = *bufferPtr;
                    tempPtr++;
                    bufferPtr++;
                    if ((size_t)(bufferPtr - readBuffer) >= readSize)
                    {
                        isSuccess = FALSE;
                        goto LINE_END;
                    }
                }
                *tempPtr = '\0';
                if ((readDanmaku -> text = (char *)malloc((strlen(tempStr) + 1) * sizeof(char))) == NULL)
                {
                    /* TODO: 释放内存 */
                    if (readDanmaku -> text != NULL)
                    {
                        free(readDanmaku->text);
                    }
                    free(readDanmaku);
//                  freeList(*danmakuHead);
                    return 5;
                }
                strcpy(readDanmaku -> text, tempStr);
            }
            else if (format.formatTable[cnt] == FORMAT_ANYNUM)
            {
                while (isDesignatedChar(*bufferPtr, "0123456789"))
                {
                    bufferPtr++;
                    if ((size_t)(bufferPtr - readBuffer) >= readSize)
                    {
                        isSuccess = FALSE;
                        goto LINE_END;
                    }
                }
            }
            else if (format.formatTable[cnt] == FORMAT_ANYSTR)
            {
                char *formatStr = format.strTable[format.formatTable[cnt+1]];
                while (!isStartWith(bufferPtr, formatStr))
                {
                    bufferPtr++;
                    if ((size_t)(bufferPtr - readBuffer) >= readSize)
                    {
                        isSuccess = FALSE;
                        goto LINE_END;
                    }
                }
            }
            else
            { /* 固定字符串 */
                char *formatStr = format.strTable[format.formatTable[cnt]];
                char *formatPtr = formatStr;

                if (cnt == 0)
                {
                    while (!isStartWith(bufferPtr, formatStr))
                    {
                        bufferPtr++;
                        if ((size_t)(bufferPtr - readBuffer) >= readSize)
                        {
                            isSuccess = FALSE;
                            goto LINE_END;
                        }
                    }
                }
                else
                {
                    if (!isStartWith(bufferPtr, formatStr))
                    {/* 匹配失败 放弃整条已读取内容 */
                        bufferPtr = startPtr + 1;
                        isSuccess = FALSE;
                        goto LINE_END;
                    }
                }

                STR_PTR_JUMP(bufferPtr, formatStr);
            } /* 模块类型判断结束 */
        }
    LINE_END:;
        if (isSuccess)
        {
            /* 没有定义的填充默认值 */
            if (readDanmaku->type == HAVE_NOT_BEEN_SET)
            {
                readDanmaku->type = setType.defaultType;
            }
            if (readDanmaku->fontSize == HAVE_NOT_BEEN_SET)
            {
                readDanmaku->fontSize = setFontsize.defaultFontsize;
            }
            if (readDanmaku->color == HAVE_NOT_BEEN_SET)
            {
                readDanmaku->color = setColor.defaultColor;
            }

            /* 并入链表 */
            if (*danmakuHead == NULL)
            {
                *danmakuHead = readDanmaku;
            }
            else
            {
                last -> next = readDanmaku;
            }
            last = readDanmaku;
            last -> next = NULL;
            status -> totalNum++;
        }
        else
        {
            if (readDanmaku -> text != NULL)
            {
                free(readDanmaku->text);
            }
            free(readDanmaku);
        }
    }

    /* 释放内存 */
    for (cnt = 0; cnt < format.strTableLen; cnt++)
    {
        free(format.strTable[cnt]);
    }

    fclose(ipfptr);
    status -> isDone = TRUE;
    return 0;
}