/*$T indentinput.cpp GC 1.140 02/27/15 15:33:11 */


/*$6
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    ZJYC
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */


#include <OS.h>

OS_CPU_XUCHAR		OS_TaskReadyList[OS_MAX_NUM_TASK / 8 + 1] = { 0 };		/* ������ */
OS_CPU_XUCHAR		OS_TaskRegGroup[OS_MAX_NUM_TASK][OS_REG_SIZE] = { 0 };	/* ����Ĵ����� */
OS_CPU_XP_XUCHAR	OS_TaskStackAddr[OS_MAX_NUM_TASK] = { 0 };				/* �����ջ��ַ */
OS_CPU_XUCHAR		OS_TaskCurLength[OS_MAX_NUM_TASK] = { 0 };				/* ��ǰ��ջ���� */
OS_CPU_XUCHAR		OS_TaskStackSize[OS_MAX_NUM_TASK] = { 0 };				/* �����ջ�ܳ��� */
OS_CPU_XUINT		OS_TaskStatus[OS_MAX_NUM_TASK] = { 0 };					/* ����״̬ */
OS_CPU_XUCHAR		OS_TaskTickList[OS_MAX_NUM_TASK / 8 + 1] = { 0 };		/* �δ��б� */
OS_CPU_XUINT		OS_TaskTickCounter[OS_MAX_NUM_TASK] = { 0 };			/* �δ������ */
OS_CPU_UCHAR		OS_CurTaskNum = 0;						/* ��ǰ���� */
OS_CPU_UCHAR		OS_NexTaskNum = 0;						/* ��һ���� */
OS_CPU_UCHAR		OS_SPDefaultValue = 0;					/* Ĭ�϶�ջָ�� */
OS_CPU_UCHAR		TH0_Reload = 0;							/* TH0 */
OS_CPU_UCHAR		TL0_Reload = 0;							/* TL0 */
OS_CPU_UCHAR		OsMaxNumOfTask = OS_MAX_NUM_TASK;		/* ��������� */
OS_CPU_UCHAR		OsCpuCharSize = OS_CPU_UCHAR_LENGTH;	/* �ֽڴ�С */
OS_CPU_XUCHAR		Task_Idle_Stack[Task_Idle_StackSize] = { 0 };	/* ���������ջ */
OS_CPU_UCHAR		OS_RegSize = OS_REG_SIZE;						/* �Ĵ������С */

/* ͳ�ƹ��� */
#if OS_State_ENABLE
OS_CPU_UINT			CPU_Usage_Counter[OS_MAX_NUM_TASK] = { 0 };
OS_CPU_UINT			CPU_TickCounter_ALL = 0;
#endif
OS_CPU_XUINT		CPU_Usage_ALL = 0;					/* CPUʹ���� */
OS_CPU_XUINT		CPU_Usage[OS_MAX_NUM_TASK] = { 0 }; /* ���������CPUʹ���� */

/* ��ʱ�� */
#if OS_TIMER_ENABLE
OS_CPU_UINT			OS_TIMER_Counter = 0;
OS_CPU_UINT			OS_TIMER_ARRAY[OS_TIMER_MAX_NUM] = { 0 };
OS_X_TMR			OS_X_Tmr = { 0 };
#endif

/* ����ϵͳ���� */
OS_X_MSG			OS_Msg_SYS = { 0 };
OS_CPU_XUCHAR		Task_SYS_Stack[Task_SYS_StackSize] = { 0 };
OS_CPU_XUCHAR		SYS_MSG_BUFF[Task_SYS_MsgLength] = { 0 };

/* ʱ���ʹ�� */
#if OS_TIMESTAMP_ENABLE
OS_CPU_ULONG		Timestamp = 0;						/* ʱ��� */
#endif

/* ��ջ��鹦�� */
#if OS_STACKCHECK_ENABLE
OS_CPU_UCHAR		OS_StackCheckCounter = 0;
#endif

/*
 =======================================================================================================================
    ����ϵͳ��ʼ��
 =======================================================================================================================
 */
