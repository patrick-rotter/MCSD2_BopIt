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

void wifible_init(UART_HandleTypeDef *uart, osSemaphoreId_t cont) {
	WIFI_BLE_Int = uart;
	contSem = cont;

	wifible_process();
	wifible_send_command("AT+GMR");
	wifible_process();
	wifible_send_command("AT+CIPMUX=1");
	wifible_process();
	wifible_send_command("AT+CWMODE=1");
}

/*
void wifible_module_power(wifible_t *ctx, uint8_t power_state) {
	if (0 != power_state) {
		HAL_GPIO_WritePin(ctx->en, ctx->enPin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(ctx->rst, ctx->rstPin, GPIO_PIN_RESET);
		osDelay(100);
		HAL_GPIO_WritePin(ctx->rst, ctx->rstPin, GPIO_PIN_SET);
		osDelay(1);
	} else {
		HAL_GPIO_WritePin(ctx->en, ctx->enPin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(ctx->rst, ctx->rstPin, GPIO_PIN_RESET);
		osDelay(100);
	}
	osDelay(2000);
}
*/

HAL_StatusTypeDef wifible_generic_write(char *data_buf, uint32_t timeout) {
	return HAL_UART_Transmit(WIFI_BLE_Int, (uint8_t*) data_buf,
			strlen(data_buf), timeout);
}

/*
 HAL_StatusTypeDef wifible_generic_read(wifible_t *ctx, char *data_buf,
 uint32_t timeout) {
 return HAL_UART_Receive_DMA(ctx->uart, data_buf, 3);
 }
 */

HAL_StatusTypeDef wifible_send_command(char *command) {
	char tmp_buf[100];
	uint8_t len = strlen(command);

	memset(tmp_buf, 0, 100);

	strncpy(tmp_buf, command, len);
	strcat(tmp_buf, "\r\n");

	return wifible_generic_write(tmp_buf, HAL_MAX_DELAY);
}

void wifible_process() {
	osSemaphoreAcquire(contSem, HAL_MAX_DELAY);
	//osDelay(5000);
}

void connectWifi(char *ssid, char *pw) {
	char cmd[150] = { 0 };
	sprintf(cmd, "AT+CWJAP=\"%s\",\"%s\"", ssid, pw);
	wifible_process();
	wifible_send_command(cmd);
}

void sendHttpPost(char *fqdn, char *path, int id, int value) {
	char cmd[200] = { 0 };
	sprintf(cmd,
			"AT+HTTPCLIENT=3,0,\"http://%s%s\",\"%s\",\"%s\",1,\"mcu=%d&cmd=%d\"",
			fqdn, path, fqdn, path, id, value);
	wifible_process();
	wifible_send_command(cmd);
}
