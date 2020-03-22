
#ifndef __TYPES_H__
#define __TYPES_H__

#include <stdint.h>

/*!< Signed integer types  */
//typedef   signed char     int8_t;
//typedef   signed short    int16_t;
//typedef   signed long     int32_t;


/*!< Unsigned integer types  */
//typedef unsigned char     uint8_t;
//typedef unsigned short    uint16_t;
//typedef unsigned long     uint32_t;

#define ARRAY_SIZE( ARRAY ) (sizeof (ARRAY) / sizeof (ARRAY[0]))

typedef uint8_t* string;

//enum {false = 0, true = !false} bool;

//typedef enum {false = 0, true = !false} bool;;

#endif //__TYPES_H__
