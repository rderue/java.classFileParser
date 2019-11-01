#include "extract.h"

/*
 * Function for obtaining the string representation of each instruction
 * The return value will be <instruction> <number of operands>
 */
void setCurrentInstruction(uint8_t opcode){
    switch (opcode){
        case 0x02:
            currentInstruction.instrName = "iconst_m1";
            currentInstruction.numOperands = 0;
            return;
        case 0x03:
            currentInstruction.instrName = "iconst_0";
            currentInstruction.numOperands = 0;
            return;
        case 0x04:
            currentInstruction.instrName = "iconst_1";
            currentInstruction.numOperands = 0;
            return;
        case 0x05:
            currentInstruction.instrName = "iconst_2";
            currentInstruction.numOperands = 0;
            return;
        case 0x06:
            currentInstruction.instrName = "iconst_3";
            currentInstruction.numOperands = 0;
            return;
        case 0x07:
            currentInstruction.instrName = "iconst_4";
            currentInstruction.numOperands = 0;
            return;
        case 0x08:
            currentInstruction.instrName = "iconst_5";
            currentInstruction.numOperands = 0;
            return;
        case 0x2a:
            currentInstruction.instrName = "aload_0";
            currentInstruction.numOperands = 0;
            return;
        case 0xb1:
            currentInstruction.instrName = "return";
            currentInstruction.numOperands = 0;
            return;
        case 0xb2:
            currentInstruction.instrName = "getstatic";
            currentInstruction.numOperands = 2;
        case 0xb6:
            currentInstruction.instrName = "invokevirtual";
            currentInstruction.numOperands = 2;
            return;
        case 0xb7:
            currentInstruction.instrName = "invokespecial";
            currentInstruction.numOperands = 2;
            return;
    }
}