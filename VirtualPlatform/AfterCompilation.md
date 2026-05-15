make BLOCK=uart run
=== Verilating block: uart ===
verilator --cc --exe --trace-fst --trace-structs --top-module tb_top -Wno-fatal -I.. -I../Hardware -I../Hardware/IO/UART -I../Hardware/IO/SPI -I../Hardware/IO/Timer -I../Hardware/IO/GPIO -I../Hardware/IO/PRNG -I../Hardware/IO/SD -I../Hardware/IO/Ethernet -I../Hardware/APU -I../Hardware/System -I../Utility/Packages -CFLAGS "-std=c++17 -O2 -I/include" -LDFLAGS "-L/lib -lriscv -lfesvr -lpthread -ldl" common/tb/tb_top.sv blocks/uart/rtl/dut_wrapper.sv common/sim/sim_main.cpp
%Warning-WIDTHEXPAND: ../Hardware/IO/UART/uart_receiver.sv:66:34: Operator ASSIGN expects 8 bits on the Assign RHS, but Assign RHS's SEL generates 5 bits.
                                                                : ... note: In instance 'tb_top.u_dut.u_uart.receiver'
   66 |                 DBIT5: rx_data_o = rx_data[7:3];
      |                                  ^
                      ../Hardware/IO/UART/uart.sv:8:1: ... note: In file included from 'uart.sv'
                      blocks/uart/rtl/dut_wrapper.sv:2:1: ... note: In file included from 'dut_wrapper.sv'
                      ... For warning description see https://verilator.org/warn/WIDTHEXPAND?v=5.035
                      ... Use "/* verilator lint_off WIDTHEXPAND */" and lint_on around source to disable this message.
%Warning-WIDTHEXPAND: ../Hardware/IO/UART/uart_receiver.sv:67:34: Operator ASSIGN expects 8 bits on the Assign RHS, but Assign RHS's SEL generates 6 bits.
                                                                : ... note: In instance 'tb_top.u_dut.u_uart.receiver'
   67 |                 DBIT6: rx_data_o = rx_data[7:2];
      |                                  ^
                      ../Hardware/IO/UART/uart.sv:8:1: ... note: In file included from 'uart.sv'
                      blocks/uart/rtl/dut_wrapper.sv:2:1: ... note: In file included from 'dut_wrapper.sv'
%Warning-WIDTHEXPAND: ../Hardware/IO/UART/uart_receiver.sv:68:34: Operator ASSIGN expects 8 bits on the Assign RHS, but Assign RHS's SEL generates 7 bits.
                                                                : ... note: In instance 'tb_top.u_dut.u_uart.receiver'
   68 |                 DBIT7: rx_data_o = rx_data[7:1];
      |                                  ^
                      ../Hardware/IO/UART/uart.sv:8:1: ... note: In file included from 'uart.sv'
                      blocks/uart/rtl/dut_wrapper.sv:2:1: ... note: In file included from 'dut_wrapper.sv'
%Warning-WIDTHTRUNC: ../Hardware/IO/UART/uart_receiver.sv:240:48: Logical operator LOGNOT expects 1 bit on the LHS, but LHS's VARREF 'stop_bit_CRT' generates 3 bits.
                                                                : ... note: In instance 'tb_top.u_dut.u_uart.receiver'
  240 |                                 stop_bit_NXT = !stop_bit_CRT;
      |                                                ^
                     ../Hardware/IO/UART/uart.sv:8:1: ... note: In file included from 'uart.sv'
                     blocks/uart/rtl/dut_wrapper.sv:2:1: ... note: In file included from 'dut_wrapper.sv'
%Warning-WIDTHEXPAND: ../Hardware/IO/UART/uart_receiver.sv:240:46: Operator ASSIGN expects 3 bits on the Assign RHS, but Assign RHS's LOGNOT generates 1 bits.
                                                                 : ... note: In instance 'tb_top.u_dut.u_uart.receiver'
  240 |                                 stop_bit_NXT = !stop_bit_CRT;
      |                                              ^
                      ../Hardware/IO/UART/uart.sv:8:1: ... note: In file included from 'uart.sv'
                      blocks/uart/rtl/dut_wrapper.sv:2:1: ... note: In file included from 'dut_wrapper.sv'
%Warning-WIDTHTRUNC: ../Hardware/IO/UART/uart_receiver.sv:242:33: Logical operator IF expects 1 bit on the If, but If's VARREF 'stop_bit_CRT' generates 3 bits.
                                                                : ... note: In instance 'tb_top.u_dut.u_uart.receiver'
  242 |                                 if (stop_bit_CRT) begin
      |                                 ^~
                     ../Hardware/IO/UART/uart.sv:8:1: ... note: In file included from 'uart.sv'
                     blocks/uart/rtl/dut_wrapper.sv:2:1: ... note: In file included from 'dut_wrapper.sv'