void OS_Init(void)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	OS_CPU_XUCHAR	ReadyListLength = sizeof(OS_TaskReadyList);
	OS_CPU_XUCHAR	i = 0;
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	PreventWarning(0);						/* ��ֹ���������� */

	for(i = 0; i < ReadyListLength; i++)
	{
		OS_TaskReadyList[i] = 0x00;			/* ���������� */
	}

	for(i = 0; i < OS_MAX_NUM_TASK; i++)
	{
		OS_TaskStackAddr[i] = OS_CPU_NULL;	/* ��ջ��ַ���� */
		OS_TaskCurLength[i] = OS_CPU_NULL;	/* ��ջ�������� */
		OS_TaskStatus[i] = OS_CPU_NULL;		/* ����״̬���� */
	}

	/* ϵͳ���� */
	TaskCreate((OS_CPU_INT) OS_SYS_Task, Task_SYS_Stack, Task_SYS_StackSize, Task_SYS_Prio);

	/* �������� */
	TaskCreate((OS_CPU_INT) TaskIdle, Task_Idle_Stack, Task_Idle_StackSize, Task_Idle_Prio);

	OS_SPDefaultValue = SP - 2;				/* ��ȡ��׼��ջλ�� */
	OS_SysTickInit();						/* �δ��ʼ�� */
}

/*
 =======================================================================================================================
    �δ�����ʱ���������
 =======================================================================================================================
 */
void TickTask(void)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	OS_CPU_UCHAR	i = 0, j = 0, Temp = 0, Temp_2 = 0;
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#if OS_State_ENABLE							/* ͳ�Ƶ�Ԫ */
	CPU_TickCounter_ALL++;
	CPU_Usage_Counter[OS_CurTaskNum]++;
	if(CPU_TickCounter_ALL >= OS_State_Period)
	{
		CPU_TickCounter_ALL = 0;
		SYS_MSG_BUFF[0] = 2;
		OS_SendMessage_INT(&OS_Msg_SYS, &SYS_MSG_BUFF, 0);
	}
#endif
#if OS_TIMER_ENABLE							/* ��ʱ����Ԫ */
	OS_TIMER_Counter++;
	{
		/*~~~~~~~~~~~~~~~~~~*/
		OS_CPU_UCHAR	i = 0;
		/*~~~~~~~~~~~~~~~~~~*/

		for(i = 0; i < OS_TIMER_MAX_NUM; i++)
		{
			if(OS_TIMER_Counter == OS_TIMER_ARRAY[i])
			{
				SYS_MSG_BUFF[1] = 1;
				OS_SendMessage_INT(&OS_Msg_SYS, &SYS_MSG_BUFF, 0);
				break;
			}
		}
	}
#endif
#if OS_TIMESTAMP_ENABLE						/* ʱ�����Ԫ */
	Timestamp++;							/* ʱ��� */
#endif
#if OS_HOOK_ENABLE							/* ���Ӻ��� */
	TickTask_HOOK();
#endif
#if OS_STACKCHECK_ENABLE					/* ��ջ��� */
	OS_StackCheckCounter++;
	if(OS_StackCheckCounter % 10 == 0)
	{
		SYS_MSG_BUFF[2] = 3;
		OS_SendMessage_INT(&OS_Msg_SYS, &SYS_MSG_BUFF, 0);
	}
#endif
#if true									/* �δ��������� */
	for(i = 0; i < sizeof(OS_TaskTickList); i++)
	{
		Temp = OS_TaskTickList[i];			/* ��ȡ�ȴ��б� */

		if(Temp != 0)
		{
			for(j = 0; j < OS_CPU_UCHAR_LENGTH; j++)
			{
				if((Temp & (1 << j)))
				{
					Temp_2 = (i + 1) * j;	/* ��ȡ�ȴ�������� */

					if(OS_TaskTickCounter[Temp_2] == 0)
					{
						/* ��������Ϊ��-�������Ϊ���� */
						DISABLE_ALL_INT();
						AddToReadyList(Temp_2);
						DeleteFromTickList(Temp_2);
						ENABLE_ALL_INT();
					}
					else
					{
						/* ��������һ */
						OS_TaskTickCounter[Temp_2]--;
					}
				}
			}
		}
	}
#endif
}

/*
 =======================================================================================================================
    �Ƕ�����ʱ��Counter����ʱ�δ�����
 =======================================================================================================================
 */
void OS_Delay_nTick(OS_CPU_UINT Counter)
{
	if(Counter == 0) return;
	DISABLE_ALL_INT();
	DeleteFromReadyList(OS_CurTaskNum); /* �Ӿ����б�ɾ�� */
	AddToTickList(OS_CurTaskNum);		/* ��ӵ��ȴ��б� */
	OS_TaskTickCounter[OS_CurTaskNum] = Counter;	/* װ��ʱ�䳤�� */
	OS_TaskSwitch();	/* ������� */
	ENABLE_ALL_INT();
}

