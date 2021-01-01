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

fact:
  addi $sp, $sp, -4
  sw $fp, 0($sp)
  move $fp, $sp
  lw $t0, 8($fp)
  li $t1, 1
  beq $t0, $t1, label0
  j label1
label0:
  lw $fp, 0($sp)
  addi $sp, $sp, 4
  move $v0, $t0
  jr $ra
  j label2
label1:
  addi $t2, $t0, -1
  addi $sp, $sp, -4
  sw $t2, 0($sp)
  addi $sp, $sp, -4
  sw $t0, 0($sp)
  addi $sp, $sp, -4
  lw $t0, -4($fp)
  sw $t0, 0($sp)
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal fact
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  move $t1, $v0
  lw $t3, -8($fp)
  mul $t2, $t3, $t1
  addi $sp, $sp, 12
  lw $fp, 0($sp)
  addi $sp, $sp, 4
  move $v0, $t2
  jr $ra
label2:

main:
  addi $sp, $sp, -4
  sw $fp, 0($sp)
  move $fp, $sp
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal read
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  move $t0, $v0
  move $t1, $t0
  move $t2, $t1
  li $t3, 1
  bgt $t1, $t3, label3
  j label4
label3:
  addi $sp, $sp, -4
  sw $t2, 0($sp)
  addi $sp, $sp, -4
  sw $t1, 0($sp)
  addi $sp, $sp, -4
  sw $t0, 0($sp)
  addi $sp, $sp, -4
  lw $t0, -8($fp)
  sw $t0, 0($sp)
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal fact
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  move $t1, $v0
  move $t2, $t1
  move $t3, $t2
  j label5
label4:
  li $t2, 1
  move $t4, $t2
label5:
  move $a0, $t2
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal write
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  li $t5, 0
  addi $sp, $sp, 16
  lw $fp, 0($sp)
  addi $sp, $sp, 4
  move $v0, $t5
  jr $ra
