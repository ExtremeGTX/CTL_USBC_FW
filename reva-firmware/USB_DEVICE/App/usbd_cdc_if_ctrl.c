#include "usbd_cdc_if_ctrl.h"
#include "at_cmds.h"

static volatile uint8_t rxBuffer[APP_RX_DATA_SIZE]; // Receive buffer
static volatile uint16_t rxBufferHeadPos = 0;       // Receive buffer write position
static volatile uint16_t rxBufferTailPos = 0;       // Receive buffer read position

static bool CDC_RX_Get_Next_Char(char *ch)
{
    if (rxBufferHeadPos == rxBufferTailPos)
        return false;

    *ch = rxBuffer[rxBufferTailPos];

    rxBufferTailPos++;

    rxBufferTailPos %= APP_RX_DATA_SIZE;

    return true;
}

static bool is_eol_char(char ch)
{
    return (ch == '\n') || (ch == '\r');
}

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

void CDC_Read_RX_FS(void)
{
    char c;
    bool has_next_char = CDC_RX_Get_Next_Char(&c);

    if (!has_next_char)
        return;

    static char buf[128];
    static size_t buf_index = 0;

    if (is_eol_char(c)) {
        buf[buf_index] = '\0';

        if (strlen(buf)){

            // MASSIVE HACK - take out of process_input.
            buf[buf_index] = '\n';
            buf[buf_index + 1] = '\0';

            process_input(buf);
        }

        buf_index = 0;
    } else {
        buf[buf_index] = c;

        buf_index++;

        if (buf_index > sizeof(buf)) {
            printf("BUFFER OVERFLOW, COMMANDS DROPPED\r\n");

            buf_index = 0;

            return;
        }
    }

    return;
}

end_of_line_e find_eol_type(uint8_t *buf, uint8_t buf_len)
{
    // printf("out val %s %d \r\n", (char *)buf, buf_len);
    if (buf_len == 0 && (buf[buf_len] == '\n' || buf[buf_len] == '\r'))
    {
        return SKIP_LF_LR;
    }
    if (buf[buf_len] == '\n')
    {

        return LF;
    }

    else if (buf[buf_len] == '\r')
    {
        return CR;
    }

    else
    {
        return N_EOL;
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
