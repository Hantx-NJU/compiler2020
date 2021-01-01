FUNCTION main :
a := #0
b := #1
i := #0
READ t5
n := t5
t3 := n
LABEL label0 :
IF i < n GOTO label1
GOTO label2
LABEL label1 :
t8 := a + b
c := t8
WRITE b
a := b
t13 := a
b := c
t16 := b
t21 := i + #1
i := t21
t19 := i
GOTO label0
LABEL label2 :
RETURN #0


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
  move $t4, $t5
  move $t6, $t4
label0:
  blt $t2, $t4, label1
  j label2
label1:
  add $t7, $t0, $t1
  move $s0, $s1
  move $a0, $t1
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal write
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  move $t0, $t1
  move $s2, $t0
  move $t1, $s0
  move $s3, $t1
  addi $s4, $t2, 1
  move $t2, $s5
  move $s6, $t2
  j label0
label2:
  li $s7, 0
  addi $sp, $sp, 0
  lw $fp, 0($sp)
  lw $ra, 4($sp)
  addi $sp, $sp, 8
  move $v0, $s7
  jr $ra
