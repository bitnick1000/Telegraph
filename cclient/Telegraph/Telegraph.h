#ifndef __EASYSHOW_H__
#define __EASYSHOW_H__

#include <winsock2.h>
#include <Windows.h>

#include <stdio.h>
#pragma comment(lib, "WS2_32")
//#pragma comment(lib, "Telegraph")

#if (defined _WIN32 && defined _DEBUG)

#define SE_NULL	NULL
//#ifdef __cplusplus
//#define EXTERN_C extern "C" {
//#define EXTERN_C_END  }
//#endif
//
//EXTERN_C
#define TELE_RGB(r,g,b)		(r<<16 | g<<8 | b | 0xFF000000)
#define WHITE		TELE_RGB(255,255,255)
#define BLACK		TELE_RGB(0,0,0)
#define RED			TELE_RGB(255,0,0)
#define GREEN		TELE_RGB(0,255,0)
#define BLUE			TELE_RGB(0,0,255)
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
}*TELE_HANDLE, TELE_STRUCT;

TELE_HANDLE tele_handle;

//void TELE_Stopwatch_Start();
//LONGLONG TELE_Stopwatch_Stop();
//void TELE_Init( );
//void TELE_Done();
//void TELE_Draw_Clear();
//void TELE_Draw_Add(double x, double y, int color);
//void TELE_Draw_Display();
//void TELE_Text_Display(const char* sz);


#define TELE_SEND_BUFFER_SIZE									2048
typedef enum
{
    TELE_TEXT_NORMAL,
    TELE_DRAW_CLEAR,
    TELE_DRAW_ADD,
    TELE_DRAW_DISPLAY,
} TELE_MARK;

typedef struct
{
    UINT32 size;
    UINT32 mark;
} TELE_SEND_STRUCT;

int TELE_SendBuffer(TELE_MARK mark, const void *buf, int buflen);

void TELE_Stopwatch_Start()
{
    LARGE_INTEGER li;
    QueryPerformanceCounter(&li);
    tele_handle->Stopwatch.timeStart = li.QuadPart;
}
LONGLONG TELE_Stopwatch_Stop()
{
    LARGE_INTEGER li;
    QueryPerformanceCounter(&li);
    return li.QuadPart - tele_handle->Stopwatch.timeStart ;
}

void TELE_Init( )
{
    int i;
    SOCKADDR_IN servAddr;
    WSADATA wsaData;
    if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        exit(0);
    }
    tele_handle = (TELE_HANDLE)malloc(sizeof(TELE_STRUCT));
    tele_handle->buf = (char*) malloc(TELE_SEND_BUFFER_SIZE);
    tele_handle->socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    tele_handle->Text.pText = (char*) malloc(1024);
    if(tele_handle->socket == INVALID_SOCKET)
    {
        printf(" Failed socket() \n");
        return ;
    }
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(11000);
    servAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
    if(connect(tele_handle->socket, (PSOCKADDR)&servAddr, sizeof(servAddr)) == -1)
    {
        printf(" Failed connect() \n");
        system("pause");
        return ;
    }
}
void TELE_Done()
{
    int i;
    free(tele_handle->buf);
    tele_handle->buf = NULL;
    closesocket(tele_handle->socket );
    free(tele_handle);
    tele_handle = NULL;
    WSACleanup();
}

void TELE_Draw_Clear()
{
    TELE_SendBuffer(TELE_DRAW_CLEAR, NULL, 0);
}
void TELE_Draw_Add(double x, double y, int color)
{
    struct
    {
        double x, y;
        int color;
    } data;
    data.x = x;
    data.y = y;
    data.color = color;
    TELE_SendBuffer(TELE_DRAW_ADD, &data, sizeof(data));
}
void TELE_Draw_Display()
{
    TELE_SendBuffer(TELE_DRAW_DISPLAY, NULL, 0);
}

void TELE_Text_Display(const char* sz)
{
    TELE_SendBuffer(TELE_TEXT_NORMAL, sz, strlen(sz));
}
int TELE_SendBuffer(TELE_MARK mark, const void *buf, int buflen)
{
    int iResult;
    TELE_SEND_STRUCT ss;
    ss.size = sizeof(mark) + buflen;
    ss.mark = mark;
    memcpy( tele_handle->buf , &ss, 8);
    memcpy( tele_handle->buf + 8, buf, buflen);
    iResult = send(tele_handle->socket, tele_handle->buf, 8 + buflen, NULL);
    if(iResult == SOCKET_ERROR  )
    {
        printf("send error\n");
        getchar();
    }
    return 0;
}


#define TELE_DEBUG_INIT()    \
	TELE_Init();

#define TELE_DEBUG_DONE()    \
	TELE_Done();

#define TELE_DEBUG_PRINTF0(format)    \
	sprintf( tele_handle->Text.pText ,format);    \
	TELE_Text_Display( tele_handle->Text.pText );

#define TELE_DEBUG_PRINTF1(format,arg1)    \
	sprintf(tele_handle->Text.pText,format,arg1);    \
	TELE_Text_Display(tele_handle->Text.pText );

#define TELE_DEBUG_PRINTF2(format,arg1,arg2)    \
	sprintf(tele_handle->Text.pText,format,arg1,arg2);    \
	TELE_Text_Display(tele_handle->Text.pText );

#define TELE_DEBUG_PRINTF3(format,arg1,arg2,arg3)    \
	sprintf(tele_handle->Text.pText,format,arg1,arg2,arg3);    \
	TELE_Text_Display(tele_handle->Text.pText );

#define TELE_DEBUG_STOPWATCH_START()    \
	TELE_Stopwatch_Start();

#define TELE_DEBUG_STOPWATCH_STOP(format)    \
	sprintf(tele_handle->Text.pText,format,TELE_Stopwatch_Stop());    \
	TELE_Text_Display(tele_handle->Text.pText );

#define TELE_DEBUG_DRAW_ADD(x,y,color)    \
	TELE_Draw_Add(x,y,color);    \
 
#define TELE_DEBUG_DRAW_CLEAR()    \
	TELE_Draw_Clear();    \
 
#define TELE_DEBUG_DRAW_DISPLAY()    \
	TELE_Draw_Display();

//EXTERN_C_END
#endif //(defined _WIN32 && defined _DEBUG)
#endif // !_EASYSHOW_H_