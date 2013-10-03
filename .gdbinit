symbol-file ./bin/kernel.bin
add-symbol-file ./bin/kernel.bin 0xc0100000
target remote localhost:1234
break boot
#break *(&idt_load + 0xc0000000)
continue
display/5i $pc
display/x $eax

