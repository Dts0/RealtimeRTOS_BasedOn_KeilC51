/*$T indentinput.cpp GC 1.140 03/06/15 16:26:57 */


/*$6
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */


#ifndef _OS_H_
#define _OS_H_

#include <APP_Cfg.h>
#include <OS_CPU.h>
#include <OS_CFG.h>

/* �������� */
#if 1

/* data ������ */
#define OS_CPU_UCHAR	unsigned char
#define OS_CPU_UINT		unsigned int
#define OS_CPU_CHAR		char
#define OS_CPU_INT		int
#define OS_CPU_LONG		long
#define OS_CPU_ULONG	unsigned long
#define OS_CPU_FLOAT	float
#define OS_CPU_DOUBLE	double

/* data ��ָ�� ָ��data������ */
#define OS_CPU_P_UCHAR	OS_CPU_UCHAR *
#define OS_CPU_P_UINT	OS_CPU_UINT *
#define OS_CPU_P_CHAR	OS_CPU_CHAR *
#define OS_CPU_P_INT	OS_CPU_INT *
#define OS_CPU_P_FLOAT	OS_CPU_FLOAT *
#define OS_CPU_P_DOUBLE OS_CPU_DOUBLE *

/* xdata ������ */
#define OS_CPU_XUCHAR	xdata unsigned char
#define OS_CPU_XUINT	xdata unsigned int
#define OS_CPU_XCHAR	xdata char
#define OS_CPU_XINT		xdata int
#define OS_CPU_XFLOAT	xdata float
#define OS_CPU_XDOUBLE	xdata double

/* data ָ�� ָ�� xdata */
#define OS_CPU_P_XUCHAR OS_CPU_UCHAR xdata *
#define OS_CPU_P_XUINT	OS_CPU_UINT xdata *
#define OS_CPU_P_XCHAR	OS_CPU_CHAR xdata *
#define OS_CPU_P_XINT	OS_CPU_INT xdata *

/* xdata ��ָ�� ָ�� data������ */
#define OS_CPU_XP_UCHAR OS_CPU_XUCHAR *
#define OS_CPU_XP_UINT	OS_CPU_XUINT *
#define OS_CPU_XP_CHAR	OS_CPU_XCHAR *
#define OS_CPU_XP_INT	OS_CPU_XINT *

/* xdata ��ָ�� ָ�� xdata ������ */
#define OS_CPU_XP_XUCHAR	OS_CPU_XUCHAR xdata *
#define OS_CPU_XP_XUINT		OS_CPU_XUINT xdata *
#define OS_CPU_XP_XCHAR		OS_CPU_XCHAR xdata *
#define OS_CPU_XP_XINT		OS_CPU_XINT xdata *

/* void ������ */
#define OS_CPU_P_VOID	void *
#define OS_CPU_XP_VOID	void xdata *
#define OS_CPU_VOID		void
#endif

/* ���ݳ��� */
#if 1
#define OS_CPU_NULL				0x00
#define OS_CPU_XUCHAR_LENGTH	8
#define OS_CPU_UCHAR_LENGTH		8
#define true					0xff
#define false					0x00
#endif

/* ״̬�� */
#define Task_Ready			0x01					/* ���� */
#define Task_Hang_NONE		0x02					/* ���� */
#define Task_Actived		0x04					/* ���� */
#define Task_Hang_DELAY		0x08					/* �ȴ���ʱ */
#define Task_Hang_SIGNAL	0x10					/* �ȴ��ź� */
#define Task_Hang_MEAASGE	0x20					/* �ȴ���Ϣ */
#define Task_Hang_MUTEX		0x40					/* �ȴ������� */

typedef struct MessageStruction_					/* ��Ϣ�ṹ�� */
{
	OS_CPU_CHAR		PendTaskNum;					/* �ȴ���������� */
	OS_CPU_CHAR		MessageSize;					/* ��Ϣ��С */
	OS_CPU_XP_VOID	MessageAddr;					/* ��Ϣָ�� */
} MessageStruction;
#define OS_MSG		MessageStruction
#define OS_X_MSG	xdata MessageStruction

