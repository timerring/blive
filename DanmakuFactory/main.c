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

#ifdef _WIN32
/* windows下 */
#include <windows.h>
#include <io.h>
#define F_OK 0
#define W_OK 2
#define R_OK 4

#else
/* linux下 */
#include <unistd.h>

#endif

#include "CDanmakuFactory.h"
#include "Define/CLIDef.h"

void printHelpInfo();
int getArgNum(int argc, char **argv, const int optionIndex);
double getArgValDouble(int argc, char **argv, const int optIndex, const char *const optName, const double errorReturnValue);
BOOL getArgValBool(int argc, char **argv, const int optIndex, const char *const optName);
COORDIN getArgValCoodr(int argc, char **argv, const int optIndex, const char *const optName, int *argNum);
void toPause(BOOL skip);
BOOL isContinue(BOOL skip);

static CONFIG defaultConfig =
{
    {1920, 1080},    /* 分辨率 */
    1.00,           /* 显示区域 */ 
    1.00,           /* 滚动区域 */
    12.0,           /* 滚动时间 */ 
    5.0,            /* 固定时间 */

    0,              /* 弹幕密度 */
    38,             /* 字号 */
                    /* 字体 */
    "Microsoft YaHei", 
    180,            /* 不透明度 */ 
    0,              /* 描边 */ 
    1,              /* 阴影 */
    FALSE,          /* 是否加粗 */ 

    TRUE,           /* 是否保存屏蔽部分 */
    FALSE,          /* 是否显示用户名 */
    TRUE,           /* 是否显示消息框 */

    {500, 1080},    /* 消息框大小 */
    {20, 0},        /* 消息框位置 */
    38,             /* 消息框内文字大小 */
    0.0f,           /* 消息框持续时长 */
    0.0f,           /* 消息框礼物最低价格限制 */
    0.0f,           /* 消息框礼物合并时间窗 */

    0,              /* 屏蔽模式 */
    0,              /* 统计模式 */
};

