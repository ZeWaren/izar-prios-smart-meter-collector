/**
  ******************************************************************************
  * @file           : PRIOS.c
  * @brief          : Logic to decode an IZAR/PRIOS payload
  * @author         : Erwan Martin <public@fzwte.net>
  ******************************************************************************
  */
#include <math.h>
#include <stdio.h>

#include "PRIOS.h"

const char * const unit_displays[] = {
    [UNKNOWN_UNIT] = "banana",
    [VOLUME_CUBIC_METER] = "m3",
};
    
/* The default encryption keys of IZAR devices */
uint8_t PRIOS_DEFAULT_KEY1[8] = {0x39, 0xBC, 0x8A, 0x10, 0xE6, 0x6D, 0x83, 0xF8};
uint8_t PRIOS_DEFAULT_KEY2[8] = {0x51, 0x72, 0x89, 0x10, 0xE6, 0x6D, 0x83, 0xF8};

/**
  * @brief  Extract a 32 bit unsigned integer encoded into a stream of data in 
  *         little-endian format.
  * @param  uint8_t *data Location of where to extract the int from.
  * @param  int offset Offset from the start of the location.
  * @retval uint32_t
  */
uint32_t read_uint32_le(const uint8_t * const data, int offset) {
    uint32_t result = *(data + offset + 3) << 24;
    result |= *(data + offset + 2) << 16;
    result |= *(data + offset + 1) << 8;
    result |= *(data + offset);
    return result;
}

/**
  * @brief  Extract a 32 bit unsigned integer encoded into a stream of data in 
  *         big-endian format.
  * @param  uint8_t *data Location of where to extract the int from.
  * @param  int offset Offset from the start of the location.
  * @retval uint32_t
  */
uint32_t read_uint32_be(const uint8_t * const data, int offset) {
    uint32_t result = *(data + offset) << 24;
    result |= *(data + offset + 1) << 16;
    result |= *(data + offset + 2) << 8;
    result |= *(data + offset + 3);
    return result;
}

/**
  * @brief  Prepare the base value used to decode a Prios packet from the key 
  *         in a stream of bytes.
  * @param  uint8_t *bytes Location of where to extract the key from.
  * @retval uint32_t
  */
uint32_t preparePRIOSKey(const uint8_t * const bytes) {
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
  * 
  * @brief  Decode a Prios payload with a key.
  * @param  uint8_t *in The location of the WMBus frame
  * @param  uint8_t payload_len The length of the payload to decode
  * @param  uint32_t key The key to use to decode
  * @param  uint8_t *out Buffer to store the decoded data
  * @retval uint8_t
  */
uint8_t decodePRIOSPayload(const uint8_t * const in, const uint8_t payload_len, uint32_t key, uint8_t *out) {
    // modify seed key with header values
    key ^= read_uint32_be(in, 2); // manufacturer + address[0-1]
    key ^= read_uint32_be(in, 6); // address[2-3] + version + type
    key ^= read_uint32_be(in, 12); // ci + some more bytes from the telegram...

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
        out[i] = in[i + 17] ^ (key & 0xFF);
        // check-byte doesn't match?
        if (out[0] != 0x4B) {
            return 0;
        }
    }

    return 1;
}

/**
 * @brief Extract the data from a PRIOS frame.
 * @param uint8_t *header_data The start of the wmbus frame application data.
 * @param uint8_t *decoded_data The data that was decrypted from the frame.
 * @param izar_reading *reading Where to store the extracted data.
 */
