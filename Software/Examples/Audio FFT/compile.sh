riscv32-unknown-elf-g++ -O1 -flto -fno-exceptions -mno-fdiv -c -nostartfiles -march=rv32im_zfinx_zba_zbs -mabi=ilp32 ../../Source/Timer.cpp -o Timer.o
riscv32-unknown-elf-g++ -O1 -flto -fno-exceptions -mno-fdiv -c -nostartfiles -march=rv32im_zfinx_zba_zbs -mabi=ilp32 ../../Source/Serial_IO.cpp -o Serial_IO.o
riscv32-unknown-elf-g++ -O1 -flto -fno-exceptions -mno-fdiv -c -nostartfiles -march=rv32im_zfinx_zba_zbs -mabi=ilp32 ../../Source/PDM2PCM.cpp -o PDM2PCM.o
riscv32-unknown-elf-g++ -O1 -flto -fno-exceptions -mno-fdiv -c -nostartfiles -march=rv32im_zfinx_zba_zbs -mabi=ilp32 ../../Source/Ethernet.cpp -o Ethernet.o
riscv32-unknown-elf-g++ -O2 -flto -fno-exceptions -mno-fdiv -c -nostartfiles -fno-use-cxa-atexit -march=rv32im_zfinx_zba_zbs -mabi=ilp32 audio_recording.cpp -o audio_recording.o
riscv32-unknown-elf-as -c -march=rv32im_zicsr_zfinx_zba_zbb -mabi=ilp32 setup.s -o setup.o

riscv32-unknown-elf-g++ -flto -O2 -march=rv32im_zicsr_zfinx_zba_zbb -nostartfiles -T linker.ld -o output.elf setup.o audio_recording.o Serial_IO.o PDM2PCM.o Ethernet.o Timer.o /opt/riscv/lib/gcc/riscv32-unknown-elf/14.2.0/libgcc.a /opt/riscv/riscv32-unknown-elf/lib/libc.a
riscv32-unknown-elf-objdump -d -j .boot -j .text output.elf > output.dump

riscv32-unknown-elf-size output.elf
riscv32-unknown-elf-objdump -h output.elf

riscv32-unknown-elf-objcopy --dump-section .boot=boot.bin output.elf
xxd -p -c 1 -g 1 boot.bin > boot.hex



riscv32-unknown-elf-objcopy -O binary output.elf program.bin
xxd -p -c 1 -g 1 program.bin > program.hex

chmod +777 boot.hex
chmod +777 program.hex


rm output.hex

cat boot.hex >> output.hex
cat program.hex >> output.hex

rm *.elf
rm *.o