int main(int argc, char **argv)
{
    FINFO outfile;
    FINFO *infile = NULL;
    int infileNum = 0;
    int argCnt = 1;
    int cnt;
    BOOL showConfig = FALSE;
    BOOL saveConfig = FALSE;
    BOOL configFileErr = FALSE;
    BOOL ignoreWarnings = FALSE;
    CONFIG config;
    char tempStr[MAX_TEXT_LENGTH], *tempPtr;
    char programPath[MAX_TEXT_LENGTH];
    char configFilePath[MAX_TEXT_LENGTH];
    outfile.isSet = FALSE;

    /* 打印程序版本信息 */
    printf("\nDanmakuFactory "VERSION" "EDITION" by hkm (hkm@tikm.org)"
           "\nhttps://github.com/hihkm/DanmakuFactory\n"
          );

    /* 获取程序运行目录 */
    tempStr[0] = '\0';
#ifdef _WIN32
    GetModuleFileName(0, tempStr, MAX_TEXT_LENGTH);
#else
    readlink("/proc/self/exe", tempStr, MAX_TEXT_LENGTH);
#endif
    filenameGetPath(programPath, tempStr, MAX_TEXT_LENGTH);

    /* 获取配置文件路径 */
    strSafeCopy(configFilePath, programPath, MAX_TEXT_LENGTH);
    strSafeCat(configFilePath, MAX_TEXT_LENGTH, CONFIG_FILE_NAME);
    
    if (strstr(configFilePath, CONFIG_FILE_NAME) == NULL)
    {
        printf("\nWARNING"
               "\nOut of buffer.");
        printf("\nNOTE"
               "\nFail to get config file path, because the path is too long.\n");
        
        configFileErr = TRUE;
        toPause(ignoreWarnings);
    }
    
    /* 解析参数 */
    if (argc <= 1)
    {
        printHelpInfo();
        return 0;
    }
    else
    {
        /* 读配置文件 */
        config = readConfig(configFilePath, defaultConfig);
        
        /* 遍历参数 */ 
        while (argCnt < argc)
        {
            if (!strcmp("-h", argv[argCnt]) || !strcmp("--help", argv[argCnt]))
            {
                printHelpInfo();
                return 0;
            }
            else if (!strcmp("-c", argv[argCnt]) || !strcmp("--config", argv[argCnt]))
            {
                showConfig = TRUE;
                argCnt += 1;
            }
            else if (!strcmp("--save", argv[argCnt]))
            {
                saveConfig = TRUE;
                showConfig = TRUE;
                argCnt += 1;
            }
            else if (!strcmp("-o", argv[argCnt]) || !strcmp("--output", argv[argCnt]))
            { /* 输出文件名 */
                switch (getArgNum(argc, argv, argCnt))
                {
                    case 0:
                        fprintf(stderr, "\nERROR"
                                        "\nOutput file must be specified.\n");
                        return 0;
                        break;
                    case 1:
                        strSafeCopy(outfile.fileName, argv[argCnt+1], FILENAME_LEN);
                        filenameGetFormat(outfile.template, outfile.fileName, FORMAT_LEN);
                        argCnt += 2;
                        break;
                    case 2:
                        strSafeCopy(outfile.template, argv[argCnt+1], FORMAT_LEN);
                        strSafeCopy(outfile.fileName, argv[argCnt+2], FILENAME_LEN);
                        argCnt += 3;
                        break;
                    default:
                        /* 传入未知参数丢给下一轮获取选项时报错 */
                        argCnt += 3;
                        break;
                }
                
                deQuotMarks(outfile.fileName);
                deQuotMarks(outfile.template);
                outfile.isSet = TRUE;
                
                /* 合法性检查 */ 
                if (!ISFORMAT(outfile.template))
                {
                    fprintf(stderr, "\nERROR"
                                    "\nUnknow format \"%s\".\n", outfile.template);
                    
                    return 0;
                }
            }
            else if (!strcmp("-i", argv[argCnt]) || !strcmp("--input", argv[argCnt]))
            { /* 输入文件名 */ 
                int num = getArgNum(argc, argv, argCnt);
                for (cnt = 0; cnt < num; cnt++)
                {
                    if ((infile = (FINFO *)realloc(infile, (infileNum+1) * sizeof(FINFO))) == NULL)
                    {
                        fprintf(stderr, "\nERROR"
                                        "\nOut of memory.\n");
                        return 0;
                    }
                    
                    strSafeCopy(tempStr, argv[argCnt + cnt + 1], MAX_TEXT_LENGTH);
                    deQuotMarks(tempStr);

                    if (ISFORMAT(tempStr))
                    {
                        strcpy(infile[infileNum].template, tempStr);
                    }
                    else
                    {
                        // sprintf_s(infile[infileNum].template, FILENAME_LEN, "%stemplates\\%s.txt", programPath, tempStr);
                        // (infile[infileNum].template)[FILENAME_LEN-1] = '\0';

                        strSafeCopy(infile[infileNum].template, programPath, FILENAME_LEN);
                        strSafeCat(infile[infileNum].template, FILENAME_LEN, "templates\\");
                        strSafeCat(infile[infileNum].template, FILENAME_LEN, tempStr);
                        strSafeCat(infile[infileNum].template, FILENAME_LEN, ".txt");
                    }

                    if (ISFORMAT(tempStr) || access(infile[infileNum].template, F_OK) == 0)
                    {
                        cnt++;
                        if (cnt >= num)
                        {
                            fprintf(stderr, "\nERROR"
                                            "\nTemplate must be followed by a filename.\n");
                            return 0;
                        }
                        
                        strcpy(infile[infileNum].fileName, argv[argCnt + cnt + 1]);
                    }
                    else
                    {
                        strcpy(infile[infileNum].fileName, argv[argCnt + cnt + 1]);
                        filenameGetFormat(infile[infileNum].template, infile[infileNum].fileName, FORMAT_LEN);

                        /* 合法性检查 */
                        if (!ISFORMAT(infile[infileNum].template))
                        {
                            fprintf(stderr, "\nERROR"
                                            "\nUnknow Template %s.\n", infile[infileNum].template);
                            return 0;
                        }
                    }
                    
                    infile[infileNum].timeShift = 0.0f;
                    deQuotMarks(infile[infileNum].fileName);

                    infileNum++;
                }
                
                argCnt += num + 1;
            }
            else if (!strcmp("-r", argv[argCnt]) || !strcmp("--resolution", argv[argCnt]))
            { /* 分辨率 */
                int argNum; /* 用空格分隔也是合法的，因此可能有两个参数，需要获取并按实况跳过 */
                COORDIN returnValue = getArgValCoodr(argc, argv, argCnt, "Resolution", &argNum);
                if (argNum == -1)
                {
                    return 0;
                }
                config.resolution = returnValue;
                
                argCnt += argNum + 1; /* 参数数量以及选项本体 */
            }
            else if (!strcmp("-x", argv[argCnt]) || !strcmp("--resx", argv[argCnt]))
            { /* 旧版本兼容 分辨率宽 */
                double returnValue = getArgValDouble(argc, argv, argCnt, "Resx", -256.00);
                if (fabs(returnValue - (-256.0)) < EPS)
                {
                    return 0;
                }
                config.resolution.x = (int)returnValue;
                
                argCnt += 2; 
            }
            else if (!strcmp("-y", argv[argCnt]) || !strcmp("--resy", argv[argCnt]))
            { /* 旧版本兼容 分辨率高 */
                double returnValue = getArgValDouble(argc, argv, argCnt, "Resy", -256.00);
                if (fabs(returnValue - (-256.0)) < EPS)
                {
                    return 0;
                }
                config.resolution.y = (int)returnValue;
                
                argCnt += 2; 
            }
            else if (!strcmp("-s", argv[argCnt]) || !strcmp("--scrolltime", argv[argCnt]))
            { /* 滚动时间 */
                double returnValue = getArgValDouble(argc, argv, argCnt, "ScrollTime", -256.00);
                if (fabs(returnValue - (-256.0)) < EPS)
                {
                    return 0;
                }
                config.scrolltime = (float)returnValue;
                
                argCnt += 2; 
            }
            else if (!strcmp("-f", argv[argCnt]) || !strcmp("--fixtime", argv[argCnt]))
            { /* 固定时间 */
                double returnValue = getArgValDouble(argc, argv, argCnt, "FixTime", -256.00);
                if (fabs(returnValue - (-256.0)) < EPS)
                {
                    return 0;
                }
                config.fixtime = (float)returnValue;
                
                argCnt += 2; 
            }
            else if (!strcmp("-d", argv[argCnt]) || !strcmp("--density", argv[argCnt]))
            { /* 弹幕密度 */
                double returnValue = getArgValDouble(argc, argv, argCnt, "Density", -256.00);
                if (fabs(returnValue - (-256.0)) < EPS)
                {
                    return 0;
                }
                config.density = (int)returnValue;
                
                argCnt += 2; 
            }
            else if (!strcmp("-S", argv[argCnt]) || !strcmp("--fontsize", argv[argCnt]))
            { /* 字号 */
                double returnValue = getArgValDouble(argc, argv, argCnt, "Fontsize", -256.00);
                if (fabs(returnValue - (-256.0)) < EPS)
                {
                    return 0;
                }
                config.fontsize = (int)returnValue;
                
                argCnt += 2; 
            }
            else if (!strcmp("-N", argv[argCnt]) || !strcmp("--fontname", argv[argCnt]))
            { /* 字号 */
                switch (getArgNum(argc, argv, argCnt))
                {
                    case 0:
                        fprintf(stderr, "\nERROR"
                                        "\nFontname must be specified.\n");
                        return 0;
                        break;
                    case 1:
                        strSafeCopy(config.fontname, argv[argCnt+1], FONTNAME_LEN);
                        deQuotMarks(config.fontname);
                        break;
                    default:
                        fprintf(stderr, "\nERROR"
                                        "\nInvalid argument \"%s\".\n", argv[argCnt+2]);
                        return 0;
                        break;
                }

                argCnt += 2; 
            }
            else if (!strcmp("-O", argv[argCnt]) || !strcmp("--opacity", argv[argCnt]))
            { /* 不透明度 */
                double returnValue = getArgValDouble(argc, argv, argCnt, "Opacity", -256.00);
                if (fabs(returnValue - (-256.0)) < EPS)
                {
                    return 0;
                }
                config.opacity = (int)returnValue;
                
                argCnt += 2; 
            }
            else if (!strcmp("-L", argv[argCnt]) || !strcmp("--outline", argv[argCnt]))
            { /* 描边 */
                double returnValue = getArgValDouble(argc, argv, argCnt, "Outline", -256.00);
                if (fabs(returnValue - (-256.0)) < EPS)
                {
                    return 0;
                }
                config.outline = returnValue;
                
                argCnt += 2; 
            }
            else if (!strcmp("-D", argv[argCnt]) || !strcmp("--shadow", argv[argCnt]))
            { /* 阴影 */
                double returnValue = getArgValDouble(argc, argv, argCnt, "Shadow", -256.00);
                if (fabs(returnValue - (-256.0)) < EPS)
                {
                    return 0;
                }
                config.shadow = returnValue;
                
                argCnt += 2; 
            }
            else if (!strcmp("-B", argv[argCnt]) || !strcmp("--bold", argv[argCnt]))
            { /* 是否加粗 */
                BOOL returnValue = getArgValBool(argc, argv, argCnt, "Bold");
                if (returnValue == BOOL_UNDETERMINED)
                {
                    return 0;
                }
                config.bold = returnValue;
                
                argCnt += 2;
            }
            else if (!strcmp("--displayarea", argv[argCnt]))
            { /* 显示区域 */
                double returnValue = getArgValDouble(argc, argv, argCnt, "DisplayArea", -256.00);
                if (fabs(returnValue - (-256.0)) < EPS)
                {
                    return 0;
                }
                config.displayarea = (float)returnValue;
                
                argCnt += 2; 
            }
            else if (!strcmp("--scrollarea", argv[argCnt]))
            { /* 滚动区域 */
                double returnValue = getArgValDouble(argc, argv, argCnt, "ScrollArea", -256.00);
                if (fabs(returnValue - (-256.0)) < EPS)
                {
                    return 0;
                }
                config.scrollarea = (float)returnValue;
                
                argCnt += 2; 
            }
            else if (!strcmp("-b", argv[argCnt]) || !strcmp("--blockmode", argv[argCnt]))
            { /* 屏蔽模式 */
                switch (getArgNum(argc, argv, argCnt))
                {
                    case 0:
                        fprintf(stderr, "\nERROR"
                                        "\n\"Blockmode\" must be specified.\n");
                        return 0;
                        break;
                    case 1:
                        break;
                    default:
                        fprintf(stderr, "\nERROR"
                                        "\nInvalid argument \"%s\".\n", argv[argCnt+2]);
                        return 0; 
                        break;
                }
                
                config.blockmode = 0;
                if (!strcmp("null", argv[argCnt+1]))
                {
                    argCnt += 2;
                    continue;
                }
                
                cnt = 0;
                char *argPtr = argv[argCnt+1];
                while (*argPtr != '\0')
                {
                    tempPtr = tempStr;
                    while (*argPtr != '\0' && *argPtr != '-' && cnt < MAX_TEXT_LENGTH)
                    {
                        *tempPtr = *argPtr;
                        tempPtr++;
                        argPtr++;
                        cnt++;
                    }
                    if (*argPtr == '-')
                    {
                        argPtr++;
                    }
                    *tempPtr = '\0';
                    
                    deQuotMarks(tempStr);
                    toLower(NULL, tempStr);
                    if (!strcmp("l2r", tempStr))
                    {
                        config.blockmode += BLK_L2R;
                    }
                    else if (!strcmp("r2l", tempStr))
                    {
                        config.blockmode += BLK_R2L;
                    }
                    else if (!strcmp("top", tempStr))
                    {
                        config.blockmode += BLK_TOP;
                    }
                    else if (!strcmp("bottom", tempStr))
                    {
                        config.blockmode += BLK_BOTTOM;
                    }
                    else if (!strcmp("special", tempStr))
                    {
                        config.blockmode += BLK_SPECIAL;
                    }
                    else if (!strcmp("color", tempStr) || !strcmp("colour", tempStr))
                    {
                        config.blockmode += BLK_COLOR;
                    }
                    else if (!strcmp("repeat", tempStr))
                    {
                        config.blockmode += BLK_REPEAT;
                    }
                    else
                    {
                        fprintf(stderr, "\nERROR"
                                        "\nInvalid type-name \"%s\".\n", tempStr);
                        return 0;
                    }
                }
                
                argCnt += 2;
            }
            else if (!strcmp("--statmode", argv[argCnt]))
            { /* 统计模式 */
                switch (getArgNum(argc, argv, argCnt))
                {
                    case 0:
                        fprintf(stderr, "\nERROR"
                                        "\n\"Statmode\" must be specified.\n");
                        return 0;
                        break;
                    case 1:
                        break;
                    default:
                        fprintf(stderr, "\nERROR"
                                        "\nInvalid argument \"%s\".\n", argv[argCnt+2]);
                        return 0; 
                        break;
                }
                
                config.statmode = 0;
                if (!strcmp("null", argv[argCnt+1]))
                {
                    argCnt += 2;
                    continue;
                }
                
                cnt = 0;
                char *argPtr = argv[argCnt+1];
                while (*argPtr != '\0')
                {
                    tempPtr = tempStr;
                    while (*argPtr != '\0' && *argPtr != '-' && cnt < MAX_TEXT_LENGTH)
                    {/* 拷贝 - 之前的字符 */
                        *tempPtr = *argPtr;
                        tempPtr++;
                        argPtr++;
                        cnt++;
                    }
                    if (*argPtr == '-')
                    {
                        argPtr++;
                    }
                    *tempPtr = '\0';
                    /* 字符串比对 */
                    deQuotMarks(tempStr);
                    toLower(NULL, tempStr);
                    if (!strcmp("table", tempStr))
                    {
                        config.statmode += TABLE;
                    }
                    else if (!strcmp("histogram", tempStr))
                    {
                        config.statmode += HISTOGRAM;
                    }
                    else
                    {
                        fprintf(stderr, "\nERROR"
                                        "\nInvalid type-name \"%s\".\n", tempStr);
                        return 0; 
                    }
                }
                
                argCnt += 2; 
            }
            else if (!strcmp("-t", argv[argCnt]) || !strcmp("--timeshift", argv[argCnt]))
            { /* 时轴偏移 因不确定文件数量，故先跳过，最后解析 */
                argCnt++;
                while (argCnt < argc && ISNUMBERIC(argv[argCnt]))
                {
                    argCnt++;
                }
            }
            else if (!strcmp("--saveblocked", argv[argCnt]))
            { /* 是否保存屏蔽部分 */
                BOOL returnValue = getArgValBool(argc, argv, argCnt, "SaveBlocked");
                if (returnValue == BOOL_UNDETERMINED)
                {
                    return 0;
                }
                config.saveBlockedPart = returnValue;
                
                argCnt += 2;
            }
            else if (!(strcmp("--showusernames", argv[argCnt])))
            { /* 是否显示用户名 */
                BOOL returnValue = getArgValBool(argc, argv, argCnt, "showUsernames");
                if (returnValue == BOOL_UNDETERMINED)
                {
                    return 0;
                }
                config.showUserNames = returnValue;
                
                argCnt += 2; 
            }
            else if (!(strcmp("--showmsgbox", argv[argCnt])))
            { /* 是否显示消息框 */
                BOOL returnValue = getArgValBool(argc, argv, argCnt, "ShowMsgbox");
                if (returnValue == BOOL_UNDETERMINED)
                {
                    return 0;
                }
                config.showMsgBox = returnValue;
                
                argCnt += 2;
            }
            else if (!(strcmp("--msgboxfontsize", argv[argCnt])))
            { /* 消息框文字大小 */
                double returnValue = getArgValDouble(argc, argv, argCnt, "MsgboxFontsize", 0);
                config.msgboxFontsize = (int)returnValue;
                
                argCnt += 2; 
            }
            else if (!(strcmp("--msgboxduration", argv[argCnt])))
            { /* 消息框持续时长 */
                double returnValue = getArgValDouble(argc, argv, argCnt, "MsgboxDuration", 0);
                config.msgboxDuration = (float)returnValue;
                
                argCnt += 2; 
            }
            else if (!(strcmp("--giftminprice", argv[argCnt])))
            { /* 按最低礼物价格屏蔽 */
                double returnValue = getArgValDouble(argc, argv, argCnt, "GiftMinPrice", 0);
                config.giftMinPrice = (float)returnValue;
                
                argCnt += 2; 
            }
            else if (!(strcmp("--giftmergetolerance", argv[argCnt])))
            { /* 相同用户相同礼物合并时间窗 */
                double returnValue = getArgValDouble(argc, argv, argCnt, "GiftMergeTolerance", 0);
                config.giftMergeTolerance = (float)returnValue;
                
                argCnt += 2; 
            }
            else if (!strcmp("--msgboxsize", argv[argCnt]))
            { /* 消息框大小 */
                int argNum; /* 用空格分隔也是合法的，因此可能有两个参数，需要获取并按实况跳过 */
                COORDIN returnValue = getArgValCoodr(argc, argv, argCnt, "MsgboxSize", &argNum);
                if (argNum == -1)
                {
                    return 0;
                }
                config.msgBoxSize = returnValue;
                
                argCnt += argNum + 1; /* 参数数量以及选项本体 */
            }
            else if (!strcmp("--msgboxpos", argv[argCnt]))
            { /* 消息框位置 */
                int argNum; /* 用空格分隔也是合法的，因此可能有两个参数，需要获取并按实况跳过 */
                COORDIN returnValue = getArgValCoodr(argc, argv, argCnt, "MsgboxPos", &argNum);
                if (argNum == -1)
                {
                    return 0;
                }
                config.msgBoxPos = returnValue;
                
                argCnt += argNum + 1; /* 参数数量以及选项本体 */
            }

            else if (!(strcmp("--ignore-warnings", argv[argCnt])))
            { /* 跳过全部警告*/
                ignoreWarnings = TRUE;
                argCnt++;
            }
            else if (!(strcmp("--check-version-"VERSION, argv[argCnt])))
            { /* 为GUI提供程序版本确认 如果是错误的版本将会报非法参数错误 */
                argCnt++;
            }
            else
            {
                fprintf(stderr, "\nERROR"
                                "\nInvalid argument %s\n", argv[argCnt]);
                return 0;
            }
        }
        
        /* 寻找并解析时间平移量 */ 
        for (argCnt = 0; argCnt < argc; argCnt++)
        {
            if (!strcmp("-t", argv[argCnt]) || !strcmp("--timeshift", argv[argCnt]))
            {
                argCnt++;
                for (cnt = 0; cnt < infileNum; cnt++)
                {
                    if (argCnt >= argc || !ISNUMBERIC(argv[argCnt]))
                    {
                        fprintf(stderr, "\nERROR"
                                        "\nToo few values in option timeshift(-t, --timeshift).");
                        fprintf(stderr, "\nNOTE"
                                        "\n%d filenames are provided, but only %d timeshift value.\n",
                               infileNum, cnt);
                        return 0;
                    }
                    
                    strSafeCopy(tempStr, argv[argCnt], MAX_TEXT_LENGTH);
                    deQuotMarks(tempStr);
                    infile[cnt].timeShift = atof(tempStr);
                    
                    argCnt++;
                }
                
                /* 有更多的数字意味着语法错误 */
                if (argCnt < argc && ISNUMBERIC(argv[argCnt]))
                {
                    fprintf(stderr, "\nERROR"
                                    "\nToo many values in option timeshift(-t, --timeshift).");
                    fprintf(stderr, "\nNOTE"
                                    "\nOnly %d filename(s) are provided, but more than %d timeshift values.\n",
                           infileNum, infileNum);
                    return 0;
                }
                
                /* 解析完毕即跳出循环 */
                break;
            }
        }  
    }
    
    /* 显示配置信息 */
    if (showConfig == TRUE)
    {
        printConfig(config);
    }

    /* 配置项合法性检查 */
    {
        /* 分辨率 */
        if (config.resolution.x <= 0 || config.resolution.y <= 0)
        {
            fprintf(stderr, "\nERROR"
                            "\n\"Resolution\" must be an integer greater than 0.\n");
            return 0;
        }
        /* 滚动时间 */
        if (config.scrolltime < EPS)
        {
            fprintf(stderr, "\nERROR"
                            "\n\"ScrollTime\" must be a real number greater than 0.00.\n");
            return 0;
        }
        /* 固定时间 */
        if (config.fixtime < EPS)
        {
            fprintf(stderr, "\nERROR"
                            "\n\"FixTime\" must be a real number greater than 0.00.\n");
            return 0;
        }
        /* 密度 */
        if (config.density < -1)
        {
            fprintf(stderr, "\nERROR"
                            "\n\"Density\" must be an integer greater than or equal to -1.\n");
            return 0;
        }
        /* 字号 */
        if (config.fontsize <= 0)
        {
            fprintf(stderr, "\nERROR"
                            "\n\"Fontsize\" must be an integer greater than 0.\n");
            return 0;
        }
        /* 不透明度 */
        if (config.opacity <= 0 || config.opacity > 255)
        {
            fprintf(stderr, "\nERROR"
                            "\n\"Opacity\" must be an integer greater than 0 and less than or equal to 255.\n");
            return 0;
        }
        /* 描边 */
        if (config.outline < 0.0 || config.outline > 4.0)
        {
            fprintf(stderr, "\nERROR"
                            "\n\"Outline\" must be an float greater than or equal to 0 and less than or equal to 4.\n");
            return 0;
        }
        /* 阴影 */
        if (config.shadow < 0.0|| config.shadow > 4.0)
        {
            fprintf(stderr, "\nERROR"
                            "\n\"Shadow\" must be an float greater than or equal to 0 and less than or equal to 4.\n");
            return 0;
        }
        /* 显示区域 */
        if (config.displayarea < EPS || config.displayarea > 1.0 + EPS)
        {
            fprintf(stderr, "\nERROR"
                            "\n\"DisplayArea\" must be a real number greater than 0.0 and less than or equal to 1.0.\n");
            return 0;
        }
        /* 滚动区域 */
        if (config.scrollarea < EPS || config.scrollarea > 1.0 + EPS)
        {
            fprintf(stderr, "\nERROR"
                            "\n\"ScrollArea\" must be a real number greater than 0.0 and less than or equal to 1.0.\n");
            return 0;
        }
        /* 字体 */
        tempPtr = config.fontname;
        while (*tempPtr != '\0')
        {/* 验证是否全部为可打印的ascii字符 */
            if (*tempPtr < 0x20 || *tempPtr > 0x7e)
            {/* ascii 非可打印字符范围 */
                printf("\nWARNING"
                    "\nSome characters of fontname are non-ASCII characters, which may cause the garbled problem.\n");
                toPause(ignoreWarnings);
                break;
            }

            tempPtr++;
        }
        
        /* 消息框文字大小 */
        if (config.msgboxFontsize <= 0)
        {
            fprintf(stderr, "\nERROR"
                            "\n\"MsgboxFontsize\" must be an integer greater than 0.\n");
            return 0;
        }
        /* 消息框大小 */
        if (config.msgBoxSize.x <= config.msgboxFontsize || config.msgBoxSize.y <= config.msgboxFontsize)
        {
            fprintf(stderr, "\nERROR"
                            "\n\"MsgBoxSize\" must be an integer greater than the \"msgboxFontsize\".\n");
            return 0;
        }
    }
    
    /* 保存配置文件 */
    if (saveConfig == TRUE && configFileErr == FALSE)
    {
        if (writeConfig(configFilePath, config) == TRUE)
        {
            printf("\nConfiguration file had been saved successfully!\n");
        }
        else
        {
            printf("\nWARNING"
                   "\nFailed to write the configuration file!\n");
            
            toPause(ignoreWarnings);
        }
    }
    
    /* 显示文件信息 */
    if (outfile.isSet == FALSE)
    {
        fprintf(stderr, "\nERROR"
                        "\nOutput file must be specified.");
        fprintf(stderr, "\nNOTE"
                        "\nUse -o or --output to specify.\n");
        return 0;
    }
    if (infileNum == 0)
    {
        fprintf(stderr, "\nERROR"
                        "\nInput file must be specified.");
        fprintf(stderr, "\nNOTE"
                        "\nUse -i or --input to specify.\n");
        return 0;
    }
    
    printf("\nInput file(s):");
    printf("\nNumber|Template     |TimeShift|FileName\n");
    for (cnt = 0; cnt < infileNum; cnt++)
    {
        strSafeCopy(tempStr, infile[cnt].template, MAX_TEXT_LENGTH);
        if (strlen(infile[cnt].template) > 10)
        {
            strcpy(tempStr + 10, "...");
            
        }
        printf("%6d|%-13s|%8.3fs|%s\n", cnt+1, tempStr, infile[cnt].timeShift, infile[cnt].fileName);
    }
    
    printf("\nOutput file:");
    printf("\nFormat|FileName");
    printf("\n%6s|%s\n", outfile.template, outfile.fileName);
    
    /* 读取文件 */
    printf("\nLoading files...\n");
    
    int returnValue;
    STATUS status;
    DANMAKU *danmakuPool = NULL;
    
    status.totalNum = 0;
    for (cnt = 0; cnt < infileNum; cnt++)
    {
        printf("Loading file \"%s\"\n", infile[cnt].fileName);
        /* 检查文件是否存在 */
        if (access(infile[cnt].fileName, F_OK) != 0)
        {
            fprintf(stderr, "\nERROR"
                            "\nNo such file.\n");
            return 0;
        }

        /* 权限检查 */
        if (access(infile[cnt].fileName, R_OK) != 0)
        {
            fprintf(stderr, "\nERROR"
                            "\nPermission denied.\n");
            return 0;
        }
        
        if (!strcmp("xml", infile[cnt].template))
        {
            returnValue = readXml(infile[cnt].fileName, &danmakuPool, "a", infile[cnt].timeShift, &status);
            
            switch (returnValue)
            {
                case 0:
                    break;
                case 1:
                    fprintf(stderr, "\nERROR [code rx%d]"
                                    "\nFailed to open file \"%s\".\n",
                           returnValue, infile[cnt].fileName
                          );
                    return 0;
                    break;
                case 2:
                case 3:
                case 4:
                    fprintf(stderr, "\nERROR [code rx%d]"
                                    "\nFailed to read file \"%s\".\n",
                           returnValue, infile[cnt].fileName
                          );
                    return 0;
                    break;
                case 5:
                case 6:
                case 7:
                    fprintf(stderr, "\nERROR [code rx%d]"
                                    "\nOut of memory.\n",
                           returnValue
                          );
                    return 0;
                    break;
                case 8:
                    printf("\nWARNING [code rx%d]"
                           "\nIncorrect file format, contiune or exit?",
                           returnValue
                          );
                    printf("\nNOTE"
                           "\nCould not load file \"%s\" as a xml file.\n", infile[cnt].fileName);
                    if (isContinue(ignoreWarnings) == FALSE)
                    {
                        return 0;
                    }
                    break;
                default:
                    fprintf(stderr, "\nERROR [code rx%d]"
                                    "\nUndefined Error\n",
                           returnValue
                          );
                    return 0;
                    break;
            }
        }
        else if (!strcmp("json", infile[cnt].template))
        {
            returnValue = readJson(infile[cnt].fileName, &danmakuPool, "a", infile[cnt].timeShift, &status);
            
            switch (returnValue)
            {
                case 0:
                    break;
                case 1:
                    fprintf(stderr, "\nERROR [code rj%d]"
                                    "\nFailed to open file \"%s\".\n",
                           returnValue, infile[cnt].fileName
                          );
                    return 0;
                    break;
                case 2:
                case 3:
                case 4:
                    fprintf(stderr, "\nERROR [code rj%d]"
                                    "\nFailed to read file \"%s\".\n",
                           returnValue, infile[cnt].fileName
                          );
                    return 0;
                    break;
                case 5:
                case 6:
                    fprintf(stderr, "\nERROR [code rj%d]"
                                    "\nOut of memory.\n",
                           returnValue
                          );
                    return 0;
                    break;
                case 7:
                    printf("\nWARNING [code rj%d]"
                           "\nIncorrect file format, contiune or exit?",
                           returnValue
                          );
                    printf("\nNOTE"
                           "\nCould not load file \"%s\" as a json file.\n", infile[cnt].fileName);
                    if (isContinue(ignoreWarnings) == FALSE)
                    {
                        return 0;
                    }
                    break;
                default:
                    fprintf(stderr, "\nERROR [code rj%d]"
                                    "\nUndefined Error.\n",
                           returnValue
                          );
                    return 0;
                    break;
            }
        }
        else if (!strcmp("ass", infile[cnt].template))
        {
            returnValue = readAss(infile[cnt].fileName, &danmakuPool, "a", NULL, infile[cnt].timeShift, &status);
            /* 解析十位错误码 */
            switch (returnValue / 10)
            {
                case 0:
                    break;
                case 1:
                    fprintf(stderr, "\nERROR [code ra%d]"
                                    "\nFailed to open file \"%s\".\n",
                           returnValue, infile[cnt].fileName
                          );
                    return 0;
                    break;
                case 2:
                case 3:
                case 4:
                    fprintf(stderr, "\nERROR [code ra%d]"
                                    "\nOut of memory.\n",
                           returnValue
                          );
                    return 0;
                    break;
                default:
                    fprintf(stderr, "\nERROR [code ra%d]"
                                    "\nUndefined Error.\n",
                           returnValue
                          );
                    return 0;
                    break;
            }
            /* 解析个位 */
            switch (returnValue % 10)
            {
                case 0:
                    break;
                case 1:
                case 2:
                case 3:
                case 4:
                case 5:
                case 6:
                    fprintf(stderr, "\nERROR [code ra%d]"
                                    "\nOut of memory.\n",
                           returnValue
                          );
                    return 0;
                    break;
                default:
                    fprintf(stderr, "\nERROR [code ra%d]"
                                    "\nUndefined Error.\n",
                           returnValue
                          );
                    return 0;
                    break;
            }
        }
        else
        {/* 使用用户自定义模板 */
            returnValue = readTemplateFile(infile[cnt].fileName, infile[cnt].template, &danmakuPool, "a", infile[cnt].timeShift,
                             &status, tempStr, MAX_TEXT_LENGTH);
            
            if (returnValue > 0)
            {
                switch (returnValue)
                {
                case 1:
                case 3:
                    /* 程序内部错误 */
                    fprintf(stderr, "\nERROR [code rt%d]"
                                    "\nFailed to open template file %s.\n",
                            returnValue, infile[cnt].template
                            );
                    return 0;
                    break;
                case 2:
                case 4:
                case 5:
                    /* 程序内部错误 */
                    fprintf(stderr, "\nERROR [code rt%d]"
                                    "\nOut of memory.\n",
                            returnValue
                            );
                    return 0;
                    break;
                default:
                    /* 程序内部错误 */
                    fprintf(stderr, "\nERROR [code rt%d]"
                                    "\nUndefined Error.\n",
                            returnValue
                            );
                    return 0;
                    break;
                }
            }
            else if (returnValue < 0)
            {
                /* 模板文件语法错误 */
                /* 程序内部错误 */
                fprintf(stderr, "\nERROR [code rt%d]"
                                "\nFormat File Syntax error."
                                "\nFilename: %s"
                                "\n%s",
                        returnValue, infile[cnt].template, tempStr
                        );
                printf("\nexit...");
                return 0;
            }
        }
        
    }

    /* 屏蔽 */
    blockByType(danmakuPool, config.blockmode, NULL);
    
    /* 读完成提示 */
    printf("\nFile Loading Complete.");
    if (status.totalNum != 0)
    {
        printf(". Danmaku Pool: %d\n", status.totalNum);
    }
    else
    {
        printf(" and nothing was read.\nexit...\n");
        return 0;
    }
    
    /* 排序 */
    printf ("\nSorting...\n");
    returnValue = sortList(&danmakuPool, NULL);
    switch (returnValue)
    {
        case 0:
            break;
        case 2:
            fprintf(stderr, "\nERROR [code s%d]"
                            "\nOut of memory.\n",
                   returnValue
                  );
            return 0;
            break;
        default:
            fprintf(stderr, "\nERROR [code s%d]"
                            "\nUndefined Error.\n",
                   returnValue
                  );
            break;
    }
    
    /* 写文件 */
    printf ("\nWritting file \"%s\"...\n", outfile.fileName);
    if (access(outfile.fileName, F_OK) == 0)
    {/* 检查文件是否存在 */
        printf ("\nWARNING"
                "\nFile \"%s\" already exists, it will be overwritten when continue.\n", outfile.fileName);
        if (isContinue(ignoreWarnings) == FALSE)
        {
            return 0;
        }
        
        /* 权限检查 */
        if (access(outfile.fileName, W_OK) != 0)
        {
            fprintf(stderr, "\nERROR"
                            "\nPermission denied.\n");
            return 0;
        }
    }
    
    if (!strcmp("ass", outfile.template))
    {
        returnValue = writeAss(outfile.fileName, danmakuPool, config, NULL, NULL);
        /* 解析百位 */
        switch (returnValue / 100)
        {
            case 0:
                break;
            case 1:
                fprintf(stderr, "\nERROR [code wa%d]"
                                "\nFailed to create file \"%s\".\n",
                       returnValue, outfile.fileName
                      );
                return 0;
                break;
            default:
                fprintf(stderr, "\nERROR [code wa%d]"
                                "\nUndefined Error.\n",
                       returnValue
                      );
                break;
        }
        /* 解析十位 */
        returnValue -= returnValue % 100;
        switch (returnValue % 10)
        {
            case 0:
                break;
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
            case 8:
                fprintf(stderr, "\nERROR [code wa%d]"
                                "\nOut of memory.\n",
                       returnValue
                      );
                return 0;
                break;
            case 9:
                    fprintf(stderr, "\nERROR [code wa%d]"
                                    "\nFailed to write file \"%s\".\n",
                           returnValue, infile[cnt].fileName
                          );
                    return 0;
                    break;
            default:
                fprintf(stderr, "\nERROR [code wa%d]"
                                "\nUndefined Error.\n",
                       returnValue
                      );
                break;
        }
        /* 个位函数错误信息已经被以上过滤完毕 无需重新报错 */
    }
    else if (!strcmp("xml", outfile.template))
    {
        returnValue = writeXml(outfile.fileName, danmakuPool, NULL);
        switch (returnValue)
        {
            case 0:
                break;
            case 2:
                fprintf(stderr, "\nERROR [code wx%d]"
                                "\nFailed to create file \"%s\".\n",
                       returnValue, outfile.fileName
                      );
                return 0;
                break;
            case 3:
                    fprintf(stderr, "\nERROR [code wx%d]"
                                    "\nFailed to write file \"%s\".\n",
                           returnValue, infile[cnt].fileName
                          );
                    return 0;
                    break;
            default:
                fprintf(stderr, "\nERROR [code wx%d]"
                                "\nUndefined Error.\n",
                       returnValue
                      );
                break;
        }
    }
    else if (!strcmp("json", outfile.template))
    {
        returnValue = writeJson(outfile.fileName, danmakuPool, NULL);
        switch (returnValue)
        {
            case 0:
                break;
            case 2:
                fprintf(stderr, "\nERROR [code wj%d]"
                                "\nFailed to create file \"%s\".\n",
                       returnValue, outfile.fileName
                      );
                return 0;
                break;
            case 3:
                    fprintf(stderr, "\nERROR [code wj%d]"
                                    "\nFailed to write file \"%s\".\n",
                           returnValue, infile[cnt].fileName
                          );
                    return 0;
                    break;
            default:
                fprintf(stderr, "\nERROR [code wj%d]"
                                "\nUndefined Error.\n",
                       returnValue
                      );
                break;
        }
    }
    
    printf("\nDone!\n");
    
    return 0;
}