typedef struct MutexStruction_						/* ����ṹ�� */
{
	OS_CPU_CHAR IsOccupy;							/* �Ƿ�ռ�� */
	OS_CPU_CHAR PendList[OS_MAX_NUM_TASK / 8 + 1];	/* �ȴ��б� */
} MutexStruction;
#define OS_MUTEX			MutexStruction
#define OS_X_MUTEX			MutexStruction

#define OS_FlagGroupLength	10
typedef struct OS_FlagGroupStruction_				/* ��־�¼��� */
{
	OS_CPU_CHAR FlagGroupShare[OS_FlagGroupLength / 8 + 1];						/* �����־ */
	OS_CPU_CHAR FlagGroupPrivate[OS_MAX_NUM_TASK][OS_FlagGroupLength / 8 + 1];	/* ˽�б�־ */
	OS_CPU_CHAR FlagGroupPendingList[OS_MAX_NUM_TASK / 8 + 1];					/* �ȴ��б� */
	OS_CPU_CHAR FlagGroupControl[OS_MAX_NUM_TASK / 8 + 1];						/* ����λ */
} OS_FlagGroupStruction;
#define OS_PEND_FLAG_GROUP_ANY_CLEAR	0x01					/* �κ�һ�������� */
#define OS_PEND_FLAG_GROUP_ANY_SET		0x02					/* �κ�һ������λ */
#define OS_PEND_FLAG_GROUP_CLEAR		0x04					/* ���**λ */
#define OS_PEND_FLAG_GROUP_SET			0x08					/* ��λ**λ */
#define OS_FLAG_GROUP					OS_FlagGroupStruction
#define OS_X_FLAG_GROUP					OS_FlagGroupStruction

/* ��ʱ������ */
#define OS_TIMER_TYPE_PERIOD	0x02							/* ���� */
#define OS_TIMER_TYPE_nTimes	0x03							/* ��� */

/* ��ʱ��״̬ */
#define OS_TIMER_STATE_Actived	0x01							/* �Ѽ��� */
typedef struct OS_TimerStruction_								/* ��ʱ���ṹ�� */
{
	OS_CPU_CHAR TimerType[OS_TIMER_MAX_NUM];					/* ��ʱ������ */
	OS_CPU_VOID (*TimerCallBackFunc[OS_TIMER_MAX_NUM]) (void);	/* �ص����� */
	OS_CPU_UINT		TimerLength[OS_TIMER_MAX_NUM];				/* ��ʱ���� */
	OS_CPU_UCHAR	State[OS_TIMER_MAX_NUM];					/* ״̬ */
	OS_CPU_UCHAR	TriggerNum[OS_TIMER_MAX_NUM];				/* �������� */
} OS_TimerStruction;
#define OS_TMR		OS_TimerStruction
#define OS_X_TMR	xdata OS_TimerStruction

/*
 =======================================================================================================================
    ���ú�
 =======================================================================================================================
 */
#define AddToReadyList(TaskNum)				{ OS_TaskReadyList[(TaskNum) / 8] |= (1 << ((TaskNum) % 8)); }
#define DeleteFromReadyList(TaskNum)		{ OS_TaskReadyList[(TaskNum) / 8] &= ~(1 << ((TaskNum) % 8)); }
#define AddToTickList(TaskNum)				{ OS_TaskTickList[(TaskNum) / 8] |= (1 << ((TaskNum) % 8)); }
#define DeleteFromTickList(TaskNum)			{ OS_TaskTickList[(TaskNum) / 8] &= ~(1 << ((TaskNum) % 8)); }
#define AddToMutexList(TaskNum, Mutex)		{ Mutex->PendList[(TaskNum) / 8] |= (1 << ((TaskNum) % 8)); }
#define DeleteFromMutexList(TaskNum, Mutex) { Mutex->PendList[(TaskNum) / 8] &= ~(1 << ((TaskNum) % 8)); }
#define AddToFlagGroupList(TaskNum, FlagGroup) \
	{ \
		FlagGroup->FlagGroupPendingList[(TaskNum) / 8] |= (1 << ((TaskNum) % 8)); \
	}
#define DeleteFromFlagGroupList(TaskNum, FlagGroup) \
	{ \
		FlagGroup->FlagGroupPendingList[(TaskNum) / 8] &= ~(1 << ((TaskNum) % 8)); \
	}
#define Set_AND_ToFlagGroup(TaskNum, FlagGroup) \
	{ \
		FlagGroup->FlagGroupControl[(TaskNum) / 8] |= (1 << ((TaskNum) % 8)); \
	}
