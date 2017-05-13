
$NOMOD51

NAME					OS_ASM

;CPU_REG_SIZE			EQU			13
CPU_XDATA_P_SIZE		EQU			2

P0						DATA	080H
P1						DATA	090H
P2						DATA	0A0H
P3						DATA	0B0H
Switch_Monitoring		BIT	0A0H.7
T0						BIT	0B0H.4
AC						BIT	0D0H.6
T1						BIT	0B0H.5
T2						BIT	090H.0
EA						BIT	0A8H.7
Switch_Remote_Control	BIT	0A0H.6
IE						DATA	0A8H
EXF2					BIT	0C8H.6
RD						BIT	0B0H.7
ES						BIT	0A8H.4
IP						DATA	0B8H
RI						BIT	098H.0
INT0					BIT	0B0H.2
CY						BIT	0D0H.7
TI						BIT	098H.1
INT1					BIT	0B0H.3
RCAP2H					DATA	0CBH
PS						BIT	0B8H.4
SP						DATA	081H
T2EX					BIT	090H.1
OV						BIT	0D0H.2
RCAP2L					DATA	0CAH
C_T2					BIT	0C8H.1
WR						BIT	0B0H.6
RCLK					BIT	0C8H.5
TCLK					BIT	0C8H.4
SBUF					DATA	099H
PCON					DATA	087H
SCON					DATA	098H
TMOD					DATA	089H
TCON					DATA	088H
IE0						BIT	088H.1
IE1						BIT	088H.3
B						DATA	0F0H
CP_RL2					BIT	0C8H.0
ACC	DATA				0E0H
ET0						BIT	0A8H.1
ET1						BIT	0A8H.3
TF0						BIT	088H.5
Speed_Control_1			BIT	090H.0
ET2						BIT	0A8H.5
TF1						BIT	088H.7
Speed_Control_2			BIT	090H.1
TF2						BIT	0C8H.7
RB8						BIT	098H.2
TH0						DATA	08CH
Speed_Control_3			BIT	090H.2
EX0						BIT	0A8H.0
IT0						BIT	088H.0
TH1						DATA	08DH
TB8						BIT	098H.3
EX1						BIT	0A8H.2
IT1						BIT	088H.2
TH2						DATA	0CDH
P						BIT	0D0H.0
SM0						BIT	098H.7
TL0						DATA	08AH
SM1						BIT	098H.6
TL1						DATA	08BH
SM2						BIT	098H.5
TL2						DATA	0CCH
PT0						BIT	0B8H.1
PT1						BIT	0B8H.3
RS0						BIT	0D0H.3
PT2						BIT	0B8H.5
TR0						BIT	088H.4
RS1						BIT	0D0H.4
TR1						BIT	088H.6
TR2						BIT	0C8H.2
PX0						BIT	0B8H.0
PX1						BIT	0B8H.2
DPH						DATA	083H
DPL						DATA	082H
EXEN2					BIT	0C8H.3
REN						BIT	098H.4
T2CON					DATA	0C8H
RXD						BIT	0B0H.0
TXD						BIT	0B0H.1
F0						BIT	0D0H.5
PSW						DATA	0D0H

