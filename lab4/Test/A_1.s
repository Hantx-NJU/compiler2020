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
  addi $sp, $sp, -232
  li $t0, 3
  sw $t0, -8($fp)
  li $t0, 8
  sw $t0, -16($fp)
  li $t0, 24
  sw $t0, -24($fp)
  lw $t0, -8($fp)
  lw $t1, -8($fp)
  mul $t2, $t0, $t1
  sw $t2, -48($fp)
  lw $t0, -24($fp)
  lw $t1, -16($fp)
  div $t0, $t1
  mflo $t2
  sw $t2, -52($fp)
  lw $t0, -48($fp)
  lw $t1, -52($fp)
  add $t2, $t0, $t1
  sw $t2, -40($fp)
  lw $t0, -40($fp)
  li $t1, 4
  mul $t2, $t0, $t1
  sw $t2, -36($fp)
  lw $t0, -36($fp)
  sw $t0, -16($fp)
  lw $t0, -16($fp)
  sw $t0, -28($fp)
  lw $a0, -16($fp)
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal write
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  li $t0, 3
  lw $t1, -8($fp)
  mul $t2, $t0, $t1
  sw $t2, -120($fp)
  lw $t0, -16($fp)
  lw $t1, -120($fp)
  add $t2, $t0, $t1
  sw $t2, -108($fp)
  lw $t0, -108($fp)
  lw $t1, -24($fp)
  sub $t2, $t0, $t1
  sw $t2, -100($fp)
  lw $t0, -100($fp)
  li $t1, 55
  mul $t2, $t0, $t1
  sw $t2, -92($fp)
  lw $t0, -92($fp)
  lw $t1, -8($fp)
  div $t0, $t1
  mflo $t2
  sw $t2, -88($fp)
  lw $t0, -88($fp)
  sw $t0, -24($fp)
  lw $t0, -24($fp)
  sw $t0, -80($fp)
  lw $a0, -24($fp)
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal write
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  li $t0, 10
  lw $t1, -8($fp)
  mul $t2, $t0, $t1
  sw $t2, -176($fp)
  lw $t0, -176($fp)
  lw $t1, -16($fp)
  mul $t2, $t0, $t1
  sw $t2, -172($fp)
  lw $t0, -8($fp)
  lw $t1, -172($fp)
  add $t2, $t0, $t1
  sw $t2, -160($fp)
  lw $t0, -24($fp)
  li $t1, 4
  mul $t2, $t0, $t1
  sw $t2, -164($fp)
  lw $t0, -160($fp)
  lw $t1, -164($fp)
  add $t2, $t0, $t1
  sw $t2, -152($fp)
  lw $t0, -16($fp)
  lw $t1, -8($fp)
  div $t0, $t1
  mflo $t2
  sw $t2, -200($fp)
  lw $t0, -200($fp)
  li $t1, 6
  sub $t2, $t0, $t1
  sw $t2, -156($fp)
  lw $t0, -152($fp)
  lw $t1, -156($fp)
  add $t2, $t0, $t1
  sw $t2, -148($fp)
  lw $t0, -148($fp)
  sw $t0, -144($fp)
  lw $t0, -144($fp)
  sw $t0, -140($fp)
  lw $a0, -144($fp)
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal write
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  li $v0, 0
  jr $ra
