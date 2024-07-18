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

#include "Config.h"

static COORDIN jsonGetCoord(const char *const jsonStr);

/* 
 * 从文件读配置文件
 * 参数:配置文件名/默认配置
 * 返回值:配置结构体
  */
CONFIG readConfig(const char *const configFileName, const CONFIG defaultConfig)
{
    int cnt;
    char ch;
    char buf[SIZE_OF_BUF], *bufPtr, *bufCopyPtr;
    CONFIG configOnfile = defaultConfig;
    
    FILE *fptr = fopen(configFileName, "r");
    if (fptr == NULL)
    {
        return configOnfile;
    }
    
    /* 找到可读区域 */
    while (fgetc(fptr) != '{' && !feof(fptr)){}
    
    /* 读取 */
    cnt = 0;
    while ((ch = fgetc(fptr)) != '}' && !feof(fptr) && cnt < SIZE_OF_BUF - 1)
    {
        buf[cnt] = ch;
        cnt++;
    }
    buf[cnt] = '\0';
    fclose(fptr);
    
    /* 解析 */
    bufPtr = buf;
    char key[KEY_LEN];
    char value[VALUE_LEN], *valuePtr;
    char typeName[TYPENAME_LEN];
    while (*bufPtr != '\0')
    {
        /* 解析key */
        if (*bufPtr == ',')
        {/* 跳过逗号 */
            bufPtr++;
        }
        strGetLeftPart(key, &bufPtr, ':', KEY_LEN);
        deQuotMarks(key);
        toLower(NULL, key);
        
        /* 解析value */
        valuePtr = value;
        bufCopyPtr = bufPtr;

        while (*bufPtr != '\0' && *bufPtr != ',')
        {
            if (*bufPtr == '[')
            {
                while (*bufPtr != ']' && *bufPtr != '\0')
                {
                    *valuePtr = *bufPtr;
                    valuePtr++;
                    bufPtr++;
                }
            }
            
            *valuePtr = *bufPtr;
            valuePtr++;
            
            if (*bufPtr != '\0')
            {
                bufPtr++;
            }
        }
        *valuePtr = '\0';
        deQuotMarks(value);

        /* 匹配 */
        valuePtr = value;
        if (strcmp("blockmode", key) == 0)
        {
            int blockmode = 0;
            strGetLeftPart(NULL, &valuePtr, '[', VALUE_LEN);
            while (*valuePtr != '\0')
            {
                /* 拆分中括号内用逗号分割的多个值 */ 
                if (strchr(valuePtr, ',') != NULL)
                {/* 后半段有逗号 */ 
                    strGetLeftPart(typeName, &valuePtr, ',', TYPENAME_LEN);
                }
                else
                {/* 后半段无逗号 */ 
                    strGetLeftPart(typeName, &valuePtr, ']', TYPENAME_LEN);
                    valuePtr += strlen(typeName);/* 将指针移到字串末尾以下一个循环跳出 */ 
                }
                
                deQuotMarks(typeName);
                toLower(NULL, typeName);
                
                if (strcmp("r2l", typeName) == 0)
                {
                    blockmode += BLK_R2L;
                }
                else if (strcmp("l2r", typeName) == 0)
                {
                    blockmode += BLK_L2R;
                }
                else if (strcmp("top", typeName) == 0)
                {
                    blockmode += BLK_TOP;
                }
                else if (strcmp("bottom", typeName) == 0)
                {
                    blockmode += BLK_BOTTOM;
                }
                else if (strcmp("special", typeName) == 0)
                {
                    blockmode += BLK_SPECIAL;
                }
                else if (strcmp("color", typeName) == 0 || strcmp("colour", typeName) == 0)
                {/* 兼容英式拼写的color */ 
                    blockmode += BLK_COLOR;
                }
                else if (strcmp("repeat", typeName) == 0)
                {
                    blockmode += BLK_REPEAT; 
                }
            }
            configOnfile.blockmode = blockmode;
        }
        else if (strcmp("statmode", key) == 0)
        {
            /* 解析value */
            int statmode = 0;
            strGetLeftPart(NULL, &valuePtr, '[', VALUE_LEN);
            while (*valuePtr != '\0')
            {
                /* 解析中括号内用逗号分割的多个值 */ 
                if (strchr(valuePtr, ',') != NULL)
                {/* 后半段有逗号 */ 
                    strGetLeftPart(typeName, &valuePtr, ',', TYPENAME_LEN);
                }
                else
                {/* 后半段无逗号 */ 
                    strGetLeftPart(typeName, &valuePtr, ']', TYPENAME_LEN);
                    valuePtr += strlen(typeName);/* 将指针移到字串末尾以下一个循环跳出 */ 
                }
                deQuotMarks(typeName);
                toLower(NULL, typeName);
                
                if (strcmp("table", typeName) == 0)
                {
                    statmode += TABLE;
                }
                else if (strcmp("histogram", typeName) == 0)
                {
                    statmode += HISTOGRAM;
                }
            }
            configOnfile.statmode = statmode;
        }
        else if (strcmp("resolution", key) == 0)
        {
            configOnfile.resolution = jsonGetCoord(value);
        }
        else if (strcmp("resx", key) == 0)
        {
            configOnfile.resolution.x = atoi(value);
        }
        else if (strcmp("resy", key) == 0)
        {
            configOnfile.resolution.y = atoi(value);
        }
        else if (strcmp("scrolltime", key) == 0)
        {
            configOnfile.scrolltime = atof(value);
        }
        else if (strcmp("fixtime", key) == 0)
        {
            configOnfile.fixtime = atof(value);
        }
        else if (strcmp("density", key) == 0)
        {
            configOnfile.density = atoi(value);
        }
        else if (strcmp("fontname", key) == 0)
        {
            strSafeCopy(configOnfile.fontname, value, FONTNAME_LEN);
        }
        else if (strcmp("fontsize", key) == 0)
        {
            configOnfile.fontsize = atoi(value);
        }
        else if (strcmp("opacity", key) == 0)
        {
            configOnfile.opacity = atoi(value);
        }
        else if (strcmp("outline", key) == 0)
        {
            configOnfile.outline = atof(value);
        }
        else if (strcmp("shadow", key) == 0)
        {
            configOnfile.shadow = atof(value);
        }
        else if (strcmp("bold", key) == 0)
        {
            configOnfile.bold = strToBool(value);
        }
        else if (strcmp("displayarea", key) == 0)
        {
            configOnfile.displayarea = atof(value);
        }
        else if (strcmp("scrollarea", key) == 0)
        {
            configOnfile.scrollarea = atof(value);
        }
        else if (strcmp("saveblocked", key) == 0)
        {
            configOnfile.saveBlockedPart = strToBool(value);
        }
        else if (strcmp("showusernames", key) == 0)
        {
            configOnfile.showUserNames = strToBool(value);
        }
        else if (strcmp("showmsgbox", key) == 0)
        {
            configOnfile.showMsgBox = strToBool(value);
        }
        else if (strcmp("msgboxfontsize", key) == 0)
        {
            configOnfile.msgboxFontsize = atoi(value);
        }
        else if (strcmp("msgboxduration", key) == 0)
        {
            configOnfile.msgboxDuration = atof(value);
        }
        else if (strcmp("giftminprice", key) == 0)
        {
            configOnfile.giftMinPrice = atof(value);
        }
        else if (strcmp("giftmergetolerance", key) == 0)
        {
            configOnfile.giftMergeTolerance = atof(value);
        }
        else if (strcmp("msgboxsize", key) == 0)
        {
            configOnfile.msgBoxSize = jsonGetCoord(value);
        }
        else if (strcmp("msgboxpos", key) == 0)
        {
            configOnfile.msgBoxPos = jsonGetCoord(value);
        }
    }
    
    return configOnfile;
}


