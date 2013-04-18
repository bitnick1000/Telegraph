#include "EasyShow.h"

#define ES_SEND_BUFFER_SIZE									2048
typedef enum
{
    ES_TEXT_NORMAL,
    ES_DRAW_CLEAR,
    ES_DRAW_ADD,
    ES_DRAW_DISPLAY,
} ES_MARK;

typedef struct
{
    UINT32 size;
    UINT32 mark;
} ES_SEND_STRUCT;

int ES_SendBuffer(ES_MARK mark, const void *buf, int buflen);

void ES_Stopwatch_Start()
{
    LARGE_INTEGER li;
    QueryPerformanceCounter(&li);
    es_handle->Stopwatch.timeStart = li.QuadPart;
}
LONGLONG ES_Stopwatch_Stop()
{
    LARGE_INTEGER li;
    QueryPerformanceCounter(&li);
    return li.QuadPart - es_handle->Stopwatch.timeStart ;
}

void ES_Init( )
{
    int i;
    SOCKADDR_IN servAddr;
    WSADATA wsaData;
    if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        exit(0);
    }
	es_handle = (ES_HANDLE)malloc(sizeof(ES_STRUCT));
    es_handle->buf = (char*) malloc(ES_SEND_BUFFER_SIZE);
    es_handle->socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	es_handle->Text.pText= (char*) malloc(1024);
    if(es_handle->socket == INVALID_SOCKET)
    {
        printf(" Failed socket() \n");
        return ;
    }
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(11000);
    servAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
    if(connect(es_handle->socket, (PSOCKADDR)&servAddr, sizeof(servAddr)) == -1)
    {
        printf(" Failed connect() \n");
        system("pause");
        return ;
    }
}
void ES_Done()
{
    int i;
	free(es_handle->buf);
	es_handle->buf=NULL;
	closesocket(es_handle->socket );
	free(es_handle);
	es_handle=NULL;
    WSACleanup();
}

void ES_Draw_Clear()
{
	ES_SendBuffer(ES_DRAW_CLEAR,NULL,0);
}
void ES_Draw_Add(double x, double y, int color)
{
    struct
    {
        double x, y;
		int color;
    } data;
    data.x = x;
    data.y = y;
    data.color = color;
	ES_SendBuffer(ES_DRAW_ADD,&data,sizeof(data));
}
void ES_Draw_Display()
{
	ES_SendBuffer(ES_DRAW_DISPLAY,NULL,0);
}

void ES_Text_Display(const char* sz)
{
    ES_SendBuffer(ES_TEXT_NORMAL, sz, strlen(sz));
}
int ES_SendBuffer(ES_MARK mark, const void *buf, int buflen)
{
    int iResult;
    ES_SEND_STRUCT ss;
    ss.size = sizeof(mark) + buflen;
    ss.mark = mark;
    memcpy( es_handle->buf , &ss, 8);
    memcpy( es_handle->buf + 8, buf, buflen);
    iResult = send(es_handle->socket, es_handle->buf, 8 + buflen, NULL);
    if(iResult == SOCKET_ERROR  )
    {
        printf("send error\n");
        getchar();
    }
    return 0;
}