/*$T indentinput.cpp GC 1.140 02/25/15 21:10:55 */


/*$6
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */

#include <config.h>
#include <STC15Fxxxx.H>

#define OS_MAX_NUM_TASK				6			/* ���������� */
#define OS_REG_SIZE					13			/* Ҫ����ļĴ����ֽڴ�С */
#define OS_CryFrequency				12000000L	/* CPUʱ�� */
#define OS_TickFrequency			100			/* OSʱ��Ƶ�� */
#define OS_CPU_Instruction_Cycle	6			/* CPUָ������ */

#define Task_0_StackSize			10			/* ����0�Ķ�ջ��С */
#define Task_1_StackSize			10
#define Task_2_StackSize			10
#define Task_3_StackSize			10
#define Task_SYS_StackSize			10			/* ����ϵͳ */
#define Task_Idle_StackSize			6			/* �������� */

#define Task_0_Prio					1			/* ����0���ȼ� */
#define Task_1_Prio					2
#define Task_2_Prio					3
#define Task_3_Prio					4
#define	Task_SYS_Prio				0
#define	Task_Idle_Prio				(OS_MAX_NUM_TASK - 1)

#define Task_SYS_MsgLength			8			/* ����ϵͳ��Ϣ��������С */
