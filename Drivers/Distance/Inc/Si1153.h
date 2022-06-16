#include "main.h"

#ifndef SI1153
#define SI1153

#define SI1153_I2C_ADDRESS (0x53 << 1)

/* The SI1153 has two register sets. The I²C registers are directly accessible to the host/user and can be
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
#define SI1153_HOSTIN0_REG (0x0a)
#define SI1153_COMMAND_REG (0x0b)
#define SI1153_IRQ_ENABLE_REG (0x0f)
#define SI1153_RESPONSE1_REG (0x10)
#define SI1153_RESPONSE0_REG (0x11)
#define SI1153_IRQ_STATUS_REG (0x12)
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


#define SI1153_PARAM_ADDR_AUTO_INCREMENT_OFF (1 << 6)

extern UART_HandleTypeDef huart2;

void Si1153_hello_world(uint8_t *echo);

uint8_t Si1153_get_part_id(I2C_HandleTypeDef hi2c);

#endif
