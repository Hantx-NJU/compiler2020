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
  addi $sp, $sp, -236
  li $t0, 1
  sw $t0, -8($fp)
  lw $t0, -8($fp)
  sw $t0, -4($fp)
  li $t0, 1
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
  lw $t0, -32($fp)
  li $t1, 1
  blt $t0, $t1, label0
  j label1
label0:
  li $a0, 0
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal write
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  j label2
label1:
  lw $t0, -32($fp)
  li $t1, 1
  beq $t0, $t1, label3
  j label6
label6:
  lw $t0, -32($fp)
  li $t1, 2
  beq $t0, $t1, label3
  j label4
label3:
  li $a0, 1
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal write
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  j label5
label4:
  lw $t0, -32($fp)
  li $t1, 2
  bgt $t0, $t1, label7
  j label8
label7:
  li $t0, 3
  sw $t0, -128($fp)
  lw $t0, -128($fp)
  sw $t0, -124($fp)
label9:
  lw $t0, -128($fp)
  lw $t1, -32($fp)
  ble $t0, $t1, label10
  j label11
label10:
  lw $t0, -8($fp)
  lw $t1, -20($fp)
  add $t2, $t0, $t1
  sw $t2, -164($fp)
  lw $t0, -164($fp)
  sw $t0, -160($fp)
  lw $t0, -160($fp)
  sw $t0, -156($fp)
  lw $t0, -20($fp)
  sw $t0, -8($fp)
  lw $t0, -8($fp)
  sw $t0, -176($fp)
  lw $t0, -160($fp)
  sw $t0, -20($fp)
  lw $t0, -20($fp)
  sw $t0, -188($fp)
  lw $t0, -128($fp)
  li $t1, 1
  add $t2, $t0, $t1
  sw $t2, -208($fp)
  lw $t0, -208($fp)
  sw $t0, -128($fp)
  lw $t0, -128($fp)
  sw $t0, -200($fp)
  j label9
label11:
  lw $a0, -160($fp)
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal write
  lw $ra, 0($sp)
  addi $sp, $sp, 4
label8:
label5:
label2:
  li $v0, 0
  jr $ra
