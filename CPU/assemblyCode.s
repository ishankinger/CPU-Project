ADDI s2, zero, 172
ADDI s3, zero, 185

AND s4, s2, s3
SW s4, 0(zero)

OR s5, s2, s3
SW s5, 4(zero)

XOR s6, s2, s3
SW s6, 8(zero)

SLLI s7, s2, 3
SW s7, 12(zero)

SLL s8, s2, s6
SW s8, 16(zero)