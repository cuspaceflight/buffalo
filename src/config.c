
#include <stdlib.h>

#include "config.h"
// #include "m3can.h"
#include "status.h"
// #include "m3flash.h"

// #define CONFIG_FLASH (0x080e0000)

struct config config;

static bool config_check_profile(void);
static bool config_check_pyros(void);
static bool config_check_pyro(uint8_t pyro);
static bool config_check_accel_cal(void);
static bool config_check_radio_freq(void);
static bool config_check_crc(void);

// static THD_WORKING_AREA(config_reporter_thd_wa, 256);
// static THD_FUNCTION(config_reporter_thd, arg) {
//     (void)arg;

//     while(true) {
//         /* Send current config over CAN every 5s */
//         m3can_send_u8(CAN_MSG_ID_CFG_PROFILE,
//                       config.profile.position,
//                       config.profile.accel_axis,
//                       config.profile.ignition_accel,
//                       config.profile.burnout_timeout,
//                       config.profile.apogee_timeout,
//                       config.profile.main_altitude,
//                       config.profile.main_timeout,
//                       config.profile.land_timeout, 8);
//         m3can_send_u8(CAN_MSG_ID_CFG_PYROS,
//                       config.pyros.pyro1,
//                       config.pyros.pyro2,
//                       config.pyros.pyro3,
//                       config.pyros.pyro4,
//                       config.pyros.pyro5,
//                       config.pyros.pyro6,
//                       config.pyros.pyro7,
//                       config.pyros.pyro8, 8);
//         m3can_send_f32(CAN_MSG_ID_CFG_ACCEL_X,
//                        config.accel_cal.x_scale,
//                        config.accel_cal.x_offset, 2);
//         m3can_send_f32(CAN_MSG_ID_CFG_ACCEL_Y,
//                        config.accel_cal.y_scale,
//                        config.accel_cal.y_offset, 2);
//         m3can_send_f32(CAN_MSG_ID_CFG_ACCEL_Z,
//                        config.accel_cal.z_scale,
//                        config.accel_cal.z_offset, 2);
//         m3can_send_u32(CAN_MSG_ID_CFG_RADIO_FREQ,
//                        config.radio_freq, 0, 1);
//         m3can_send_u32(CAN_MSG_ID_CFG_CRC,
//                        config.crc, 0, 1);

//         /* Check the config. Sets an error inside the relevant config check
//          * functions, so no need to handle the error case here.
//          */
//         if(config_check()) {
//             m3status_set_ok(COMPONENT_CFG);
//         }

//         chThdSleepMilliseconds(5000);
//     }
// }

bool config_load() {
    // bool rv = m3flash_read((uint32_t*)CONFIG_FLASH,
    //                        (uint32_t*)&config, sizeof(config)/4);

    // if(!rv) {
    //     m3status_set_error(COMPONENT_FLASH, ERROR_FLASH_CRC);
    // }

    // return rv;
    return false;
}

void config_save() {
    // bool rv = flash_write((uint32_t*)&config,
    //                         (uint32_t*)CONFIG_FLASH,
    //                         sizeof(config)/4);
    // if(!rv) {
    //     status_set_error(COMPONENT_FLASH, ERROR_FLASH_WRITE);
    // }
}

void config_init() {
    status_set_init(COMPONENT_CFG);

    if(!config_load()) {
        status_set_error(COMPONENT_CFG, ERROR_CFG_READ);
    }

    // chThdCreateStatic(config_reporter_thd_wa,
    //                   sizeof(config_reporter_thd_wa),
    //                   NORMALPRIO, config_reporter_thd, NULL);
}


static bool config_check_profile() {
    bool ok = true;
    ok &= config.profile.position >= 1;
    ok &= config.profile.position <= 2;
    ok &= config.profile.accel_axis >= 1;
    ok &= config.profile.accel_axis <= 6;
    if(!ok) {
        status_set_error(COMPONENT_CFG, ERROR_CFG_CHK_PROFILE);
    }
    return ok;
}

