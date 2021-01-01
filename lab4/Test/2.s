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

foo:
  addi $sp, $sp, -4
  sw $fp, ($sp)
  move $fp, $sp
  lw $t0, 8($fp)
  move $a0, $t0
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal write
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  li $t1, 0
  lw $fp, 0($sp)
  lw $ra, 4($sp)
  addi $sp, $sp, 8
  move $v0, $t1
  jr $ra

main:
  addi $sp, $sp, -4
  sw $fp, ($sp)
  move $fp, $sp
  addi $sp, $sp, -4
  li $t0, 5
  sw $t0, ($sp)
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal foo
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  move $t1, $v0
  li $t2, 0
  addi $sp, $sp, 4
  lw $fp, 0($sp)
  addi $sp, $sp, 4
  move $v0, $t2
  jr $ra
