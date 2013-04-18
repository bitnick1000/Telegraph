#ifndef _EASYSHOW_H_
#define _EASYSHOW_H_

#include <winsock2.h>
#include <Windows.h>

#include <stdio.h>
#pragma comment(lib, "WS2_32")
#pragma comment(lib, "easyshow")

#if (defined _WIN32 && defined _DEBUG)

#define SE_NULL	NULL
//#ifdef __cplusplus
//#define EXTERN_C extern "C" {
//#define EXTERN_C_END  }
//#endif
//
//EXTERN_C
#define _CRT_SECURE_NO_WARNINGS
#define ES_RGB(r,g,b)		(r<<16 | g<<8 | b | 0xFF000000)
#define WHITE		ES_RGB(255,255,255)
#define BLACK		ES_RGB(0,0,0)
#define RED			ES_RGB(255,0,0)
#define GREEN		ES_RGB(0,255,0)
#define BLUE			ES_RGB(0,0,255)
#define YELLOW		RED | GREEN
typedef struct
{
    SOCKET socket;
    char* buf;
    struct
    {
        char* pText;
    } Text;
    struct
    {
        LONGLONG timeStart;
    } Stopwatch;
}*ES_HANDLE, ES_STRUCT;

ES_HANDLE es_handle;

void ES_Stopwatch_Start();
LONGLONG ES_Stopwatch_Stop();
void ES_Init( );
void ES_Done();
void ES_Draw_Clear();
void ES_Draw_Add(double x, double y, int color);
void ES_Draw_Display();
void ES_Text_Display(const char* sz);


#define DEBUG_REMOTE_INIT()    \
	ES_Init();

#define DEBUG_REMOTE_DONE()    \
	ES_Done();

#define DEBUG_REMOTE_PRINTF0(format)    \
	sprintf( es_handle->Text.pText ,format);    \
	ES_Text_Display( es_handle->Text.pText );

#define DEBUG_REMOTE_PRINTF1(format,arg1)    \
	sprintf(es_handle->Text.pText,format,arg1);    \
	ES_Text_Display(es_handle->Text.pText );

#define DEBUG_REMOTE_PRINTF2(format,arg1,arg2)    \
	sprintf(es_handle->Text.pText,format,arg1,arg2);    \
	ES_Text_Display(es_handle->Text.pText );

#define DEBUG_REMOTE_PRINTF3(format,arg1,arg2,arg3)    \
	sprintf(es_handle->Text.pText,format,arg1,arg2,arg3);    \
	ES_Text_Display(es_handle->Text.pText );

#define DEBUG_REMOTE_STOPWATCH_START()    \
	ES_Stopwatch_Start();

#define DEBUG_REMOTE_STOPWATCH_STOP(format)    \
	sprintf(es_handle->Text.pText,format,ES_Stopwatch_Stop());    \
	ES_Text_Display(es_handle->Text.pText );

#define DEBUG_REMOTE_DRAW_ADD(x,y,color)    \
	ES_Draw_Add(x,y,color);    \
 
#define DEBUG_REMOTE_DRAW_CLEAR()    \
	ES_Draw_Clear();    \
 
#define DEBUG_REMOTE_DRAW_DISPLAY()    \
	ES_Draw_Display();

//EXTERN_C_END
#endif //(defined _WIN32 && defined _DEBUG)
#endif // !_EASYSHOW_H_