?PR?OS_TaskSwitch?OS_ASM				SEGMENT CODE 
?PR?OS_TaskSwitch_Isr?OS_ASM			SEGMENT CODE
?PR?Extern_0_Int_?OS_ASM				SEGMENT CODE 
?PR?Extern_1_Int_?OS_ASM				SEGMENT CODE 
?PR?Serial_Int_?OS_ASM					SEGMENT CODE 
?PR?Timer_1_Int_?OS_ASM					SEGMENT CODE  
EXTRN									XDATA (OS_TaskStackAddr)
EXTRN									XDATA (OS_TaskStatus)
EXTRN									XDATA (OS_TaskCurLength)
EXTRN									XDATA (OS_TaskReadyList)
EXTRN									XDATA (OS_TaskRegGroup)
EXTRN									DATA (OS_CurTaskNum)
EXTRN									DATA (OS_NexTaskNum)
EXTRN									DATA (OS_SPDefaultValue)
EXTRN									DATA (TH0_Reload)
EXTRN									DATA (TL0_Reload)
EXTRN									DATA (OsMaxNumOfTask)
EXTRN									DATA (OsCpuCharSize)
EXTRN									DATA (OS_RegSize)
EXTRN									CODE (?C?PSTPTR)
EXTRN									CODE (TickTask)
PUBLIC									OS_TaskSwitch_Isr
PUBLIC									OS_TaskSwitch
PUBLIC									Extern_0_Int_
PUBLIC									Extern_1_Int_
PUBLIC									Serial_Int_
PUBLIC									Timer_1_Int_
EXTRN									CODE (Extern_0_Int)
EXTRN									CODE (Extern_1_Int)
EXTRN									CODE (Serial_Int)
EXTRN									CODE (Timer_1_Int)
EXTRN									CODE (TaskSwitch_HOOK)
DISABLE_ALL_INT	MACRO							;�ر����жϺ�
	CLR		EA
ENDM
ENABLE_ALL_INT	MACRO							;�������жϺ�
	SETB	EA
ENDM

PUSH_ALL_REG	MACRO							;ѹջ��
	PUSH	ACC
	PUSH	B
	PUSH	DPH
	PUSH	DPL
	PUSH	PSW
	PUSH	AR0
	PUSH	AR1
	PUSH	AR2
	PUSH	AR3
	PUSH	AR4
	PUSH	AR5
	PUSH	AR6
	PUSH	AR7
ENDM

POP_ALL_REG		MACRO							;��ջ��
	POP		AR7
	POP		AR6
	POP		AR5
	POP		AR4
	POP		AR3
	POP		AR2
	POP		AR1
	POP		AR0
	POP		PSW
	POP		DPL
	POP		DPH
	POP		B
	POP		ACC
ENDM


TIMER_INT_RELOAD	MACRO						;��ʱ����װ��
	MOV		TH0,TH0_Reload
	MOV		TL0,TL0_Reload
ENDM

SAVE_CONTEXT	MACRO							;�������ĺ�
	LCALL	GET_REG_GROUP_ADDR
	MOV		A,SP
	SUBB	A,OS_RegSize
	MOV		R0,A
	INC		R0
	MOV		R1,OS_RegSize
	LCALL	COPY_DATA_TOX
	MOV		R1,#CPU_XDATA_P_SIZE
	LCALL	GET_POINT_ARRAY
	MOV		A,SP
	SUBB	A,OS_RegSize
	MOV		B,OS_SPDefaultValue
	SUBB	A,B
	MOV		R1,A
	MOV		B,R1
	MOV		R0,OS_SPDefaultValue
	INC		R0
	LCALL	COPY_DATA_TOX
	MOV		R1,B
	LCALL	SAVE_STACK_LENGTH
ENDM

RESTORE_CONTEXT		MACRO						;�ָ����ĺ�
	LCALL	GET_STACK_LENGTH
	LCALL	GET_POINT_ARRAY
	MOV		R0,OS_SPDefaultValue
	CLR		A									;�����ջ�İ�ȫλ�ã���ֹ��ɱ
	ADD		A,R1								;��ν��ɱ���������޸����Լ��Ķ�ջ���PCָ��
	ADD		A,OS_RegSize						;���º������ܷ��أ��˴�����Ҫ�����
	ADD		A,R0								;�õ�һ�����ᱻ�޸ĵĶ�ջ��ַ��ȷ����ȫ
	MOV		SP,A								;�˴���ջҪ�������λ��
	INC		R0
	LCALL	COPY_DATA_FX						;<-��ɱ��Ҫ�����������ɵ�
	LCALL	GET_REG_GROUP_ADDR
	MOV		R1,OS_RegSize
	LCALL	COPY_DATA_FX
	DEC		R0
	MOV		SP,R0
ENDM