/*
 =======================================================================================================================
    ����ϵͳ
 =======================================================================================================================
 */
void OS_Start(void)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	OS_CPU_XUCHAR		ActivedTaskNum;
	OS_CPU_P_UCHAR		pTemp = (OS_CPU_P_UCHAR) 0x00 + OS_SPDefaultValue;
	OS_CPU_XP_XUCHAR	pTemp_2 = OS_TaskStackAddr[0];	/* ָ������0 */
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	/* Ѱ���Ѿ���������� */
	for(ActivedTaskNum = 0; ActivedTaskNum < OS_MAX_NUM_TASK; ActivedTaskNum++)
	{
		if((OS_TaskStatus[ActivedTaskNum] & Task_Actived) != 0)
		{
			pTemp_2 = OS_TaskStackAddr[ActivedTaskNum];

			break;
		}
	}

	if(ActivedTaskNum == OS_MAX_NUM_TASK)				/* δ�ҵ��������񣬳��� */
	{
		while(1) DISABLE_ALL_INT();
	}

	/* ָ�����ȼ�����Ѽ������� */
	pTemp++;

	*pTemp = pTemp_2[0];

	pTemp++;

	*pTemp = pTemp_2[1];
}

/*
 =======================================================================================================================
    ����һ������(TaskName:�������� Stack:�����ջ��ַ
    StackSize:��ջ��С TaskPrio: :�������ȼ�)
 =======================================================================================================================
 */
void TaskCreate(OS_CPU_INT TaskName, OS_CPU_P_XUCHAR Stack, OS_CPU_UCHAR StackSize, OS_CPU_CHAR TaskPrio)
{
#if OS_HOOK_ENABLE
	TaskCreate_HOOK();	/* ���ù��� */
#endif
	if((OS_TaskStatus[TaskPrio] & Task_Actived) == 0)	/* ȷ������δ������ */
	{
		DISABLE_ALL_INT();
		AddToReadyList(TaskPrio);						/* �����Ϊ���� */
		OS_TaskStackAddr[TaskPrio] = Stack;				/* ��������ջ��ַ */
		OS_TaskStackSize[TaskPrio] = StackSize;			/* ��������ջ���� */
		OS_TaskStatus[TaskPrio] |= Task_Actived;		/* �����Ѽ��� */
		Stack[1] = (TaskName & 0xff00) >> 8;			/* ��ջ���������ַ */
		Stack[0] = TaskName & 0x00ff;
		OS_TaskCurLength[TaskPrio] = 2;					/* ��ջ����Ϊ2�ֽ� */
		ENABLE_ALL_INT();
	}
	else
	{
		while(1) DISABLE_ALL_INT();						/* �����Ѿ������� */
	}
}

/*
 =======================================================================================================================
    ϵͳ�δ��ʼ��
 =======================================================================================================================
 */
void OS_SysTickInit(void)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~*/
	OS_CPU_XUINT	Temp_1 = 0;
	OS_CPU_XUINT	Temp_2 = 0;
	/*~~~~~~~~~~~~~~~~~~~~~~~*/

	DISABLE_ALL_INT();
	Temp_1 = OS_CryFrequency / OS_CPU_Instruction_Cycle / 1000000L; /* һ΢�뼸������ */
	Temp_2 = 1000000L / OS_TickFrequency * Temp_1;					/* �δ���Ҫ�������� */
	Temp_2 = 65536 - Temp_2;
	TH0_Reload = Temp_2 / 256;
	TL0_Reload = Temp_2 % 256;
	TMOD &= 0xF0;	/* ���ö�ʱ��ģʽ */
	TMOD |= 0x01;	/* ���ö�ʱ��ģʽ */
	TH0 = TH0_Reload;
	TL0 = TL0_Reload;
	ET0 = 1;
	TR0 = 1;
	ENABLE_ALL_INT();
}

/*
 =======================================================================================================================
    �ֶ������л�����
 =======================================================================================================================
 */
void OS_TaskSwitchManual(void)
{
	OS_TaskSwitch();	/* �����л� */
}

/*
 =======================================================================================================================
    �����źţ�RecevNum���źŽ��������ȼ��ţ�
 =======================================================================================================================
 */
