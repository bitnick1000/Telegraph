

#include <stdio.h>
#include <iostream>
using namespace std;
//CInitSock initSock;		// ��ʼ��Winsock��
#include "RemoteStopwatch.hpp"

int main()
{
	//DEBUG_STOPWATCH_INIT();
	printf("remote stopwatch started!\n");
	DEBUG_STOPWATCH_START("aaa");
	for(int i=0;i<10000;i++)
		printf(" ");
	DEBUG_STOPWATCH_STOP();
	printf("remote stopwatch stoped!\n");
	system("pause");
}
//int main()
//{
//    // �����׽���
//    SOCKET s = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
//    if(s == INVALID_SOCKET)
//    {
//        printf(" Failed socket() \n");
//        return 0;
//    }
//
//    // Ҳ�������������bind������һ�����ص�ַ
//    // ����ϵͳ�����Զ�����
//
//    // ��дԶ�̵�ַ��Ϣ
//    sockaddr_in servAddr;
//    servAddr.sin_family = AF_INET;
//    servAddr.sin_port = htons(11000);
//
//    servAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
//
//    if(::connect(s, (sockaddr*)&servAddr, sizeof(servAddr)) == -1)
//    {
//        printf(" Failed connect() \n");
//		system("pause");
//        return 0;
//    }
//    while(true)
//    {
//        char buf[256];
//		gets(buf);
//        //cin >> buf;
//		strcat(buf,"\n");
//        if(0 == strcmp(buf, "quit\n"))
//            break;
//        send(s, buf, 250, NULL);
//    }
//
//    ::closesocket(s);
//    return 0;
//}
