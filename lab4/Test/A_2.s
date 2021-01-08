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

main:
  move $fp, $sp
  addi $sp, $sp, -512
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal read
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  sw $v0, -12($fp)
  lw $t0, -12($fp)
  sw $t0, -8($fp)
  lw $t0, -8($fp)
  sw $t0, -4($fp)
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal read
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  sw $v0, -24($fp)
  lw $t0, -24($fp)
  sw $t0, -20($fp)
  lw $t0, -20($fp)
  sw $t0, -16($fp)
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal read
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  sw $v0, -36($fp)
  lw $t0, -36($fp)
  sw $t0, -32($fp)
  lw $t0, -32($fp)
  sw $t0, -28($fp)
  lw $t0, -8($fp)
  lw $t1, -20($fp)
  add $t2, $t0, $t1
  sw $t2, -60($fp)
  lw $t0, -60($fp)
  lw $t1, -32($fp)
  bgt $t0, $t1, label4
  j label1
label4:
  lw $t0, -20($fp)
  lw $t1, -32($fp)
  add $t2, $t0, $t1
  sw $t2, -76($fp)
  lw $t0, -76($fp)
  lw $t1, -8($fp)
  bgt $t0, $t1, label3
  j label1
label3:
  lw $t0, -8($fp)
  lw $t1, -32($fp)
  add $t2, $t0, $t1
  sw $t2, -92($fp)
  lw $t0, -92($fp)
  lw $t1, -20($fp)
  bgt $t0, $t1, label0
  j label1
label0:
  lw $t0, -8($fp)
  lw $t1, -20($fp)
  add $t2, $t0, $t1
  sw $t2, -120($fp)
  lw $t0, -120($fp)
  lw $t1, -32($fp)
  add $t2, $t0, $t1
  sw $t2, -116($fp)
  lw $t0, -116($fp)
  sw $t0, -112($fp)
  lw $t0, -112($fp)
  sw $t0, -108($fp)
  lw $t0, -8($fp)
  lw $t1, -20($fp)
  beq $t0, $t1, label8
  j label6
label8:
  lw $t0, -8($fp)
  lw $t1, -32($fp)
  beq $t0, $t1, label5
  j label6
label5:
  li $t0, 1
  sw $t0, -172($fp)
  lw $t0, -172($fp)
  sw $t0, -168($fp)
  j label7
label6:
  lw $t0, -8($fp)
  lw $t1, -20($fp)
  beq $t0, $t1, label9
  j label13
label13:
  lw $t0, -8($fp)
  lw $t1, -32($fp)
  beq $t0, $t1, label9
  j label12
label12:
  lw $t0, -20($fp)
  lw $t1, -32($fp)
  beq $t0, $t1, label9
  j label10
label9:
  li $t0, 2
  sw $t0, -172($fp)
  lw $t0, -172($fp)
  sw $t0, -224($fp)
  j label11
label10:
  li $t0, 0
  sw $t0, -256($fp)
  lw $t0, -8($fp)
  lw $t1, -8($fp)
  mul $t2, $t0, $t1
  sw $t2, -284($fp)
  lw $t0, -20($fp)
  lw $t1, -20($fp)
  mul $t2, $t0, $t1
  sw $t2, -288($fp)
  lw $t0, -284($fp)
  lw $t1, -288($fp)
  add $t2, $t0, $t1
  sw $t2, -276($fp)
  lw $t0, -32($fp)
  lw $t1, -32($fp)
  mul $t2, $t0, $t1
  sw $t2, -280($fp)
  lw $t0, -276($fp)
  lw $t1, -280($fp)
  beq $t0, $t1, label19
  j label20
label19:
  li $t0, 1
  sw $t0, -256($fp)
label20:
  lw $t0, -256($fp)
  li $t1, 0
  bne $t0, $t1, label14
  j label18
label18:
  li $t0, 0
  sw $t0, -320($fp)
  lw $t0, -8($fp)
  lw $t1, -8($fp)
  mul $t2, $t0, $t1
  sw $t2, -348($fp)
  lw $t0, -32($fp)
  lw $t1, -32($fp)
  mul $t2, $t0, $t1
  sw $t2, -352($fp)
  lw $t0, -348($fp)
  lw $t1, -352($fp)
  add $t2, $t0, $t1
  sw $t2, -340($fp)
  lw $t0, -20($fp)
  lw $t1, -20($fp)
  mul $t2, $t0, $t1
  sw $t2, -344($fp)
  lw $t0, -340($fp)
  lw $t1, -344($fp)
  beq $t0, $t1, label21
  j label22
label21:
  li $t0, 1
  sw $t0, -320($fp)
label22:
  lw $t0, -320($fp)
  li $t1, 0
  bne $t0, $t1, label14
  j label17
label17:
  li $t0, 0
  sw $t0, -384($fp)
  lw $t0, -20($fp)
  lw $t1, -20($fp)
  mul $t2, $t0, $t1
  sw $t2, -412($fp)
  lw $t0, -32($fp)
  lw $t1, -32($fp)
  mul $t2, $t0, $t1
  sw $t2, -416($fp)
  lw $t0, -412($fp)
  lw $t1, -416($fp)
  add $t2, $t0, $t1
  sw $t2, -404($fp)
  lw $t0, -8($fp)
  lw $t1, -8($fp)
  mul $t2, $t0, $t1
  sw $t2, -408($fp)
  lw $t0, -404($fp)
  lw $t1, -408($fp)
  beq $t0, $t1, label23
  j label24
label23:
  li $t0, 1
  sw $t0, -384($fp)
label24:
  lw $t0, -384($fp)
  li $t1, 0
  bne $t0, $t1, label14
  j label15
label14:
  li $t0, 3
  sw $t0, -172($fp)
  lw $t0, -172($fp)
  sw $t0, -448($fp)
  j label16
label15:
  li $t0, 0
  sw $t0, -172($fp)
  lw $t0, -172($fp)
  sw $t0, -460($fp)
label16:
label11:
label7:
  lw $a0, -172($fp)
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal write
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  lw $a0, -112($fp)
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal write
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  j label2
label1:
  li $t0, 0
  li $t1, 1
  sub $t2, $t0, $t1
  sw $t2, -492($fp)
  lw $a0, -492($fp)
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal write
  lw $ra, 0($sp)
  addi $sp, $sp, 4
label2:
  li $v0, 0
  jr $ra
