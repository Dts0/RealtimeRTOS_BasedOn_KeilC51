/*$T indentinput.cpp GC 1.140 03/06/15 16:03:12 */


/*$6
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */


#include <APP_Cfg.h>
#include <OS.h>
OS_CPU_XUCHAR	Task_0_Stack[Task_0_StackSize] = { 0 }; /* 任务0堆栈 */
OS_CPU_XUCHAR	Task_1_Stack[Task_1_StackSize] = { 0 };
OS_CPU_XUCHAR	Task_2_Stack[Task_2_StackSize] = { 0 };
OS_CPU_XUCHAR	Task_3_Stack[Task_3_StackSize] = { 0 };

sbit Test_1 = P3^6;
sbit Test_2 = P4^1;
sbit Test_3 = P4^3;
sbit Test_4 = P4^2;

#define TurnDownAll()	{Test_1 = 0;Test_2 = 0;Test_3 = 0;Test_4 = 0;}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void Task_0(void)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	OS_CPU_XUINT	Counter = 0x00;
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	while(1)
	{
		OS_Delay_nTick(5);
		Test_1 = ~Test_1;
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void Task_1(void)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	OS_CPU_XUINT	Counter = 0x00;
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	while(1)
	{
		OS_Delay_nTick(10);
		Test_2 = ~Test_2;
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void Task_2(void)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	OS_CPU_XUINT	Counter = 0x00;
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	while(1)
	{
		OS_Delay_nTick(15);
		Test_3 = ~Test_3;
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void Task_3(void)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	OS_CPU_XUINT	Counter = 0x00;
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	while(1)
	{
		OS_Delay_nTick(20);
		Test_4 = ~Test_4;
	}
}

/*
 =======================================================================================================================
    定时器1初始化
 =======================================================================================================================
 */
void Task_Timer1(void)
{
	TMOD &= 0x0F;	/* 设置定时器模式 */
	TMOD |= 0x10;	/* 设置定时器模式 */
	TL1 = 0xF0;		/* 设置定时初值 */
	TH1 = 0xD8;		/* 设置定时初值 */
	TF1 = 0;		/* 清除TF1标志 */
	TR1 = 1;		/* 定时器1开始计时 */
	ET1 = 1;
	ENABLE_ALL_INT();
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void TimerTask_1(void)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	OS_CPU_XUINT	Counter = 0x00;
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	Counter += 100;
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void TimerTask_2(void)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	OS_CPU_XUINT	Counter = 0x00;
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	Counter += 100;
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void main(void)
{
	TurnDownAll();
	OS_Init();
	TaskCreate((OS_CPU_INT) Task_0, Task_0_Stack, Task_0_StackSize, Task_0_Prio);
	TaskCreate((OS_CPU_INT) Task_1, Task_1_Stack, Task_1_StackSize, Task_1_Prio);
	TaskCreate((OS_CPU_INT) Task_2, Task_2_Stack, Task_2_StackSize, Task_2_Prio);
	TaskCreate((OS_CPU_INT) Task_3, Task_3_Stack, Task_3_StackSize, Task_3_Prio);
	Task_Timer1();
	OS_TIMER_ADD((OS_CPU_INT) TimerTask_1, 10, OS_TIMER_TYPE_PERIOD, 100);
	OS_TIMER_ADD((OS_CPU_INT) TimerTask_2, 10, OS_TIMER_TYPE_nTimes, 4);
	OS_Start();

	while(1);
}
