; Vernamova sifra na architekture DLX
; Author: Evgeny Torbin
; Login: xtorbi00

        .data 0x04          	; zacatek data segmentu v pameti
login:  .asciiz "xtorbi00"  	; <-- nahradte vasim loginem
cipher: .space 9 		; sem ukladejte sifrovane znaky (za posledni nezapomente dat 0)

        .align 2            	; dale zarovnavej na ctverice (2^2) bajtu
laddr:  .word login         	; 4B adresa vstupniho textu (pro vypis)
caddr:  .word cipher        	; 4B adresa sifrovaneho retezce (pro vypis)

        .text 0x40          	; adresa zacatku programu v pameti
        .global main

main:

	; enctyption key characters

	addi r13, r0, 20	; r13 = 20 - 't' character
	addi r23, r0, 15	; r23 = 15 - 'o' character

	addi r17, r0, 0		; r17 = i - login character counter
	addi r9, r0, 0		; r9 = 0 - flag to select character 

login_while_cond:

	lb r20, login(r17)		; r20 = login[i] - load character
	sgei r20, r20, 97		; r20 = r20 >= 97
	beqz r20, login_while_end	; if r20 == 0 jump to the end of while
	nop
	nop
	
login_while_body:

	lb r20, login(r17)		; load character again, because we have used r20 in comparison
	beqz r9, first_key		; if r9 == 0 jump to the first_key label
	nop
	nop
	j second_key
	nop
	nop

first_key:

	; add ASCII value of the first encryption key

	add r20, r20, r13		; r20 = r20 + r13
	sgti r9, r20, 122		; r9 = r20 > 122
	bnez r9, positive_overflow	; if r9 != 0 jump to the positive_overflow label
	nop
	nop
	j positive_overflow_end		; else jump to the positive_overflow_end
	nop
	nop

positive_overflow:

	; positive overflow detected, subtract 26 from the character
	; 26 is an amount of characters a-z in ASCII table

	subi r20, r20, 26		; r20 = r20 - 26

positive_overflow_end:

	addi r9, r0, 1			; r9 = 1 - change encryption key for next iteration
	j key_selection_end		; jump to key_selection_end label
	nop
	nop

second_key:

	; subtract ASCII value of the second encryption key

	sub r20, r20, r23		; r20 = r20 - r23
	slti r9, r20, 97		; r9 = r20 < 97
	bnez r9, negative_overflow	; if r9 != 0 jump to the negative_overflow label
	nop
	nop
	j negative_overflow_end		; else jump to the negative_overflow_end
	nop
	nop

negative_overflow:
	
	; negative overflow detected, add 26 to the character
	; 26 is an amount of characters a-z in ASCII table

	addi r20, r20, 26		; r20 = r20 + 26

negative_overflow_end:

	addi r9, r0, 0			; r9 = 0 - change encryption key for next iteration

key_selection_end:

	sb cipher(r17), r20		; cipher[i] = r20 - store result character
	addi r17, r17, 1		; r17 = r17 + 1 - increment counter
	j login_while_cond		; jump to the while condition
	nop
	nop

login_while_end:

	sb cipher(r17), r0		; add NULL terminated byte to the end of output

end:    addi r14, r0, caddr 	; <-- pro vypis sifry nahradte laddr adresou caddr
        trap 5  		; vypis textoveho retezce (jeho adresa se ocekava v r14)
        trap 0  		; ukonceni simulace