/* 
 * 写配置文件
 * 参数：配置文件名/配置信息
 * 返回值：布尔值 是否成功
  */
BOOL writeConfig(const char *const configFileName, const CONFIG newConfig)
{
    FILE *fptr = fopen(configFileName, "w");
    
    char tempStr[MAX_TEXT_LENGTH];
    CONFIG newConfigCopy = newConfig;
    if (fptr == NULL)
    {
        return FALSE;
    }
    
    fprintf(fptr,
            "{\n"
            "    \"resolution\": [%d, %d],\n"
            "    \"scrolltime\": %f,\n"
            "    \"fixtime\": %f,\n"
            "    \"density\": %d,\n"
            "    \"fontname\": \"%s\",\n"
            "    \"fontsize\": %d,\n"
            "    \"opacity\": %d,\n"
            "    \"outline\": %.1f,\n"
            "    \"shadow\": %.1f,\n"
            "    \"displayArea\": %f,\n"
            "    \"scrollArea\": %f,\n"
            "    \"bold\": %s,\n"
            "    \"showUsernames\": %s,\n"
            "    \"showMsgbox\": %s,\n"
            "    \"msgboxSize\": [%d, %d],\n"
            "    \"msgboxPos\": [%d, %d],\n"
            "    \"msgboxFontsize\": %d,\n"
            "    \"msgboxDuration\": %.2f,\n"
            "    \"giftMinPrice\": %.2f,\n"
            "    \"giftMergeTolerance\": %.2f,\n",
            newConfigCopy.resolution.x, newConfigCopy.resolution.y,
            newConfigCopy.scrolltime,
            newConfigCopy.fixtime,
            newConfigCopy.density,
            newConfigCopy.fontname,
            newConfigCopy.fontsize,
            newConfigCopy.opacity,
            newConfigCopy.outline,
            newConfigCopy.shadow,
            newConfigCopy.displayarea,
            newConfigCopy.scrollarea,
            boolToStr(tempStr, newConfigCopy.bold),
            boolToStr(tempStr, newConfigCopy.showUserNames),
            boolToStr(tempStr, newConfigCopy.showMsgBox),
            newConfigCopy.msgBoxSize.x, newConfigCopy.msgBoxSize.y,
            newConfigCopy.msgBoxPos.x, newConfigCopy.msgBoxPos.y,
            newConfigCopy.msgboxFontsize,
            newConfigCopy.msgboxDuration,
            newConfigCopy.giftMinPrice,
            newConfigCopy.giftMergeTolerance
    );
    

    /* 写是否保存屏蔽部分 */
    if (newConfigCopy.saveBlockedPart == FALSE)
    {
        fprintf(fptr, "    \"saveblocked\": false, \n");
    }

    /* 写屏蔽模式 */
    {
        /* L2R, R2L, TOP, BOTTOM, SPECIAL, COLOR, REPEAT */
        fprintf(fptr, "    \"blockmode\": [");
        if (newConfigCopy.blockmode & BLK_L2R)
        {
            fprintf(fptr, "L2R");
            /* 判断是否需要逗号 */
            newConfigCopy.blockmode -= BLK_L2R;
            if (newConfigCopy.blockmode != 0)
            {
                fprintf(fptr, ", ");
            }
        }
        if (newConfigCopy.blockmode & BLK_R2L)
        {
            fprintf(fptr, "R2L");
            /* 判断是否需要逗号 */
            newConfigCopy.blockmode -= BLK_R2L;
            if (newConfigCopy.blockmode != 0)
            {
                fprintf(fptr, ", ");
            }
        }
        if (newConfigCopy.blockmode & BLK_TOP)
        {
            fprintf(fptr, "top");
            /* 判断是否需要逗号 */
            newConfigCopy.blockmode -= BLK_TOP;
            if (newConfigCopy.blockmode != 0)
            {
                fprintf(fptr, ", ");
            }
        }
        if (newConfigCopy.blockmode & BLK_BOTTOM)
        {
            fprintf(fptr, "bottom");
            /* 判断是否需要逗号 */
            newConfigCopy.blockmode -= BLK_BOTTOM;
            if (newConfigCopy.blockmode != 0)
            {
                fprintf(fptr, ", ");
            }
        }
        if (newConfigCopy.blockmode & BLK_SPECIAL)
        {
            fprintf(fptr, "special");
            /* 判断是否需要逗号 */
            newConfigCopy.blockmode -= BLK_SPECIAL;
            if (newConfigCopy.blockmode != 0)
            {
                fprintf(fptr, ", ");
            }
        }
        if (newConfigCopy.blockmode & BLK_COLOR)
        {
            fprintf(fptr, "color");
            /* 判断是否需要逗号 */
            newConfigCopy.blockmode -= BLK_COLOR;
            if (newConfigCopy.blockmode != 0)
            {
                fprintf(fptr, ", ");
            }
        }
        if (newConfigCopy.blockmode & BLK_REPEAT)
        {
            fprintf(fptr, "repeat");
            /* 判断是否需要逗号 */
            newConfigCopy.blockmode -= BLK_REPEAT;
            if (newConfigCopy.blockmode != 0)
            {
                fprintf(fptr, ", ");
            }
        }
        
        fprintf(fptr, "],\n");
    }
    
    /* 写统计模式 */
    {
        /* TABLE, HISTOGRAM */ 
        fprintf(fptr, "    \"statmode\": [");
        if (newConfigCopy.statmode & TABLE)
        {
            fprintf(fptr, "table");
            /* 判断是否需要逗号 */
            newConfigCopy.statmode -= TABLE;
            if (newConfigCopy.statmode != 0)
            {
                fprintf(fptr, ", ");
            }
            
        }
        if (newConfigCopy.statmode & HISTOGRAM)
        {
            fprintf(fptr, "histogram");
            /* 判断是否需要逗号 */
            newConfigCopy.statmode -= HISTOGRAM;
            if (newConfigCopy.statmode != 0)
            {
                fprintf(fptr, ", ");
            }
        }
        
        fprintf(fptr, "]\n");
    }
    
    fprintf(fptr, "}");
    
    if (ferror(fptr))
    {
        fclose(fptr);
        return FALSE; 
    }
    
    fclose(fptr);
    
    return TRUE;
}

