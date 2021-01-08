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

Joseph:
  move $fp, $sp
  addi $sp, $sp, -852
  lw $t0, 8($fp)
  sw $t0, -416($fp)
  li $t0, 0
  sw $t0, -412($fp)
  lw $t0, -416($fp)
  sw $t0, -420($fp)
  li $t0, 0
  sw $t0, -428($fp)
  li $t0, 0
  sw $t0, -436($fp)
label0:
  lw $t0, -412($fp)
  lw $t1, -416($fp)
  blt $t0, $t1, label1
  j label2
label1:
  addi $t0, $fp, -400
  sw $t0, -476($fp)
  lw $t0, -412($fp)
  li $t1, 4
  mul $t2, $t0, $t1
  sw $t2, -484($fp)
  lw $t0, -476($fp)
  lw $t1, -484($fp)
  add $t2, $t0, $t1
  sw $t2, -492($fp)
  li $t0, 1
  lw $t1, -492($fp)
  sw $t0, 0($t1)
  lw $t0, -492($fp)
  sw $t0, -460($fp)
  lw $t0, -412($fp)
  li $t1, 1
  add $t2, $t0, $t1
  sw $t2, -504($fp)
  lw $t0, -504($fp)
  sw $t0, -412($fp)
  lw $t0, -412($fp)
  sw $t0, -496($fp)
  j label0
label2:
label3:
  lw $t0, -420($fp)
  li $t1, 2
  bgt $t0, $t1, label4
  j label5
label4:
  addi $t0, $fp, -400
  sw $t0, -476($fp)
  lw $t0, -428($fp)
  li $t1, 4
  mul $t2, $t0, $t1
  sw $t2, -564($fp)
  lw $t0, -476($fp)
  lw $t1, -564($fp)
  add $t2, $t0, $t1
  sw $t2, -572($fp)
  lw $t0, -572($fp)
  lw $t0, 0($t0)
  sw $t0, -576($fp)
  lw $t0, -576($fp)
  li $t1, 1
  beq $t0, $t1, label6
  j label7
label6:
  lw $t0, -436($fp)
  li $t1, 1
  add $t2, $t0, $t1
  sw $t2, -588($fp)
  lw $t0, -588($fp)
  sw $t0, -436($fp)
  lw $t0, -436($fp)
  sw $t0, -580($fp)
  li $t0, 3
  lw $t1, -436($fp)
  beq $t0, $t1, label8
  j label9
label8:
  addi $t0, $fp, -400
  sw $t0, -476($fp)
  lw $t0, -428($fp)
  li $t1, 4
  mul $t2, $t0, $t1
  sw $t2, -640($fp)
  lw $t0, -476($fp)
  lw $t1, -640($fp)
  add $t2, $t0, $t1
  sw $t2, -648($fp)
  li $t0, 0
  lw $t1, -648($fp)
  sw $t0, 0($t1)
  lw $t0, -648($fp)
  sw $t0, -616($fp)
  li $t0, 0
  sw $t0, -436($fp)
  lw $t0, -436($fp)
  sw $t0, -652($fp)
  lw $t0, -420($fp)
  li $t1, 1
  sub $t2, $t0, $t1
  sw $t2, -672($fp)
  lw $t0, -672($fp)
  sw $t0, -420($fp)
  lw $t0, -420($fp)
  sw $t0, -664($fp)
label9:
label7:
  lw $t0, -428($fp)
  li $t1, 1
  add $t2, $t0, $t1
  sw $t2, -692($fp)
  lw $t0, -692($fp)
  sw $t0, -428($fp)
  lw $t0, -428($fp)
  sw $t0, -684($fp)
  lw $t0, -428($fp)
  lw $t1, -416($fp)
  beq $t0, $t1, label10
  j label11
label10:
  li $t0, 0
  sw $t0, -428($fp)
  lw $t0, -428($fp)
  sw $t0, -720($fp)
label11:
  j label3
label5:
  li $t0, 0
  sw $t0, -412($fp)
  lw $t0, -412($fp)
  sw $t0, -732($fp)
label12:
  lw $t0, -412($fp)
  lw $t1, -416($fp)
  blt $t0, $t1, label13
  j label14
label13:
  addi $t0, $fp, -400
  sw $t0, -476($fp)
  lw $t0, -412($fp)
  li $t1, 4
  mul $t2, $t0, $t1
  sw $t2, -792($fp)
  lw $t0, -476($fp)
  lw $t1, -792($fp)
  add $t2, $t0, $t1
  sw $t2, -800($fp)
  lw $t0, -800($fp)
  lw $t0, 0($t0)
  sw $t0, -804($fp)
  li $t0, 1
  lw $t1, -804($fp)
  beq $t0, $t1, label15
  j label16
label15:
  lw $t0, -412($fp)
  li $t1, 1
  add $t2, $t0, $t1
  sw $t2, -812($fp)
  lw $a0, -812($fp)
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal write
  lw $ra, 0($sp)
  addi $sp, $sp, 4
label16:
  lw $t0, -412($fp)
  li $t1, 1
  add $t2, $t0, $t1
  sw $t2, -832($fp)
  lw $t0, -832($fp)
  sw $t0, -412($fp)
  lw $t0, -412($fp)
  sw $t0, -824($fp)
  j label12
label14:
  li $v0, 0
  jr $ra

main:
  move $fp, $sp
  addi $sp, $sp, -108
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
  li $t0, 0
  sw $t0, -28($fp)
  lw $t0, -8($fp)
  li $t1, 3
  bgt $t0, $t1, label22
  j label21
label22:
  lw $t0, -8($fp)
  li $t1, 100
  ble $t0, $t1, label20
  j label21
label20:
  li $t0, 1
  sw $t0, -28($fp)
label21:
  lw $t0, -28($fp)
  li $t1, 0
  bne $t0, $t1, label18
  j label17
label17:
  li $t0, 0
  li $t1, 1
  sub $t2, $t0, $t1
  sw $t2, -76($fp)
  lw $a0, -76($fp)
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal write
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  j label19
label18:
  lw $t0, -8($fp)
  addi $sp, $sp, -4
  sw $t0, 0($sp)
  addi $sp, $sp, -8
  sw $ra, 4($sp)
  sw $fp, 0($sp)
  jal Joseph
  move $sp, $fp
  lw $fp, 0($sp)
  lw $ra, 4($sp)
  addi $sp, $sp, 12
  sw $v0, -88($fp)
label19:
  li $v0, 0
  jr $ra
