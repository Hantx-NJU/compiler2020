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

DigitSum:
  move $fp, $sp
  addi $sp, $sp, -68
  lw $t0, 8($fp)
  sw $t0, -12($fp)
  lw $t0, -12($fp)
  li $t1, 0
  beq $t0, $t1, label0
  j label1
label0:
  li $v0, 0
  jr $ra
label1:
  li $t0, 10
  addi $sp, $sp, -4
  sw $t0, 0($sp)
  lw $t0, -12($fp)
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
  li $t1, 10
  div $t0, $t1
  mflo $t2
  sw $t2, -48($fp)
  lw $t0, -48($fp)
  addi $sp, $sp, -4
  sw $t0, 0($sp)
  addi $sp, $sp, -8
  sw $ra, 4($sp)
  sw $fp, 0($sp)
  jal DigitSum
  move $sp, $fp
  lw $fp, 0($sp)
  lw $ra, 4($sp)
  addi $sp, $sp, 12
  sw $v0, -32($fp)
  lw $t0, -28($fp)
  lw $t1, -32($fp)
  add $t2, $t0, $t1
  sw $t2, -24($fp)
  lw $v0, -24($fp)
  jr $ra

main:
  move $fp, $sp
  addi $sp, $sp, -76
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
  lw $t0, -8($fp)
  li $t1, 0
  blt $t0, $t1, label2
  j label3
label2:
  li $t0, 0
  li $t1, 1
  sub $t2, $t0, $t1
  sw $t2, -40($fp)
  lw $a0, -40($fp)
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal write
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  j label4
label3:
  lw $t0, -8($fp)
  addi $sp, $sp, -4
  sw $t0, 0($sp)
  addi $sp, $sp, -8
  sw $ra, 4($sp)
  sw $fp, 0($sp)
  jal DigitSum
  move $sp, $fp
  lw $fp, 0($sp)
  lw $ra, 4($sp)
  addi $sp, $sp, 12
  sw $v0, -56($fp)
  lw $a0, -56($fp)
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal write
  lw $ra, 0($sp)
  addi $sp, $sp, 4
label4:
  li $v0, 0
  jr $ra
