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

main:
  move $fp, $sp
  addi $sp, $sp, -156
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
  li $t0, 400
  addi $sp, $sp, -4
  sw $t0, 0($sp)
  lw $t0, -8($fp)
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
  lw $t0, -28($fp)
  li $t1, 0
  beq $t0, $t1, label0
  j label1
label0:
  li $t0, 1
  sw $t0, -52($fp)
  lw $t0, -52($fp)
  sw $t0, -48($fp)
  j label2
label1:
  li $t0, 4
  addi $sp, $sp, -4
  sw $t0, 0($sp)
  lw $t0, -8($fp)
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
  sw $v0, -76($fp)
  lw $t0, -76($fp)
  li $t1, 0
  beq $t0, $t1, label6
  j label4
label6:
  li $t0, 100
  addi $sp, $sp, -4
  sw $t0, 0($sp)
  lw $t0, -8($fp)
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
  sw $v0, -96($fp)
  lw $t0, -96($fp)
  li $t1, 0
  bne $t0, $t1, label3
  j label4
label3:
  li $t0, 1
  sw $t0, -52($fp)
  lw $t0, -52($fp)
  sw $t0, -116($fp)
  j label5
label4:
  li $t0, 0
  sw $t0, -52($fp)
  lw $t0, -52($fp)
  sw $t0, -128($fp)
label5:
label2:
  lw $a0, -52($fp)
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal write
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  li $v0, 0
  jr $ra
