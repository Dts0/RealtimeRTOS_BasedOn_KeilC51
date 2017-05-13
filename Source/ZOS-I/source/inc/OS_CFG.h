/*$T indentinput.cpp GC 1.140 02/27/15 15:32:59 */


/*$6
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */


#ifndef _OS_CFG_H_
#define _OS_CFG_H_

#define ENABLE					1
#define DISANLE					0

#define OS_Signal_ENABLE		ENABLE	/* �ź���ʹ�� */
#define OS_Message_ENABLE		ENABLE	/* ��Ϣʹ�� */
#define OS_Mutex_ENABLE			ENABLE	/* ������ʹ�� */
#define OS_State_ENABLE			ENABLE	/* ͳ������ʹ�� */
#define OS_State_Period			500		/* ͳ�����ڣ��δ����� */
#define StateRate				100		/* ͳ�����ķŴ��� 19% * 100 = 19 */
#define OS_TIMER_ENABLE			ENABLE	/* ��ʱ��ʹ�� */
#define OS_TIMER_MAX_NUM		4		/* ��ʱ����Ŀ */
#define OS_TIMESTAMP_ENABLE		ENABLE	/* ʱ���ʹ�� */
#define OS_HOOK_ENABLE			ENABLE	/* ���Ӻ���ʹ�� */
#define OS_STACKCHECK_ENABLE	ENABLE	/* ��ջ��� */
#define OS_StackSafeSurplus		2		/* ��ջ��ȫʣ���� */
#endif
