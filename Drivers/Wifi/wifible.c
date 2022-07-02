/*
 * wifible.c
 *
 *  Created on: May 20, 2022
 *      Author: Florian
 */

#include "wifible.h"
#include "string.h"
#include "stdio.h"

UART_HandleTypeDef *WIFI_BLE_Int;

// ------------------------------------------------ PUBLIC FUNCTION DEFINITIONS

/**
 * @brief Initializes the WIFI Modul, sets multiconnection mode and sets WIFI-Mode to Station
 * @param uart: Pointer to the UART Handle, which is used for communication with the WifiModul
 */
void wifible_init(UART_HandleTypeDef *uart) {
	msgrx_init(uart);
	osDelay(500);
	wifible_send_command("AT+GMR");
	wifible_process();
	wifible_send_command("AT+CIPMUX=1");
	wifible_process();
	wifible_send_command("AT+CWMODE=1");
	wifible_process();
}

/**
 * @brief Writes data to the WIFI-Modul
 * @param data_buf: The Buffer containing the data that should be written to the WIFI-Modul
 * @param timeout: The timeout for the UART_Transmit Function
 * @return returns HAL_StatusTypeDef containing information if the operation was successful
 */
HAL_StatusTypeDef wifible_generic_write(char *data_buf, uint32_t timeout) {
	size_t len = strlen(data_buf);
	HAL_StatusTypeDef result = HAL_UART_Transmit(WIFI_BLE_Int, (uint8_t*) data_buf,
			len, timeout);
	return result;
}

/**
 * @brief Sends a Command to the WIFI-Modul
 * @param command: Char-Pointer to the data containing the command
 * @return returns HAL_StatusTypeDef containing information if the operation was successful
 */
HAL_StatusTypeDef wifible_send_command(char *command) {
	char tmp_buf[200];
	uint8_t len = strlen(command);

	memset(tmp_buf, 0, 200);

	strncpy(tmp_buf, command, len);
	strcat(tmp_buf, "\r\n");

	return wifible_generic_write(tmp_buf, HAL_MAX_DELAY);
}

/**
 * @brief Processes the output of the command, sends it to the StdOUT and waits for the command to finish
 */
void wifible_process() {
	char buffer[8] = { 0 };
	while (true) {
		for (int i = 0; i < 7; i++) {
			buffer[i] = buffer[i + 1];
		}
		while (msgrxIsEmpty()) {
			HAL_Delay(10);
		}
		buffer[7] = (uint8_t) msgrxGet();
		printf("%c", buffer[7]);
		if (strstr(buffer, "OK\r\n") != NULL
				|| strstr(buffer, "ERROR\r\n") != NULL) {
			break;
		}
	}
}

/**
 * @brief Connects to a WIFI with the given ssid and password
 * @param ssid: Char-Pointer to the data containing the ssid of the wifi
 * @param pw: Char-Pointer to the data containing the password of the wifi
 */
void connectWifi(char *ssid, char *pw) {
	char cmd[150] = { 0 };
	sprintf(cmd, "AT+CWJAP=\"%s\",\"%s\"", ssid, pw);
	wifible_send_command(cmd);
	wifible_process();
}

/**
 * @brief Sends a HTTP POST Request to a fqdn
 * @param fqdn: Char-Pointer to the data containing the Destination of the Request
 * @param path: Char-Pointer to the data containing the Requested Path
 * @param id: Char-Pointer to the data containing the id of the mcu
 * @param value: Char-Pointer to the data containing the value that will be transmitted
 */
void sendHttpPost(char *fqdn, char *path, int id, int value) {
	char cmd[200] = { 0 };
	sprintf(cmd,
			"AT+HTTPCLIENT=3,0,\"http://%s%s\",\"%s\",\"%s\",1,\"mcu=%d&cmd=%d\"",
			fqdn, path, fqdn, path, id, value);
	wifible_send_command(cmd);
	wifible_process();
}

/**
 * @brief Initializes DMA Receive and sets the UART Handle
 * @param huart: UART-Handle for the communication with the WIFI-Modul
 */
void msgrx_init(UART_HandleTypeDef *huart) {
	WIFI_BLE_Int = huart;
	HAL_UART_Receive_DMA(WIFI_BLE_Int, circ_buffer, BUFF_Size);
	rd_ptr = 0;
}

/**
 * @brief Checks if the Buffer contains new Elements
 * @return returns a boolean value
 */
static bool msgrxIsEmpty(void) {
	if (rd_ptr == DMA_WRITE_PTR) {
		return true;
	}
	return false;
}

/**
 * @brief Reads one new element
 * @return returns a uint8 Value of the new element
 */
static uint8_t msgrxGet(void) {
	uint8_t c = 0;
	if (rd_ptr != DMA_WRITE_PTR) {
		c = circ_buffer[rd_ptr++];
		rd_ptr &= (BUFF_Size - 1);
	}
	return c;
}
