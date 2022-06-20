/*
 * wifible.c
 *
 *  Created on: May 20, 2022
 *      Author: Florian
 */

#include "wifible.h"
#include "string.h"
#include "stdio.h"

// ------------------------------------------------ PUBLIC FUNCTION DEFINITIONS

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

HAL_StatusTypeDef wifible_generic_write(char *data_buf, uint32_t timeout) {
	size_t len = strlen(data_buf);
	HAL_StatusTypeDef result = HAL_UART_Transmit(WIFI_BLE_Int, (uint8_t*) data_buf,
			len, timeout);
	return result;
}

HAL_StatusTypeDef wifible_send_command(char *command) {
	char tmp_buf[200];
	uint8_t len = strlen(command);

	memset(tmp_buf, 0, 200);

	strncpy(tmp_buf, command, len);
	strcat(tmp_buf, "\r\n");

	return wifible_generic_write(tmp_buf, HAL_MAX_DELAY);
}

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

void connectWifi(char *ssid, char *pw) {
	char cmd[150] = { 0 };
	sprintf(cmd, "AT+CWJAP=\"%s\",\"%s\"", ssid, pw);
	wifible_send_command(cmd);
	wifible_process();
}

void sendHttpPost(char *fqdn, char *path, int id, int value) {
	char cmd[200] = { 0 };
	sprintf(cmd,
			"AT+HTTPCLIENT=3,0,\"http://%s%s\",\"%s\",\"%s\",1,\"mcu=%d&cmd=%d\"",
			fqdn, path, fqdn, path, id, value);
	wifible_send_command(cmd);
	wifible_process();
}

void msgrx_init(UART_HandleTypeDef *huart) {
	WIFI_BLE_Int = huart;
	HAL_UART_Receive_DMA(WIFI_BLE_Int, circ_buffer, BUFF_Size);
	rd_ptr = 0;
}

static bool msgrxIsEmpty(void) {
	if (rd_ptr == DMA_WRITE_PTR) {
		return true;
	}
	return false;
}

static uint8_t msgrxGet(void) {
	uint8_t c = 0;
	if (rd_ptr != DMA_WRITE_PTR) {
		c = circ_buffer[rd_ptr++];
		rd_ptr &= (BUFF_Size - 1);
	}
	return c;
}