/* 打印配置信息 */
void printConfig(CONFIG config)
{
    printf("\n"
           "Configuration:\n"
           "Resolution: %dx%d | ScrollTime: %.3f | FixTime: %.3f | Density: %d",
           config.resolution.x, config.resolution.y,
           config.scrolltime, config.fixtime, config.density
          );
    if (config.density == -1)
    {
        printf("(non-overlap)");
    }
    else if (config.density == 0)
    {
        printf("(unlimit)");
    }
    
    printf(" | Fontname: \"%s\" | Fontsize: %d | Opacity: %d | Outline: %.1f",
           config.fontname, config.fontsize, config.opacity, config.outline
          );
    if (fabs(config.outline) < EPS)
    {
        printf("(disable)");
    }
    
    printf(" | Shadow: %.1f", config.shadow);
    if (fabs(config.shadow) < EPS)
    {
        printf("(disable)");
    }

    printf(" | Bold: ");
    if (config.bold == FALSE)
    {
        printf("false");
    }
    else
    {
        printf("true");
    }
    
    printf(" | DisplayArea: %.3f", config.displayarea);
    if (fabs(config.displayarea - 1) < EPS)
    {
        printf("(full)");
    }
    
    printf(" | ScrollArea: %.3f", config.scrollarea);
    if (fabs(config.scrollarea - 1) < EPS)
    {
        printf("(full)");
    }

    printf(" | SaveBlocked: ");
    if (config.saveBlockedPart == FALSE)
    {
        printf("false");
    }
    else
    {
        printf("true");
    }

    printf(" | ShowUsernames: ");
    if (config.showUserNames == FALSE)
    {
        printf("false");
    }
    else
    {
        printf("true");
    }

    printf("\nShowMessageBox: ");
    if (config.showMsgBox == FALSE)
    {
        printf("false");
    }
    else
    {
        printf("true");
    }

    printf(" | MessageBoxSize: %dx%d | MessageBoxPosition: (%d, %d) | MessageBoxFontsize: %d | MessageBoxDuration: %.2f", 
           config.msgBoxSize.x, config.msgBoxSize.y, config.msgBoxPos.x, config.msgBoxPos.y, config.msgboxFontsize, config.msgboxDuration);

    printf(" | GiftMinPrice: CNY %.2f | GiftMergeTolerance: %.2f", config.giftMinPrice, config.giftMergeTolerance);
    
    printf("\nBlockMode: ");
    if (config.blockmode == 0)
    {
        printf("null(disable)");
    }
    else
    {
        if (config.blockmode & BLK_L2R)
        {
            printf("L2R(left to right) ");
        }
        if (config.blockmode & BLK_R2L)
        {
            printf("R2L(right to left) ");
        }
        if (config.blockmode & BLK_TOP)
        {
            printf("top ");
        }
        if (config.blockmode & BLK_BOTTOM)
        {
            printf("bottom ");
        }
        if (config.blockmode & BLK_SPECIAL)
        {
            printf("special ");
        }
        if (config.blockmode & BLK_COLOR)
        {
            printf("color ");
        }
        if (config.blockmode & BLK_REPEAT)
        {
            printf("repeat ");
        }
    }
    
    printf("\nStatMode:  ");
    if (config.statmode == 0)
    {
        printf("null(disable)");
    }
    else
    {
        if (config.statmode & TABLE)
        {
            printf("table ");
        }
        if (config.statmode & HISTOGRAM)
        {
            printf("histogram ");
        }
    }
    
    printf("\n");
    return;
}

/* 文本获取坐标 */
static COORDIN jsonGetCoord(const char *const jsonStr)
{
    char *jsonStrPtr = (char *)jsonStr;
    char number[MAX_TEXT_LENGTH];
    COORDIN returnValue;
    strGetLeftPart(NULL, &jsonStrPtr, '[', VALUE_LEN);
    strGetLeftPart(number, &jsonStrPtr, ',', MAX_TEXT_LENGTH);
    returnValue.x = atoi(number);
    strGetLeftPart(number, &jsonStrPtr, ']', MAX_TEXT_LENGTH);
    returnValue.y = atoi(number);

    return returnValue;
}
