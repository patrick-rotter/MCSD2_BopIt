#include "main.h"

#ifndef SI1153
#define SI1153

#define SI1153_I2C_ADDRESS (0x53 << 1)

/* @file Si1153.h
 * The SI1153 has two register sets. The I²C registers are directly accessible to the host/user and can be
 * read and written to directly, using the I²C interface. The second set of registers is the parameter table,
 * which is only indirectly accessible to the host. To read and write to the parameter table, one must make
 * use of the "COMMAND" I²C register. Writing a valid command code to the "COMMAND" register causes the
 * sensor to perform a corresponding operation. After every write to the "COMMAND" register, the "RESPONSE0"
 * register should be read and its contents examined, as it reports errors as well as increments every time
 * a command has been successfully executed. One of the main uses of the "COMMAND" register is to configure
 * the SI1153. This is accomplished by writing a byte into the "HOSTIN0" I²C register, and then writing the
 * "PARAM_SET" command into the "COMMAND" register. A successful execution of "PARAM_SET" causes the
 * written byte to be copied into the "RESPONSE1" I²C register.*/

/* SI1153 I²C registers */

#define SI1153_PART_ID_REG (0x00)
/* Configuration parameters are to be written into this register before calling "PARAM_SET". If successful,
 * the contents of this register will be copied into "RESPONSE1" after the command has been executed. */
#define SI1153_HOSTIN0_REG (0x0a)
#define SI1153_COMMAND_REG (0x0b)
#define SI1153_IRQ_ENABLE_REG (0x0f)
#define SI1153_RESPONSE1_REG (0x10)
#define SI1153_RESPONSE0_REG (0x11)
#define SI1153_IRQ_STATUS_REG (0x12)
/* The registers below contain captured sensor data after a measurement */
#define SI1153_HOSTOUT0_REG (0x13)
#define SI1153_HOSTOUT1_REG (0x14)
#define SI1153_HOSTOUT2_REG (0x15)
#define SI1153_HOSTOUT3_REG (0x16)
#define SI1153_HOSTOUT4_REG (0x17)

/* SI1153 commands */

#define SI1153_RST_CMD_CTR_COMMAND (0x00)
#define SI1153_RESET_SW_COMMAND (0x01)
#define SI1153_FORCE_MEASUREMENTS_COMMAND (0x11)
#define SI1153_PAUSE_MEASUREMENTS_COMMAND (0x12)
#define SI1153_START_MEASUREMENTS_COMMAND (0x13)
/* The following two macros are not commands of their own - they need to be logical or'd with a 6-bit
 * address pointing to a parameter in the SI1153's parameter table to be valid. */
#define SI1153_PARAM_QUERY_COMMAND_PREFIX (0x40)
#define SI1153_PARAM_SET_COMMAND_PREFIX (0x80)

/* SI1153 internal parameter table */

#define SI1153_I2C_ADDR_PARAM_ADDR (0x00)
#define SI1153_CHAN_LIST_PARAM_ADDR (0x01)
/* Channel 0 configuration */
#define SI1153_ADCCONFIG0_PARAM_ADDR (0x02)
#define SI1153_ADCSENS0_PARAM_ADDR (0x03)
#define SI1153_ADCPOST0_PARAM_ADDR (0x04)
#define SI1153_MEASCONFIG0_PARAM_ADDR (0x05)
/* Global configuration */
#define SI1153_MEASRATE_H_PARAM_ADDR (0x1a)
#define SI1153_MEASRATE_L_PARAM_ADDR (0x1b)
#define SI1153_MEASCOUNT0_PARAM_ADDR (0x1c)
#define SI1153_MEASCOUNT1_PARAM_ADDR (0x1d)
#define SI1153_MEASCOUNT2_PARAM_ADDR (0x1e)
#define SI1153_THRESHOLD0_H_PARAM_ADDR (0x25)
#define SI1153_THRESHOLD0_L_PARAM_ADDR (0x26)
#define SI1153_THRESHOLD1_H_PARAM_ADDR (0x27)
#define SI1153_THRESHOLD1_L_PARAM_ADDR (0x28)

#define SI1153_PARAM_ADDR_AUTO_INCREMENT_OFF (1 << 6)

/* SI1153 default channel settings */

/* @brief Use a 1024 clock cycle (48.8µs minimum) measurement time and the small infrared photodiode */
#define SI1153_ADCCONFIG_DEFAULT (0x00)
/* @brief Low range, no measurement accumulation. The four LSBs determine the measurement time multiplier
 * (with the multiplication base being 48.8µs, as specified in the default ADCCONFIG). This gives
 * us a measurement time of around 48.8µs * 2 ≈ 0.1ms */
