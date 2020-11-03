$ ceedling project:binary_search clean clobber test:test_binary_search test:find_minimum

$ ceedling project:pma clean clobber test:test_pma


# Semihosting
$ ceedling verbosity[4] project:arm_toolchain clean clobber test:test_pma_read test:test_ep_reg

	in more details:

1. $ arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb --specs=nosys.specs -Og -ggdb -c ../stm32/startup.S -o ../stm32/startup.o
2. $ ceedling verbosity[4] project:arm_toolchain clean clobber test:test_pma_read test:test_ep_reg
3. $ /opt/SEGGER/JLinkGDBServer
4. $ arm-none-eabi-gdb
		(gdb) monitor semihosting enable
		(gdb) continue
5. $ telnet localhost 2333
