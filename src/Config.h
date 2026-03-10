#pragma once

// LED 
#define BLINK_PERIOD_MS 500
#define BREATH_STEP_MS 3
#define LED_PIN 16
#define LED_MAX 255 // Intentional


// Signal generator
#define POWER_MIN_DBM -17
#define POWER_MAX_DBM 12
#define FREQ_MIN_MHZ 2400
#define FREQ_MAX_MHZ 2500

#define RADIO_MOSI  13
#define RADIO_MISO  12
#define RADIO_SCLK  14
#define RADIO_NSS   15
#define RADIO_DIO1   4
#define RADIO_BUSY   5
#define RADIO_RST    2


// Signal power amp
#define PIN_RXE 9   // LNA
#define PIN_TXE 10  // PA
#define SAFE_SWITCH_DELAY_MS 1


// WiFi
#define WIFI_SSID "SX1280 Signal Generator"
#define WIFI_PASSWORD ""