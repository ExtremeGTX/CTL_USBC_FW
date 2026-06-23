/**
 ******************************************************************************
 * @file    board_config.h
 * @brief   Persistent board configuration stored in internal flash.
 ******************************************************************************
 */

#ifndef _BOARD_CONFIG_H_
#define _BOARD_CONFIG_H_

#include "stdbool.h"
#include "stddef.h"

#define BOARD_ID_MAX_LEN 32U

/**
 * @brief Return the stored board ID, or an empty string if none is stored.
 */
const char *BoardConfig_GetBoardId(void);

/**
 * @brief Validate a board ID string before storing it.
 */
bool BoardConfig_IsValidBoardId(const char *board_id);

/**
 * @brief Store a board ID in the reserved flash configuration page.
 */
bool BoardConfig_SetBoardId(const char *board_id);

#endif
