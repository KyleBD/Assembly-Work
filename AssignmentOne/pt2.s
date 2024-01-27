.text  # The numbers that turn into executable instructions
.global _start
_start:

/* r13 should contain the grade of the person with the student number, -1 if not found */
/* r10 has the student number being searched */


	movia r10, 718293		# r10 is where you put the student number being searched for
	movi r3, 0
	movi r13, -1
	movi r12, 0
	movia r8, Snumbers
/* Your code goes here  */
body:
	ldw r12, 3(r8) #moved the address by three to align the memory addres
	addi r8, r8, 4 #increment  #the student number index.
	
	beq r12, r10, next #the student number is found
	beq r12, r0, nfound #go to done when terminated
	addi r3, r3, 1 #index for the grades address for the corresponding student number
	br body
	
next:
	movia r9, Grades
	add r9, r9, r3 #use our index r3 to get the corresponding grades address
	ldb r13, 0(r9) #changed to byte instead of word 
	movia r14, result #the result is found so we will set the result to the found value
	stb r13, 0(r14)#changed to byte instead of word 
	
	/*movia r20, result 
	ldw r21, 0(r20)*/ #This is done to ensure that result is storing the correct resulting value 
	br iloop
	
nfound:
	movia r14, result #the value is not found so we will set result to -1
	stb r13, 0(r14) #changed to byte instead of word 

iloop: br iloop


.data  	# the numbers that are the data 

/* result should hold the grade of the student number put into r10, or
-1 if the student number isn't found */ 

result: .byte 0
		
/* Snumbers is the "array," terminated by a zero of the student numbers  */
Snumbers: .word 10392584, 423195, 644370, 496059, 296800
        .word 265133, 68943, 718293, 315950, 785519
        .word 982966, 345018, 220809, 369328, 935042
        .word 467872, 887795, 681936, 0

/* Grades is the corresponding "array" with the grades, in the same order*/
Grades: .byte 99, 68, 90, 85, 91, 67, 80
        .byte 66, 95, 91, 91, 99, 76, 68  
        .byte 69, 93, 90, 72