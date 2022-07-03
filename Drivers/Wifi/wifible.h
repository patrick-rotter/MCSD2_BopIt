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
#include "stdbool.h"

#define BUFF_Size       128  /* must be power of two */

extern UART_HandleTypeDef *WIFI_BLE_Int;
static uint8_t circ_buffer[BUFF_Size];
static uint32_t rd_ptr;

#define DMA_WRITE_PTR ( (BUFF_Size - WIFI_BLE_Int->hdmarx->Instance->CNDTR) & (BUFF_Size - 1) )


/**
 * @brief Error type
 */
typedef uint8_t wifible_error_t;

typedef enum {
	DIST_LESS_THAN_5 = 11,
	DIST_MORE_THAN_15,
	DIST_10,

	JOYSTICK_UP = 21,
	JOYSTICK_LEFT,
	JOYSTICK_RIGHT,

	TOUCH_BOTTOM = 31,
	TOUCH_MID,
	TOUCH_TOP
} bop_it_command;

/**
 * @brief Initialization function.
 * @param uart The UART-Interface
 *
 * @description This function initializes all necessary pins and peripherals used for this click.
 */
void wifible_init(UART_HandleTypeDef *uart);

/**
 * @brief Generic write function.
 * @param wifible Click object.
 * @param data_buf Data buffer for sends.
 * @param len Number of bytes for sends.
 */
HAL_StatusTypeDef wifible_generic_write(char *data_buf, uint32_t timeout);


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

/**
 * @brief Processes the output of the command, sends it to the StdOUT and waits for the command to finish
 */
void wifible_process();

/**
 * @brief Sends a HTTP POST Request to a fqdn
 * @param fqdn: Char-Pointer to the data containing the Destination of the Request
 * @param path: Char-Pointer to the data containing the Requested Path
 * @param id: Char-Pointer to the data containing the id of the mcu
 * @param value: Char-Pointer to the data containing the value that will be transmitted
 */
void sendHttpPost(char *fqdn, char *path, int id, int value);

/**
 * @brief Initializes DMA Receive and sets the UART Handle
 * @param huart: UART-Handle for the communication with the WIFI-Modul
 */
void msgrx_init(UART_HandleTypeDef *huart);

/**
 * @brief Checks if the Buffer contains new Elements
 * @return returns a boolean value
 */
static bool msgrxIsEmpty(void);

/**
 * @brief Reads one new element
 * @return returns a uint8 Value of the new element
 */
static uint8_t msgrxGet(void);

#endif /* INC_WIFIBLE_H_ */
