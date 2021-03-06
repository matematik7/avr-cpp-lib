/* File: random32.cpp
 * Contains a 32-bit random integer generator.
 */
/* Copyright (c) 2012-2013 Domen Ipavec (domen.ipavec@z-v.si)
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation files
 * (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
 
#include "random32.h"

#include <avr/eeprom.h> 

uint32_t m32_w = 0xDEADBEEFul;    /* must not be zero */
uint32_t m32_z = 0xBEEFDEADul;    /* must not be zero */

/*
  Read random seed from eeprom and write a new random one.
*/
void init_random32(uint32_t seed)
{
    static uint32_t EEMEM sstate;

    m32_w = eeprom_read_dword(&sstate);

    // Check if it's unwritten EEPROM (first time). Use something funny
    // in that case.
    if (m32_w == 0xfffffful) {
        m32_w = seed;
    }
    
    m32_z = (~m32_w) + 0x11111111ul;
    if (m32_z == 0) {
        m32_z = 0xBEEFDEADul;
    }

    uint32_t rand = 0;
    while (rand == 0) {
        rand = get_random32();
    }
    
    eeprom_write_dword(&sstate, rand);
}

uint32_t get_random32()
{
    m32_z = 36969 * (m32_z & 65535) + (m32_z >> 16);
    m32_w = 18000 * (m32_w & 65535) + (m32_w >> 16);
    return (m32_z << 16) + m32_w;  /* 32-bit result */
}

uint32_t get_random32(uint32_t max) {
	return get_random32() / (RANDOM32_MAX / max + 1);
}

uint32_t get_random_gauss(uint32_t max) {
	max /= 6;
	uint32_t result = 0;
	for (uint8_t i = 0; i < 6; i++) {
		result += get_random32(max);
	}
	return result;
}