static bool config_check_pyro(uint8_t pyro) {
    uint8_t usage = pyro & CONFIG_PYRO_USAGE_MASK;
    uint8_t current = pyro & CONFIG_PYRO_CURRENT_MASK;
    uint8_t type = pyro & CONFIG_PYRO_TYPE_MASK;

    /* Check usage is one of the valid options. */
    if(usage != CONFIG_PYRO_USAGE_NONE     &&
       usage != CONFIG_PYRO_USAGE_DROGUE   &&
       usage != CONFIG_PYRO_USAGE_MAIN     &&
       usage != CONFIG_PYRO_USAGE_DARTSEP)
    {
        return false;
    }

    /* Check current is one of the valid options. */
    if(current != CONFIG_PYRO_CURRENT_NONE &&
       current != CONFIG_PYRO_CURRENT_1A   &&
       current != CONFIG_PYRO_CURRENT_3A)
    {
        return false;
    }

    /* All possible bit patterns are valid options for type. */

    /* If usage is not NONE, check current and type are not either. */
    if(usage != CONFIG_PYRO_USAGE_NONE &&
       (current == CONFIG_PYRO_CURRENT_NONE ||
        type == CONFIG_PYRO_TYPE_NONE))
    {
        return false;
    }

    /* If usage is NONE, check current and type are too. */
    if(usage == CONFIG_PYRO_USAGE_NONE &&
       (current != CONFIG_PYRO_CURRENT_NONE ||
        type != CONFIG_PYRO_TYPE_NONE))
    {
        return false;
    }

    return true;
}


static bool config_check_pyros() {
    if(config_check_pyro(config.pyros.pyro1) &&
       config_check_pyro(config.pyros.pyro2) &&
       config_check_pyro(config.pyros.pyro3) &&
       config_check_pyro(config.pyros.pyro4) &&
       config_check_pyro(config.pyros.pyro5) &&
       config_check_pyro(config.pyros.pyro6) &&
       config_check_pyro(config.pyros.pyro7) &&
       config_check_pyro(config.pyros.pyro8))
    {
        return true;
    } else {
        status_set_error(COMPONENT_CFG, ERROR_CFG_CHK_PYROS);
        return false;
    }
}

static bool config_check_accel_cal(void)
{
    bool ok = true;
    ok &= config.accel_cal.x_scale > 0.0035f;
    ok &= config.accel_cal.x_scale < 0.0043f;
    ok &= config.accel_cal.y_scale > 0.0035f;
    ok &= config.accel_cal.y_scale < 0.0043f;
    ok &= config.accel_cal.z_scale > 0.0035f;
    ok &= config.accel_cal.z_scale < 0.0043f;
    ok &= config.accel_cal.x_offset > -40.0f;
    ok &= config.accel_cal.x_offset <  40.0f;
    ok &= config.accel_cal.y_offset > -40.0f;
    ok &= config.accel_cal.y_offset <  40.0f;
    ok &= config.accel_cal.z_offset > -64.0f;
    ok &= config.accel_cal.z_offset <  64.0f;
    if(!ok) {
        status_set_error(COMPONENT_CFG, ERROR_CFG_CHK_ACCEL_CAL);
    }
    return ok;
}

static bool config_check_radio_freq(void)
{
    bool ok = true;
    ok &= config.radio_freq > 868000000;
    ok &= config.radio_freq < 870000000;
    if(!ok) {
        status_set_error(COMPONENT_CFG, ERROR_CFG_CHK_RADIO_FREQ);
    }
    return ok;
}

