#include "usbd_cdc_if_ctrl.h"
#include "at_cmds.h"

static volatile uint8_t rxBuffer[APP_RX_DATA_SIZE]; // Receive buffer
static volatile uint16_t rxBufferHeadPos = 0;       // Receive buffer write position
static volatile uint16_t rxBufferTailPos = 0;       // Receive buffer read position

static bool CDC_RX_Get_Next_Char(char *ch)
{
    // If buffer is empty
    if (rxBufferHeadPos == rxBufferTailPos)
        return false;

    // Set passed in char to new value in Rx buffer
    *ch = rxBuffer[rxBufferTailPos];

    // Increment tail position
    rxBufferTailPos++;

    // Make sure the tail position does not go beyond
    // the RX buffer size
    rxBufferTailPos %= APP_RX_DATA_SIZE;

    return true;
}

static bool is_eol_char(char ch)
{
    // Return if CR or LF detected
    return (ch == '\n') || (ch == '\r');
}

uint8_t CDC_fill_receive_buffer(USBD_HandleTypeDef *hUsbDeviceFS, uint8_t *incoming_Buf, uint32_t *Len)
{

    uint32_t len = (uint32_t)*Len;
    uint16_t tempHeadPos = rxBufferHeadPos;

    for (uint32_t i = 0; i < len; i++)
    {
        rxBuffer[tempHeadPos] = incoming_Buf[i];
        tempHeadPos = (uint16_t)((uint16_t)(tempHeadPos + 1) % APP_RX_DATA_SIZE);

        if (tempHeadPos == rxBufferTailPos)
        {
            USBD_CDC_ReceivePacket(hUsbDeviceFS);
            return USBD_FAIL;
        }
    }

    rxBufferHeadPos = tempHeadPos;

    USBD_CDC_ReceivePacket(hUsbDeviceFS);
    return (USBD_OK);
}

void CDC_Read_RX_FS(void)
{
    char c;
    // Local buffer for processing data in rxBuffer
    static char buf[LOCAL_BUFFER_SIZE];
    static size_t buf_index = 0;

    // Get next character from the Rx buffer
    bool has_next_char = CDC_RX_Get_Next_Char(&c);

    // If no new char exit
    if (!has_next_char)
        return;

    // IF CR or LF seen
    if (is_eol_char(c))
    {

        // Clear the EOL
        buf[buf_index] = '\0';

        // If EOL; anything except '\r' '\n'?
        // buf index is only incremented if anything except LF or CR is seen
        if (buf_index > 0)
        {
            process_input(buf);
        }
        buf_index = 0;
    }
    else
    {

        buf[buf_index] = c;
        buf_index++;

        if (buf_index >= (sizeof(buf) / sizeof(buf[0])))
        {
            buf_index = 0;
            printf("BUFFER OVERFLOW, COMMANDS DROPPED\r\n");
            return;
        }
    }

    return;
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

void CDC_Flush_RX_buffer()
{
    for (int i = 0; i < (sizeof(rxBuffer) / sizeof(rxBuffer[0])); i++)
    {
        rxBuffer[i] = 0;
    }

    rxBufferHeadPos = 0;
    rxBufferTailPos = 0;
}
