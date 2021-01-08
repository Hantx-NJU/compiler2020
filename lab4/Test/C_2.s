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

IsLeap:
  move $fp, $sp
  addi $sp, $sp, -96
  lw $t0, 8($fp)
  sw $t0, -28($fp)
  li $t0, 400
  addi $sp, $sp, -4
  sw $t0, 0($sp)
  lw $t0, -28($fp)
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
  sw $v0, -20($fp)
  lw $t0, -20($fp)
  li $t1, 0
  beq $t0, $t1, label0
  j label3
label3:
  li $t0, 4
  addi $sp, $sp, -4
  sw $t0, 0($sp)
  lw $t0, -28($fp)
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
  sw $v0, -44($fp)
  lw $t0, -44($fp)
  li $t1, 0
  beq $t0, $t1, label4
  j label1
label4:
  li $t0, 100
  addi $sp, $sp, -4
  sw $t0, 0($sp)
  lw $t0, -28($fp)
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
  sw $v0, -64($fp)
  lw $t0, -64($fp)
  li $t1, 0
  bne $t0, $t1, label0
  j label1
label0:
  li $v0, 1
  jr $ra
  j label2
label1:
  li $v0, 0
  jr $ra
label2:

main:
  move $fp, $sp
  addi $sp, $sp, -584
  li $t0, 0
  sw $t0, -8($fp)
  li $t0, 0
  sw $t0, -16($fp)
  addi $t0, $fp, -36
  sw $t0, -60($fp)
  li $t0, 0
  li $t1, 4
  mul $t2, $t0, $t1
  sw $t2, -68($fp)
  lw $t0, -60($fp)
  lw $t1, -68($fp)
  add $t2, $t0, $t1
  sw $t2, -76($fp)
  li $t0, 0
  lw $t1, -76($fp)
  sw $t0, 0($t1)
  lw $t0, -76($fp)
  sw $t0, -44($fp)
  addi $t0, $fp, -36
  sw $t0, -60($fp)
  li $t0, 1
  li $t1, 4
  mul $t2, $t0, $t1
  sw $t2, -104($fp)
  lw $t0, -60($fp)
  lw $t1, -104($fp)
  add $t2, $t0, $t1
  sw $t2, -112($fp)
  li $t0, 31
  lw $t1, -112($fp)
  sw $t0, 0($t1)
  lw $t0, -112($fp)
  sw $t0, -80($fp)
  addi $t0, $fp, -36
  sw $t0, -60($fp)
  li $t0, 2
  li $t1, 4
  mul $t2, $t0, $t1
  sw $t2, -140($fp)
  lw $t0, -60($fp)
  lw $t1, -140($fp)
  add $t2, $t0, $t1
  sw $t2, -148($fp)
  li $t0, 28
  lw $t1, -148($fp)
  sw $t0, 0($t1)
  lw $t0, -148($fp)
  sw $t0, -116($fp)
  addi $t0, $fp, -36
  sw $t0, -60($fp)
  li $t0, 3
  li $t1, 4
  mul $t2, $t0, $t1
  sw $t2, -176($fp)
  lw $t0, -60($fp)
  lw $t1, -176($fp)
  add $t2, $t0, $t1
  sw $t2, -184($fp)
  li $t0, 31
  lw $t1, -184($fp)
  sw $t0, 0($t1)
  lw $t0, -184($fp)
  sw $t0, -152($fp)
  addi $t0, $fp, -36
  sw $t0, -60($fp)
  li $t0, 4
  li $t1, 4
  mul $t2, $t0, $t1
  sw $t2, -212($fp)
  lw $t0, -60($fp)
  lw $t1, -212($fp)
  add $t2, $t0, $t1
  sw $t2, -220($fp)
  li $t0, 30
  lw $t1, -220($fp)
  sw $t0, 0($t1)
  lw $t0, -220($fp)
  sw $t0, -188($fp)
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal read
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  sw $v0, -232($fp)
  lw $t0, -232($fp)
  sw $t0, -228($fp)
  lw $t0, -228($fp)
  sw $t0, -224($fp)
  li $t0, 1900
  sw $t0, -240($fp)
  lw $t0, -240($fp)
  sw $t0, -236($fp)
label5:
  lw $t0, -240($fp)
  lw $t1, -228($fp)
  blt $t0, $t1, label6
  j label7