%Warning-WIDTHTRUNC: ../Hardware/IO/UART/uart_transmitter.sv:219:48: Logical operator LOGNOT expects 1 bit on the LHS, but LHS's VARREF 'stop_bit_CRT' generates 3 bits.
                                                                   : ... note: In instance 'tb_top.u_dut.u_uart.transmitter'
  219 |                                 stop_bit_NXT = !stop_bit_CRT;
      |                                                ^
                     ../Hardware/IO/UART/uart.sv:7:1: ... note: In file included from 'uart.sv'
                     blocks/uart/rtl/dut_wrapper.sv:2:1: ... note: In file included from 'dut_wrapper.sv'
%Warning-WIDTHEXPAND: ../Hardware/IO/UART/uart_transmitter.sv:219:46: Operator ASSIGN expects 3 bits on the Assign RHS, but Assign RHS's LOGNOT generates 1 bits.
                                                                    : ... note: In instance 'tb_top.u_dut.u_uart.transmitter'
  219 |                                 stop_bit_NXT = !stop_bit_CRT;
      |                                              ^
                      ../Hardware/IO/UART/uart.sv:7:1: ... note: In file included from 'uart.sv'
                      blocks/uart/rtl/dut_wrapper.sv:2:1: ... note: In file included from 'dut_wrapper.sv'
%Warning-WIDTHTRUNC: ../Hardware/IO/UART/uart_transmitter.sv:221:33: Logical operator IF expects 1 bit on the If, but If's VARREF 'stop_bit_CRT' generates 3 bits.
                                                                   : ... note: In instance 'tb_top.u_dut.u_uart.transmitter'
  221 |                                 if (stop_bit_CRT) begin
      |                                 ^~
                     ../Hardware/IO/UART/uart.sv:7:1: ... note: In file included from 'uart.sv'
                     blocks/uart/rtl/dut_wrapper.sv:2:1: ... note: In file included from 'dut_wrapper.sv'
%Warning-WIDTHEXPAND: ../Hardware/IO/UART/uart_registers.sv:268:45: Operator ASSIGN expects 32 bits on the Assign RHS, but Assign RHS's VARREF 'rx_buffer_read' generates 8 bits.
                                                                  : ... note: In instance 'tb_top.u_dut.u_uart.registers'
  268 |                 UART_RX_BUFFER: read_data_o = rx_buffer_read;
      |                                             ^
                      ../Hardware/IO/UART/uart.sv:5:1: ... note: In file included from 'uart.sv'
                      blocks/uart/rtl/dut_wrapper.sv:2:1: ... note: In file included from 'dut_wrapper.sv'
%Warning-WIDTHEXPAND: ../Hardware/IO/UART/uart_registers.sv:270:41: Operator ASSIGN expects 32 bits on the Assign RHS, but Assign RHS's VARREF 'event_register' generates 5 bits.
                                                                  : ... note: In instance 'tb_top.u_dut.u_uart.registers'
  270 |                 UART_EVENT: read_data_o = event_register;
      |                                         ^
                      ../Hardware/IO/UART/uart.sv:5:1: ... note: In file included from 'uart.sv'
                      blocks/uart/rtl/dut_wrapper.sv:2:1: ... note: In file included from 'dut_wrapper.sv'
%Warning-CASEINCOMPLETE: ../Hardware/IO/UART/uart_registers.sv:265:13: Case values incompletely covered (example pattern 0x1)
  265 |             case (read_address_i)
      |             ^~~~
                         ../Hardware/IO/UART/uart.sv:5:1: ... note: In file included from 'uart.sv'
                         blocks/uart/rtl/dut_wrapper.sv:2:1: ... note: In file included from 'dut_wrapper.sv'
%Warning-CASEINCOMPLETE: ../Hardware/IO/UART/../../System/synchronous_buffer.sv:104:17: Case values incompletely covered (example pattern 0x0)
  104 |                 case ({write_i, read_i})
      |                 ^~~~
                         ../Hardware/IO/UART/uart_registers.sv:5:1: ... note: In file included from 'uart_registers.sv'
                         ../Hardware/IO/UART/uart.sv:5:1: ... note: In file included from 'uart.sv'
                         blocks/uart/rtl/dut_wrapper.sv:2:1: ... note: In file included from 'dut_wrapper.sv'
%Warning-CASEINCOMPLETE: ../Hardware/IO/UART/uart_transmitter.sv:125:13: Case values incompletely covered (example pattern 0x5)
  125 |             case (state_CRT)
      |             ^~~~
                         ../Hardware/IO/UART/uart.sv:7:1: ... note: In file included from 'uart.sv'
                         blocks/uart/rtl/dut_wrapper.sv:2:1: ... note: In file included from 'dut_wrapper.sv'
