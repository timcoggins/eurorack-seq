
#ifndef GRIDS_HARDWARE_CONFIG_H_
#define GRIDS_HARDWARE_CONFIG_H_

#pragma once

#include <Arduino.h>

namespace config {

    // ======================================================================
    // ENGINE CONFIG
    // ======================================================================

    #define MAX_TRACKS 1
    #define MAX_SEQUENCES 8
    #define MAX_STEPS 64
    #define MAX_NOTE_VALUE 16
    #define MAX_MOD_VALUE 8

    #define RANDOM_MIN_NOTE 0
    #define RANDOM_MAX_NOTE MAX_NOTE_VALUE
    #define RANDOM_MIN_NOTE_LENGTH 1
    #define RANDOM_MAX_NOTE_LENGTH 1

    #define RANDOM_GATE_PROBABILITY 30

    #define DEFAULT_START_POS 17
    #define DEFAULT_END_POS 32
    #define DEFAULT_LOOP_ENABLE true

    #define DEFAULT_CURSOR_POS 0
    #define DEFAULT_CURSOR_LENGTH 4
    #define DEFAULT_TRACK 0

    #define SMALL_TRACK_SIZE 3

    // ======================================================================
    // UI CONFIG
    // ======================================================================

    #define SHOW_GRID_EVERY 4
    #define MAX_UI_PAGES 2

    // ======================================================================
    // HARDWARE CONFIG
    // ======================================================================

    // I2C
    #define I2C_SDA         39
    #define I2C_SCL         40

    // DAC
    #define DAC_ADDR        0x62
    #define DAC_VALUES      4096                   // For 12 bit DAC

    // PINS
    #define GATE_PIN        1
    #define CLOCK_PIN       3

    #define TEMPO_PIN       A1

    #define RANDOM_SEED_PIN 0

    // ENCODER
    #define ENC_PIN1        21
    #define ENC_PIN2        20
    #define ENC_BUTTON_PIN  47

    #define ROTARYSTEPS     1
    #define ROTARYMIN       0
    #define ROTARYMAX       16

    // BUTTONS
    #define B1_PIN          48
    #define B2_PIN          45
    #define B3_PIN          35
    #define B4_PIN          36
    #define B5_PIN          37
    #define B6_PIN          38
    #define B7_PIN          41
    #define B8_PIN          42
    #define B9_PIN          11
    #define B10_PIN         19

    // Matrix
    // uint8_t rgbPins[]  = {4, 12, 13, 14, 15, 9};
    // uint8_t addrPins[] = {16, 17, 18, 10};
    // uint8_t clockPin   = 8;                         // Must be on same port as rgbPins
    // uint8_t latchPin   = 5;
    // uint8_t oePin      = 6;
}

#endif  // GRIDS_HARDWARE_CONFIG_H_