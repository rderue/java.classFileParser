//
// Created by Ryan on 10/11/2019.
//

#ifndef EXTRACTCLASS_ATTRIBUTEINFO_H
#define EXTRACTCLASS_ATTRIBUTEINFO_H


#include <cstdint>
#include "extract.h"

struct attributeInfo {
    uint16_t attribute_name_index;      /* Constant Pool Index containing the name of the attribute */
    uint32_t attribute_length;          /* Indicates length of subsequent information in bytes      */
    uint8_t info[];                     /* Variable length info for a specific attributeInfo type   */
};

struct constantValue_attribute{
    uint16_t attribute_name_index;      /* Index in the constant pool to the name of the attribute  */
    uint32_t attribute_length;          /* Length of subsequent information in bytes (MUST BE 2)    */
    uint16_t constantValue_index;       /* Constant pool index for a constant variable              */
};

struct code_attribute{
     uint16_t attribute_name_index;     /* Index in the constant pool to "Code"                     */
     uint32_t attribute_length;         /* Length of subsequent information in bytes                */
     uint16_t max_stack;                /* Maximum depth of the operand stack at any given time     */
     uint16_t max_locals;               /* Number of local variables in the allocated local var[]   */
     uint32_t code_length;              /* Number of bytes in the code array                        */
     uint8_t * code;                    /* The actual bytes containing the bytecode                 */
     uint16_t exception_table_length;   /* The number of entries in the exception table             */
     struct exception{
         uint16_t start_pc;
         uint16_t end_pc;
         uint16_t handler_pc;
         uint16_t catch_type;
     };
     exception * exceptionTable;        /* In order definition of exception handlers in the code [] */
     uint16_t attributes_count;         /* The number of attributes contained in the attribute []   */
     struct attributeInfo * attributes; /* Notably may be a local variable []                       */
};

struct exception_attribute{
     uint16_t attribute_name_index;     /* Index in the constant pool to "Exceptions                */
     uint32_t attribute_length;         /* Length of subsequent information in bytes                */
     uint16_t number_of_exceptions;     /* The number of entries in the exception_index_table       */
     uint16_t exception_index_table[];  /* Each element is a constant pool index for the class      */
};



#endif //EXTRACTCLASS_ATTRIBUTEINFO_H
