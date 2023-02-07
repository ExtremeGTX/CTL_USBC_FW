target extended-remote | \
       openocd -f openocd.cfg -c "gdb_port pipe; log_output openocd.log"

monitor cortex_m vector_catch hard_err reset


# See documentation below.
define load-reset-halt
  load

  # Halt then continue because just "monitor reset" lets the target keep
  # running in the background, while gdb keeps its prompt up and so
  # expects it to be halted.
  monitor reset halt
end

document load-reset-halt
Convenience function to load the binary.

We need to do a full reset of the target when we (beyond what just a
"load" does) because then any connected USB serial will re-establish
its connection.
end


# See documentation below.
#
# Copied from:
#     https://mcuoneclipse.com/2015/07/05/debugging-arm-cortex-m-hard-faults-with-gdb-custom-command/
define armex
  printf "EXEC_RETURN (LR):\n",
  info registers $lr
    if ($lr & (0x4 == 0x4))
      printf "Uses MSP 0x%x return.\n", $msp
      set $armex_base = $msp
    else
      printf "Uses PSP 0x%x return.\n", $psp
      set $armex_base = $psp
    end

    printf "xPSR            0x%x\n", *(uint32_t *)($armex_base+28)
    printf "ReturnAddress   0x%x\n", *(uint32_t *)($armex_base+24)
    printf "LR (R14)        0x%x\n", *(uint32_t *)($armex_base+20)
    printf "R12             0x%x\n", *(uint32_t *)($armex_base+16)
    printf "R3              0x%x\n", *(uint32_t *)($armex_base+12)
    printf "R2              0x%x\n", *(uint32_t *)($armex_base+8)
    printf "R1              0x%x\n", *(uint32_t *)($armex_base+4)
    printf "R0              0x%x\n", *(uint32_t *)($armex_base)
    printf "Return instruction:\n"
    x/i *(uint32_t *)($armex_base+24)
    printf "LR instruction:\n"
    x/i *(uint32_t *)($armex_base+20)
end

document armex
ARMv7 Exception entry behavior.
xPSR, ReturnAddress, LR (R14), R12, R3, R2, R1, and R0
end
