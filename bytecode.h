//
// Created by Ryan on 11/1/2019.
//

#ifndef EXTRACTCLASS_BYTECODE_H
#define EXTRACTCLASS_BYTECODE_H

struct instruction{
    std::string instrName;
    short numOperands;
};

 //= (struct instruction *) malloc(sizeof(struct instruction));

void setCurrentInstruction(uint8_t opcode);

#endif //EXTRACTCLASS_BYTECODE_H
