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
  addi $sp, $sp, -528
  li $t0, 0
  sw $t0, -8($fp)
label0:
  lw $t0, -8($fp)
  li $t1, 5
  blt $t0, $t1, label1
  j label2
label1:
  addi $t0, $fp, -28
  sw $t0, -72($fp)
  lw $t0, -8($fp)
  li $t1, 4
  mul $t2, $t0, $t1
  sw $t2, -80($fp)
  lw $t0, -72($fp)
  lw $t1, -80($fp)
  add $t2, $t0, $t1
  sw $t2, -88($fp)
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal read
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  sw $v0, -64($fp)
  lw $t0, -64($fp)
  lw $t1, -88($fp)
  sw $t0, 0($t1)
  lw $t0, -88($fp)
  sw $t0, -56($fp)
  lw $t0, -8($fp)
  li $t1, 1
  add $t2, $t0, $t1
  sw $t2, -100($fp)
  lw $t0, -100($fp)
  sw $t0, -8($fp)
  lw $t0, -8($fp)
  sw $t0, -92($fp)
  j label0
label2:
  li $t0, 0
  sw $t0, -8($fp)
  lw $t0, -8($fp)
  sw $t0, -112($fp)
label3:
  lw $t0, -8($fp)
  li $t1, 4
  blt $t0, $t1, label4
  j label5
label4:
  lw $t0, -8($fp)
  li $t1, 1
  add $t2, $t0, $t1
  sw $t2, -152($fp)
  lw $t0, -152($fp)
  sw $t0, -148($fp)
  lw $t0, -148($fp)
  sw $t0, -144($fp)
label6:
  lw $t0, -148($fp)
  li $t1, 5
  blt $t0, $t1, label7
  j label8
label7:
  addi $t0, $fp, -28
  sw $t0, -72($fp)
  lw $t0, -8($fp)
  li $t1, 4
  mul $t2, $t0, $t1
  sw $t2, -212($fp)
  lw $t0, -72($fp)
  lw $t1, -212($fp)
  add $t2, $t0, $t1
  sw $t2, -220($fp)
  lw $t0, -220($fp)
  lw $t0, 0($t0)
  sw $t0, -224($fp)
  addi $t0, $fp, -28
  sw $t0, -72($fp)
  lw $t0, -148($fp)
  li $t1, 4
  mul $t2, $t0, $t1
  sw $t2, -240($fp)
  lw $t0, -72($fp)
  lw $t1, -240($fp)
  add $t2, $t0, $t1
  sw $t2, -248($fp)
  lw $t0, -248($fp)
  lw $t0, 0($t0)
  sw $t0, -252($fp)
  lw $t0, -224($fp)
  lw $t1, -252($fp)
  bgt $t0, $t1, label9
  j label10
label9:
  addi $t0, $fp, -28
  sw $t0, -72($fp)
  lw $t0, -8($fp)
  li $t1, 4
  mul $t2, $t0, $t1
  sw $t2, -280($fp)
  lw $t0, -72($fp)
  lw $t1, -280($fp)
  add $t2, $t0, $t1
  sw $t2, -288($fp)
  lw $t0, -288($fp)
  lw $t0, 0($t0)
  sw $t0, -260($fp)
  lw $t0, -260($fp)
  sw $t0, -256($fp)
  addi $t0, $fp, -28
  sw $t0, -72($fp)
  lw $t0, -8($fp)
  li $t1, 4
  mul $t2, $t0, $t1
  sw $t2, -316($fp)
  lw $t0, -72($fp)
  lw $t1, -316($fp)
  add $t2, $t0, $t1
  sw $t2, -324($fp)
  addi $t0, $fp, -28
  sw $t0, -72($fp)
  lw $t0, -148($fp)
  li $t1, 4
  mul $t2, $t0, $t1
  sw $t2, -340($fp)
  lw $t0, -72($fp)
  lw $t1, -340($fp)
  add $t2, $t0, $t1
  sw $t2, -348($fp)
  lw $t0, -348($fp)
  lw $t0, 0($t0)
  sw $t0, -352($fp)
  lw $t0, -352($fp)
  lw $t1, -324($fp)
  sw $t0, 0($t1)
  lw $t0, -324($fp)
  sw $t0, -292($fp)
  addi $t0, $fp, -28
  sw $t0, -72($fp)
  lw $t0, -148($fp)
  li $t1, 4
  mul $t2, $t0, $t1
  sw $t2, -380($fp)
  lw $t0, -72($fp)
  lw $t1, -380($fp)
  add $t2, $t0, $t1
  sw $t2, -388($fp)
  lw $t0, -260($fp)
  lw $t1, -388($fp)
  sw $t0, 0($t1)
  lw $t0, -388($fp)
  sw $t0, -356($fp)
label10:
  lw $t0, -148($fp)
  li $t1, 1
  add $t2, $t0, $t1
  sw $t2, -400($fp)
  lw $t0, -400($fp)
  sw $t0, -148($fp)
  lw $t0, -148($fp)
  sw $t0, -392($fp)
  j label6
label8:
  lw $t0, -8($fp)
  li $t1, 1
  add $t2, $t0, $t1
  sw $t2, -420($fp)
  lw $t0, -420($fp)
  sw $t0, -8($fp)
  lw $t0, -8($fp)
  sw $t0, -412($fp)
  j label3
label5:
  li $t0, 0
  sw $t0, -8($fp)
  lw $t0, -8($fp)
  sw $t0, -432($fp)
label11:
  lw $t0, -8($fp)
  li $t1, 5
  blt $t0, $t1, label12
  j label13
label12:
  addi $t0, $fp, -28
  sw $t0, -72($fp)
  lw $t0, -8($fp)
  li $t1, 4
  mul $t2, $t0, $t1
  sw $t2, -484($fp)
  lw $t0, -72($fp)
  lw $t1, -484($fp)
  add $t2, $t0, $t1
  sw $t2, -492($fp)
  lw $t0, -492($fp)
  lw $t0, 0($t0)
  sw $t0, -496($fp)
  lw $a0, -496($fp)
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal write
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  lw $t0, -8($fp)
  li $t1, 1
  add $t2, $t0, $t1
  sw $t2, -508($fp)
  lw $t0, -508($fp)
  sw $t0, -8($fp)
  lw $t0, -8($fp)
  sw $t0, -500($fp)
  j label11
label13:
  li $v0, 0
  jr $ra
