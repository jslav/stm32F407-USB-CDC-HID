/**
  ******************************************************************************
  * @file    usbd_cdc.h
  * @author  MCD Application Team
  * @brief   header file for the usbd_cdc.c file.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2015 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                      www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USB_CDC_H
#define __USB_CDC_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include  "usbd_ioreq.h"

/**
  * @}
  */
extern USBD_ClassTypeDef USBD_COMPOSITE_CDC_HID;
#define USBD_COMPOSITE_CDC_HID_CLASS &USBD_COMPOSITE_CDC_HID

#ifdef __cplusplus
}
#endif

#endif  /* __USB_CDC_H */
/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
