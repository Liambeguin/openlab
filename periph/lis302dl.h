/*
 * This file is part of DronolabSTM32-embedded
 *
 * DronolabSTM32-embedded is a student project designed
 * for flying drones.
 *
 * Go to < dronolab.etsmtl.ca > to find out more about
 * the project
 *
 *
 */


/*
 * lis302dl.h
 *
 *  Created on: 2013-10-13
 *      Author: liam
 */

#ifndef LIS302DL_H_
#define LIS302DL_H_

#include <stdint.h>

void lis302dl_read_whoami();

/**
  * @brief  Writes bytes to the LIS302DL.
  * @param  data : pointer to the buffer  containing the data to be written to the LIS302DL.
  * @param  reg_addr : LIS302DL's internal address to write to.
  * @param  lenght: Number of bytes to write.
  */
void lis302dl_write(uint8_t* data, uint8_t reg_addr, uint16_t length);

/**
  * @brief  Reads a block of data from the LIS302DL.
  * @param  pBuffer : pointer to the buffer that receives the data read from the LIS302DL.
  * @param  ReadAddr : LIS302DL's internal address to read from.
  * @param  NumByteToRead : number of bytes to read from the LIS302DL.
  * @retval None
  */
void lis302dl_read(uint8_t* data, uint8_t reg_addr, uint16_t length);

#endif /* LIS302DL_H_ */
