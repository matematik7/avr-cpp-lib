/* File: cc1101.cpp
 * Interface to a transceiver cc1101.
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

#include "cc1101.h"

#include <util/delay.h>

using namespace avr_cpp_lib;

CC1101::CC1101(transceive_t t, OutputPin csn, InputPin so)
: transceive(t), CSn(csn), SO(so) {
	CSn.set();
}

void CC1101::reset() {
	CSn.clear();
	CSn.set();
	_delay_us(40);
	CSn.clear();
	while (SO.isSet());
	transceive(SRES);
	while (SO.isSet());
	CSn.set();
}

void CC1101::command(const uint8_t address) {
	CSn.clear();
	transceive(address);	
	CSn.set();
}

void CC1101::write(const uint8_t address, const uint8_t data) {
	CSn.clear();
	transceive(address);
	transceive(data);
	CSn.set();
}

void CC1101::writeBurst(const uint8_t address, const uint8_t * data, uint8_t n) {
	CSn.clear();
	transceive(address | BURST);
	for (; n > 0; n--) {
		transceive(*data);
		data++;
	}
	CSn.set();
}

uint8_t CC1101::read(const uint8_t address) {
	CSn.clear();
	transceive(address | READ);
	uint8_t tmp = transceive(0);
	CSn.set();
	return tmp;
}

void CC1101::readBurst(const uint8_t address, uint8_t * data, uint8_t n) {
	CSn.clear();
	transceive(address | READ | BURST);
	for (; n > 0; n--) {
		*data = transceive(0);
		data++;
	}
	CSn.set();
}