#define Set_OR_ToFlagGroup(TaskNum, FlagGroup) \
	{ \
		FlagGroup->FlagGroupControl[(TaskNum) / 8] &= ~(1 << ((TaskNum) % 8)); \
	}

/* �ⲿ�������� */
extern OS_CPU_XUCHAR	OS_TaskReadyList[OS_MAX_NUM_TASK / 8 + 1];	/* ������ */
extern OS_CPU_XUCHAR	OS_TaskRegGroup[OS_MAX_NUM_TASK][OS_REG_SIZE];	/* ����Ĵ����� */
extern OS_CPU_XP_XUCHAR OS_TaskStackAddr[OS_MAX_NUM_TASK];				/* �����ջ��ַ */
extern OS_CPU_XUCHAR	OS_TaskCurLength[OS_MAX_NUM_TASK];				/* ��ǰ��ջ���� */
extern OS_CPU_XUINT		OS_TaskStatus[OS_MAX_NUM_TASK];					/* ����״̬ */
extern OS_CPU_UCHAR		OS_CurTaskNum;				/* ��ǰ���� */
extern OS_CPU_UCHAR		OS_NexTaskNum;				/* ��һ���� */
extern OS_CPU_UCHAR		OS_SPDefaultValue;			/* Ĭ�϶�ջָ�� */
extern OS_CPU_UCHAR		TH0_Reload;					/* TH0 */
extern OS_CPU_UCHAR		TL0_Reload;					/* TL0 */
extern OS_CPU_XUINT		CPU_Usage_ALL;				/* CPUʹ���� */
extern OS_CPU_XUINT		CPU_Usage[OS_MAX_NUM_TASK]; /* ���������CPUʹ���� */
extern OS_X_MSG			OS_Msg_SYS;

/* �ⲿ�������� */
void					OS_Init(void);
void					OS_SysTickInit(void);
void					TaskCreate
						(
							OS_CPU_INT		TaskName,
							OS_CPU_P_XUCHAR Stack,
							OS_CPU_UCHAR	StackSize,
							OS_CPU_CHAR		TaskPrio
						);
void					OS_Start(void);
void					OS_TaskSwitchManual(void);
void					OS_Delay_nTick(OS_CPU_UINT Counter);
void					OS_PendSignal(void);
void					OS_SendSignal(OS_CPU_UCHAR RecevNum);
void OS_PendMessage (OS_MSG * Message) reentrant;
void OS_SendMessage (OS_MSG * Message, OS_CPU_XP_VOID MsgAddr, OS_CPU_CHAR Size) reentrant;
void OS_OccupyMutex (OS_MUTEX * Mutex) reentrant;
void OS_ReleaseMutex (OS_MUTEX * Mutex) reentrant;
void	TaskIdle(void);
void	PreventWarning(OS_CPU_UCHAR Nothing);
void	OS_TaskSwitch(void);
void	OS_TaskSwitch_Isr(void);
void	TickTask(void);
void	TaskResume(OS_CPU_UCHAR Prio);
void	TaskSuspend(OS_CPU_UCHAR Prio);
void	OS_SendSignal_INT(OS_CPU_UCHAR RecevNum);
void OS_SendMessage_INT (OS_MSG * Message, OS_CPU_XP_VOID MsgAddr, OS_CPU_CHAR Size) reentrant;
void OS_ReleaseMutex_INT (OS_MUTEX * Mutex) reentrant;
void	Extern_0_Int(void);
void	Extern_1_Int(void);
void	Serial_Int(void);
void	Timer_1_Int(void);
void	OS_StateTask(void);
void	OS_SYS_Task(void);
void	OS_TMR_Task(void);
void	OS_TIMER_ADD(OS_CPU_INT TaskName, OS_CPU_INT TimerLength, OS_CPU_CHAR TimerType, OS_CPU_UCHAR TriggerNum);
void	OS_StackCheck_Task(void);

/* ���Ӻ��� */
void	TickTask_HOOK(void);
void	TaskCreate_HOOK(void);
void	TaskSwitch_HOOK(void);
void	StackCheckWarning(OS_CPU_UCHAR TaskPrio, OS_CPU_UCHAR CurStackLeft);

/* END */
#endif
