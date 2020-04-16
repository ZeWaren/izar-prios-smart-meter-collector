#include <stdio.h>
#include <stdint.h>
#include <string.h>

const uint8_t PRIOS_DEFAULT_KEY1[8] = {0x39, 0xBC, 0x8A, 0x10, 0xE6, 0x6D, 0x83, 0xF8};
const uint8_t PRIOS_DEFAULT_KEY2[8] = {0x51, 0x72, 0x89, 0x10, 0xE6, 0x6D, 0x83, 0xF8};

/**
 * Extract a 32 bit unsigned integer encoded into a stream of data in little-endian format.
 */
uint32_t read_uint32_le(const uint8_t *data, int offset) {
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
	return *(uint32_t *)(data + offset);
#else
	return __builtin_bswap32(*(uint32_t *)(data + offset));
#endif
}

/**
 *Extract a 32 bit unsigned integer encoded into a stream of data in big-endian format.
 */
uint32_t read_uint32_be(const uint8_t *data, int offset) {
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
	return __builtin_bswap32(*(uint32_t *)(data + offset));
#else
	return *(uint32_t *)(data + offset);
#endif
}

/**
 * Prepare the base value used to decode a Prios packet from the key in a stream of bytes.
 */
uint32_t prepare_key(const uint8_t *bytes) {
    uint32_t key1 = read_uint32_be(bytes, 0);
    uint32_t key2 = read_uint32_be(bytes, 4);
    uint32_t key = key1 ^ key2;
    return key;
}

/**
 * Decode a Prios payload with a key.
 * Original author: Jacek Tomasiak
 * Reference:
 * - "Suggestion: Add support for Water Sappel/IZAR RC 868 #20": https://github.com/weetmuts/wmbusmeters/issues/20
 * - https://github.com/skazi0/wmbusmeters/blob/7e6d075d940c4d668f75de461d26e9d7258df719/src/meter_izar.cc#L166
 */
uint8_t decode_prios_payload(const uint8_t *in, const uint8_t payload_len, uint32_t key, uint8_t *out) {
    // modify seed key with header values
    key ^= read_uint32_be(in, 2); // manufacturer + address[0-1]
    key ^= read_uint32_be(in, 6); // address[2-3] + version + type
    key ^= read_uint32_be(in, 10); // ci + some more bytes from the telegram...

    for (int i = 0; i < payload_len; ++i) {
        // calculate new key (LFSR)
        // https://en.wikipedia.org/wiki/Linear-feedback_shift_register
        for (int j = 0; j < 8; ++j) {
            // calculate new bit value (xor of selected bits from previous key)
            uint8_t bit = ((key & 0x2) != 0) ^ ((key & 0x4) != 0) ^ ((key & 0x800) != 0) ^ ((key & 0x80000000) != 0);
            // shift key bits and add new one at the end
            key = (key << 1) | bit;
        }
        // decode i-th content byte with fresh/last 8-bits of key
        out[i] = in[i + 15] ^ (key & 0xFF);
        // check-byte doesn't match?
        if (out[0] != 0x4B) {
            return 0;
        }
    }

    return 1;
}

/**
 * Get the water metrics from a decoded Prios payload.
 */
void get_metrics_from_prios_payload(const uint8_t *payload, float *total_consumption, float *last_month_total_consumption) {
	*total_consumption = ((float) read_uint32_le(payload, 1)) / 1000;
	*last_month_total_consumption = ((float) read_uint32_le(payload, 5)) / 1000;
}

int main(void) {

	const uint8_t frame[26] = {0x19, 0x44, 0x30, 0x4C, 0x1B, 0x8C, 0xD7, 0x20, 0xD4, 0x01, 0xA2, 0x13, 0x12, 0x00, 0x13, 0x7C, 0xBA, 0x27, 0x3B, 0x12, 0x13, 0x75, 0x9E, 0x1F, 0xEF, 0xBF};
	uint8_t out[120];
	memset(out, 0, 120);

	uint32_t key = prepare_key(PRIOS_DEFAULT_KEY1);
	uint8_t r = decode_prios_payload(frame, 26 - 15, key, out);

	for (uint8_t i=0; i<26-15; i++) {
		printf("%x ", out[i]);
	}
	printf("\n");

	float total_consumption, last_month_total_consumption;
	get_metrics_from_prios_payload(out, &total_consumption, &last_month_total_consumption);

	printf("total: %f\n", total_consumption);
	printf("last month: %f\n", last_month_total_consumption);

	return r;
}
