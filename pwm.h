// pwm.h
// A few functions for easy software pwm.

#ifndef PWM_H
#define PWM_H

#include <stdint.h>

namespace avr_cpp_lib {

	struct pwm_channel {
		uint8_t volatile * const port;
		uint8_t const channel;
		uint8_t volatile value;
	};

	class pwm_worker {
		public:
			pwm_worker(pwm_channel *, uint8_t (* const f)());
			void cycle();
			
		private:
			pwm_channel * const data;
			uint8_t latest;
			uint8_t (* const get_i)();
	};
}

#endif