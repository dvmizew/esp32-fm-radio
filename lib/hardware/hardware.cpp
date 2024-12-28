#include "hardware.h"

i2s_config_t amp_i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX), // enable TX mode
    .sample_rate = 44100,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
    .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
    .communication_format = (i2s_comm_format_t)(I2S_COMM_FORMAT_I2S_MSB | I2S_COMM_FORMAT_I2S_LSB),
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1, // ???
    .dma_buf_count = 8,
    .dma_buf_len = 64,
    .use_apll = false,
    .tx_desc_auto_clear = true
    // .fixed_mclk = 0
};

i2s_pin_config_t amp_config = {
    // .mck_io_num = I2S_PIN_NO_CHANGE,
    .bck_io_num = AMP_BLCK,
    .ws_io_num = AMP_LRC,
    .data_out_num = AMP_DIN,
    .data_in_num = I2S_PIN_NO_CHANGE
};

void blinkInternalLED() {
    pinMode(INTERNAL_LED, OUTPUT);
    while (true) {
        digitalWrite(INTERNAL_LED, HIGH);
        delay(1000);
        digitalWrite(INTERNAL_LED, LOW);
        delay(1000);
    }
}

void printI2CDevices() {
    Wire.begin();
    byte error, address;
    int nDevices = 0;

    Serial.println("Scanning...");

    for (address = 1; address < 127; address++) {
        Wire.beginTransmission(address);
        error = Wire.endTransmission();

        if (error == 0) {
            Serial.print("I2C device found at address 0x");
            if (address < 16) {
                Serial.print("0");
            }
            Serial.println(address, HEX);
            nDevices++;
        } else if (error == 4) {
            Serial.print("Unknown error at address 0x");
            if (address < 16) {
                Serial.print("0");
            }
            Serial.println(address, HEX);
        }
    }

    if (nDevices == 0)
        Serial.println("No I2C devices found\n");
    else
        Serial.println("Done\n");
}