/* 打印帮助信息 */
void printHelpInfo()
{
    printf("\nUsage:"
           "\nDanmakuFactory -o [outfile_format] outfile -i [infile1_template] infile1 [infile2_template] infile2 ... "
           "[-t infile1_timeshift ...] [configuration value] ..."
           "\n"
           "\nFormat:"
           "\nxml, json, ass"
           "\n"
           "\nTemplate:"
           "\nxml, json, ass or a template name"
           "\n"
           "\nConfigurations:"
           "\n-r, --resolution    Specify the value of resolution."
           "\n                    Use any character to connect width and height, like \"1920x1080\"."
           "\n-s, --scrolltime    Specify the time of rolling danmaku to across the screen."
           "\n-f, --fixtime       Specify the time of fix danmaku show on the screen."
           "\n-d, --density       Specify the maximum number of danmaku could show on the screen at the same time."
           "\n                    Special value: -1 non-overlap, 0 unlimit"
           "\n"
           "\n-S, --fontsize      Specify the fontsize of general danmaku."
           "\n-N, --fontname      Specify the fontname of general danmaku."
           "\n-O, --opacity       Specify the opacity of danmaku EXCEPT the special danmaku(range: 1-255)."
           "\n-L, --outline       Specify the width of outline for each danmaku(range: 0-4)."
           "\n-D, --shadow        Specify the depth of shadow for each danmaku(range: 0-4)."
           "\n-B, --bold          Specify whether the font should be boldface."
           "\n                    Available value: TRUE, FALSE"
           "\n"
           "\n--displayarea       Specify the percent of display area on the screen(range: 0.0-1.0)."
           "\n--scrollarea        Specify the percent of scroll area of rolling danmaku on the screen(range: 0.0-1.0)."
           "\n"
           "\n-b, --blockmode     Specify the type of danmaku which will not show on the screen."
           "\n                    Use '-' to connect the type-name, like \"L2R-TOP-BOTTOM\"."
           "\n                    Available value: L2R, R2L, TOP, BOTTOM, SPECIAL, COLOR, REPEAT"
           "\n"
           "\n--statmode          Specify the type of statistic box which will show on the screen."
           "\n                    Use '-' to connect the type-name, like \"TABLE-HISTOGRAM\"."
           "\n                    Available value: TABLE, HISTOGRAM"
           "\n"
           "\n--showusernames     Specify whether show usernames or not."
           "\n                    Available value: TRUE, FALSE"
           "\n--showmsgbox        Specify whether show message box or not."
           "\n                    Available value: TRUE, FALSE"
           "\n--msgboxsize        Specify the size of message box."
           "\n                    Use any character to connect width and height, like \"400x1000\"."
           "\n--msgboxpos         Specify the position of message box."
           "\n                    Use any character to connect posX and posY, like \"50x50\"."
           "\n--msgboxfontsize    Specify the fontsize of message box."
           "\n--msgboxduration    Specify the duration of message box."
           "\n                    If set, will overwrite default value read from xml."
           "\n--giftminprice      Specify the the minimum price of the gifts."
           "\n--giftmergetoleranceSpecify the the time window of merging same user, same type gifts."
           "\n                    If set, will overwrite default value read from xml raw data."
           "\n"
           "\nOther options:"
           "\n-h, --help          Display this help and version information than exit."
           "\n-c, --config        Display configuration information and exit."
           "\n--save              Save configuration as current command settings."
           "\n--ignore-warnings   Ignore all warnings, like `-y, --yes`."
           "\n"
           "\nExample:"
           "\nDanmakuFactory -o ass \"outfile.ass\" -i xml \"infile1.xml\""
           "\nDanmakuFactory -o \"outfile.ass\" -i \"infile1.xml\""
           "\nDanmakuFactory -o \"outfile.ass\" -i \"infile1.xml\" \"infile2.json\" \"infile3.ass\" -t 0.0 5.0 7.0\n"
    );
    
    return;
}

