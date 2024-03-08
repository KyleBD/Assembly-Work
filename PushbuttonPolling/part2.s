.text
clock_delay: 
	movia r5, delayTime
delay_loop: 
	addi r5, r5, -1
	beq r5, r0, return_state
	br delay_loop
return_state:
	ret

.global _start
_start:
	.equ edgeCap, 0xFF20005C
	.equ LEDs, 0xFF200000
	.equ KEYs, 0xFF200050
	.equ delayTime, 1000000
	movia r11, 255
	movia r12, 0
	
main:
    movia r25, LEDs
    stwio r12, (r25)
	call clock_delay
	addi r12, r12, 1
	beq r12, r11, endloop
	br main
	

endloop:
	br endloop
	
	




			
	