label6:
  lw $t0, -240($fp)
  addi $sp, $sp, -4
  sw $t0, 0($sp)
  addi $sp, $sp, -8
  sw $ra, 4($sp)
  sw $fp, 0($sp)
  jal IsLeap
  move $sp, $fp
  lw $fp, 0($sp)
  lw $ra, 4($sp)
  addi $sp, $sp, 12
  sw $v0, -280($fp)
  lw $t0, -280($fp)
  li $t1, 0
  bne $t0, $t1, label8
  j label9
label8:
  lw $t0, -8($fp)
  li $t1, 366
  add $t2, $t0, $t1
  sw $t2, -304($fp)
  lw $t0, -304($fp)
  sw $t0, -8($fp)
  lw $t0, -8($fp)
  sw $t0, -296($fp)
  j label10
label9:
  lw $t0, -8($fp)
  li $t1, 365
  add $t2, $t0, $t1
  sw $t2, -324($fp)
  lw $t0, -324($fp)
  sw $t0, -8($fp)
  lw $t0, -8($fp)
  sw $t0, -316($fp)
label10:
  lw $t0, -240($fp)
  li $t1, 1
  add $t2, $t0, $t1
  sw $t2, -344($fp)
  lw $t0, -344($fp)
  sw $t0, -240($fp)
  lw $t0, -240($fp)
  sw $t0, -336($fp)
  j label5
label7:
  lw $t0, -228($fp)
  addi $sp, $sp, -4
  sw $t0, 0($sp)
  addi $sp, $sp, -8
  sw $ra, 4($sp)
  sw $fp, 0($sp)
  jal IsLeap
  move $sp, $fp
  lw $fp, 0($sp)
  lw $ra, 4($sp)
  addi $sp, $sp, 12
  sw $v0, -364($fp)
  lw $t0, -364($fp)
  li $t1, 0
  bne $t0, $t1, label11
  j label12
label11:
  addi $t0, $fp, -36
  sw $t0, -60($fp)
  li $t0, 2
  li $t1, 4
  mul $t2, $t0, $t1
  sw $t2, -404($fp)
  lw $t0, -60($fp)
  lw $t1, -404($fp)
  add $t2, $t0, $t1
  sw $t2, -412($fp)
  li $t0, 29
  lw $t1, -412($fp)
  sw $t0, 0($t1)
  lw $t0, -412($fp)
  sw $t0, -380($fp)
label12:
  li $t0, 1
  sw $t0, -240($fp)
  lw $t0, -240($fp)
  sw $t0, -416($fp)
label13:
  lw $t0, -240($fp)
  li $t1, 5
  blt $t0, $t1, label14
  j label15
label14:
  addi $t0, $fp, -36
  sw $t0, -60($fp)
  lw $t0, -240($fp)
  li $t1, 4
  mul $t2, $t0, $t1
  sw $t2, -480($fp)
  lw $t0, -60($fp)
  lw $t1, -480($fp)
  add $t2, $t0, $t1
  sw $t2, -488($fp)
  lw $t0, -488($fp)
  lw $t0, 0($t0)
  sw $t0, -492($fp)
  lw $t0, -8($fp)
  lw $t1, -492($fp)
  add $t2, $t0, $t1
  sw $t2, -456($fp)
  lw $t0, -456($fp)
  sw $t0, -8($fp)
  lw $t0, -8($fp)
  sw $t0, -448($fp)
  lw $t0, -240($fp)
  li $t1, 1
  add $t2, $t0, $t1
  sw $t2, -504($fp)
  lw $t0, -504($fp)
  sw $t0, -240($fp)
  lw $t0, -240($fp)
  sw $t0, -496($fp)
  j label13
label15:
  li $t0, 7
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
  sw $v0, -524($fp)
  lw $t0, -524($fp)
  sw $t0, -520($fp)
  lw $t0, -520($fp)
  sw $t0, -516($fp)
  li $t0, 14
  lw $t1, -520($fp)
  sub $t2, $t0, $t1
  sw $t2, -548($fp)
  lw $t0, -548($fp)
  sw $t0, -544($fp)
  lw $t0, -544($fp)
  sw $t0, -540($fp)
  li $a0, 5
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal write
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  lw $a0, -544($fp)
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal write
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  li $v0, 0
  jr $ra