LOAD_NEXT_PRIO		MACRO						;װ����һ���ȼ�
	PUSH	ACC
	MOV		A,OS_NexTaskNum
	MOV		OS_CurTaskNum,A
	POP		ACC
ENDM

RSEG	?PR?OS_TaskSwitch?OS_ASM
OS_TaskSwitch:
USING	0
		DISABLE_ALL_INT								;�����ж�
		PUSH_ALL_REG							;ѹջ����
		LCALL	TaskSwitch_HOOK					;���ù��Ӻ���
		LCALL	FIND_NEXT_TASK					;Ѱ����һ���ȼ�����
		MOV		A,OS_CurTaskNum					;�ж��Ƿ���Ҫ�����л�
		CJNE	A,OS_NexTaskNum,TASK_SWITCH_1	;��һ���������ǵ�ǰ�������л�
		LJMP	TASK_BACK_1						;��һ���������ǵ�ǰ���񣬲����л�
		TASK_SWITCH_1:							;��ʼ�����л�
		SAVE_CONTEXT							;��������
		LOAD_NEXT_PRIO							;���ȼ�����
		RESTORE_CONTEXT							;�ָ�����
		TASK_BACK_1:							;�������л�ֱ�ӷ���
		POP_ALL_REG								;��ջ�ָ�
		ENABLE_ALL_INT								;���ж�
RET
CSEG	AT	00003H
	LJMP	Extern_0_Int_
CSEG	AT	0000BH
	LJMP	OS_TaskSwitch_Isr
CSEG	AT	00013H
	LJMP	Extern_1_Int_
CSEG	AT	0001BH 
	LJMP	Timer_1_Int_
CSEG	AT	00023H
	LJMP	Serial_Int_
RSEG	?PR?OS_TaskSwitch_Isr?OS_ASM
OS_TaskSwitch_Isr:
USING	0
		DISABLE_ALL_INT								;�����ж�
		TIMER_INT_RELOAD						;��ʱ����װ
		PUSH_ALL_REG							;ѹջ����
		LCALL	TickTask						;ִ�еδ�����
		LCALL	FIND_NEXT_TASK					;Ѱ����һ���ȼ�����
		MOV		A,OS_CurTaskNum					;�ж��Ƿ���Ҫ�����л�
		CJNE	A,OS_NexTaskNum,TASK_SWITCH_2	;��һ���������ǵ�ǰ�������л�
		LJMP	TASK_BACK_2						;��һ���������ǵ�ǰ���񣬲����л�
		TASK_SWITCH_2:							;��ʼ�����л�
		SAVE_CONTEXT							;��������
		LOAD_NEXT_PRIO							;���ȼ�����
		RESTORE_CONTEXT							;�ָ�����
		TASK_BACK_2:							;�������л�ֱ�ӷ���
		POP_ALL_REG								;��ջ�ָ�
		ENABLE_ALL_INT								;���ж�
RETI
RSEG	?PR?Extern_0_Int_?OS_ASM
Extern_0_Int_:
USING	0
		DISABLE_ALL_INT								;�����ж�
		PUSH_ALL_REG							;ѹջ����
		LCALL	Extern_0_Int
		LCALL	FIND_NEXT_TASK					;Ѱ����һ���ȼ�����
		MOV		A,OS_CurTaskNum					;�ж��Ƿ���Ҫ�����л�
		CJNE	A,OS_NexTaskNum,TASK_SWITCH_3	;��һ���������ǵ�ǰ�������л�
		LJMP	TASK_BACK_3						;��һ���������ǵ�ǰ���񣬲����л�
		TASK_SWITCH_3:							;��ʼ�����л�
		SAVE_CONTEXT							;��������
		LOAD_NEXT_PRIO							;���ȼ�����
		RESTORE_CONTEXT							;�ָ�����
		TASK_BACK_3:							;�������л�ֱ�ӷ���
		POP_ALL_REG								;��ջ�ָ�
		ENABLE_ALL_INT								;���ж�
