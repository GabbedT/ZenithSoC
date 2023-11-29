riscv32-unknown-elf-g++ -fno-exceptions -mno-fdiv -c -nostartfiles -march=rv32im_zfinx_zba_zbs -mabi=ilp32 ../Source/UART.cpp -o UART.o
riscv32-unknown-elf-g++ -fno-exceptions -mno-fdiv -c -nostartfiles -march=rv32im_zfinx_zba_zbs -mabi=ilp32 uart_test.cpp -o uart_test.o
riscv32-unknown-elf-as -c -march=rv32im_zicsr_zfinx_zba_zbb -mabi=ilp32 setup.s -o setup.o

riscv32-unknown-elf-ld -T linker.ld -o output.elf setup.o uart_test.o UART.o
riscv32-unknown-elf-objdump -d -j .boot -j .text output.elf > output.dump

riscv32-unknown-elf-size output.elf
riscv32-unknown-elf-objdump -h output.elf

riscv32-unknown-elf-objcopy --dump-section .boot=boot.bin output.elf
xxd -p -c 1 -g 1 boot.bin > boot.hex

riscv32-unknown-elf-objcopy -O binary output.elf program.bin
xxd -p -c 1 -g 1 program.bin > program.hex

chmod +777 boot.hex
chmod +777 program.hex

rm *.elf
rm *.o