#ifndef __khrplatform_h_
#define __khrplatform_h_

/*
** Copyright (c) 2008-2018 The Khronos Group Inc.
**
** Permission is hereby granted, free of charge, to any person obtaining a
** copy of this software and/or associated documentation files (the
** "Materials"), to deal in the Materials without restriction, including
** without limitation the rights to use, copy, modify, merge, publish,
** distribute, sublicense, and/or sell copies of the Materials, and to
** permit persons to whom the Materials are furnished to do so, subject to
** the following conditions:
**
** The above copyright notice and this permission notice shall be included
** in all copies or substantial portions of the Materials.
*/

#include <stdint.h>
#include <stddef.h>

/* Khronos basic types */
typedef int32_t                 khronos_int32_t;
typedef uint32_t                khronos_uint32_t;
typedef int64_t                 khronos_int64_t;
typedef uint64_t                khronos_uint64_t;
typedef signed   char           khronos_int8_t;
typedef unsigned char           khronos_uint8_t;
typedef signed   short int      khronos_int16_t;
typedef unsigned short int      khronos_uint16_t;
typedef intptr_t                khronos_intptr_t;
typedef uintptr_t               khronos_uintptr_t;
typedef ptrdiff_t               khronos_ssize_t;
typedef size_t                  khronos_usize_t;
typedef float                   khronos_float_t;
typedef khronos_uint64_t        khronos_utime_nanoseconds_t;
typedef khronos_int64_t         khronos_stime_nanoseconds_t;

#define KHRONOS_SUPPORT_INT64   1
#define KHRONOS_SUPPORT_FLOAT   1

#endif /* __khrplatform_h_ */
