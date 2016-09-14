/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#pragma once

/*
   Binary constant generator macro by Tom Torfs
   All macro's evaluate to compile-time constants
*/

/* Helper macros */
#define HEX__(n) 0x##n##LU
#define B8__(x)                                               \
  ((x & 0x0000000FLU) ? 1 : 0) + ((x & 0x000000F0LU) ? 2 : 0) \
                                                              \
    + ((x & 0x00000F00LU) ? 4 : 0)                            \
                                                              \
    + ((x & 0x0000F000LU) ? 8 : 0)                            \
                                                              \
    + ((x & 0x000F0000LU) ? 16 : 0)                           \
                                                              \
    + ((x & 0x00F00000LU) ? 32 : 0)                           \
                                                              \
    + ((x & 0x0F000000LU) ? 64 : 0)                           \
                                                              \
    + ((x & 0xF0000000LU) ? 128 : 0)

/* For upto 8-bit binary constants */
#define B8(d) ((unsigned char)B8__(HEX__(d)))

/* For upto 16-bit binary constants, MSB first */
#define B16(dmsb, dlsb) (((unsigned short)B8(dmsb) << 8) + B8(dlsb))

/* For upto 32-bit binary constants, MSB first */
#define B32(dmsb, db2, db3, dlsb)                                   \
  (((unsigned long)B8(dmsb) << 24) + ((unsigned long)B8(db2) << 16) \
                                                                    \
    + ((unsigned long)B8(db3) << 8)                                 \
                                                                    \
    + B8(dlsb))