void OS_SendSignal(OS_CPU_UCHAR RecevNum)
{
#if OS_Signal_ENABLE
	if(RecevNum == OS_CurTaskNum) return;				/* ���Լ������ź� */
	if(OS_TaskStatus[RecevNum] & Task_Hang_SIGNAL)		/* ȷ�������Ƿ��ڵ� */
	{
		DISABLE_ALL_INT();
		OS_TaskStatus[RecevNum] &= ~Task_Hang_SIGNAL;	/* ����ȴ�״̬ */
		AddToReadyList(RecevNum);						/* ��������� */
		OS_TaskSwitch();	/* �����л� */
		ENABLE_ALL_INT();
	}

#else
	RecevNum = RecevNum;
#endif
}

/*
 =======================================================================================================================
    �����ź�(ISR)(RecevNum:�źŽ��������ȼ���)
 =======================================================================================================================
 */
void OS_SendSignal_INT(OS_CPU_UCHAR RecevNum)
{
#if OS_Signal_ENABLE
	if(OS_TaskStatus[RecevNum] & Task_Hang_SIGNAL)		/* ȷ�������Ƿ��ڵ� */
	{
		DISABLE_ALL_INT();
		OS_TaskStatus[RecevNum] &= ~Task_Hang_SIGNAL;	/* ����ȴ�״̬ */
		AddToReadyList(RecevNum);						/* ��������� */
		ENABLE_ALL_INT();
	}

#else
	RecevNum = RecevNum;
#endif
}

/*
 =======================================================================================================================
    �ȴ��ź�
 =======================================================================================================================
 */
void OS_PendSignal(void)
{
#if OS_Signal_ENABLE
	DISABLE_ALL_INT();
	OS_TaskStatus[OS_CurTaskNum] |= Task_Hang_SIGNAL;	/* ״̬�ȴ�SIGNAL */
	DeleteFromReadyList(OS_CurTaskNum);					/* ɾ�������� */
	OS_TaskSwitch();						/* �����л� */
	ENABLE_ALL_INT();
#else
#endif
}

/*
 * �ȴ���Ϣ��������Message����Ϣ�ṹ���ַ��Size����Ϣ��С��
 */
void OS_PendMessage (OS_MSG * Message) reentrant
{
#if OS_Message_ENABLE
	OS_MSG	*Temp = Message;
	DISABLE_ALL_INT();
	Message->PendTaskNum = OS_CurTaskNum;	/* ��¼��Ϣ�ȴ��� */
	DeleteFromReadyList(OS_CurTaskNum);		/* ɾ�������� */
	OS_TaskStatus[OS_CurTaskNum] |= Task_Hang_MEAASGE;	/* ״̬�ȴ�MESSAGE */
	OS_TaskSwitch();	/* �����л� */
	ENABLE_ALL_INT();
	Message = Temp;		/* ���� */
	return;				/* ������Ϣָ�� */
#else
	Message = Message;
	return;
#endif
}

/*
 * ������Ϣ��Message����Ϣ�ṹ���ַ��Size����Ϣ��С��
 * MsgAddr����Ϣ��ַ��
 */
void OS_SendMessage (OS_MSG * Message, OS_CPU_XP_VOID MsgAddr, OS_CPU_CHAR Size) reentrant
{
#if OS_Message_ENABLE
	if(OS_TaskStatus[Message->PendTaskNum] & Task_Hang_MEAASGE)		/* ȷ���Ƿ��ڵ� */
	{
		/* �ȴ��߾��� */
		AddToReadyList(Message->PendTaskNum);
		OS_TaskStatus[Message->PendTaskNum] &= ~Task_Hang_MEAASGE;	/* ���״̬ */
		Message->MessageAddr = MsgAddr; /* д����Ϣ��ַ */
		Message->MessageSize = Size;	/* д����Ϣ��С */
		OS_TaskSwitch();				/* �����л� */
	}
#else
	Message = Message;
	MsgAddr = MsgAddr;
	Size = Size;
#endif
}

/*
 * ������Ϣ��Message����Ϣ�ṹ���ַ��Size����Ϣ��С��
 * MsgAddr����Ϣ��ַ��(ISR)
 */
