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
  addi $sp, $sp, -548
  li $t0, 10
  sw $t0, -52($fp)
  addi $t0, $fp, -40
  sw $t0, -72($fp)
  li $t0, 0
  li $t1, 4
  mul $t2, $t0, $t1
  sw $t2, -80($fp)
  lw $t0, -72($fp)
  lw $t1, -80($fp)
  add $t2, $t0, $t1
  sw $t2, -88($fp)
  li $t0, 2
  lw $t1, -88($fp)
  sw $t0, 0($t1)
  lw $t0, -88($fp)
  sw $t0, -56($fp)
  li $t0, 1
  sw $t0, -96($fp)
  lw $t0, -96($fp)
  sw $t0, -92($fp)
  li $t0, 3
  sw $t0, -108($fp)
  lw $t0, -108($fp)
  sw $t0, -104($fp)
label0:
  lw $t0, -96($fp)
  lw $t1, -52($fp)
  blt $t0, $t1, label1
  j label2
label1:
  li $t0, 0
  sw $t0, -140($fp)
  lw $t0, -140($fp)
  sw $t0, -136($fp)
label3:
  addi $t0, $fp, -40
  sw $t0, -72($fp)
  lw $t0, -140($fp)
  li $t1, 4
  mul $t2, $t0, $t1
  sw $t2, -188($fp)
  lw $t0, -72($fp)
  lw $t1, -188($fp)
  add $t2, $t0, $t1
  sw $t2, -196($fp)
  lw $t0, -196($fp)
  lw $t0, 0($t0)
  sw $t0, -200($fp)
  addi $t0, $fp, -40
  sw $t0, -72($fp)
  lw $t0, -140($fp)
  li $t1, 4
  mul $t2, $t0, $t1
  sw $t2, -216($fp)
  lw $t0, -72($fp)
  lw $t1, -216($fp)
  add $t2, $t0, $t1
  sw $t2, -224($fp)
  lw $t0, -224($fp)
  lw $t0, 0($t0)
  sw $t0, -228($fp)
  lw $t0, -200($fp)
  lw $t1, -228($fp)
  mul $t2, $t0, $t1
  sw $t2, -160($fp)
  lw $t0, -160($fp)
  lw $t1, -108($fp)
  ble $t0, $t1, label4
  j label5
label4:
  addi $t0, $fp, -40
  sw $t0, -72($fp)
  lw $t0, -140($fp)
  li $t1, 4
  mul $t2, $t0, $t1
  sw $t2, -272($fp)
  lw $t0, -72($fp)
  lw $t1, -272($fp)
  add $t2, $t0, $t1
  sw $t2, -280($fp)
  lw $t0, -280($fp)
  lw $t0, 0($t0)
  sw $t0, -284($fp)
  addi $t0, $fp, -40
  sw $t0, -72($fp)
  lw $t0, -140($fp)
  li $t1, 4
  mul $t2, $t0, $t1
  sw $t2, -308($fp)
  lw $t0, -72($fp)
  lw $t1, -308($fp)
  add $t2, $t0, $t1
  sw $t2, -316($fp)
  lw $t0, -316($fp)
  lw $t0, 0($t0)
  sw $t0, -320($fp)
  lw $t0, -108($fp)
  lw $t1, -320($fp)
  div $t0, $t1
  mflo $t2
  sw $t2, -256($fp)
  lw $t0, -284($fp)
  lw $t1, -256($fp)
  mul $t2, $t0, $t1
  sw $t2, -248($fp)
  lw $t0, -108($fp)
  lw $t1, -248($fp)
  beq $t0, $t1, label6
  j label7
label6:
  lw $t0, -108($fp)
  li $t1, 2
  add $t2, $t0, $t1
  sw $t2, -332($fp)
  lw $t0, -332($fp)
  sw $t0, -108($fp)
  lw $t0, -108($fp)
  sw $t0, -324($fp)
  li $t0, 1
  sw $t0, -140($fp)
  lw $t0, -140($fp)
  sw $t0, -344($fp)
  j label8
label7:
  lw $t0, -140($fp)
  li $t1, 1
  add $t2, $t0, $t1
  sw $t2, -364($fp)
  lw $t0, -364($fp)
  sw $t0, -140($fp)
  lw $t0, -140($fp)
  sw $t0, -356($fp)
label8:
  j label3
label5:
  addi $t0, $fp, -40
  sw $t0, -72($fp)
  lw $t0, -96($fp)
  li $t1, 4
  mul $t2, $t0, $t1
  sw $t2, -400($fp)
  lw $t0, -72($fp)
  lw $t1, -400($fp)
  add $t2, $t0, $t1
  sw $t2, -408($fp)
  lw $t0, -108($fp)
  lw $t1, -408($fp)
  sw $t0, 0($t1)
  lw $t0, -408($fp)
  sw $t0, -376($fp)
  lw $t0, -96($fp)
  li $t1, 1
  add $t2, $t0, $t1
  sw $t2, -420($fp)
  lw $t0, -420($fp)
  sw $t0, -96($fp)
  lw $t0, -96($fp)
  sw $t0, -412($fp)
  lw $t0, -108($fp)
  li $t1, 2
  add $t2, $t0, $t1
  sw $t2, -440($fp)
  lw $t0, -440($fp)
  sw $t0, -108($fp)
  lw $t0, -108($fp)
  sw $t0, -432($fp)
  j label0
label2:
  li $t0, 0
  sw $t0, -140($fp)
  lw $t0, -140($fp)
  sw $t0, -452($fp)
label9:
  lw $t0, -140($fp)
  lw $t1, -96($fp)
  blt $t0, $t1, label10
  j label11
label10:
  addi $t0, $fp, -40
  sw $t0, -72($fp)
  lw $t0, -140($fp)
  li $t1, 4
  mul $t2, $t0, $t1
  sw $t2, -504($fp)
  lw $t0, -72($fp)
  lw $t1, -504($fp)
  add $t2, $t0, $t1
  sw $t2, -512($fp)
  lw $t0, -512($fp)
  lw $t0, 0($t0)
  sw $t0, -516($fp)
  lw $a0, -516($fp)
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal write
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  lw $t0, -140($fp)
  li $t1, 1
  add $t2, $t0, $t1
  sw $t2, -528($fp)
  lw $t0, -528($fp)
  sw $t0, -140($fp)
  lw $t0, -140($fp)
  sw $t0, -520($fp)
  j label9
label11:
  li $v0, 0
  jr $ra
