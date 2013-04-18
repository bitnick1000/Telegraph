#include <stdio.h>

#include <winsock2.h>
#include <Windows.h>

#pragma comment(lib, "WS2_32")

#if (defined _WIN32 && defined _DEBUG)
class CRemoteStopwatch
{
public:
    CRemoteStopwatch()
    {
        InitSocket();
        m_socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if(m_socket == INVALID_SOCKET)
        {
            printf(" Failed socket() \n");
            return ;
        }
        sockaddr_in servAddr;
        servAddr.sin_family = AF_INET;
        servAddr.sin_port = htons(11000);
        servAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
        if(::connect(m_socket, (sockaddr*)&servAddr, sizeof(servAddr)) == -1)
        {
            printf(" Failed connect() \n");
            return ;
        }
    }
    ~CRemoteStopwatch()
    {
        DoneSocket();
    }
public:
    void Start()
    {
        LARGE_INTEGER li;
        QueryPerformanceCounter(&li);
        m_timeStart = li.QuadPart;
    }
    void Restart()
    {
        LARGE_INTEGER li;
        QueryPerformanceCounter(&li);
        m_timeStart = li.QuadPart;
    }
    LONGLONG Stop()
    {
        LARGE_INTEGER li;
        QueryPerformanceCounter(&li);
        return li.QuadPart - m_timeStart;
    }
    int Send(const char *buf, int len)
    {
        return send(m_socket, buf, len, NULL);
    }
    int Send(LONGLONG i)
    {
		char highbuf[11];
		char lowbuf[11];
		char sendbuf[22];
		memset(sendbuf,0,12);
		int high=*(int*)&i;
		int low=*((int*)&i+1);
		itoa(high,highbuf,10);
		itoa(low,lowbuf,10);
		strcat(sendbuf,highbuf);
		strcat(sendbuf,lowbuf);
        return send(m_socket, sendbuf,strlen(sendbuf), NULL);
    }
private:
    SOCKET m_socket;
    LONGLONG m_timeStart;
private:
    void InitSocket(BYTE minorVer = 2, BYTE majorVer = 2)
    {
        // ≥ı ºªØWS2_32.dll
        WSADATA wsaData;
        WORD sockVersion = MAKEWORD(minorVer, majorVer);
        if(::WSAStartup(sockVersion, &wsaData) != 0)
        {
            exit(0);
        }
    }
    void DoneSocket()
    {
        ::WSACleanup();
    }
};

CRemoteStopwatch sw;

//#define DEBUG_STOPWATCH_INIT()    \
			//CRS::m_sw=new CRemoteStopwatch();

#define DEBUG_STOPWATCH_START(name)    \
	sw.Send(name,strlen(name));    \
	sw.Start();    

#define DEBUG_STOPWATCH_STOP()    \
	sw.Send(sw.Stop());    

#endif