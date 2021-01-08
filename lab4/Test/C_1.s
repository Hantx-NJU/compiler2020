.data
_prompt: .asciiz "Enter an integer:"
_ret: .asciiz "\n"
.globl main
.text
read:
  li $v0, 4
  la $a0, _prompt
  syscall
  li $v0, 5
  syscall
  jr $ra

write:
  li $v0, 1
  syscall
  li $v0, 4
  la $a0, _ret
  syscall
  move $v0, $0
  jr $ra

mod:
  move $fp, $sp
  addi $sp, $sp, -36
  lw $t0, 8($fp)
  sw $t0, -8($fp)
  lw $t0, 12($fp)
  sw $t0, -28($fp)
  lw $t0, -8($fp)
  lw $t1, -28($fp)
  div $t0, $t1
  mflo $t2
  sw $t2, -16($fp)
  lw $t0, -16($fp)
  lw $t1, -28($fp)
  mul $t2, $t0, $t1
  sw $t2, -12($fp)
  lw $t0, -8($fp)
  lw $t1, -12($fp)
  sub $t2, $t0, $t1
  sw $t2, -4($fp)
  lw $v0, -4($fp)
  jr $ra

gcd:
  move $fp, $sp
  addi $sp, $sp, -56
  lw $t0, 8($fp)
  sw $t0, -12($fp)
  lw $t0, 12($fp)
  sw $t0, -20($fp)
  lw $t0, -12($fp)
  li $t1, 0
  beq $t0, $t1, label0
  j label1
label0:
  lw $v0, -20($fp)
  jr $ra
label1:
  lw $t0, -12($fp)
  addi $sp, $sp, -4
  sw $t0, 0($sp)
  lw $t0, -20($fp)
  addi $sp, $sp, -4
  sw $t0, 0($sp)
  addi $sp, $sp, -8
  sw $ra, 4($sp)
  sw $fp, 0($sp)
  jal mod
  move $sp, $fp
  lw $fp, 0($sp)
  lw $ra, 4($sp)
  addi $sp, $sp, 16
  sw $v0, -28($fp)
  lw $t0, -12($fp)
  addi $sp, $sp, -4
  sw $t0, 0($sp)
  lw $t0, -28($fp)
  addi $sp, $sp, -4
  sw $t0, 0($sp)
  addi $sp, $sp, -8
  sw $ra, 4($sp)
  sw $fp, 0($sp)
  jal gcd
  move $sp, $fp
  lw $fp, 0($sp)
  lw $ra, 4($sp)
  addi $sp, $sp, 16
  sw $v0, -24($fp)
  lw $v0, -24($fp)
  jr $ra

lcm:
  move $fp, $sp
  addi $sp, $sp, -40
  lw $t0, 8($fp)
  sw $t0, -16($fp)
  lw $t0, 12($fp)
  sw $t0, -20($fp)
  lw $t0, -16($fp)
  lw $t1, -20($fp)
  mul $t2, $t0, $t1
  sw $t2, -8($fp)
  lw $t0, -20($fp)
  addi $sp, $sp, -4
  sw $t0, 0($sp)
  lw $t0, -16($fp)
  addi $sp, $sp, -4
  sw $t0, 0($sp)
  addi $sp, $sp, -8
  sw $ra, 4($sp)
  sw $fp, 0($sp)
  jal gcd
  move $sp, $fp
  lw $fp, 0($sp)
  lw $ra, 4($sp)
  addi $sp, $sp, 16
  sw $v0, -12($fp)
  lw $t0, -8($fp)
  lw $t1, -12($fp)
  div $t0, $t1
  mflo $t2
  sw $t2, -4($fp)
  lw $v0, -4($fp)
  jr $ra

main:
  move $fp, $sp
  addi $sp, $sp, -956
  li $t0, 0
  sw $t0, -8($fp)
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal read
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  sw $v0, -428($fp)
  lw $t0, -428($fp)
  sw $t0, -424($fp)
  lw $t0, -424($fp)
  sw $t0, -420($fp)
  li $t0, 0
  sw $t0, -436($fp)
  lw $t0, -436($fp)
  sw $t0, -432($fp)
label2:
  lw $t0, -436($fp)
  lw $t1, -424($fp)
  blt $t0, $t1, label3
  j label4
label3:
  addi $t0, $fp, -208
  sw $t0, -480($fp)
  lw $t0, -436($fp)
  li $t1, 4
  mul $t2, $t0, $t1
  sw $t2, -488($fp)
  lw $t0, -480($fp)
  lw $t1, -488($fp)
  add $t2, $t0, $t1
  sw $t2, -496($fp)
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal read
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  sw $v0, -472($fp)
  lw $t0, -472($fp)
  lw $t1, -496($fp)
  sw $t0, 0($t1)
  lw $t0, -496($fp)
  sw $t0, -464($fp)
  addi $t0, $fp, -412
  sw $t0, -516($fp)
  lw $t0, -436($fp)
  li $t1, 4
  mul $t2, $t0, $t1
  sw $t2, -524($fp)
  lw $t0, -516($fp)
  lw $t1, -524($fp)
  add $t2, $t0, $t1
  sw $t2, -532($fp)
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal read
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  sw $v0, -508($fp)
  lw $t0, -508($fp)
  lw $t1, -532($fp)
  sw $t0, 0($t1)
  lw $t0, -532($fp)
  sw $t0, -500($fp)
  lw $t0, -436($fp)
  li $t1, 1
  add $t2, $t0, $t1
  sw $t2, -544($fp)
  lw $t0, -544($fp)
  sw $t0, -436($fp)
  lw $t0, -436($fp)
  sw $t0, -536($fp)
  j label2
