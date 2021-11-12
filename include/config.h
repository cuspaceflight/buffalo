#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>
#include <stdbool.h>

#define CONFIG_POSITION_DART (1)
#define CONFIG_POSITION_CORE (2)

#define CONFIG_ACCEL_AXIS_X  (1)
#define CONFIG_ACCEL_AXIS_NX (2)
#define CONFIG_ACCEL_AXIS_Y  (3)
#define CONFIG_ACCEL_AXIS_NY (4)
#define CONFIG_ACCEL_AXIS_Z  (5)
#define CONFIG_ACCEL_AXIS_NZ (6)

#define CONFIG_PYRO_USAGE_MASK         (0xF0)
#define CONFIG_PYRO_CURRENT_MASK       (0x0C)
#define CONFIG_PYRO_TYPE_MASK          (0x03)
#define CONFIG_PYRO_USAGE_NONE         (0x00)
#define CONFIG_PYRO_USAGE_DROGUE       (0x10)
#define CONFIG_PYRO_USAGE_MAIN         (0x20)
#define CONFIG_PYRO_USAGE_DARTSEP      (0x30)
#define CONFIG_PYRO_CURRENT_NONE       (0x00)
#define CONFIG_PYRO_CURRENT_1A         (0x04)
#define CONFIG_PYRO_CURRENT_3A         (0x08)
#define CONFIG_PYRO_TYPE_NONE          (0x00)
#define CONFIG_PYRO_TYPE_EMATCH        (0x01)
#define CONFIG_PYRO_TYPE_TALON         (0x02)
#define CONFIG_PYRO_TYPE_METRON        (0x03)

struct config {

    struct {
        /* M3FC position. 1=dart 2=core */
        uint8_t position;

        /* Accelerometer "up" axis. 1=X 2=-X 3=Y 4=-Y 5=Z 6=-Z */
        uint8_t accel_axis;

        /* Ignition detection acceleration threshold, in m/s/s */
        uint8_t ignition_accel;

        /* Burnout detection timeout, in 0.1s since launch */
        uint8_t burnout_timeout;

        /* Apogee detection timeout, in s since launch */
        uint8_t apogee_timeout;

        /* Altitude to release main parachute, in 10m above launch altitude */
        uint8_t main_altitude;

        /* Main release timeout, in s since apogee */
        uint8_t main_timeout;

        /* Landing detection timeout, in 10s since launch */
        uint8_t land_timeout;
    } __attribute__((packed)) profile;

    struct {
        /* Pyro channels configuration.
         * Top four bits usage: 0000=NONE, 0001=DROGUE, 0010=MAIN, 0011=DARTSEP
         * Next two bits current: 00=NONE, 01=1A, 10=3A
         * Bottom two bits type: 00=NONE, 01=EMATCH, 10=TALON, 11=METRON
         */
        uint8_t pyro1, pyro2, pyro3, pyro4, pyro5, pyro6, pyro7, pyro8;
    } __attribute__((packed)) pyros;

    struct {
        /* Accelerometer scale in g/LSB and 0g offset in LSB for each axis */
        float x_scale, x_offset;
        float y_scale, y_offset;
        float z_scale, z_offset;
    } __attribute__((packed)) accel_cal;

    /* Radio carrier frequency in Hz */
    uint32_t radio_freq;

    /* CRC32 of the whole config */
    uint32_t crc;
} __attribute__((packed)) __attribute__((aligned(4)));

extern struct config config;

void config_init(void);
void config_save(void);
bool config_load(void);
bool config_check(void);

/* CAN packet handling routines */
void config_handle_set_profile(uint8_t* data, uint8_t datalen);
void config_handle_set_pyros(uint8_t* data, uint8_t datalen);
void config_handle_set_accel_cal_x(uint8_t* data, uint8_t datalen);
void config_handle_set_accel_cal_y(uint8_t* data, uint8_t datalen);
void config_handle_set_accel_cal_z(uint8_t* data, uint8_t datalen);
void config_handle_set_radio_freq(uint8_t* data, uint8_t datalen);
void config_handle_set_crc(uint8_t* data, uint8_t datalen);
void config_handle_load(uint8_t* data, uint8_t datalen);
void config_handle_save(uint8_t* data, uint8_t datalen);


#endif
