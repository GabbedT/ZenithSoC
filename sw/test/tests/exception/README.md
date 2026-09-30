# Exception coverage

This self-checking test covers every software-reachable synchronous exception
that ApogeoRV can currently generate: illegal instruction (2), breakpoint (3),
load/store address-misaligned (4/6), load/store access fault (5/7), and
environment calls from U/M mode (8/11). It checks `mcause`,
`mepc`, `mstatus.MPP`, and trap-time interrupt masking before resuming.

Cause 0 has decoder/front-end logic, but is not software-reachable with the C
extension's 16-bit instruction alignment: branch/JAL targets are multiples of
two and JALR clears bit zero. Cause 1 (instruction access fault) is named in
`apogeo_exception_vectors.svh`
but has no generating path in the instruction front end. Causes 9/10 and
12--15 require S/H modes or paging, which this M/U, no-MMU core does not
implement. Those causes cannot be stimulated honestly by software on the
current hardware.
