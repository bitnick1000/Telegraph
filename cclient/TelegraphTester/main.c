
#include <Telegraph.h>
#include <math.h>
int Fibonacci(int n)
{
    int i;
    int prev = 1;
    int prevprev = 1;
    int temp;
    for(i = 2; i < n; i++)
    {
		temp=prev;
		prev=prev+prevprev;
		prevprev=temp;
    }
	return prev;
}
void TestPrint()
{
	int i;
	TELE_DEBUG_INIT();
	TELE_DEBUG_PRINTF0("connected!\n");
	TELE_DEBUG_PRINTF2("%s %s!\n","hello","server");
	for( i=1;i<=22;i++)
	{
		TELE_DEBUG_PRINTF1("%d ",Fibonacci(i));
	}
	TELE_DEBUG_PRINTF0("\n");
	TELE_DEBUG_DONE();
}
void TestStopwatch()
{
	int i;
	double d=0.0;
	TELE_DEBUG_INIT();
	TELE_DEBUG_STOPWATCH_START();
	for(i=0;i<10000;i++)
	{
		d+=Fibonacci(i);
	}
	TELE_DEBUG_STOPWATCH_STOP("10,000 times = %lld\n");
	TELE_DEBUG_DONE();
}
void TestDraw()
{
	int i,j;
	int x, y;
	double pi; 
	int circle=2;
	pi=3.1415926;
	TELE_DEBUG_INIT();
	TELE_DEBUG_DRAW_CLEAR();
	//Sleep(200);
	//TELE_DEBUG_DRAW_ADD(0,-circle,0xFF00FF00);
	//TELE_DEBUG_DRAW_ADD(0,circle,0xFF00FF00);
	for(i=0;i<360*circle;i+=10)
	{
		TELE_DEBUG_DRAW_ADD((double)i,sin(pi*i/180),RED);
	}
	for(i=0;i<360*circle;i++)
	{
		TELE_DEBUG_DRAW_ADD((double)i,cos(pi*i/180)+2,GREEN);
	}
	for(i=0;i<360*circle;i++)
	{
		TELE_DEBUG_DRAW_ADD((double)i,cos(pi*i/180)+4,BLUE);
	}
	TELE_DEBUG_DRAW_DISPLAY();
	TELE_DEBUG_DONE();
}
int main()
{
	TestPrint();
	TestStopwatch();
	TestDraw();
	return 0;
}