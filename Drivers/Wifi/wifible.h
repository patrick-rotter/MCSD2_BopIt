/*
 * wifible.h
 *
 *  Created on: 20.05.2022
 *      Author: Florian
 */

#ifndef INC_WIFIBLE_H_
#define INC_WIFIBLE_H_

#include "cmsis_os.h"
#include "main.h"

UART_HandleTypeDef *WIFI_BLE_Int;
osSemaphoreId_t contSem;

/**
 * @brief Click ctx object definition.
 */
typedef struct {
	// Output pins

	GPIO_TypeDef *rst;
	uint16_t rstPin;
	GPIO_TypeDef *en;
	uint16_t enPin;

	// Modules

	UART_HandleTypeDef *uart;

} wifible_t;

/**
 * @brief Error type
 */
typedef uint8_t wifible_error_t;

/**
 * @brief Initialization function.
 * @param wifible Click object.
 * @param cfg Click configuration structure.
 *
 * @description This function initializes all necessary pins and peripherals used for this click.
 */
void wifible_init(UART_HandleTypeDef *uart, osSemaphoreId_t cont);

/**
 * @brief Power module.
 *
 * @param ctx           Click object.
 * @param power_state   State of pin.
 */
void wifible_module_power(wifible_t *ctx, uint8_t power_state);

/**
 * @brief Generic write function.
 * @param wifible Click object.
 * @param data_buf Data buffer for sends.
 * @param len Number of bytes for sends.
 */
HAL_StatusTypeDef wifible_generic_write(char *data_buf, uint32_t timeout);

/**
 * @brief Generic read function.
 * @param wifible Click object.
 * @param data_buf Data buffer for read data.
 * @param max_len The maximum length of data that can be read.
 * @return Number of reads data.
 */
HAL_StatusTypeDef wifible_generic_read(wifible_t *ctx, char *data_buf,
		uint32_t timeout);

/**
 * @brief Function for send command.
 *
 * @param command   Data to be send.
 */
HAL_StatusTypeDef wifible_send_command(char *command);

/**
 * @brief Connects to an WLAN
 *
 * @param ssid			SSID of the WLAN
 * @param pw			Password of the WLAN
 */
void connectWifi(char *ssid, char *pw);

void wifible_process();

void sendHttpPost(char *fqdn, char *path, int id, int value);

#endif /* INC_WIFIBLE_H_ */
