#ifndef STATUS_H
#define STATUS_H
#include <stdbool.h>

// STATUS
#pragma once

#define status_set_init(x)
#define status_set_ok(x)
#define status_set_error(x, y)
// ------


#define COMPONENT_MC           (1)
#define COMPONENT_SE           (2)
#define COMPONENT_CFG          (3)
#define COMPONENT_UI_BEEPER    (4)
#define COMPONENT_UI_LEDS      (5)
#define COMPONENT_ACCEL        (6)
#define COMPONENT_BARO         (7)
#define COMPONENT_FLASH        (8)
#define COMPONENT_MC_PYRO      (9)
#define COMPONENT_MC_PSU       (11)
#define COMPONENT_MOCK         (10)

#define ERROR_FLASH_CRC        (1)
#define ERROR_FLASH_WRITE      (2)
#define ERROR_CFG_READ         (3)
#define ERROR_CFG_CHK_PROFILE  (8)
#define ERROR_CFG_CHK_PYROS    (9)
#define ERROR_ACCEL_BAD_ID     (10)
#define ERROR_ACCEL_SELFTEST   (11)
#define ERROR_ACCEL_TIMEOUT    (12)
#define ERROR_ACCEL_AXIS       (13)
#define ERROR_SE_PRESSURE      (14)
#define ERROR_MC_PYRO_ARM      (15)
#define ERROR_MC_PYRO_CONT     (4)
#define ERROR_MC_PYRO_SUPPLY   (5)
#define ERROR_MC_PSU_BATTLESHORT (21)
#define ERROR_MOCK_ENABLED     (16)
#define ERROR_CAN_BAD_COMMAND  (17)
#define ERROR_CFG_CHK_ACCEL_CAL  (18)
#define ERROR_CFG_CHK_RADIO_FREQ (19)
#define ERROR_CFG_CHK_CRC        (20)


#endif