void OS_SendMessage_INT (OS_MSG * Message, OS_CPU_XP_VOID MsgAddr, OS_CPU_CHAR Size) reentrant
{
#if OS_Message_ENABLE
	if(OS_TaskStatus[Message->PendTaskNum] & Task_Hang_MEAASGE)		/* ȷ���Ƿ��ڵ� */
	{
		/* �ȴ��߾��� */
		AddToReadyList(Message->PendTaskNum);
		OS_TaskStatus[Message->PendTaskNum] &= ~Task_Hang_MEAASGE;	/* ���״̬ */
		Message->MessageAddr = MsgAddr; /* д����Ϣ��ַ */
		Message->MessageSize = Size;	/* д����Ϣ��С */
	}
#else
	Message = Message;
	MsgAddr = MsgAddr;
	Size = Size;
#endif
}

/*
 =======================================================================================================================
    ռ�û����壬����������Ѿ���ռ�ã���ȴ�(
    Mutex:�������ַ)
 =======================================================================================================================
 */
void OS_OccupyMutex(OS_MUTEX *Mutex)reentrant
{
#if OS_Mutex_ENABLE
	OS_MUTEX * Temp = Mutex;
	if(Mutex->IsOccupy == 0x00)					/* δ��ռ�� */
	{
		Mutex->IsOccupy = OS_CurTaskNum + 1;
	}

	if(Mutex->IsOccupy != OS_CurTaskNum + 1)	/* ��������Ѿ���ռ�� */
	{
		DISABLE_ALL_INT();
		DeleteFromReadyList(OS_CurTaskNum);		/* ɾ�������� */
		OS_TaskStatus[OS_CurTaskNum] |= Task_Hang_MUTEX;	/* ״̬�ȴ�MUTEX */
		AddToMutexList(OS_CurTaskNum, Mutex);				/* ���뵽MUTEX�ȴ��� */
		OS_TaskSwitch();						/* �����л� */
		Mutex = Temp;
		ENABLE_ALL_INT();
		Mutex->IsOccupy = OS_CurTaskNum + 1;	/* ռ�гɹ� */
	}
	else
	{
		Mutex->IsOccupy = OS_CurTaskNum + 1;	/* ռ�гɹ� */
	}

#else
	Mutex = Mutex;
#endif
}

/*
 =======================================================================================================================
    �ͷŻ����壬ͬʱ�л�����Mutex���������ַ��
 =======================================================================================================================
 */
void OS_ReleaseMutex(OS_MUTEX *Mutex)reentrant
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~*/
#if OS_Mutex_ENABLE
	OS_CPU_UCHAR	i = 0, j = 0;
	/*~~~~~~~~~~~~~~~~~~~~~~~~~*/

	Mutex->IsOccupy = 0x00;					/* δ��ռ�� */

	for(i = 0; i < OS_MAX_NUM_TASK; i++)	/* �����ȴ��б� */
	{
		if(Mutex->PendList[i / 8] != 0x00)
		{
			if(Mutex->PendList[i / 8] & (1 << (i % 8)))		/* Ѱ�ҵȴ����� */
			{
				DISABLE_ALL_INT();
				OS_TaskStatus[i] &= ~Task_Hang_MUTEX;		/* ����ȴ�MUTEX״̬ */
				DeleteFromMutexList(i, Mutex);				/* ��MUTEX�ȴ��б�ɾ�� */
				AddToReadyList(i);
				OS_TaskReadyList[i / 8] |= (1 << (i % 8));	/* ��������� */
				OS_TaskSwitch();	/* �����л� */
				ENABLE_ALL_INT();
			}
		}
	}

#else
	Mutex = Mutex;
#endif
}

/*
 =======================================================================================================================
    �ͷŻ����壬ͬʱ�л�����(ISR)��Mutex���������ַ��
 =======================================================================================================================
 */
void OS_ReleaseMutex_INT(OS_MUTEX *Mutex)reentrant
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~*/
#if OS_Mutex_ENABLE
	OS_CPU_UCHAR	i = 0, j = 0;
	/*~~~~~~~~~~~~~~~~~~~~~~~~~*/

	Mutex->IsOccupy = 0x00;					/* δ��ռ�� */

	for(i = 0; i < OS_MAX_NUM_TASK; i++)	/* �����ȴ��б� */
	{
		if(Mutex->PendList[i / 8] != 0x00)
		{
			if(Mutex->PendList[i / 8] & (1 << (i % 8)))		/* Ѱ�ҵȴ����� */
			{
				DISABLE_ALL_INT();
				OS_TaskStatus[i] &= ~Task_Hang_MUTEX;		/* ����ȴ�MUTEX״̬ */
				DeleteFromMutexList(i, Mutex);				/* ��MUTEX�ȴ��б�ɾ�� */
				AddToReadyList(i);
				OS_TaskReadyList[i / 8] |= (1 << (i % 8));	/* ��������� */
				ENABLE_ALL_INT();
			}
		}
	}