%Warning-CASEINCOMPLETE: ../Hardware/IO/UART/uart_receiver.sv:147:13: Case values incompletely covered (example pattern 0x5)
  147 |             case (state_CRT)
      |             ^~~~
                         ../Hardware/IO/UART/uart.sv:8:1: ... note: In file included from 'uart.sv'
                         blocks/uart/rtl/dut_wrapper.sv:2:1: ... note: In file included from 'dut_wrapper.sv'
%Warning-UNOPTFLAT: ../Hardware/IO/UART/uart.sv:50:70: Signal unoptimizable: Circular combinational logic: 'tb_top.u_dut.u_uart.tx_idle'
   50 |     logic [7:0] rx_data, tx_data; logic uart_rts, transmit, tx_done, tx_idle, rx_done, rx_idle, parity_error, tx_empty;
      |                                                                      ^~~~~~~
                    blocks/uart/rtl/dut_wrapper.sv:2:1: ... note: In file included from 'dut_wrapper.sv'
                    ../Hardware/IO/UART/uart.sv:50:70:      Example path: tb_top.u_dut.u_uart.tx_idle
                    ../Hardware/IO/UART/uart.sv:139:21:      Example path: ASSIGNW
                    ../Hardware/IO/UART/uart.sv:50:51:      Example path: tb_top.u_dut.u_uart.transmit
                    ../Hardware/IO/UART/uart_transmitter.sv:110:9:      Example path: ALWAYS
                    ../Hardware/IO/UART/uart.sv:50:70:      Example path: tb_top.u_dut.u_uart.tx_idle
- V e r i l a t i o n   R e p o r t: Verilator 5.035 devel rev v5.034-16-gfc302a520
- Verilator: Built from 0.086 MB sources in 13 modules, into 0.227 MB in 13 C++ files needing 0.000 MB
- Verilator: Walltime 0.080 s (elab=0.008, cvt=0.028, bld=0.000); cpu 0.043 s on 1 threads; alloced 11.656 MB
make -C obj_dir -f Vtb_top.mk -j$(nproc)
make[1]: Entering directory '/home/gabriele/Desktop/Projects/ZenithSoC/VirtualPlatform/obj_dir'
g++  -I.  -MMD -I/usr/local/share/verilator/include -I/usr/local/share/verilator/include/vltstd -DVM_COVERAGE=0 -DVM_SC=0 -DVM_TIMING=0 -DVM_TRACE=1 -DVM_TRACE_FST=1 -DVM_TRACE_VCD=0 -faligned-new -fcf-protection=none -Wno-bool-operation -Wno-shadow -Wno-sign-compare -Wno-subobject-linkage -Wno-tautological-compare -Wno-uninitialized -Wno-unused-but-set-parameter -Wno-unused-but-set-variable -Wno-unused-parameter -Wno-unused-variable    -std=c++17 -O2 -I/include   -Os  -c -o sim_main.o ../common/sim/sim_main.cpp
g++ -Os  -I.  -MMD -I/usr/local/share/verilator/include -I/usr/local/share/verilator/include/vltstd -DVM_COVERAGE=0 -DVM_SC=0 -DVM_TIMING=0 -DVM_TRACE=1 -DVM_TRACE_FST=1 -DVM_TRACE_VCD=0 -faligned-new -fcf-protection=none -Wno-bool-operation -Wno-shadow -Wno-sign-compare -Wno-subobject-linkage -Wno-tautological-compare -Wno-uninitialized -Wno-unused-but-set-parameter -Wno-unused-but-set-variable -Wno-unused-parameter -Wno-unused-variable    -std=c++17 -O2 -I/include   -c -o verilated.o /usr/local/share/verilator/include/verilated.cpp
g++ -Os  -I.  -MMD -I/usr/local/share/verilator/include -I/usr/local/share/verilator/include/vltstd -DVM_COVERAGE=0 -DVM_SC=0 -DVM_TIMING=0 -DVM_TRACE=1 -DVM_TRACE_FST=1 -DVM_TRACE_VCD=0 -faligned-new -fcf-protection=none -Wno-bool-operation -Wno-shadow -Wno-sign-compare -Wno-subobject-linkage -Wno-tautological-compare -Wno-uninitialized -Wno-unused-but-set-parameter -Wno-unused-but-set-variable -Wno-unused-parameter -Wno-unused-variable    -std=c++17 -O2 -I/include   -c -o verilated_fst_c.o /usr/local/share/verilator/include/verilated_fst_c.cpp
g++ -Os  -I.  -MMD -I/usr/local/share/verilator/include -I/usr/local/share/verilator/include/vltstd -DVM_COVERAGE=0 -DVM_SC=0 -DVM_TIMING=0 -DVM_TRACE=1 -DVM_TRACE_FST=1 -DVM_TRACE_VCD=0 -faligned-new -fcf-protection=none -Wno-bool-operation -Wno-shadow -Wno-sign-compare -Wno-subobject-linkage -Wno-tautological-compare -Wno-uninitialized -Wno-unused-but-set-parameter -Wno-unused-but-set-variable -Wno-unused-parameter -Wno-unused-variable    -std=c++17 -O2 -I/include   -c -o verilated_threads.o /usr/local/share/verilator/include/verilated_threads.cpp
python3 /usr/local/share/verilator/bin/verilator_includer -DVL_INCLUDE_OPT=include Vtb_top.cpp Vtb_top___024root__DepSet_h7f86b803__0.cpp Vtb_top___024root__DepSet_h9ad02603__0.cpp Vtb_top__Trace__0.cpp Vtb_top__ConstPool_0.cpp Vtb_top___024root__Slow.cpp Vtb_top___024root__DepSet_h7f86b803__0__Slow.cpp Vtb_top___024root__DepSet_h9ad02603__0__Slow.cpp Vtb_top___024unit__Slow.cpp Vtb_top___024unit__DepSet_hba4cee98__0__Slow.cpp Vtb_top__Syms.cpp Vtb_top__Trace__0__Slow.cpp Vtb_top__TraceDecls__0__Slow.cpp > Vtb_top__ALL.cpp
echo "" > Vtb_top__ALL.verilator_deplist.tmp
g++ -Os  -I.  -MMD -I/usr/local/share/verilator/include -I/usr/local/share/verilator/include/vltstd -DVM_COVERAGE=0 -DVM_SC=0 -DVM_TIMING=0 -DVM_TRACE=1 -DVM_TRACE_FST=1 -DVM_TRACE_VCD=0 -faligned-new -fcf-protection=none -Wno-bool-operation -Wno-shadow -Wno-sign-compare -Wno-subobject-linkage -Wno-tautological-compare -Wno-uninitialized -Wno-unused-but-set-parameter -Wno-unused-but-set-variable -Wno-unused-parameter -Wno-unused-variable    -std=c++17 -O2 -I/include   -c -o Vtb_top__ALL.o Vtb_top__ALL.cpp
In file included from ../common/sim/sim_main.cpp:16:
/include/riscv/mmu.h:69:30: warning: default member initializers for bit-fields only available with ‘-std=c++20’ or ‘-std=gnu++20’
   69 |   const bool forced_virt : 1 {false};
      |                              ^
