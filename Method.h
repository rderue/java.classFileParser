//
// Created by Ryan on 10/11/2019.
//

#ifndef EXTRACTCLASS_METHOD_H
#define EXTRACTCLASS_METHOD_H

#include "extract.h"
#include <cstdint>

enum method_access_mask{
    MACC_PUBLIC              = 0x0001,       /* Declared Public. May be accessed outside its package         */
    MACC_PRIVATE             = 0X0002,       /* Declared, Private. Only accessible within this class         */
    MACC_PROTECTED           = 0X0004,       /* Declared Protected. Accessible from subclasses               */
    MACC_STATIC              = 0X0008,       /* Declared static.                                             */
    MACC_FINAL               = 0X0010,       /* Declared final; Must not be overridden                       */
    MACC_SYNCHRONIZED        = 0X0020,       /* Declared synchronized Invocation is wrapped by a monitor use */
    MACC_BRIDGE              = 0X0040,       /* Bridge method generated by the compiler                      */
    MACC_VARARGS             = 0X0080,       /* Declared with a variable number of arguments                 */
    MACC_NATIVE              = 0X0100,       /* Implemented in a language other than Java                    */
    MACC_ABSTRACT            = 0X0400,       /* Declared Abstract no implementation is provided              */
    MACC_STRICT              = 0X0800,       /* Declared strictfp, floating point mode is FP-strict          */
    MACC_SYNTHETIC           = 0X1000        /* Declared synthetic so it is not present in source code       */
};

struct Method {
    uint16_t access_flags;
    uint16_t name_index;
    uint16_t descriptor_index;
    uint16_t attributes_count;
    std::vector<void *> * attributes;
};

void printMethodAccessMask(uint16_t mask);

void printMethodBytecode(struct Method);

#endif //EXTRACTCLASS_METHOD_H