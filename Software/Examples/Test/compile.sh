riscv32-unknown-elf-g++ -O2 -fno-exceptions -mno-fdiv -c -nostartfiles -march=rv32im_zfinx_zba_zbs -mabi=ilp32 ../../Source/UART.cpp -o UART.o
riscv32-unknown-elf-g++ -O2 -fno-exceptions -mno-fdiv -c -nostartfiles -march=rv32im_zfinx_zba_zbs -mabi=ilp32 ../../Source/GPIO.cpp -o GPIO.o
riscv32-unknown-elf-g++ -O2 -fno-exceptions -mno-fdiv -c -nostartfiles -march=rv32im_zfinx_zba_zbs -mabi=ilp32 ../../Source/Timer.cpp -o Timer.o
riscv32-unknown-elf-g++ -O2 -fno-exceptions -mno-fdiv -c -nostartfiles -march=rv32im_zfinx_zba_zbs -mabi=ilp32 ../../Source/SPI.cpp -o SPI.o
riscv32-unknown-elf-g++ -O2 -fno-exceptions -mno-fdiv -c -nostartfiles -march=rv32im_zfinx_zba_zbs -mabi=ilp32 test.cpp -o test.o
riscv32-unknown-elf-as -c -march=rv32im_zicsr_zfinx_zba_zbb -mabi=ilp32 setup.s -o setup.o

riscv32-unknown-elf-ld -T linker.ld -o output.elf setup.o test.o UART.o GPIO.o Timer.o SPI.o /opt/riscv/lib/gcc/riscv32-unknown-elf/12.2.0/libgcc.a /opt/riscv/riscv64-unknown-elf/lib/rv32im/ilp32/libc.a
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