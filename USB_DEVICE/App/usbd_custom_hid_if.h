/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : usbd_custom_hid_if.h
  * @version        : v1.0_Cube
  * @brief          : Header for usbd_custom_hid_if.c file.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USBD_CUSTOM_HID_IF_H__
#define __USBD_CUSTOM_HID_IF_H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
 /* Includes ------------------------------------------------------------------*/
 #include  "usbd_ioreq.h"

 /** @addtogroup STM32_USB_DEVICE_LIBRARY
   * @{
   */

 /** @defgroup USBD_CUSTOM_HID
   * @brief This file is the Header file for USBD_customhid.c
   * @{
   */


 /** @defgroup USBD_CUSTOM_HID_Exported_Defines
   * @{
   */
 #define CUSTOM_HID_EPIN_SIZE                         64

 #define CUSTOM_HID_EPOUT_SIZE                        64

 #define USB_CUSTOM_HID_CONFIG_DESC_SIZ               41U
 #define USB_CUSTOM_HID_DESC_SIZ                      9U

 #ifndef CUSTOM_HID_HS_BINTERVAL
 #define CUSTOM_HID_HS_BINTERVAL                      0x05U
 #endif /* CUSTOM_HID_HS_BINTERVAL */

 #ifndef CUSTOM_HID_FS_BINTERVAL
 #define CUSTOM_HID_FS_BINTERVAL                      0x05U
 #endif /* CUSTOM_HID_FS_BINTERVAL */

 #ifndef USBD_CUSTOMHID_OUTREPORT_BUF_SIZE
 #define USBD_CUSTOMHID_OUTREPORT_BUF_SIZE            0x02U
 #endif /* USBD_CUSTOMHID_OUTREPORT_BUF_SIZE */

 #ifndef USBD_CUSTOM_HID_REPORT_DESC_SIZE
 #define USBD_CUSTOM_HID_REPORT_DESC_SIZE             163U
 #endif /* USBD_CUSTOM_HID_REPORT_DESC_SIZE */

 #define CUSTOM_HID_DESCRIPTOR_TYPE                   0x21U
 #define CUSTOM_HID_REPORT_DESC                       0x22U

 #define CUSTOM_HID_REQ_SET_PROTOCOL                  0x0BU
 #define CUSTOM_HID_REQ_GET_PROTOCOL                  0x03U

 #define CUSTOM_HID_REQ_SET_IDLE                      0x0AU
 #define CUSTOM_HID_REQ_GET_IDLE                      0x02U

 #define CUSTOM_HID_REQ_SET_REPORT                    0x09U
 #define CUSTOM_HID_REQ_GET_REPORT                    0x01U
 /**
   * @}
   */


 /** @defgroup USBD_CORE_Exported_TypesDefinitions
   * @{
   */
 typedef enum
 {
   CUSTOM_HID_IDLE = 0U,
   CUSTOM_HID_BUSY,
 } CUSTOM_HID_StateTypeDef;

 typedef struct _USBD_CUSTOM_HID_Itf
 {
   uint8_t *pReport;
   int8_t (* Init)(void);
   int8_t (* DeInit)(void);
   int8_t (* OutEvent)(uint8_t event_idx, uint8_t state);

 } USBD_CUSTOM_HID_ItfTypeDef;

 typedef struct
 {
   uint8_t  Report_buf[USBD_CUSTOMHID_OUTREPORT_BUF_SIZE];
   uint32_t Protocol;
   uint32_t IdleState;
   uint32_t AltSetting;
   uint32_t IsReportAvailable;
   CUSTOM_HID_StateTypeDef state;
 } USBD_CUSTOM_HID_HandleTypeDef;
 /**
   * @}
   */



 /** @defgroup USBD_CORE_Exported_Macros
   * @{
   */

 /**
   * @}
   */

 /** @defgroup USBD_CORE_Exported_Variables
   * @{
   */

 extern USBD_ClassTypeDef USBD_CUSTOM_HID;
 #define USBD_CUSTOM_HID_CLASS &USBD_CUSTOM_HID
 /**
   * @}
   */

 /** @defgroup USB_CORE_Exported_Functions
   * @{
   */
 uint8_t USBD_CUSTOM_HID_SendReport(USBD_HandleTypeDef *pdev,
                                    uint8_t *report, uint16_t len);

 uint8_t USBD_CUSTOM_HID_ReceivePacket(USBD_HandleTypeDef *pdev);

//#include "usbd_customhid.h"

/* USER CODE BEGIN INCLUDE */

/* USER CODE END INCLUDE */

/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
  * @brief For Usb device.
  * @{
  */

/** @defgroup USBD_CUSTOM_HID USBD_CUSTOM_HID
  * @brief Usb custom human interface device module.
  * @{
  */

/** @defgroup USBD_CUSTOM_HID_Exported_Defines USBD_CUSTOM_HID_Exported_Defines
  * @brief Defines.
  * @{
  */

/* USER CODE BEGIN EXPORTED_DEFINES */

/* USER CODE END EXPORTED_DEFINES */

/**
  * @}
  */

/** @defgroup USBD_CUSTOM_HID_Exported_Types USBD_CUSTOM_HID_Exported_Types
  * @brief Types.
  * @{
  */

/* USER CODE BEGIN EXPORTED_TYPES */

/* USER CODE END EXPORTED_TYPES */

/**
  * @}
  */

/** @defgroup USBD_CUSTOM_HID_Exported_Macros USBD_CUSTOM_HID_Exported_Macros
  * @brief Aliases.
  * @{
  */

/* USER CODE BEGIN EXPORTED_MACRO */

/* USER CODE END EXPORTED_MACRO */

/**
  * @}
  */

/** @defgroup USBD_CUSTOM_HID_Exported_Variables USBD_CUSTOM_HID_Exported_Variables
  * @brief Public variables.
  * @{
  */

/** CUSTOMHID Interface callback. */
extern USBD_CUSTOM_HID_ItfTypeDef USBD_CustomHID_fops_FS;

/* USER CODE BEGIN EXPORTED_VARIABLES */
extern USBD_HandleTypeDef hUsbDeviceFS;
extern USBD_HandleTypeDef hUsbDeviceHS;

/* USER CODE END EXPORTED_VARIABLES */

/**
  * @}
  */

/** @defgroup USBD_CUSTOM_HID_Exported_FunctionsPrototype USBD_CUSTOM_HID_Exported_FunctionsPrototype
  * @brief Public functions declaration.
  * @{
  */

/* USER CODE BEGIN EXPORTED_FUNCTIONS */

/* USER CODE END EXPORTED_FUNCTIONS */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __USBD_CUSTOM_HID_IF_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
