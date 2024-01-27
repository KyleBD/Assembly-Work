.text
ONES:
	andi r7, r4, 0x1 #isolating the LSB
	xori r3, r7, 0x1 #XOR on the LSB will determine if it is a 1 or 0
	add r5, r5, r3 #counting 0's
	add r2, r2, r7 #counting 1's
	addi r9, r9, -1 #decrement of index 
	beq r9, r0, return  #loop has ran 32 times
	srli r4, r4, 1 #right shift to examine the next bit 
	br ONES 
return:
	ret

Greater0: 
	bge r5, r11, backtoloop1 #Comparison to keep track of the largest number of 1's
	ret
	
backtoloop1:
	mov r11, r5 #Moving the greatest 
	ret
	
Greater1:
	bge r2, r6, backtoloop2 
	ret

backtoloop2:
	mov r6, r2	#Comparison to keep track of the largest number of 0's
	ret
	
Buffer:
	addi r9, r9, -1 #decrement of index 
	beq r9, r0, return
	br Buffer


/* Program to Count the number of 1's and Zeroes in a sequence of 32-bit words,
and determines the largest of each */


.global _start
_start:	
	movia r6, 0 #LargestOnes
	movia r11, 0 #LargestZeros   
	movia r10, TEST_NUM
	ldw r4, 0(r10)
loop:  
	movia r2, 0
	movia r5, 0
	movia r9, 32 #loop decrement
	call ONES
	addi r10, r10, 4
	ldw r4, 0(r10)
	beq r4, r0, store
	call Greater1
	call Greater0
	br loop 
	/* Your code here  */
store:
	movia r13,LargestOnes
	movia r14, LargestZeroes
	stw r6, 0(r13)
	stw r11, (r14)
	br endiloop
endiloop: 
	movia r9, 10000000
	.equ LEDs, 0xFF200000
	movia r25, LEDs
	stwio r6, (r25) #LargestOnes
	call Buffer
	movia r9, 10000000
	.equ LEDs, 0xFF200000
	movia r25, LEDs
	stwio r11, (r25) #LargestZeroes
	call Buffer
	br endiloop


.data
TEST_NUM:  .word 0x4a01fead, 0xF677D671,0xDC9758D5,0xEBBD45D2,0x8059519D
            .word 0x76D8F0D2, 0xB98C9BB5, 0xD7EC3A9E, 0xD9BADC01, 0x89B377CD
            .word 0  # end of list 

LargestOnes: .word 0
LargestZeroes: .word 0