label4:
  addi $t0, $fp, -412
  sw $t0, -516($fp)
  li $t0, 0
  li $t1, 4
  mul $t2, $t0, $t1
  sw $t2, -580($fp)
  lw $t0, -516($fp)
  lw $t1, -580($fp)
  add $t2, $t0, $t1
  sw $t2, -588($fp)
  lw $t0, -588($fp)
  lw $t0, 0($t0)
  sw $t0, -560($fp)
  lw $t0, -560($fp)
  sw $t0, -556($fp)
  li $t0, 1
  sw $t0, -436($fp)
  lw $t0, -436($fp)
  sw $t0, -592($fp)
label5:
  lw $t0, -436($fp)
  lw $t1, -424($fp)
  blt $t0, $t1, label6
  j label7
label6:
  addi $t0, $fp, -412
  sw $t0, -516($fp)
  lw $t0, -436($fp)
  li $t1, 4
  mul $t2, $t0, $t1
  sw $t2, -656($fp)
  lw $t0, -516($fp)
  lw $t1, -656($fp)
  add $t2, $t0, $t1
  sw $t2, -664($fp)
  lw $t0, -664($fp)
  lw $t0, 0($t0)
  sw $t0, -668($fp)
  lw $t0, -668($fp)
  addi $sp, $sp, -4
  sw $t0, 0($sp)
  lw $t0, -560($fp)
  addi $sp, $sp, -4
  sw $t0, 0($sp)
  addi $sp, $sp, -8
  sw $ra, 4($sp)
  sw $fp, 0($sp)
  jal lcm
  move $sp, $fp
  lw $fp, 0($sp)
  lw $ra, 4($sp)
  addi $sp, $sp, 16
  sw $v0, -632($fp)
  lw $t0, -632($fp)
  sw $t0, -560($fp)
  lw $t0, -560($fp)
  sw $t0, -624($fp)
  lw $t0, -436($fp)
  li $t1, 1
  add $t2, $t0, $t1
  sw $t2, -684($fp)
  lw $t0, -684($fp)
  sw $t0, -436($fp)
  lw $t0, -436($fp)
  sw $t0, -676($fp)
  j label5
label7:
  li $t0, 0
  sw $t0, -436($fp)
  lw $t0, -436($fp)
  sw $t0, -696($fp)
label8:
  lw $t0, -436($fp)
  lw $t1, -424($fp)
  blt $t0, $t1, label9
  j label10
label9:
  addi $t0, $fp, -208
  sw $t0, -480($fp)
  lw $t0, -436($fp)
  li $t1, 4
  mul $t2, $t0, $t1
  sw $t2, -768($fp)
  lw $t0, -480($fp)
  lw $t1, -768($fp)
  add $t2, $t0, $t1
  sw $t2, -776($fp)
  lw $t0, -776($fp)
  lw $t0, 0($t0)
  sw $t0, -780($fp)
  addi $t0, $fp, -412
  sw $t0, -516($fp)
  lw $t0, -436($fp)
  li $t1, 4
  mul $t2, $t0, $t1
  sw $t2, -804($fp)
  lw $t0, -516($fp)
  lw $t1, -804($fp)
  add $t2, $t0, $t1
  sw $t2, -812($fp)
  lw $t0, -812($fp)
  lw $t0, 0($t0)
  sw $t0, -816($fp)
  lw $t0, -560($fp)
  lw $t1, -816($fp)
  div $t0, $t1
  mflo $t2
  sw $t2, -752($fp)
  lw $t0, -780($fp)
  lw $t1, -752($fp)
  mul $t2, $t0, $t1
  sw $t2, -744($fp)
  lw $t0, -8($fp)
  lw $t1, -744($fp)
  add $t2, $t0, $t1
  sw $t2, -736($fp)
  lw $t0, -736($fp)
  sw $t0, -8($fp)
  lw $t0, -8($fp)
  sw $t0, -728($fp)
  lw $t0, -436($fp)
  li $t1, 1
  add $t2, $t0, $t1
  sw $t2, -828($fp)
  lw $t0, -828($fp)
  sw $t0, -436($fp)
  lw $t0, -436($fp)
  sw $t0, -820($fp)
  j label8
label10:
  lw $t0, -560($fp)
  addi $sp, $sp, -4
  sw $t0, 0($sp)
  lw $t0, -8($fp)
  addi $sp, $sp, -4
  sw $t0, 0($sp)
  addi $sp, $sp, -8
  sw $ra, 4($sp)
  sw $fp, 0($sp)
  jal gcd
  move $sp, $fp
  lw $fp, 0($sp)
  lw $ra, 4($sp)
  addi $sp, $sp, 16
  sw $v0, -848($fp)
  lw $t0, -848($fp)
  sw $t0, -844($fp)
  lw $t0, -844($fp)
  sw $t0, -840($fp)
  lw $t0, -8($fp)
  lw $t1, -844($fp)
  div $t0, $t1
  mflo $t2
  sw $t2, -872($fp)
  lw $t0, -872($fp)
  sw $t0, -8($fp)
  lw $t0, -8($fp)
  sw $t0, -864($fp)
  lw $t0, -560($fp)
  lw $t1, -844($fp)
  div $t0, $t1
  mflo $t2
  sw $t2, -892($fp)
  lw $t0, -892($fp)
  sw $t0, -560($fp)
  lw $t0, -560($fp)
  sw $t0, -884($fp)
  lw $t0, -560($fp)
  li $t1, 1
  beq $t0, $t1, label11
  j label12
label11:
  lw $a0, -8($fp)
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal write
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  j label13
label12:
  lw $a0, -8($fp)
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal write
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  lw $a0, -560($fp)
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal write
  lw $ra, 0($sp)
  addi $sp, $sp, 4
label13:
  li $v0, 0
  jr $ra
