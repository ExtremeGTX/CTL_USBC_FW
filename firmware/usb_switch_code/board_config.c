/**
 ******************************************************************************
 * @file    board_config.c
 * @brief   Persistent board configuration stored in internal flash.
 ******************************************************************************
 */

#include "board_config.h"
#include "stm32f1xx_hal.h"
#include <stdint.h>
#include <string.h>

#define BOARD_CONFIG_FLASH_ADDR 0x0800FC00U
#define BOARD_CONFIG_MAGIC 0x42444944U /* "BDID" */
#define BOARD_CONFIG_VERSION 1U

typedef struct
{
  uint32_t magic;
  uint16_t version;
  uint16_t board_id_len;
  char board_id[BOARD_ID_MAX_LEN];
  uint32_t checksum;
} board_config_flash_t;

static const board_config_flash_t *const flash_config =
    (const board_config_flash_t *)BOARD_CONFIG_FLASH_ADDR;

static char cached_board_id[BOARD_ID_MAX_LEN + 1U];
static bool cache_loaded = false;

static bool is_valid_board_id_char(char ch)
{
  return ((ch >= '0') && (ch <= '9')) || ((ch >= 'A') && (ch <= 'Z')) ||
         ((ch >= 'a') && (ch <= 'z')) || (ch == '_') || (ch == '-');
}

static uint32_t calculate_checksum(const board_config_flash_t *config)
{
  uint32_t checksum = config->magic ^ config->version ^ config->board_id_len;

  for (uint32_t i = 0U; i < BOARD_ID_MAX_LEN; i++)
  {
    checksum = (checksum << 5) - checksum + (uint8_t)config->board_id[i];
  }

  return checksum;
}

static bool is_valid_flash_config(const board_config_flash_t *config)
{
  if ((config->magic != BOARD_CONFIG_MAGIC) ||
      (config->version != BOARD_CONFIG_VERSION) ||
      (config->board_id_len == 0U) ||
      (config->board_id_len > BOARD_ID_MAX_LEN) ||
      (config->checksum != calculate_checksum(config)))
  {
    return false;
  }

  for (uint32_t i = 0U; i < config->board_id_len; i++)
  {
    if (!is_valid_board_id_char(config->board_id[i]))
    {
      return false;
    }
  }

  return true;
}

static void load_cache(void)
{
  if (cache_loaded)
  {
    return;
  }

  memset(cached_board_id, 0, sizeof(cached_board_id));

  if (is_valid_flash_config(flash_config))
  {
    memcpy(cached_board_id, flash_config->board_id, flash_config->board_id_len);
    cached_board_id[flash_config->board_id_len] = '\0';
  }

  cache_loaded = true;
}

const char *BoardConfig_GetBoardId(void)
{
  load_cache();
  return cached_board_id;
}

bool BoardConfig_IsValidBoardId(const char *board_id)
{
  size_t board_id_len = 0U;

  if (board_id == NULL)
  {
    return false;
  }

  while (board_id[board_id_len] != '\0')
  {
    if ((board_id_len >= BOARD_ID_MAX_LEN) ||
        !is_valid_board_id_char(board_id[board_id_len]))
    {
      return false;
    }

    board_id_len++;
  }

  return board_id_len > 0U;
}

bool BoardConfig_SetBoardId(const char *board_id)
{
  board_config_flash_t config;
  FLASH_EraseInitTypeDef erase_init;
  uint32_t page_error = 0U;
  HAL_StatusTypeDef status;
  uint32_t address = BOARD_CONFIG_FLASH_ADDR;
  const uint16_t *config_halfwords = (const uint16_t *)&config;

  if (!BoardConfig_IsValidBoardId(board_id))
  {
    return false;
  }

  memset(&config, 0xFF, sizeof(config));
  config.magic = BOARD_CONFIG_MAGIC;
  config.version = BOARD_CONFIG_VERSION;
  config.board_id_len = (uint16_t)strlen(board_id);
  memcpy(config.board_id, board_id, config.board_id_len);
  config.checksum = calculate_checksum(&config);

  status = HAL_FLASH_Unlock();
  if (status != HAL_OK)
  {
    return false;
  }

  erase_init.TypeErase = FLASH_TYPEERASE_PAGES;
  erase_init.PageAddress = BOARD_CONFIG_FLASH_ADDR;
  erase_init.NbPages = 1U;

  status = HAL_FLASHEx_Erase(&erase_init, &page_error);
  if (status == HAL_OK)
  {
    for (uint32_t i = 0U; i < (sizeof(config) / sizeof(uint16_t)); i++)
    {
      status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, address,
                                 config_halfwords[i]);
      if (status != HAL_OK)
      {
        break;
      }

      address += sizeof(uint16_t);
    }
  }

  (void)HAL_FLASH_Lock();

  if ((status != HAL_OK) || !is_valid_flash_config(flash_config))
  {
    cache_loaded = false;
    return false;
  }

  memcpy(cached_board_id, board_id, config.board_id_len);
  cached_board_id[config.board_id_len] = '\0';
  cache_loaded = true;
  return true;
}
