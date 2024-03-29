#ifndef __TELEGRAPH_H__
#define __TELEGRAPH_H__

#include <winsock2.h>
#include <Windows.h>

#include <stdio.h>
#pragma comment(lib, "WS2_32")
#pragma comment(lib,"Telegraph")

#if (defined _WIN32 && defined _DEBUG)
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
class Telegraph
{
public:
    Telegraph()
    {
        init();
    }
    ~Telegraph()
    {
        done();
    }
    static Telegraph* Instance()
    {
        if(!instance)
        {
            instance = new Telegraph();
        }
        return instance;
    }
    void stopwatchStart()
    {
        LARGE_INTEGER li;
        QueryPerformanceCounter(&li);
        tele_handle->Stopwatch.timeStart = li.QuadPart;
    }
    LONGLONG stopwatchStop()
    {
        LARGE_INTEGER li;
        QueryPerformanceCounter(&li);
        return li.QuadPart - tele_handle->Stopwatch.timeStart ;
    }

    void init( )
    {
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
    void done()
    {
        free(tele_handle->buf);
        tele_handle->buf = NULL;
        closesocket(tele_handle->socket );
        free(tele_handle);
        tele_handle = NULL;
        WSACleanup();
    }

    void drawClear()
    {
        sendBuffer(TELE_DRAW_CLEAR, NULL, 0);
    }
    void drawAdd(double x, double y, int color)
    {
        struct
        {
            double x, y;
            int color;
        } data;
        data.x = x;
        data.y = y;
        data.color = color;
        sendBuffer(TELE_DRAW_ADD, &data, sizeof(data));
    }
    void drawDisplay()
    {
        sendBuffer(TELE_DRAW_DISPLAY, NULL, 0);
    }

    void textDisplay(const char* sz)
    {
        sendBuffer(TELE_TEXT_NORMAL, sz, strlen(sz));
    }
    int sendBuffer(TELE_MARK mark, const void *buf, int buflen)
    {
        int iResult;
        int len = buflen + 8 ;
        TELE_SEND_STRUCT ss;
        ss.size = sizeof(mark) + buflen;
        ss.mark = mark;
        memcpy( tele_handle->buf , &ss, 8);
        memcpy( tele_handle->buf + 8, buf, buflen);
        iResult = send(tele_handle->socket, tele_handle->buf, len , NULL);
        if(iResult == SOCKET_ERROR  )
        {
            printf("send error\n");
            getchar();
        }
        return 0;
    }
public:
    TELE_HANDLE tele_handle;
private:
    static Telegraph* instance;
};

#define TELE_DEBUG_PRINTF0(format)    \
	sprintf(Telegraph::Instance()->tele_handle->Text.pText ,format);    \
	Telegraph::Instance()->textDisplay(Telegraph::Instance()->tele_handle->Text.pText);

#define TELE_DEBUG_PRINTF1(format,arg1)    \
	sprintf(Telegraph::Instance()->tele_handle->Text.pText,format,arg1);    \
	Telegraph::Instance()->textDisplay(Telegraph::Instance()->tele_handle->Text.pText);

#define TELE_DEBUG_PRINTF2(format,arg1,arg2)    \
	sprintf(Telegraph::Instance()->tele_handle->Text.pText,format,arg1,arg2);    \
	Telegraph::Instance()->textDisplay(Telegraph::Instance()->tele_handle->Text.pText);

#define TELE_DEBUG_PRINTF3(format,arg1,arg2,arg3)    \
	sprintf(Telegraph::Instance()->tele_handle->Text.pText,format,arg1,arg2,arg3);    \
	Telegraph::Instance()->textDisplay(Telegraph::Instance()->tele_handle->Text.pText);

#define TELE_DEBUG_STOPWATCH_START()    \
	Telegraph::Instance()->stopwatchStart();

#define TELE_DEBUG_STOPWATCH_STOP(format)    \
	sprintf(tele_handle->Text.pText,format,TELE_Stopwatch_Stop());    \
	TELE_Text_Display(tele_handle->Text.pText );

#define TELE_DEBUG_DRAW_ADD(x,y,color)    \
	Telegraph::Instance()->drawAdd(x,y,color);    \
 
#define TELE_DEBUG_DRAW_CLEAR()    \
	Telegraph::Instance()->drawClear();    \
 
#define TELE_DEBUG_DRAW_DISPLAY()    \
	Telegraph::Instance()->drawDisplay();
#endif //(defined _WIN32 && defined _DEBUG)
#endif // !_EASYSHOW_H_