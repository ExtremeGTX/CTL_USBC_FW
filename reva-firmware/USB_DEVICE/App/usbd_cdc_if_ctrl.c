#include "usbd_cdc_if_ctrl.h"

volatile uint8_t rxBuffer[APP_RX_DATA_SIZE]; // Receive buffer
volatile uint16_t rxBufferHeadPos = 0;       // Receive buffer write position
volatile uint16_t rxBufferTailPos = 0;       // Receive buffer read position

void register_buffers(USBD_HandleTypeDef *hUsbDeviceFS, uint8_t *Tx_buf, uint8_t *Rx_buf)
{
    USBD_CDC_SetTxBuffer(hUsbDeviceFS, Tx_buf, 0);
    USBD_CDC_SetRxBuffer(hUsbDeviceFS, Rx_buf);
}

uint8_t CDC_fill_receive_buffer(USBD_HandleTypeDef *hUsbDeviceFS, uint8_t *incoming_Buf, uint32_t *Len)
{

    uint32_t len = (uint32_t)*Len;          // Get length
    uint16_t tempHeadPos = rxBufferHeadPos; // Increment temp head pos while writing, then update main variable when complete

    for (uint32_t i = 0; i < len; i++)
    {
        rxBuffer[tempHeadPos] = incoming_Buf[i];
        tempHeadPos = (uint16_t)((uint16_t)(tempHeadPos + 1) % APP_RX_DATA_SIZE);

        if (tempHeadPos == rxBufferTailPos)
        {
            return USBD_FAIL;
        }
    }

    rxBufferHeadPos = tempHeadPos;

    USBD_CDC_ReceivePacket(hUsbDeviceFS);
    return (USBD_OK);
}

uint8_t CDC_Read_RX_FS(uint8_t *Buf, uint16_t Len)
{
    uint16_t bytesAvailable = CDC_RX_Buffer_len();
    bool new_line = false;

    static size_t buf_consumed = 0;
    if (buf_consumed >= LOCAL_BUFFER_SIZE)
    {
        printf("BUFFER OVERFLOW, FEW COMMANDS DROPPED\r\n");
        local_buffer_flush(Buf, LOCAL_BUFFER_SIZE);
        buf_consumed = 0;
    }

    for (uint16_t i = 0; i < bytesAvailable; i++)
    {

        if ((buf_consumed + 1) >= LOCAL_BUFFER_SIZE)
        {
            printf("BUFFER OVERFLOW, FEW COMMANDS DROPPED\r\n");
            local_buffer_flush(Buf, LOCAL_BUFFER_SIZE);
        }
        Buf[buf_consumed] = rxBuffer[rxBufferTailPos];
        if (Buf[buf_consumed] == '\n' || Buf[buf_consumed] == '\r')
        {
            new_line = true;
            buf_consumed = (buf_consumed + 1) % LOCAL_BUFFER_SIZE;
            rxBufferTailPos = (uint16_t)((uint16_t)(rxBufferTailPos + 1) % APP_RX_DATA_SIZE);
            break;
        }
        buf_consumed = (buf_consumed + 1) % LOCAL_BUFFER_SIZE;
        rxBufferTailPos = (uint16_t)((uint16_t)(rxBufferTailPos + 1) % APP_RX_DATA_SIZE);
    }
    if (new_line)
    {
        buf_consumed = 0;
        return USB_CDC_RX_BUFFER_OK;
    }
    else
    {
        return USB_CDC_RX_BUFFER_NO_DATA;
    }
}

uint8_t CDC_transmit_logic(USBD_HandleTypeDef *hUsbDeviceFS, uint8_t *Buf, uint16_t Len)
{

    uint8_t result = USBD_OK;
    USBD_CDC_HandleTypeDef *hcdc = (USBD_CDC_HandleTypeDef *)(hUsbDeviceFS->pClassData);
    if (hcdc->TxState != 0)
    {
        return USBD_BUSY;
    }
    USBD_CDC_SetTxBuffer(hUsbDeviceFS, Buf, Len);
    result = USBD_CDC_TransmitPacket(hUsbDeviceFS);
    return result;
}

uint16_t CDC_RX_Buffer_len()
{
    return (uint16_t)(rxBufferHeadPos - rxBufferTailPos) % APP_RX_DATA_SIZE;
}

void CDC_Flush_RX_buffer()
{
    for (int i = 0; i < APP_RX_DATA_SIZE; i++)
    {
        rxBuffer[i] = 0;
    }

    rxBufferHeadPos = 0;
    rxBufferTailPos = 0;
}

void local_buffer_flush(uint8_t *buf, size_t buff_size)
{
    memset(buf, 0, buff_size);
}