/include/riscv/mmu.h:70:23: warning: default member initializers for bit-fields only available with ‘-std=c++20’ or ‘-std=gnu++20’
   70 |   const bool hlvx : 1 {false};
      |                       ^
/include/riscv/mmu.h:71:21: warning: default member initializers for bit-fields only available with ‘-std=c++20’ or ‘-std=gnu++20’
   71 |   const bool lr : 1 {false};
      |                     ^
/include/riscv/mmu.h:72:28: warning: default member initializers for bit-fields only available with ‘-std=c++20’ or ‘-std=gnu++20’
   72 |   const bool ss_access : 1 {false};
      |                            ^
/include/riscv/mmu.h:73:30: warning: default member initializers for bit-fields only available with ‘-std=c++20’ or ‘-std=gnu++20’
   73 |   const bool clean_inval : 1 {false};
      |                              ^
g++    sim_main.o verilated.o verilated_fst_c.o verilated_threads.o Vtb_top__ALL.a   -lz -L/lib -lriscv -lfesvr -lpthread -ldl  -pthread -lpthread -latomic   -o Vtb_top
rm Vtb_top__ALL.verilator_deplist.tmp
make[1]: Leaving directory '/home/gabriele/Desktop/Projects/ZenithSoC/VirtualPlatform/obj_dir'
=== Verilator build done ===
=== Compiling firmware for block: uart ===
riscv32-unknown-elf-gcc -march=rv32im_zicsr -mabi=ilp32 -nostdlib \
    -T common/sw/link.ld common/sw/startup.S blocks/uart/sw/firmware.c \
    -I common/sw \
    -O2 -o out/firmware.elf
riscv32-unknown-elf-objdump -d out/firmware.elf > out/firmware.dis
=== Firmware built: out/firmware.elf ===
=== Running VP: BLOCK=uart IO_BASE=0x00004000 ===
./obj_dir/Vtb_top +firmware=out/firmware.elf \
    +io_base=0x00004000 +io_size=0x2000
[VP] Starting simulation with firmware: out/firmware.elf
[VP] MMIO range: 0x4000 - 0x5fff


HERE IT STALLS