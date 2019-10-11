//
// Created by Ryan on 10/10/2019.
//

#ifndef EXTRACTCLASS_ACCESSMASK_H
#define EXTRACTCLASS_ACCESSMASK_H

#include "extract.h"

enum access_mask{
    ACC_PUBLIC              = 0x0001,       /* Declared Public. May be accessed outside its package         */
    ACC_PRIVATE             = 0X0002,       /* Declared, Private. Only accessible within this class         */
    ACC_PROTECTED           = 0X0004,       /* Declared Protected. Accessible from subclasses               */
    ACC_STATIC              = 0X0008,       /* Declared static.                                             */
    ACC_FINAL               = 0X0010,       /* Declared final; No subclass is allowed                       */
    ACC_SUPER               = 0X0020,       /* Function "invokespecial" must treat this as a Super class    */
    ACC_VOLATILE            = 0X0040,       /* Declared Volatile. cannot be cached                          */
    ACC_TRANSIENT           = 0X0080,       /* Declared Transient. No Read/Write by persistent obj manager  */
    ACC_INTERFACE           = 0X0200,       /* Is an interface and not a class                              */
    ACC_ABSTRACT            = 0X0400,       /* Declared Abstract so it cannot be instantiated               */
    ACC_SYNTHETIC           = 0X1000,       /* Declared synthetic so it is not present in source code       */
    ACC_ANNOTATION          = 0X2000,       /* Is an annotation type                                        */
    ACC_ENUM                = 0X4000        /* Is an enum type                                              */
};

void printAccessTypes(uint16_t mask);

#endif //EXTRACTCLASS_ACCESSMASK_H
