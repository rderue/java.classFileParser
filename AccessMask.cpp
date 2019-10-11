//
// Created by Ryan on 10/10/2019.
//

#include "AccessMask.h"
#include "extract.h"

void printAccessTypes(uint16_t mask){
    std::cout << "-------------------------------------\nAccess Flags:\n\t";
    if ((mask & ACC_PUBLIC) == ACC_PUBLIC){
        std::cout << "Public ";
    }

    if ((mask & ACC_PRIVATE) == ACC_PRIVATE){
        std::cout << "Private ";
    }

    if ((mask & ACC_PROTECTED) == ACC_PROTECTED){
        std::cout << "Protected ";
    }

    if ((mask & ACC_STATIC) == ACC_STATIC){
        std::cout << "Static ";
    }

    if ((mask & ACC_FINAL) == ACC_FINAL){
        std::cout << "Final ";
    }

    if ((mask & ACC_VOLATILE) == ACC_VOLATILE){
        std::cout << "Volatile ";
    }

    if ((mask & ACC_TRANSIENT) == ACC_TRANSIENT){
        std::cout << "Transient ";
    }

    if ((mask & ACC_SUPER) == ACC_SUPER){
        std::cout << "Super ";
    }

    if ((mask & ACC_INTERFACE) == ACC_INTERFACE){
        std::cout << "Interface ";
    }

    if ((mask & ACC_ABSTRACT) == ACC_ABSTRACT){
        std::cout << "Abstract ";
    }

    if ((mask & ACC_SYNTHETIC) == ACC_SYNTHETIC){
        std::cout << "Synthetic";
    }

    if ((mask & ACC_ANNOTATION) == ACC_ANNOTATION){
        std::cout << "Annotation";
    }

    if ((mask & ACC_ENUM) == ACC_ENUM){
        std::cout << "Enum";
    }
    std::cout << std::endl;
}