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

/* 弹幕类型重定义规范 */
/* +-------------+---------+ */
/* |  类型名称   |  编号   | */
/* +-------------+---------+ */
/* |  右左滚动   |    1    | */
/* +-------------+---------+ */
/* |  左右滚动   |    2    | */
/* +-------------+---------+ */
/* |  上方固定   |    3    | */
/* +-------------+---------+ */
/* |  下方固定   |    4    | */
/* +-------------+---------+ */
/* | B站特殊弹幕 |    5    | */
/* +-------------+---------+ */

#include "CDanmakuFactory.h"

static void JSErrorExit(FILE *ipF, DANMAKU *head, DANMAKU *ptr);

/* 
 * 读取JSON文件加入弹幕池 
 * 参数：
 * 文件名/链表头/读取模式（"n"清空新建 / "a"尾部追加）/时轴偏移量 
 * 返回值：
 * 0 正常退出
 * 1 打开文件失败 
 * 2 3 4 读取文件发生错误
 * 5 节点空间申请失败
 * 6 文本部分空间申请失败
 * 7 文件未能按正确格式读入
  */
int readJson(const char *const ipFile, DANMAKU **head, const char *mode, const float timeShift, STATUS *const status)
{
    FILE *ipF;
    DANMAKU *now = NULL, *last = NULL;
    int cnt;
    
    /* 刷新status */
    if (status != NULL)
    {
        status -> function = (void *)readJson;
        (status -> completedNum) = 0;
    }
    
    /* 打开文件 */
    if((ipF = fopen(ipFile, "r")) == NULL)
    {
        #if PRINT_ERR == TRUE
        printf("\n[X] 文件打开失败"); 
        #endif
        return 1;
    }
    
    /* 判断读入方式 */
    if(*head == NULL || *mode == 'n')
    {/* 新建模式 */
        freeList(*head);
        *head = NULL;
    }
    else if(*mode == 'a')
    {/* 追加模式 */
        now = last = *head;
        if((*head) -> next != NULL)
        {
            while((last -> next) -> next != NULL)
            {
                last = last -> next;
            }
            now = last -> next;
            last -> next = now;
        }
    }
    
    /* 读取json文件 */
    float t_time;
    short t_type;
    short t_fontSize;
    int t_color;
    char ch, t_text[MAX_TEXT_LENGTH];
    
    while(!feof(ipF))
    {
        /* 寻找每条弹幕的开始 */
        while(fgetc(ipF) != '{' || fgetc(ipF) != '\"' ||  fgetc(ipF) != 'c' || fgetc(ipF) != '\"')
        {
            if(feof(ipF))
            {
                goto ENDREAD;
            }
            if(ferror(ipF))
            {
                JSErrorExit(ipF, *head, last);
                #if PRINT_ERR == TRUE
                printf("\n[X] 读文件发生错误"); 
                #endif
                return 2;
            }
        }
        /* 时间轴，颜色，类型，字号，uid(?)，时间戳，?，文本内容 */
        /* {"c":"343,16777215,1,25,1234567,1540706080,f11e5b6c-c3c8-4f5f-b09e-b660f857e6f7","m":"2333333333"}, */
        
        fscanf(ipF, ":\"%f,%d,%hd,%hd,", &t_time, &t_color, &t_type, &t_fontSize);
        
        /* 寻找文本部分的开头 */
        while(fgetc(ipF) != ':')
        {
            if(feof(ipF))
            {
                goto ENDREAD;
            }
            if(ferror(ipF))
            {
                JSErrorExit(ipF, *head, last);
                #if PRINT_ERR == TRUE
                printf("\n[X] 读文件发生错误"); 
                #endif
                return 3;
            }
        }
        
        /* 读取文本部分 */ 
        cnt = 0;
        while((ch = fgetc(ipF)) != '}' && cnt < MAX_TEXT_LENGTH - 1)
        {
            t_text[cnt] = ch;
            if(feof(ipF))
            {
                break;
            }
            if(ferror(ipF))
            {
                JSErrorExit(ipF, *head, last);
                #if PRINT_ERR == TRUE
                printf("\n[X] 读文件发生错误"); 
                #endif
                return 4;
            }
            cnt++;
        }
        t_text[cnt] = '\0';
        deQuotMarks(t_text);
        
        /* 申请一个节点的空间 */
        if((now = (DANMAKU *)malloc(sizeof(DANMAKU))) == NULL)
        {
            JSErrorExit(ipF, *head, last);
            #if PRINT_ERR == TRUE
            printf("\n[X] 申请内存空间失败"); 
            #endif
            return 5;
        }
        /* 申请文本部分的空间 */ 
        if((now -> text = (char *)malloc(strlen(t_text) + 1)) == NULL)
        {
            JSErrorExit(ipF, *head, now);
            #if PRINT_ERR == TRUE
            printf("\n[X] 申请内存空间失败"); 
            #endif
            return 6;
        }
        
        if(*head == NULL)
        {
            *head = last = now;
        }
        
        /* +----------+-----------+-----------+ */
        /* | 类型名称 | 原类型编号| 新类型编号| */
        /* +----------+-----------+-----------+ */
        /* | 右左滚动 |     1     |     1     | */
        /* +----------+-----------+-----------+ */
        /* | 左右滚动 |     6     |     2     | */
        /* +----------+-----------+-----------+ */
        /* | 上方固定 |     5     |     3     | */
        /* +----------+-----------+-----------+ */
        /* | 下方固定 |     4     |     4     | */
        /* +----------+-----------+-----------+ */
        switch(t_type)
        {/* 将xml定义的类型编号转换为程序统一定义的类型编号（如上表） */ 
            case 1:
            {
                now -> type = 1;
                break;
            }
            case 6:
            {
                now -> type = 2;
                break;
            }
            case 5:
            {
                now -> type = 3;
                break;
            }
            case 4:
            {
                now -> type = 4;
                break;
            }
            default:
            {
                now -> type = t_type;
                break;
            }
        }
        
        /* 如果时间加偏移量是负数则置 0 */
        t_time += timeShift;
        if (t_time < EPS) {
            t_time = 0.0f;
        }
        now->time = GET_MS_FLT(t_time);
        now -> fontSize = t_fontSize;
        now -> color = t_color;
        strcpy(now -> text, t_text);
        
        last -> next = now;
        last = now;
        
        /* 更新状态 */
        if (status != NULL)
        {
            (status->totalNum)++;
            (status->completedNum)++;
        }
    }/* 结束 while */
    ENDREAD:
    if (now != NULL)
    {
        now -> next = NULL;
    }
    else
    {
        #if PRINT_ERR == TRUE
        printf("\n[X] 文件不能按正确格式读入"); 
        #endif
        return 7;
    }
    fclose(ipF);
    
    /* 刷新status */
    if (status != NULL)
    {
        status -> isDone = TRUE;
    }
    return 0;
}

