symbol-file ./bin/kernel.bin
add-symbol-file ./bin/kernel.bin 0xc0100000
target remote localhost:1234
#break boot
#break *(&idt_load + 0xc0000000)
#break *(&kernel_init+0xc0000000)
#break *(&k_itoa+0xc0000000)
break *(&kernel_main+0xc0000000)
continue
display/5i $pc
display/x $eax

