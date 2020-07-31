$ ceedling project:binary_search clean clobber test:test_binary_search test:find_minimum

$ ceedling project:pma clean clobber test:test_pma

$ ceedling verbosity[4] project:arm_toolchain clean clobber test:test_pma

$ arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb --specs=nosys.specs -Og -ggdb -c ../stm32/startup.S -o ../stm32/startup.o

