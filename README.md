# MCSD2_BopIt

* Florian Parzer (ic20b081@technikum-wien.at)
* Nestor Hiebl (ic20b015@technikum-wien.at)
* Patrick Rotter (ic20b029@technikum-wien.at)

This project is a simple, modern reinterpretation of the 90s toy [Bop It](https://en.wikipedia.org/wiki/Bop_It).
___
### 1. MCU + peripherals

##### 1.1 Required hardware
* [STM32L432KC](https://www.st.com/en/microcontrollers-microprocessors/stm32l432kc.html) MCU
* [Mikroe Joystick Click](https://www.mikroe.com/joystick-click)
* [Mikroe Proximity 13 Click](https://www.mikroe.com/proximity-13-click)
* [Mikroe CapSense Click](https://www.mikroe.com/capsense-click)
* [Mikroe Wifi BLE Click](https://www.reichelt.com/de/en/wifi-ble-click-board-esp32-wroom-32-mikroe-3542-p314143.html?r=1)

##### 1.2 Important Information

Code generation via .ioc-file causes a known bug to initialize UART and DMA in wrong order which may cause undefined behaviour.
Please ensure that the initialization in each main.c is in the order below:

```
  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  MX_I2C1_Init();
  MX_DMA_Init();
  MX_USART1_UART_Init();
  ```
##### 1.3 Documentation


Please refer to the doxygen documentation for more details.
___
### 2. Web

##### 2.1 Web-Dependancies

[Node](https://nodejs.org/en/download/) must be installed manually. All other dependencies will automatically be installed with the npm package manager.
(Used versions are in parenthesis.)
* Node (v16.15.0)

###### 2.1.1 Backend
* Nodemon (v2.0.16)
* Express (v4.18.1)

###### 2.1.2 Frontend
* React (v18.1.0)
* Framer Motion (v6.3.11)

##### 2.2 Running the application

Navigate into MCSD2_BopIt/Web/server and MCSD2_BopIt/Web/client and run ```npm install``` to install dependencies.
To run the server, run ```node app.js``` in MCSD2_BopIt/Web/server.
In order to run the client, execute ```npm start```.


