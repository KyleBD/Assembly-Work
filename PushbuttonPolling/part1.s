.text 
KEY0:
	ldwio r2, (r8) 	
	beq r2, r0, task0
	br KEY0
task0:
	movi r9, 1
	stwio r9, (r25)
	ret
KEY1:
	ldwio r2, (r8) 	
	beq r2, r0, task1_check
	br KEY1
task1_check:
	movi r14, 14
	ble r9, r14, add_task1
	ret
add_task1:
	addi r9, r9, 1
	stwio r9, (r25)
	ret
KEY2:
	ldwio r2, (r8) 	
	beq r2, r0, task2_check
	br KEY2
task2_check:
	bge r9, r15, sub_task2
	ret
sub_task2:
	addi r9, r9, -1
	stwio r9, (r25)
	ret
KEY3:	
	ldwio r2, (r8) 	
	beq r2, r0, task_3
	br KEY3
task_3:
	stwio r0, (r25)
	ldwio r2, (r8) 	
	bne r2, r0, task0
	br task_3
	
.global _start
_start:
	.equ LEDs, 0xFF200000
	movia r25, LEDs
	movi r9, 0 #LED Data Value 
	movi r14, 14
	movi r15, 2
	br polling

polling:
	
	movia r8, 0xFF200050 #KEY address 
	ldwio r2, 0(r8) 	
	movia r3,0x0001 #representing 1 for KEY0
	beq r2, r3, key0call
	slli r3, r3, 1 #to represent 2 for KEY1
	beq r2, r3, key1call
	slli r3, r3, 1 #to represent 4 for KEY2
	beq r2, r3, key2call
	slli r3, r3, 1 #to represent 8 fro KEY3
	beq r2, r3, key3call  
	br polling   
	

key0call:
	call KEY0
	br polling
key1call:
	call KEY1
	br polling
key3call:
	call KEY3
	br polling
key2call:
	call KEY2
	br polling