/* 获取指定下标选项参数个数 */
int getArgNum(int argc, char **argv, const int optionIndex)
{
    int cnt;
    
    for (cnt = optionIndex + 1; cnt < argc; cnt++)
    {
        if (argv[cnt][0] == '-')
        {
            break;
        }
    }
    
    return cnt - optionIndex - 1;
}

/* 获取一个实数参数 */
double getArgValDouble(int argc, char **argv, const int optIndex, const char *const optName, const double errorReturnValue)
{
    double value;
    char argValue[MAX_TEXT_LENGTH];
    if (optIndex+1 >= argc)
    {
        fprintf(stderr, "\nERROR"
                        "\n\"%s\" must be specified.\n", optName);
        return errorReturnValue;
    }
    
    strSafeCopy(argValue, argv[optIndex+1], MAX_TEXT_LENGTH);
    if (!ISNUMBERIC(argValue))
    {
        fprintf(stderr, "\nERROR"
                        "\nInvalid argument \"%s\".\n", argValue);
        return errorReturnValue;
    }
    
    switch (getArgNum(argc, argv, optIndex))
    {
        /* 0个代表后一个参数为负数被解释成选项 */
        case 0:
        case 1:
            deQuotMarks(argValue);
            value = atof(argValue);
            break;
        default:
            fprintf(stderr, "\nERROR"
                            "\nInvalid argument \"%s\".\n", argv[optIndex+2]);
            return errorReturnValue;
            break;
    }
    
    return value;
}