/* 
 * 写json文件
 * 参数：输出文件名/弹幕池/状态
 * 返回值：
 * 0 正常退出
 * 1 弹幕池为空
 * 2 创建文件失败
 * 3 写文件发生错误
  */
int writeJson(const char *const fileName, DANMAKU *danmakuHead, STATUS *const status)
{
    /* 刷新status */
    if (status != NULL)
    {
        status -> function = (void *)writeJson;
        (status -> completedNum) = 0;
    }
    
    if (danmakuHead == NULL)
    {
        return 1;
    }
    
    FILE *opF;
    if ((opF = fopen(fileName, "w")) == NULL)
    {
        return 2;
    }
    
    int typeInJson;
    char tempText[32];
    DANMAKU *ptr = danmakuHead;
    
    fprintf(opF, "[[],[],[");
    while (ptr != NULL)
    {
        /* +----------+-----------+-----------+ */
        /* | 类型名称 | 原类型编号| 新类型编号| */
        /* +----------+-----------+-----------+ */
        /* | 右左滚动 |     1     |     1     | */
        /* +----------+-----------+-----------+ */
        /* | 左右滚动 |     6     |     2     | */
        /* +----------+-----------+-----------+ */
        /* | 上方固定 |     5     |     3     | */
        /* +----------+-----------+-----------+ */
        /* | 下方固定 |     4     |     4     | */
        /* +----------+-----------+-----------+ */
        
        if (IS_R2L(ptr))
        {
            typeInJson = 1;
        }
        else if (IS_L2R(ptr))
        {
            typeInJson = 6;
        }
        else if (IS_TOP(ptr))
        {
            typeInJson = 5;
        }
        else if (IS_BTM(ptr))
        {
            typeInJson = 4;
        }
        else
        {
            ptr = ptr -> next;
            continue;
        }
        
        fprintf(opF, "\n{\"c\":\"%s,%d,%d,%d,,,\",\"m\":\"%s\"},",
                     intTimeToStr(tempText, ptr->time, 3),
                     ptr -> color,
                     typeInJson,
                     ptr -> fontSize, 
                     ptr -> text
               );
        
        if (ferror(opF))
        {
            fclose(opF);
            return 3;
        }
        ptr = ptr -> next;
        
        /* 刷新status */
        if (status != NULL)
        {
            (status -> completedNum)++;
        }
    }
    fprintf(opF, "\n]]");
    
    fclose(opF);
    
    /* 刷新status */
    if (status != NULL)
    {
        status -> isDone = TRUE;
    }
    return 0;
}

/* 
 * 出错后程序退出前的处理
 * 参数： 
 * 文件指针/链表头指针/最后一个未结尾节点指针/
  */
static void JSErrorExit(FILE *ipF, DANMAKU *head, DANMAKU *ptr)
{
    fclose(ipF);
    if(head != NULL)
    {
        ptr -> next = NULL;
        freeList(head);
    }
    return;
}
