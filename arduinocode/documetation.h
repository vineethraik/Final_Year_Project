/*

adress of i2c io expander
A2  A1  A0  A6 A5 A4 A3 A2 A1 A0   R   W  R/W
VSS VSS VSS 0  1  0  0  0  0  0 - 40h 41h 20h
VSS VSS VDD 0  1  0  0  0  0  1 - 42h 43h 21h
VSS VDD VSS 0  1  0  0  0  1  0 - 44h 45h 22h
VSS VDD VDD 0  1  0  0  0  1  1 - 46h 47h 23h
VDD VSS VSS 0  1  0  0  1  0  0 - 48h 49h 24h
VDD VSS VDD 0  1  0  0  1  0  1 - 4Ah 4Bh 25h
VDD VDD VSS 0  1  0  0  1  1  0 - 4Ch 4Dh 26h
VDD VDD VDD 0  1  0  0  1  1  1 - 4Eh 4Fh 27h
*/


