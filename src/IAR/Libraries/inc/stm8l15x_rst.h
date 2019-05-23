/**
  ******************************************************************************
  * @file    stm8l15x_rst.h
  * @author  MCD Application Team
  * @version V1.5.0
  * @date    13-May-2011
  * @brief   This file contains all the functions prototypes for the RST firmware
  *          library.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************  
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM8L15x_RST_H
#define __STM8L15x_RST_H

/* Includes ------------------------------------------------------------------*/
#include "stm8l15x.h"

/** @addtogroup STM8L15x_StdPeriph_Driver
  * @{
  */
  
/** @addtogroup RST
  * @{
  */ 
  
/* Exported variables ------------------------------------------------------- */
/* Exported types ------------------------------------------------------------*/
/** @defgroup RST_Exported_Types
  * @{
  */

/** @defgroup RST_Flags 
  * @{
  */
typedef enum {
  RST_FLAG_PORF   = (uint8_t)0x01, /*!< POR reset flag */
  RST_FLAG_SWIMF  = (uint8_t)0x08, /*!< SWIM reset flag */
  RST_FLAG_ILLOPF = (uint8_t)0x04, /*!< Illegal opcode reset flag */
  RST_FLAG_IWDGF  = (uint8_t)0x02, /*!< Independent watchdog reset flag */
  RST_FLAG_WWDGF  = (uint8_t)0x10, /*!< Window watchdog reset flag */
  RST_FLAG_BORF   = (uint8_t)0x20  /*!< BOR reset flag */
} RST_FLAG_TypeDef;

#define IS_RST_FLAG(FLAG)  (((FLAG) == RST_FLAG_PORF) || ((FLAG) == RST_FLAG_BORF) || \
                            ((FLAG) == RST_FLAG_IWDGF) || ((FLAG) == RST_FLAG_ILLOPF) || \
                            ((FLAG) == RST_FLAG_WWDGF) || ((FLAG) == RST_FLAG_SWIMF))
/**
  * @}
  */
  
/**
  * @}
  */
/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
/* Flag management functions **************************************************/
FlagStatus RST_GetFlagStatus(RST_FLAG_TypeDef RST_Flag);
void RST_ClearFlag(RST_FLAG_TypeDef RST_Flag);

/* NRST Pin configuration function ********************************************/
void RST_GPOutputEnable(void);

#endif /* __STM8L15x_RST_H */
/**
  * @}
  */
  
/**
  * @}
  */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
