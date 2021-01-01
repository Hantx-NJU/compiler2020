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
  addi $sp, $sp, -4
  sw $fp, ($sp)
  move $fp, $sp
  li $t0, 0
  li $t1, 1
  li $t2, 0
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal read
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  move $t3, $v0
  move $t4, $t3
  move $t5, $t4
label0:
  blt $t2, $t4, label1
  j label2
label1:
  add $t6, $t0, $t1
  move $t7, $t6
  move $a0, $t1
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal write
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  move $t0, $t1
  move $s0, $t0
  move $t1, $t7
  move $s1, $t1
  addi $s2, $t2, 1
  move $t2, $s2
  move $s3, $t2
  j label0
label2:
  li $s4, 0
  lw $fp, 0($sp)
  addi $sp, $sp, 4
  move $v0, $s4
  jr $ra
