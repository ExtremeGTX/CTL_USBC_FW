#pragma once

#include "usbd_cdc_if.h"
#include "stdint.h"
#include "stdio.h"

/**
 * @brief Enum defining USB CDC buffer status
 */
typedef enum
{
    USB_CDC_RX_BUFFER_OK = 0U,
    USB_CDC_RX_BUFFER_NO_DATA
} USB_CDC_RX_BUFFER_StatusTypeDef;

/**
 * @brief Handle incoming data over serial and fill it in the receive data buffer.
 * @Note call inside CDC_Receive_fs(uint8_t *Buf, uint32_t *Len) function in usbd_cdc_if.c
 *
 * @param USBD_HandleTypeDef: USB device handle structure
 * @param uint8_t *incoming_Buf: incoming Rx buffer from CDC_Receive_fs(uint8_t *Buf, uint32_t *Len)
 * @param uint32_t *Len: length pointer from CDC_Receive_fs(uint8_t *Buf, uint32_t *Len)
 */
uint8_t CDC_fill_receive_buffer(USBD_HandleTypeDef *hUsbDeviceFS, uint8_t *incoming_Buf, uint32_t *Len);

/**
 * @brief Handle transmitting data over USB logic
 * @Note call in uint8_t CDC_Transmit_FS(uint8_t *Buf, uint16_t Len)
 *
 * @param USBD_HandleTypeDef: USB device handle structure
 * @param uint8_t *Buf: incoming Tx buffer from uint8_t CDC_Transmit_FS(uint8_t *Buf, uint16_t Len)
 * @param uint16_t Len: Length of Tx buffer received from CDC_transmit_logic(USBD_HandleTypeDef *hUsbDeviceFS, uint8_t *Buf, uint16_t Len)
 */
uint8_t CDC_transmit_logic(USBD_HandleTypeDef *hUsbDeviceFS, uint8_t *Buf, uint16_t Len);

/**
 * @brief Get data from the receive buffer and populate provided
 * buffer with it. Data is populated until a LF or CR
 *
 * @param uint8_t *Buf: External buffer to fill data from CDC receive buffer
 * @param uint8_t Len: Len of of the external buffer
 */
void CDC_Read_RX_FS(void);

/**
 * @brief Flush  CDC external buffer
 */
void CDC_Flush_RX_buffer();
