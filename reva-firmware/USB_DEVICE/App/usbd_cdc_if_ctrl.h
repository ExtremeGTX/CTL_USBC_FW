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

typedef enum
{
    N_EOL = 0u,
    SKIP_LF_LR,
    LF, // Line feed
    CR, // Carriage return
} end_of_line_e;

/**
 * @brief Register Tx and Rx buffer
 * @Note Call once when initializing CDC
 *
 * @param USBD_HandleTypeDef: USB device handle structure
 * @param uint8_t *Tx_buf: Pointer to transmit buffer to register
 * @param uint8_t *Rx_buf: Pointer to receive buffer to register
 */
void register_buffers(USBD_HandleTypeDef *hUsbDeviceFS, uint8_t *Tx_buf, uint8_t *Rx_buf);

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
uint8_t CDC_Read_RX_FS(uint8_t *Buf, uint16_t Len);

/**
 * @brief Get len of new data to read from the receive buffer
 */
uint16_t CDC_RX_Buffer_len();

/**
 * @brief Flush  CDC external buffer
 */
void CDC_Flush_RX_buffer();

/**
 * @brief Clean external buffer
 *
 * @param uint8_t *buf: Pointer to the buffer
 * @param uint8_t buff_size: Size of buffer
 */
void local_buffer_flush(uint8_t *buf, size_t buff_size);

end_of_line_e find_eol_type(uint8_t *buf, uint8_t buf_len);

uint8_t CDC_RX_queue(uint8_t *buf, uint16_t Len);