#else
	Mutex = Mutex;
#endif
}

/*
 =======================================================================================================================
    Ϊ��ֹ����������
 =======================================================================================================================
 */
void PreventWarning(OS_CPU_UCHAR Nothing)
{
	if(Nothing)
	{
		OS_PendMessage(OS_CPU_NULL);
		OS_SendMessage(OS_CPU_NULL, OS_CPU_NULL, OS_CPU_NULL);
		OS_SendMessage_INT(OS_CPU_NULL, OS_CPU_NULL, OS_CPU_NULL);
		OS_OccupyMutex(OS_CPU_NULL);
		OS_ReleaseMutex(OS_CPU_NULL);
		OS_ReleaseMutex_INT(OS_CPU_NULL);
		OS_Init();
		TickTask();
		OS_Delay_nTick(0);
		OS_Start();
		OS_SysTickInit();
		OS_TaskSwitchManual();
		OS_SendSignal(0);
		OS_PendSignal();
		OS_SendSignal_INT(OS_CPU_NULL);
		TaskSuspend(0);
		TaskResume(0);
		TickTask_HOOK();
		TaskCreate_HOOK();
		TaskSwitch_HOOK();
		StackCheckWarning(OS_CPU_NULL, OS_CPU_NULL);
		OS_StateTask();
		OS_SYS_Task();
		OS_TMR_Task();
		OS_TIMER_ADD(OS_CPU_NULL, OS_CPU_NULL, OS_CPU_NULL, OS_CPU_NULL);
	}
}

/*
 =======================================================================================================================
    ��������
 =======================================================================================================================
 */
void TaskIdle(void)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~*/
	OS_CPU_XUCHAR	i = 0;
	/*~~~~~~~~~~~~~~~~~~~~~~~~~*/

	while(1)
	{
		for(i = 0; i < 240; i++)
		{
			;
		}
	}
}

/*
 =======================================================================================================================
    ����ָ�(Prio:���ָ���������ȼ���)
 =======================================================================================================================
 */
void TaskResume(OS_CPU_UCHAR Prio)
{
	DISABLE_ALL_INT();
	AddToReadyList(Prio);
	OS_TaskSwitch();
	ENABLE_ALL_INT();
}

/*
 =======================================================================================================================
    ������ͣ(Prio:����ͣ��������ȼ���)
 =======================================================================================================================
 */
void TaskSuspend(OS_CPU_UCHAR Prio)
{
	DISABLE_ALL_INT();
	DeleteFromReadyList(Prio);
	OS_TaskSwitch();
	ENABLE_ALL_INT();
}

/*
 =======================================================================================================================
    ͳ������ÿOS_State_Period���δ�ִ��һ�Σ�����������ڼ�ʱ��ͳ�����񼸺�ʧЧ
 =======================================================================================================================
 */
void OS_StateTask(void)
{
	/*~~~~~~~~~~~~~~~~~~*/
#if OS_State_ENABLE
	OS_CPU_UCHAR	i = 0;
	/*~~~~~~~~~~~~~~~~~~*/

	for(i = 0; i < OS_MAX_NUM_TASK; i++)
	{
		/* �����CPUռ���� */
		CPU_Usage[i] = CPU_Usage_Counter[i] * StateRate * 1.0 / OS_State_Period;
		CPU_Usage_Counter[i] = 0;
	}

	CPU_Usage_ALL = StateRate - CPU_Usage[OS_MAX_NUM_TASK - 1]; /* CPUʹ���� */
#else
#endif
}

/*
 =======================================================================================================================
    ����ϵͳ����(��ʱ����ͳ������ͨ��Message����)
 =======================================================================================================================
 */
void OS_SYS_Task(void)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	OS_CPU_UCHAR	Inf = 0, i = 0;
	OS_CPU_P_XCHAR	pBuff = 0x1234;
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	while(1)
	{
		OS_PendMessage(&OS_Msg_SYS);
		pBuff = OS_Msg_SYS.MessageAddr;
		for(i = 0; i < Task_SYS_MsgLength; i++)
		{
			switch(pBuff[i])
			{
			case 0:		break;
			case 1:		{ OS_TMR_Task(); break; }
			case 2:		{ OS_StateTask(); break; }
			case 3:		{ OS_StackCheck_Task(); break; }
			default:	break;
			}

			pBuff[i] = 0;
		}
	}
}