RETI
RSEG	?PR?Extern_1_Int_?OS_ASM
Extern_1_Int_:
USING	0
		DISABLE_ALL_INT								;�����ж�
		PUSH_ALL_REG							;ѹջ����
		LCALL	Extern_1_Int
		LCALL	FIND_NEXT_TASK					;Ѱ����һ���ȼ�����
		MOV		A,OS_CurTaskNum					;�ж��Ƿ���Ҫ�����л�
		CJNE	A,OS_NexTaskNum,TASK_SWITCH_4	;��һ���������ǵ�ǰ�������л�
		LJMP	TASK_BACK_4						;��һ���������ǵ�ǰ���񣬲����л�
		TASK_SWITCH_4:							;��ʼ�����л�
		SAVE_CONTEXT							;��������
		LOAD_NEXT_PRIO							;���ȼ�����
		RESTORE_CONTEXT							;�ָ�����
		TASK_BACK_4:							;�������л�ֱ�ӷ���
		POP_ALL_REG								;��ջ�ָ�
		ENABLE_ALL_INT								;���ж�

RETI
RSEG	?PR?Serial_Int_?OS_ASM
Serial_Int_:
USING	0
		DISABLE_ALL_INT								;�����ж�
		PUSH_ALL_REG							;ѹջ����
		LCALL	Serial_Int
		LCALL	FIND_NEXT_TASK					;Ѱ����һ���ȼ�����
		MOV		A,OS_CurTaskNum					;�ж��Ƿ���Ҫ�����л�
		CJNE	A,OS_NexTaskNum,TASK_SWITCH_5	;��һ���������ǵ�ǰ�������л�
		LJMP	TASK_BACK_5						;��һ���������ǵ�ǰ���񣬲����л�
		TASK_SWITCH_5:							;��ʼ�����л�
		SAVE_CONTEXT							;��������
		LOAD_NEXT_PRIO							;���ȼ�����
		RESTORE_CONTEXT							;�ָ�����
		TASK_BACK_5:							;�������л�ֱ�ӷ���
		POP_ALL_REG								;��ջ�ָ�
		ENABLE_ALL_INT								;���ж�

RETI
RSEG	?PR?Timer_1_Int_?OS_ASM
Timer_1_Int_:
USING	0
		DISABLE_ALL_INT								;�����ж�
		PUSH_ALL_REG							;ѹջ����
		LCALL	Timer_1_Int
		LCALL	FIND_NEXT_TASK					;Ѱ����һ���ȼ�����
		MOV		A,OS_CurTaskNum					;�ж��Ƿ���Ҫ�����л�
		CJNE	A,OS_NexTaskNum,TASK_SWITCH_6	;��һ���������ǵ�ǰ�������л�
		LJMP	TASK_BACK_6						;��һ���������ǵ�ǰ���񣬲����л�
		TASK_SWITCH_6:							;��ʼ�����л�
		SAVE_CONTEXT							;��������
		LOAD_NEXT_PRIO							;���ȼ�����
		RESTORE_CONTEXT							;�ָ�����
		TASK_BACK_6:							;�������л�ֱ�ӷ���
		POP_ALL_REG								;��ջ�ָ�
		ENABLE_ALL_INT								;���ж�

RETI

DPTR_SHIFT:										;DPTR���ƺ�����
												;������ƫ������0xr6r7��,dptr��Ҫƫ�Ƶĵ�ַ
	PUSH	ACC
	CLR		C
	MOV		A,DPL
	ADD		A,AR7
	MOV		DPL,A
	MOV		A,DPH
	ADDC	A,AR6
	MOV		DPH,A
	POP		ACC
RET

GET_REG_GROUP_ADDR:								;��ȡCPU�Ĵ������ָ�롣
												;��������
	PUSH	ACC
	PUSH	B
	PUSH	AR6
	PUSH	AR7
	MOV		A,OS_RegSize
	MOV		B,OS_CurTaskNum
	MUL		AB
	MOV		AR6,B
	MOV		AR7,A
	MOV		DPTR,#OS_TaskRegGroup
	LCALL	DPTR_SHIFT
	POP		AR7
	POP		AR6
	POP		B
	POP		ACC
