//
// Created by Ryan on 10/10/2019.
//

#ifndef EXTRACTCLASS_FIELD_H
#define EXTRACTCLASS_FIELD_H

#include "extract.h"

struct Field {
    uint16_t    access_flags;                   /* Mask of flags for permissions of this field  */
    uint16_t    name_index;                     /* Index into the constant pool for the name    */
    uint16_t    descriptor_index;               /* Index into the constant pool for the desc.   */
    uint16_t    attribute_count;                /* Size of the attributes array                 */
    attribute_info attributes[];
};

#endif //EXTRACTCLASS_FIELD_H