/*
 =======================================================================================================================
    ��ʱ������
 =======================================================================================================================
 */
void OS_TMR_Task(void)
{
	/*~~~~~~~~~~~~~~~~~~*/
#if OS_TIMER_ENABLE
	OS_CPU_UCHAR	i = 0;
	/*~~~~~~~~~~~~~~~~~~*/

	for(i = 0; i < OS_TIMER_MAX_NUM; i++)
	{
		if(OS_TIMER_ARRAY[i] == OS_TIMER_Counter)
		{
			if((OS_X_Tmr.State[i] & OS_TIMER_STATE_Actived) != 0x00)	/* ȷ���Ѽ��� */
			{
				if(OS_X_Tmr.TimerType[i] == OS_TIMER_TYPE_nTimes)		/* ��� */
				{
					if(OS_X_Tmr.TriggerNum[i] > 0)
					{
						OS_X_Tmr.TriggerNum[i]--;
						OS_X_Tmr.TimerCallBackFunc[i]();
						OS_TIMER_ARRAY[i] += OS_X_Tmr.TimerLength[i];
					}
					else
					{
						OS_X_Tmr.State[i] &= ~OS_TIMER_STATE_Actived;
					}
				}

				if(OS_X_Tmr.TimerType[i] == OS_TIMER_TYPE_PERIOD)		/* ������ */
				{
					OS_X_Tmr.TimerCallBackFunc[i]();
					OS_TIMER_ARRAY[i] += OS_X_Tmr.TimerLength[i];
				}
			}
		}
	}
#endif
}

/*
 =======================================================================================================================
    ��Ӷ�ʱ��(����������ȼ�����)(TaskName:��������
    TimerLength:��ʱ���� TimerType:��ʱ������ TriggerNum:ִ�д���)
 =======================================================================================================================
 */
void OS_TIMER_ADD(OS_CPU_INT TaskName, OS_CPU_INT TimerLength, OS_CPU_CHAR TimerType, OS_CPU_UCHAR TriggerNum)
{
	/*~~~~~~~~~~~~~~~~~~*/
#if OS_TIMER_ENABLE
	OS_CPU_UCHAR	i = 0;
	/*~~~~~~~~~~~~~~~~~~*/

	for(i = 0; i < OS_TIMER_MAX_NUM; i++)
	{
		if((OS_X_Tmr.State[i] & OS_TIMER_STATE_Actived) == 0x00)		/* Ѱ��δ�������� */
		{
			DISABLE_ALL_INT();
			OS_X_Tmr.TimerCallBackFunc[i] = (OS_CPU_P_VOID) TaskName;	/* �������� */
			OS_X_Tmr.TimerLength[i] = TimerLength;						/* ��ʱ���� */
			OS_X_Tmr.TimerType[i] = TimerType;				/* ��ʱ������ */
			OS_X_Tmr.TriggerNum[i] = TriggerNum;			/* �������� */
			OS_TIMER_ARRAY[i] += TimerLength;
			OS_X_Tmr.State[i] |= OS_TIMER_STATE_Actived;	/* ��ʱ������ */
			ENABLE_ALL_INT();
			break;
		}
	}

	if(i == OS_TIMER_MAX_NUM)
	{
		while(1) DISABLE_ALL_INT();
	}

#else
	TaskName = TaskName;
	TimerLength = TimerLength;
	TimerType = TimerType;
	TriggerNum = TriggerNum;
#endif
}

/*
 =======================================================================================================================
    ��ջ��ȫ���
 =======================================================================================================================
 */
void OS_StackCheck_Task(void)
{
	/*~~~~~~~~~~~~~~~~~~*/
	OS_CPU_UCHAR	i = 0;
	/*~~~~~~~~~~~~~~~~~~*/

	for(i = 0; i < OS_MAX_NUM_TASK; i++)
	{
		if(OS_TaskCurLength[i] >= (OS_TaskStackSize[i] - OS_StackSafeSurplus))
		{
#if OS_HOOK_ENABLE
			StackCheckWarning(i, (OS_TaskStackSize[i] - OS_TaskCurLength[i]));
#endif
		}
	}
}
