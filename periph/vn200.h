#include "uart.h"

#define PARSER_BUFFER_SIZE 256

typedef enum {
	// Configuration registers
	USERTAG,
	MODEL_NUMBER,
	HARDWARE_REV,
	SERIAL_NUMBER,
	FW_VERSION,
	SERIAL_BAUDRATE,
	ASYNC_DATA_OUTPUT_TYPE,
	ASYNC_DATA_OUTPUT_FREQ,

	// Measurement registers
	YAW_PITCH_ROLL
} vn200_regID_t;

void vn200_init(uart_t uart);
uint8_t* vn200_getModel (void);
void vn200_decodeUartRx (void);
void vn200_getReg(vn200_regID_t regID);
void vn200_sendRequest(uint8_t const *request, uint8_t size);

typedef struct {

}vn200_t;

/* VN200 Datastore*/
vn200_t IMU;