#define SI1153_ADCSENS_DEFAULT (0x02)
/* @brief 16 bit measurements, with no accumulation shift. Interrupt thresholds deactivated. */
#define SI1153_ADCPOST_DEFAULT (0x00)
/* @brief Use measurement counter 0 (meaning it is mandatory to set it up), LEDs disabled. */
#define SI1153_MEASCONFIG_DEFAULT (0x40)

/* SI1153 default global settings */

#define SI1153_CHAN_LIST_DEFAULT (0x01)
#define SI1153_MEASRATE_H_DEFAULT (0x00)
#define SI1153_MEASRATE_L_DEFAULT (0x02)
/* All channels using MEASCOUNT0 will have a measurement period of 4ms */
#define SI1153_MEASCOUNT0_DEFAULT (0x05)

/* SI1153 RESPONSE0 register format masks */

#define SI1153_RESPONSE_RUNNING_MASK (1 << 7)
#define SI1153_RESPONSE_SUSPEND_MASK (1 << 6)
#define SI1153_RESPONSE_SLEEP_MASK (1 << 5)
#define SI1153_RESPONSE_ERROR_MASK (1 << 4)
#define SI1153_RESPONSE_COMMAND_COUNTER_MASK (0x0f)

/**
 * @brief Reads a single byte from a specified register on the Si1153.
 *
 * @param hi2c The handle of the I²C connection the Si1153 is on.
 * @param device_register The Si1153 register to be read.
 * @param data The read value will be written to this byte.
 *
 * @retval HAL_OK on success, HAL_ERROR on any kind of error, HAL_BUSY if the I²C connection is busy.
 */
HAL_StatusTypeDef Si1153_generic_read_single(I2C_HandleTypeDef *hi2c, uint8_t device_register, uint8_t *data);

/**
 * @brief Writes a single byte to a specified register on the Si1153.
 *
 * @param hi2c The handle of the I²C connection the Si1153 is on.
 * @param device_register The Si1153 register to be written to.
 * @param data The data to be written.
 *
 * @retval HAL_OK on success, HAL_ERROR on any kind of error, HAL_BUSY if the I²C connection is busy.
 */
HAL_StatusTypeDef Si1153_generic_write_single(I2C_HandleTypeDef *hi2c, uint8_t device_register, uint8_t data);

/**
 * @brief Get the part id of the Si1153. Should always yield the final the digits of the device
 * identifier, in hexadecimal. In the case of the Si1153, this should be 0x53.
 */
HAL_StatusTypeDef Si1153_get_part_id(I2C_HandleTypeDef *hi2c, uint8_t *read_result);

/**
 * @brief Query a value from the sensor's internal parameter table. Internally, this is done by
 * sending a command consisting of the SI1153_PARAM_QUERY_COMMAND_PREFIX and a 6-bit address
 * pointing to a location in the parameter table.
 *
 * @param read_result If successful, the value of the queried parameter will be written into this
 * address. If not, no write will occur.
 *
 * @retval Returns HAL_OK on success. If HAL_ERROR has been returned, it can mean one of several
 * things. Either there was an I2C connection error, the command could not be executed by the
 * sensor, or too many command execution attempts were necessary.
 */
HAL_StatusTypeDef Si1153_query_param(I2C_HandleTypeDef *hi2c, uint8_t parameter, uint8_t *read_result);

/**
 * @brief Set a value to the sensor's internal parameter table. Internally, this is done by
 * sending a command consisting of the SI1153_PARAM_SET_COMMAND_PREFIX and a 6-bit address
 * pointing to a location in the parameter table.
 *
 * @param parameter_value If successful, this value will be written into the corresponding
 * location in the parameter table. If not, no write will occur.
 *
 * @retval Returns HAL_OK on success. If HAL_ERROR has been returned, it can mean one of several
 * things. Either there was an I2C connection error, the command could not be executed by the
 * sensor, or too many command execution attempts were necessary.
 */
HAL_StatusTypeDef Si1153_set_param(I2C_HandleTypeDef *hi2c, uint8_t parameter, uint8_t parameter_value);

/**
 * @brief Resets the Si1153's internal command counter to 0. The command counter consists of 4
 * bits and increments on every successful command execution.
 */
HAL_StatusTypeDef Si1153_reset_command_counter(I2C_HandleTypeDef *hi2c);

/**
 * @brief Initiate autonomous mode, in which the Si1153 will periodically execute the measurements
 * specified in its configuration and place their results in the HOSTOUTx registers.
 */
HAL_StatusTypeDef Si1153_start_autonomous_mode(I2C_HandleTypeDef *hi2c);

/**
 * @brief Reads the first two HOSTOUT registers under the assumption that the Si1153 is configured
 * as to output its measurements to that exact location in a 16-bit format.
 */
HAL_StatusTypeDef Si1153_read_channel_0_16bit(I2C_HandleTypeDef *hi2c, uint16_t *channel_value);

#endif
