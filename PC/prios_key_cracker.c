//
// Take a bunch of PRIOS WMBus frames, and find a decryption key that allows decoding all of them.
// Unpolished code: do not use in production.
//

#include <stdio.h>
#include <stdint.h>
#include <time.h>

// Use the PRIOS functions from the ST code.
#include <PRIOS.h>
#include "config.h"

// Declare those since they're not exported by the ST code.
uint32_t read_uint32_le(uint8_t *data, int offset);
uint32_t read_uint32_be(uint8_t *data, int offset);
uint32_t preparePRIOSKey(uint8_t *bytes);

// Try to decode a payload with a key:
uint8_t try_key(uint8_t *key_bytes, uint8_t *frame, uint8_t *out) {
    uint32_t prepared_key = preparePRIOSKey(key_bytes);
    return decodePRIOSPayload(frame, 11, prepared_key, out);
}

// Check that a decoded payload is coherent with the data we expect:
uint8_t check_decoded_payload(uint8_t *decoded_frame, uint32_t *total_consumption, uint32_t *last_month_total_consumption, uint8_t *year, uint8_t *month, uint8_t *day) {
    // Check that the consumptions look correct:
    *total_consumption = read_uint32_le(decoded_frame, 1);
    *last_month_total_consumption = read_uint32_le(decoded_frame, 5);
    if (*last_month_total_consumption > *total_consumption) {
        return 0;
    }
#ifdef CONSUMPTION_RANGE_MIN
    if (*total_consumption < CONSUMPTION_RANGE_MIN) {
        return 0;
    }
    if (*last_month_total_consumption < CONSUMPTION_RANGE_MIN) {
        return 0;
    }
#endif
#ifdef CONSUMPTION_RANGE_MAX
    if (*total_consumption > CONSUMPTION_RANGE_MAX) {
        return 0;
    }
    if (*last_month_total_consumption > CONSUMPTION_RANGE_MAX) {
        return 0;
    }
#endif

    // Check that the date is correct:
    *year = ((decoded_frame[10] & 0xF0) >> 1) + ((decoded_frame[9] & 0xE0) >> 5);
    *month = decoded_frame[10] & 0xF;
    *day = decoded_frame[9] & 0x1F;
    if (*year > 99 || *month > 12 || *day > 31) {
        return 0;
    }
#ifdef TEST_YEAR
    if (*year != TEST_YEAR) {
        return 0;
    }
#endif
#ifdef TEST_MONTH
    if (*month != TEST_MONTH) {
        return 0;
    }
#endif
#ifdef TEST_DAY
    if (*day != TEST_DAY) {
        return 0;
    }
#endif

    return 1;
}

int main(void) {
	uint32_t total_consumption; uint32_t last_month_total_consumption; uint8_t year; uint8_t month; uint8_t day;
    uint8_t found_keys = 0;
    uint8_t decoded_frame[11];
    time_t prevtime = 0;

    // Loop over all the possible keys:
    for (uint64_t i=0; i<0xffffffffffffffff; i++) {
        time_t curtime = time(NULL);
        if (curtime > prevtime + 1) {
            printf("%.16llx keys tried\n", i);
            prevtime = curtime;
        }

        // Test all frames in sequence until one fails:
        uint8_t success = 1;
        for (uint8_t j=0, count=sizeof(frames) / sizeof(frames[0]); j<count; j++) {
	        // Check if the payload can be decoded:
	        if (!try_key((uint8_t *) &i, frames[j], decoded_frame)) {
	        	success = 0;
	            break;
	        }

	        // Check the decoded payload for consistency:
	        if (!check_decoded_payload(decoded_frame, &total_consumption, &last_month_total_consumption, &year, &month, &day)) {
	        	success = 0;
	            break;
	        }
        }

        if (success) {
	        printf(
	        	"Candidate key: {0x%.2x, 0x%.2x, 0x%.2x, 0x%.2x, 0x%.2x, 0x%.2x, 0x%.2x, 0x%.2x}: First frame: current: %d, H0: %d H0 date: %.2d-%.2d-%.2d\n",
	        	(uint8_t)(i & 0xFF), (uint8_t)((i >> 8) & 0xFF), (uint8_t)((i >> 16) & 0xFF), (uint8_t)((i >> 24) & 0xFF), (uint8_t)((i >> 32) & 0xFF), (uint8_t)((i >> 40) & 0xFF), (uint8_t)((i >> 48) & 0xFF), (uint8_t)((i >> 56) & 0xFF),
	        	total_consumption, last_month_total_consumption, year, month, day
	        );
	        found_keys++;
        }
    }

    return found_keys > 0;
}