static bool config_check_crc(void)
{
    // uint32_t crc;
    // uint32_t *src = (uint32_t*)&config;

    // /* Subtract one so we don't compute over the stored checksum */
    // size_t n = (sizeof(struct config) / 4) - 1;

    // RCC->AHB1ENR |= RCC_AHB1ENR_CRCEN;
    // CRC->CR |= CRC_CR_RESET;
    // for(size_t i=0; i<n; i++) {
    //     CRC->DR = src[i];
    // }
    // crc = CRC->DR;
    // RCC->AHB1ENR &= ~RCC_AHB1ENR_CRCEN;

    // bool ok = crc == config.crc;

    // if(!ok) {
    //     status_set_error(COMPONENT_CFG, ERROR_CFG_CHK_CRC);
    // }

    // return ok;
    return false;
}

bool config_check() {
    bool ok = true;
    ok &= config_check_profile();
    ok &= config_check_pyros();
    ok &= config_check_accel_cal();
    ok &= config_check_radio_freq();
    ok &= config_check_crc();
    return ok;
}

void config_handle_set_profile(uint8_t* data, uint8_t datalen) {
    if(datalen != 8) {
        status_set_error(COMPONENT_CFG, ERROR_CAN_BAD_COMMAND);
        return;
    }

    config.profile.position   = data[0];
    config.profile.accel_axis      = data[1];
    config.profile.ignition_accel  = data[2];
    config.profile.burnout_timeout = data[3];
    config.profile.apogee_timeout  = data[4];
    config.profile.main_altitude   = data[5];
    config.profile.main_timeout    = data[6];
    config.profile.land_timeout    = data[7];
    config_check();
}

void config_handle_set_pyros(uint8_t* data, uint8_t datalen) {
    if(datalen != 8) {
        status_set_error(COMPONENT_CFG, ERROR_CAN_BAD_COMMAND);
        return;
    }

    config.pyros.pyro1     = data[0];
    config.pyros.pyro2     = data[1];
    config.pyros.pyro3     = data[2];
    config.pyros.pyro4     = data[3];
    config.pyros.pyro5     = data[4];
    config.pyros.pyro6     = data[5];
    config.pyros.pyro7     = data[6];
    config.pyros.pyro8     = data[7];
    config_check();
}

void config_handle_set_accel_cal_x(uint8_t* data, uint8_t datalen) {
    if(datalen != 8) {
        status_set_error(COMPONENT_CFG, ERROR_CAN_BAD_COMMAND);
        return;
    }

    float *fdata = (float*)data;
    config.accel_cal.x_scale  = fdata[0];
    config.accel_cal.x_offset = fdata[1];
}

void config_handle_set_accel_cal_y(uint8_t* data, uint8_t datalen) {
    if(datalen != 8) {
        status_set_error(COMPONENT_CFG, ERROR_CAN_BAD_COMMAND);
        return;
    }

    float *fdata = (float*)data;
    config.accel_cal.y_scale  = fdata[0];
    config.accel_cal.y_offset = fdata[1];
}

void config_handle_set_accel_cal_z(uint8_t* data, uint8_t datalen) {
    if(datalen != 8) {
        status_set_error(COMPONENT_CFG, ERROR_CAN_BAD_COMMAND);
        return;
    }

    float *fdata = (float*)data;
    config.accel_cal.z_scale  = fdata[0];
    config.accel_cal.z_offset = fdata[1];
}

void config_handle_set_radio_freq(uint8_t* data, uint8_t datalen) {
    if(datalen != 4) {
        status_set_error(COMPONENT_CFG, ERROR_CAN_BAD_COMMAND);
        return;
    }

    uint32_t *u32data = (uint32_t*)data;
    config.radio_freq = u32data[0];
}

void config_handle_set_crc(uint8_t* data, uint8_t datalen) {
    if(datalen != 4) {
        status_set_error(COMPONENT_CFG, ERROR_CAN_BAD_COMMAND);
        return;
    }

    uint32_t *u32data = (uint32_t*)data;
    config.crc = u32data[0];
}

void config_handle_load(uint8_t* data, uint8_t datalen) {
    (void)data;
    (void)datalen;
    config_load();
    config_check();
}

void config_handle_save(uint8_t* data, uint8_t datalen) {
    (void)data;
    (void)datalen;
    config_save();
    config_load();
    config_check();
}