void parsePRIOSFrame(const uint8_t * const header_data, const uint8_t * const decoded_data, izar_reading * const reading) {
    // Extract the header values:
    reading->radio_interval = 1 << ((header_data[0] & 0x0F) + 2);
    reading->random_generator = (header_data[0] >> 4) & 0x3;

    // Extract the battery remaining life:
    reading->remaining_battery_life = (header_data[1] & 0x1F) / 2.0;

    // Read the alarms:
    reading->alarms.general_alarm = header_data[0] >> 7;
    reading->alarms.leakage_currently = header_data[1] >> 7;
    reading->alarms.leakage_previously = header_data[1] >> 6 & 0x1;
    reading->alarms.meter_blocked = header_data[1] >> 5 & 0x1;
    reading->alarms.back_flow = header_data[2] >> 7;
    reading->alarms.underflow = header_data[2] >> 6 & 0x1;
    reading->alarms.overflow = header_data[2] >> 5 & 0x1;
    reading->alarms.submarine = header_data[2] >> 4 & 0x1;
    reading->alarms.sensor_fraud_currently = header_data[2] >> 3 & 0x1;
    reading->alarms.sensor_fraud_previously = header_data[2] >> 2 & 0x1;
    reading->alarms.mechanical_fraud_currently = header_data[2] >> 1 & 0x1;
    reading->alarms.mechanical_fraud_previously = header_data[2] & 0x1;

    // Read the readings:
    reading->current_reading = read_uint32_le(decoded_data, 1);
    reading->h0_reading = read_uint32_le(decoded_data, 5);

    // Apply the multiplier to the values:
    int8_t multiplier_exponent = (header_data[3] & 0x07) - 6;
    if (multiplier_exponent > 0) {
        reading->current_reading *= pow(10, multiplier_exponent);
        reading->h0_reading *= pow(10, multiplier_exponent);
    } else if (multiplier_exponent < 0) {
        reading->current_reading /= pow(10, -multiplier_exponent);
        reading->h0_reading /= pow(10, -multiplier_exponent);
    }

    // Extract the measurement unit:
    uint8_t unit_type = header_data[3] >> 3;
    if (unit_type == 0x02) {
        reading->unit_type = VOLUME_CUBIC_METER;
    } else {
        reading->unit_type = UNKNOWN_UNIT;
    }

    // Extract the date:
    reading->h0_year = ((decoded_data[10] & 0xF0) >> 1) + ((decoded_data[9] & 0xE0) >> 5);
    if (reading->h0_year > 80) {
        reading->h0_year += 1900;
    } else {
        reading->h0_year += 2000;
    }
    reading->h0_month = decoded_data[10] & 0xF;
    reading->h0_day = decoded_data[9] & 0x1F;
}

/**
 * @brief Print an entire IZAR reading to the standard output device, as CSV data.
 * @param uint32_t A_Id The identifier of the device the reading was from.
 * @param izar_reading *reading The reading to print
 */
void printIZARReadingAsCSV(const uint32_t A_Id, const izar_reading * const reading) {
    printf(
        "%.6x,%f,%f,%s,%.2d,%.2d,%.2d,%.1f,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\r\n",
        A_Id,
        reading->current_reading,
        reading->h0_reading,
        unit_displays[reading->unit_type],
        reading->h0_year,
        reading->h0_month,
        reading->h0_day,
        reading->remaining_battery_life,
        reading->radio_interval,
        reading->random_generator,
        reading->alarms.general_alarm,
        reading->alarms.leakage_currently,
        reading->alarms.leakage_previously,
        reading->alarms.meter_blocked,
        reading->alarms.back_flow,
        reading->alarms.underflow,
        reading->alarms.overflow,
        reading->alarms.submarine,
        reading->alarms.sensor_fraud_currently,
        reading->alarms.sensor_fraud_previously,
        reading->alarms.mechanical_fraud_currently,
        reading->alarms.mechanical_fraud_previously
    );
}

/**
  * @brief Get the water metrics from a WMBus Prios frame.
  * @param uint8_t *payload The location of the WMBus frame
  * @param izar_reading *reading Where to store the extracted data
  */
uint8_t getMetricsFromPRIOSWMBusFrame(const uint8_t * const frame, izar_reading * const reading) {
    /* Decode the payload */
    uint32_t key = preparePRIOSKey(PRIOS_DEFAULT_KEY1);
    uint8_t decodedPayload[32];
    uint8_t r = decodePRIOSPayload(frame, 26 - 15, key, decodedPayload);
    if (!r) {
          return 0;
    }
    
    /* Extract the data from the frame and the decoded data: */
    parsePRIOSFrame(frame + 13, decodedPayload, reading);

    /* Return: */
    return 1;
}