RET

COPY_DATA_TOX:									;�ڴ浽��帴�ơ�
												;������r0�׵�ַ��r1������dptr����ַ
	PUSH	ACC
	LOOP_2:
	MOV		A,@R0
	MOVX	@DPTR,A
	INC		R0
	INC		DPTR
	DJNZ	R1,LOOP_2
	POP		ACC
RET

COPY_DATA_FX:									;��浽�ڴ渴�ơ�
												;������r0�ڴ��׵�ַ��r1������dptr����׵�ַ
	PUSH	ACC
	LOOP_3:
	MOVX	A,@DPTR
	MOV		@R0,A
	INC		R0
	INC		DPTR
	DJNZ	R1,LOOP_3
	POP		ACC
RET

GET_POINT_ARRAY:								;��ָ������Ԫ��Ϊ��ַ��
												;��������
	PUSH	ACC
	PUSH	B
	PUSH	AR6
	PUSH	AR7
	MOV		DPTR,#OS_TaskStackAddr
	MOV		A,OS_CurTaskNum
	MOV		B,#2
	MUL		AB
	MOV		R6,B
	MOV		R7,A
	LCALL	DPTR_SHIFT
	MOVX	A,@DPTR
	PUSH	ACC
	INC		DPTR
	MOVX	A,@DPTR
	PUSH	ACC
	POP		DPL
	POP		DPH
	POP		AR7
	POP		AR6
	POP		B
	POP		ACC
RET

SAVE_STACK_LENGTH:								;�����ջ���ȡ�
												;������r1��ջ����
	PUSH	DPH
	PUSH	DPL
	PUSH	ACC
	PUSH	AR6
	PUSH	AR7
	MOV		DPTR,#OS_TaskCurLength
	MOV		A,OS_CurTaskNum
	MOV		R6,#00H
	MOV		R7,A
	LCALL	DPTR_SHIFT
	MOV		A,R1
	MOVX	@DPTR,A
	POP		AR7
	POP		AR6
	POP		ACC
	POP		DPL
	POP		DPH
RET

GET_STACK_LENGTH:								;��ȡ��ջ���ȡ�
												;�������ޣ�
												;����r1��ջ����
	PUSH	DPH
	PUSH	DPL
	PUSH	ACC
	PUSH	AR6
	PUSH	AR7
	MOV		DPTR,#OS_TaskCurLength
	MOV		A,OS_CurTaskNum
	MOV		R6,#00H
	MOV		R7,A
	LCALL	DPTR_SHIFT
	MOVX	A,@DPTR
	MOV		R1,A
	POP		AR7
	POP		AR6
	POP		ACC
	POP		DPL
	POP		DPH
RET

FIND_NEXT_TASK:									;Ѱ��������ȼ�����
												;�������ޡ�
												;����OS_NexTaskNum
	PUSH	DPL
	PUSH	DPH
	PUSH	AR0
	PUSH	AR1
	PUSH	ACC
	PUSH	B
	MOV		DPTR,#OS_TaskReadyList
	MOV		R0,OsMaxNumOfTask
	MOV		A,R0
	MOV		B,OsCpuCharSize
	DIV		AB
	INC		A
	MOV		R0,A
	LOOP_MAIN:
	MOVX	A,@DPTR
	JZ		LOOP_4
	CLR		C
	MOV		R1,OsCpuCharSize
	LOOP_SELF:
	RRC		A
	JC		LOOP_5
	DJNZ	R1,LOOP_SELF
	LOOP_5:
	MOV		A,OsCpuCharSize
	CLR		C
	SUBB	A,R1
	MOV		B,R0
	MUL		AB
	MOV		OS_NexTaskNum,A
	LJMP	END_LOOP
	LOOP_4:
	DJNZ	R0,END_LOOP
	INC		DPTR
	LJMP	LOOP_MAIN
	END_LOOP:
	POP		B
	POP		ACC
	POP		AR1
	POP		AR0
	POP		DPH
	POP		DPL
RET
END
