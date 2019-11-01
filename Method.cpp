//
// Created by Ryan on 10/11/2019.
//

#include "extract.h"
extern struct instruction currentInstruction;

void printMethodAccessMask(uint16_t mask) {
    std::cout << "-------------------------------------\nAccess Flags:\n\t";
    if ((mask & MACC_PUBLIC) == MACC_PUBLIC) {
        std::cout << "Public ";
    }

    if ((mask & MACC_PRIVATE) == MACC_PRIVATE) {
        std::cout << "Private ";
    }

    if ((mask & MACC_PROTECTED) == MACC_PROTECTED) {
        std::cout << "Protected ";
    }

    if ((mask & MACC_STATIC) == MACC_STATIC) {
        std::cout << "Static ";
    }

    if ((mask & MACC_FINAL) == MACC_FINAL) {
        std::cout << "Final ";
    }

    if ((mask & MACC_SYNCHRONIZED) == MACC_SYNCHRONIZED) {
        std::cout << "Synchronized ";
    }

    if ((mask & MACC_BRIDGE) == MACC_BRIDGE) {
        std::cout << "Bridge ";
    }

    if ((mask & MACC_VARARGS) == MACC_VARARGS) {
        std::cout << "VArgs ";
    }

    if ((mask & MACC_NATIVE) == MACC_NATIVE) {
        std::cout << "Native ";
    }

    if ((mask & MACC_ABSTRACT) == MACC_ABSTRACT) {
        std::cout << "Abstract ";
    }

    if ((mask & MACC_STRICT) == MACC_STRICT) {
        std::cout << "Strict ";
    }

    if ((mask & MACC_SYNTHETIC) == MACC_SYNTHETIC) {
        std::cout << "Synthetic";
    }

    std::cout << std::endl;
}

void printMethodBytecode(struct Method m){
    for (auto &codeAttr : *m.attributes){
        for (int i = 0; i < ((code_attribute *) codeAttr)->code_length; i++) {
            setCurrentInstruction((short) *(((code_attribute *) codeAttr)->code + i));
            if (currentInstruction.instrName != "") {
                std::cout << currentInstruction.instrName << std::endl;
            }
        }
    }
}