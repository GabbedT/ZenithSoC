#-------------------------------------------------------------------------------
# Floorplan constraints for 100 MHz timing closure.
#
# Background: all 500 failing implementation paths are route-dominated
# (44-78% route delay). The placer scatters the CPU complex across the entire
# die (X[0..89] Y[32..108], ~6800 slices) while every CPU BRAM/DSP sits in the
# left-edge columns (X0-X2, rows 9-28): predictor/BTB/cache accesses cross the
# die and nets between adjacent SLICEs measure 0.65-1.3 ns. Logic depth alone
# (2.3-5.6 ns) is within budget everywhere, so no RTL change is needed.
#
# The fix is a coarse floorplan: the CPU complex is confined to a contiguous
# left-side region hugging its BRAM/DSP columns, the MIG (fabric-based, no MCB
# hard block) to the right, and the APU to the top. Regions are oversized
# (~2-3x the cell count) so placement quality is preserved. Cross-module nets
# (AXI to MIG/peripherals, global clocks) are unaffected: CONTAIN_ROUTING only
# constrains nets whose endpoints are both inside the region.
#
# Sizes verified against the routed design: CPU 6811 slices + RAMB X0-X2 rows
# 9-25 + DSP X1 rows 23-28; MIG 2877 slices X[28..89] Y[31..128]; APU 2439
# slices X[4..58] Y[103..169] + RAMB18 X0 row 66 + DSP X0 rows 50-67.
# GND/VCC, IOB (fixed pad sites) and BUFG cells are kept out of the pblocks.
#
# NOTE: resize_pblock ranges are corner-to-corner rectangles written with a
# colon (SLICE_X0Y9:SLICE_X55Y110); a space-separated list would add many
# single-site regions instead.
#-------------------------------------------------------------------------------

# --- CPU complex (core + caches + trace unit) ---
create_pblock pblock_cpu
add_cells_to_pblock [get_pblocks pblock_cpu] [get_cells ApogeoRV]
remove_cells_from_pblock [get_pblocks pblock_cpu] \
    [get_cells -hier -filter {PARENT =~ "ApogeoRV*" && (PRIMITIVE_TYPE =~ OTHERS.others.* || PRIMITIVE_TYPE =~ IOB.* || PRIMITIVE_TYPE =~ BUFG.*)}]
resize_pblock [get_pblocks pblock_cpu] -add {SLICE_X0Y9:SLICE_X55Y110}
set_property CONTAIN_ROUTING true [get_pblocks pblock_cpu]

# --- DDR memory controller (MIG, fabric implementation) ---
create_pblock pblock_mig
add_cells_to_pblock [get_pblocks pblock_mig] [get_cells ddr_controller]
remove_cells_from_pblock [get_pblocks pblock_mig] \
    [get_cells -hier -filter {PARENT =~ "ddr_controller*" && (PRIMITIVE_TYPE =~ OTHERS.others.* || PRIMITIVE_TYPE =~ IOB.* || PRIMITIVE_TYPE =~ BUFG.*)}]
resize_pblock [get_pblocks pblock_mig] -add {SLICE_X56Y20:SLICE_X89Y128}
set_property CONTAIN_ROUTING true [get_pblocks pblock_mig]

# --- Audio processing unit ---
# No CONTAIN_ROUTING here: the wave-mixer adder trees are the densest logic
# in the design and the containment constraint left 107 pins unrouted.
create_pblock pblock_apu
add_cells_to_pblock [get_pblocks pblock_apu] [get_cells audio_processing_unit]
remove_cells_from_pblock [get_pblocks pblock_apu] \
    [get_cells -hier -filter {PARENT =~ "audio_processing_unit*" && (PRIMITIVE_TYPE =~ OTHERS.others.* || PRIMITIVE_TYPE =~ IOB.* || PRIMITIVE_TYPE =~ BUFG.*)}]
resize_pblock [get_pblocks pblock_apu] -add {SLICE_X0Y50:SLICE_X59Y199}