/* 获取一个布尔参数 */
BOOL getArgValBool(int argc, char **argv, const int optIndex, const char *const optName)
{
    char argValue[MAX_TEXT_LENGTH];
    switch (getArgNum(argc, argv, optIndex))
    {
        case 0:
            fprintf(stderr, "\nERROR"
                            "\n\"%s\" must be specified.\n", optName);
            return BOOL_UNDETERMINED;
            break;
        case 1:
            break;
        default:
            fprintf(stderr, "\nERROR"
                            "\nInvalid argument \"%s\".\n", argv[optIndex+2]);
            return BOOL_UNDETERMINED; 
            break;
    }

    strSafeCopy(argValue, argv[optIndex+1], MAX_TEXT_LENGTH);
    deQuotMarks(argValue);
    toLower(NULL, argValue);
    if (!strcmp("true", argValue))
    {
        return TRUE;
    }
    else if (!strcmp("false", argValue))
    {
        return FALSE;
    }

    fprintf(stderr, "\nERROR"
                    "\nInvalid value of \"%s\"."
                    "\nNote"
                    "\nIt must be either \"true\" or \"false\".\n",
                    optName);
    return BOOL_UNDETERMINED;
}

/* 获取一个坐标值 */
COORDIN getArgValCoodr(int argc, char **argv, const int optIndex, const char *const optName, int *argNum)
{
    char argStr[MAX_TEXT_LENGTH];
    char *argStrPtr;
    char *numXStrPtr = NULL, *numYStrPtr = NULL;
    COORDIN returnValue = {-1, -1};

    /* 从第一个参数中读取 */
    if (optIndex+1 >= argc)
    {
        fprintf(stderr, "\nERROR"
                        "\n\"%s\" must be specified.\n", optName);
        *argNum = -1;
        return returnValue;
    }
    else
    {
        strSafeCopy(argStr, argv[optIndex+1], MAX_TEXT_LENGTH);
        numXStrPtr = argStrPtr = argStr;
        while (TRUE)
        {
            if (!isDesignatedChar(*argStrPtr, "-.0123456789"))
            {
                if (*argStrPtr != '\0')
                {
                    *argStrPtr = '\0';
                    argStrPtr++;
                    numYStrPtr = argStrPtr;
                }
                break;
            }
            argStrPtr++;
        }

        *argNum = 1; /* 刷新参数数量 */
    }
    
    /* 如果第一个参数无法获取两个数字，尝试读取下一个参数 */
    if (numYStrPtr == NULL)
    {
        if (optIndex+2 < argc)
        {
            numYStrPtr = argv[optIndex+2];
            *argNum = 2; /* 刷新参数数量 */
        }
        else
        {
            fprintf(stderr, "\nERROR"
                            "\nInvalid value of \"%s\"."
                            "\nNote"
                            "\nExample: --%s 100x100\n", optName, optName);
            *argNum = -1;
            return returnValue;
        }
    }
    
    if (!ISNUMBERIC(numXStrPtr) || !ISNUMBERIC(numYStrPtr))
    {
        fprintf(stderr, "\nERROR"
                        "\nInvalid value of \"%s\"."
                        "\nNote"
                        "\nExample: --%s 100x100\n", optName, optName);
        *argNum = -1;
        return returnValue;
    }

    returnValue.x = atoi(numXStrPtr);
    returnValue.y = atoi(numYStrPtr);
    return returnValue;
}

/* 暂停 按回车继续 */
void toPause(BOOL skip)
{
    if (skip)
    {
        printf("\nSkip!\n");
        return;
    }
    printf("\nPress ENTER to continue.\n");
    fflush(stdin);
    getchar();
}

/* 是否继续 */
BOOL isContinue(BOOL skip)
{
    if (skip)
    {
        printf("\nSkip!\n");
        return TRUE;
    }
    printf("\nPress 'Y' or 'y' to continue, any other key to exit.\n");
    printf("> ");
    fflush(stdin);
    char ch = getchar();
    if (ch == 'Y' || ch == 'y')
    {
        return TRUE;
    }
    
    return FALSE;
}
