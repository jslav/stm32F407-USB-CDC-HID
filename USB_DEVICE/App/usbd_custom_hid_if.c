/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : usbd_custom_hid_if.c
  * @version        : v1.0_Cube
  * @brief          : USB Device Custom HID interface file.
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


/* USER CODE BEGIN INCLUDE */
/* Includes ------------------------------------------------------------------*/
#include "usbd_custom_hid_if.h"
#include "composite_internal.h"

/* USER CODE END INCLUDE */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
  * @brief Usb device.
  * @{
  */

/** @addtogroup USBD_CUSTOM_HID
  * @{
  */

/** @defgroup USBD_CUSTOM_HID_Private_TypesDefinitions USBD_CUSTOM_HID_Private_TypesDefinitions
  * @brief Private types.
  * @{
  */

/* USER CODE BEGIN PRIVATE_TYPES */

/* USER CODE END PRIVATE_TYPES */

/**
  * @}
  */

/** @defgroup USBD_CUSTOM_HID_Private_Defines USBD_CUSTOM_HID_Private_Defines
  * @brief Private defines.
  * @{
  */

/* USER CODE BEGIN PRIVATE_DEFINES */

/* USER CODE END PRIVATE_DEFINES */

/**
  * @}
  */

/** @defgroup USBD_CUSTOM_HID_Private_Macros USBD_CUSTOM_HID_Private_Macros
  * @brief Private macros.
  * @{
  */

/* USER CODE BEGIN PRIVATE_MACRO */

/* USER CODE END PRIVATE_MACRO */

/**
  * @}
  */

/** @defgroup USBD_CUSTOM_HID_Private_Variables USBD_CUSTOM_HID_Private_Variables
  * @brief Private variables.
  * @{
  */

/** Usb HID report descriptor. */
__ALIGN_BEGIN static uint8_t CUSTOM_HID_ReportDesc_FS[USBD_CUSTOM_HID_REPORT_DESC_SIZE] __ALIGN_END =
{
  /* USER CODE BEGIN 0 */
	    0x05, 0x0A, // USAGE_PAGE (Ordinal)
	    0x09, 0x01, // USAGE (0)
	    0xa1, 0x01, // COLLECTION (Application)
	    0x09, 0x01, //     USAGE(01)
	    0x15, 0x00, //     LOGICAL_MINIMUM (0)
	    0x25, 0xff, //     LOGICAL_MAXIMUM (255)
	    0x95, 0x3f, //     REPORT_COUNT (63) 63 bytes
	    0x75, 0x08, //     REPORT_SIZE (8) 8 bits
	    0x81, 0x02, //     INPUT (Data,Var,Abs)
	    0x09, 0x02, //     USAGE(02)
	    0x91, 0x02, //     OUTPUT (Data,Var,Abs)
  /* USER CODE END 0 */
  0xC0    /*     END_COLLECTION	             */
};

/* USER CODE BEGIN PRIVATE_VARIABLES */

/* USER CODE END PRIVATE_VARIABLES */

/**
  * @}
  */

/** @defgroup USBD_CUSTOM_HID_Exported_Variables USBD_CUSTOM_HID_Exported_Variables
  * @brief Public variables.
  * @{
  */
extern USBD_HandleTypeDef hUsbDeviceFS;

/* USER CODE BEGIN EXPORTED_VARIABLES */

/* USER CODE END EXPORTED_VARIABLES */
/**
  * @}
  */

/** @defgroup USBD_CUSTOM_HID_Private_FunctionPrototypes USBD_CUSTOM_HID_Private_FunctionPrototypes
  * @brief Private functions declaration.
  * @{
  */

static int8_t CUSTOM_HID_Init_FS(void);
static int8_t CUSTOM_HID_DeInit_FS(void);
static int8_t CUSTOM_HID_OutEvent_FS(uint8_t event_idx, uint8_t state);

/**
  * @}
  */
/*
USBD_CUSTOM_HID_ItfTypeDef USBD_CustomHID_fops_FS =
{
  CUSTOM_HID_ReportDesc_FS,
  CUSTOM_HID_Init_FS,
  CUSTOM_HID_DeInit_FS,
  CUSTOM_HID_OutEvent_FS
};
*/
/** @defgroup USBD_CUSTOM_HID_Private_Functions USBD_CUSTOM_HID_Private_Functions
  * @brief Private functions.
  * @{
  */

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initializes the CUSTOM HID media low layer
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CUSTOM_HID_Init_FS(void)
{
  /* USER CODE BEGIN 4 */
  return (USBD_OK);
  /* USER CODE END 4 */
}

/**
  * @brief  DeInitializes the CUSTOM HID media low layer
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CUSTOM_HID_DeInit_FS(void)
{
  /* USER CODE BEGIN 5 */
  return (USBD_OK);
  /* USER CODE END 5 */
}

/**
  * @brief  Manage the CUSTOM HID class events
  * @param  event_idx: Event index
  * @param  state: Event state
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CUSTOM_HID_OutEvent_FS(uint8_t event_idx, uint8_t state)
{
  /* USER CODE BEGIN 6 */
  UNUSED(event_idx);
  UNUSED(state);

  /* Start next USB packet transfer once data processing is completed */
  USBD_CUSTOM_HID_ReceivePacket(&hUsbDeviceFS);

  return (USBD_OK);
  /* USER CODE END 6 */
}

/* USER CODE BEGIN 7 */
/**
  * @brief  Send the report to the Host
  * @param  report: The report to be sent
  * @param  len: The report length
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
/*
static int8_t USBD_CUSTOM_HID_SendReport_FS(uint8_t *report, uint16_t len)
{
  return USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, report, len);
}
*/
/* USER CODE END 7 */

/* USER CODE BEGIN PRIVATE_FUNCTIONS_IMPLEMENTATION */

uint8_t USBD_CUSTOM_HID_Init(USBD_HandleTypeDef *pdev, uint8_t cfgidx)
{
  UNUSED(cfgidx);

  if (pdev->pClassData == NULL)
  {
    return (uint8_t)USBD_EMEM;
  }

  USBD_CUSTOM_HID_HandleTypeDef *hhid = &((struct composite_CDC_HID_Handle*) pdev->pClassData)->chid;
//  hhid->IF=&USBD_CustomHID_fops_FS;

  if (pdev->dev_speed == USBD_SPEED_HIGH)
  {
    pdev->ep_in[CUSTOM_HID_EPIN_ADDR & 0xFU].bInterval = CUSTOM_HID_HS_BINTERVAL;
    pdev->ep_out[CUSTOM_HID_EPOUT_ADDR & 0xFU].bInterval = CUSTOM_HID_HS_BINTERVAL;
  }
  else   /* LOW and FULL-speed endpoints */
  {
    pdev->ep_in[CUSTOM_HID_EPIN_ADDR & 0xFU].bInterval = CUSTOM_HID_FS_BINTERVAL;
    pdev->ep_out[CUSTOM_HID_EPOUT_ADDR & 0xFU].bInterval = CUSTOM_HID_FS_BINTERVAL;
  }

  /* Open EP IN */
  (void)USBD_LL_OpenEP(pdev, CUSTOM_HID_EPIN_ADDR, USBD_EP_TYPE_INTR, CUSTOM_HID_EPIN_SIZE);

  pdev->ep_in[CUSTOM_HID_EPIN_ADDR & 0xFU].is_used = 1U;

  /* Open EP OUT */
  (void)USBD_LL_OpenEP(pdev, CUSTOM_HID_EPOUT_ADDR, USBD_EP_TYPE_INTR, CUSTOM_HID_EPOUT_SIZE);

  pdev->ep_out[CUSTOM_HID_EPOUT_ADDR & 0xFU].is_used = 1U;

  hhid->state = CUSTOM_HID_IDLE;

  CUSTOM_HID_Init_FS();

  /* Prepare Out endpoint to receive 1st packet */
  (void)USBD_LL_PrepareReceive(pdev, CUSTOM_HID_EPOUT_ADDR, hhid->Report_buf, USBD_CUSTOMHID_OUTREPORT_BUF_SIZE);

  return (uint8_t)USBD_OK;
}

uint8_t USBD_CUSTOM_HID_DeInit(USBD_HandleTypeDef *pdev, uint8_t cfgidx)
{
  UNUSED(cfgidx);

  /* Close CUSTOM_HID EP IN */
  (void)USBD_LL_CloseEP(pdev, CUSTOM_HID_EPIN_ADDR);
  pdev->ep_in[CUSTOM_HID_EPIN_ADDR & 0xFU].is_used = 0U;
  pdev->ep_in[CUSTOM_HID_EPIN_ADDR & 0xFU].bInterval = 0U;

  /* Close CUSTOM_HID EP OUT */
  (void)USBD_LL_CloseEP(pdev, CUSTOM_HID_EPOUT_ADDR);
  pdev->ep_out[CUSTOM_HID_EPOUT_ADDR & 0xFU].is_used = 0U;
  pdev->ep_out[CUSTOM_HID_EPOUT_ADDR & 0xFU].bInterval = 0U;

  /* FRee allocated memory */
  CUSTOM_HID_DeInit_FS();

  return (uint8_t)USBD_OK;
}

__ALIGN_BEGIN static uint8_t USBD_CUSTOM_HID_Desc[USB_CUSTOM_HID_DESC_SIZ] __ALIGN_END =
{
  /* 18 */
  0x09,                                               /* bLength: CUSTOM_HID Descriptor size */
  CUSTOM_HID_DESCRIPTOR_TYPE,                         /* bDescriptorType: CUSTOM_HID */
  0x11,                                               /* bCUSTOM_HIDUSTOM_HID: CUSTOM_HID Class Spec release number */
  0x01,
  0x00,                                               /* bCountryCode: Hardware target country */
  0x01,                                               /* bNumDescriptors: Number of CUSTOM_HID class descriptors to follow */
  0x22,                                               /* bDescriptorType */
  USBD_CUSTOM_HID_REPORT_DESC_SIZE,                   /* wItemLength: Total length of Report descriptor */
  0x00,
};

uint8_t USBD_CUSTOM_HID_Setup(USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req)
{
  USBD_CUSTOM_HID_HandleTypeDef *hhid = &((struct composite_CDC_HID_Handle*) pdev->pClassData)->chid;

  uint16_t len = 0U;
  uint8_t  *pbuf = NULL;
  uint16_t status_info = 0U;
  USBD_StatusTypeDef ret = USBD_OK;

  switch (req->bmRequest & USB_REQ_TYPE_MASK)
  {
  case USB_REQ_TYPE_CLASS:
    switch (req->bRequest)
    {
    case CUSTOM_HID_REQ_SET_PROTOCOL:
      hhid->Protocol = (uint8_t)(req->wValue);
      break;

    case CUSTOM_HID_REQ_GET_PROTOCOL:
      (void)USBD_CtlSendData(pdev, (uint8_t *)&hhid->Protocol, 1U);
      break;

    case CUSTOM_HID_REQ_SET_IDLE:
      hhid->IdleState = (uint8_t)(req->wValue >> 8);
      break;

    case CUSTOM_HID_REQ_GET_IDLE:
      (void)USBD_CtlSendData(pdev, (uint8_t *)&hhid->IdleState, 1U);
      break;

    case CUSTOM_HID_REQ_SET_REPORT:
      hhid->IsReportAvailable = 1U;
      (void)USBD_CtlPrepareRx(pdev, hhid->Report_buf, req->wLength);
      break;

    default:
      USBD_CtlError(pdev, req);
      ret = USBD_FAIL;
      break;
    }
    break;

  case USB_REQ_TYPE_STANDARD:
    switch (req->bRequest)
    {
    case USB_REQ_GET_STATUS:
      if (pdev->dev_state == USBD_STATE_CONFIGURED)
      {
        (void)USBD_CtlSendData(pdev, (uint8_t *)&status_info, 2U);
      }
      else
      {
        USBD_CtlError(pdev, req);
        ret = USBD_FAIL;
      }
      break;

    case USB_REQ_GET_DESCRIPTOR:
      if ((req->wValue >> 8) == CUSTOM_HID_REPORT_DESC)
      {
        len = MIN(USBD_CUSTOM_HID_REPORT_DESC_SIZE, req->wLength);
        pbuf = &CUSTOM_HID_ReportDesc_FS[0];
      }
      else
      {
        if ((req->wValue >> 8) == CUSTOM_HID_DESCRIPTOR_TYPE)
        {
          pbuf = USBD_CUSTOM_HID_Desc;
          len = MIN(USB_CUSTOM_HID_DESC_SIZ, req->wLength);
        }
      }

      (void)USBD_CtlSendData(pdev, pbuf, len);
      break;

    case USB_REQ_GET_INTERFACE:
      if (pdev->dev_state == USBD_STATE_CONFIGURED)
      {
        (void)USBD_CtlSendData(pdev, (uint8_t *)&hhid->AltSetting, 1U);
      }
      else
      {
        USBD_CtlError(pdev, req);
        ret = USBD_FAIL;
      }
      break;

    case USB_REQ_SET_INTERFACE:
      if (pdev->dev_state == USBD_STATE_CONFIGURED)
      {
        hhid->AltSetting = (uint8_t)(req->wValue);
      }
      else
      {
        USBD_CtlError(pdev, req);
        ret = USBD_FAIL;
      }
      break;

    case USB_REQ_CLEAR_FEATURE:
      break;

    default:
      USBD_CtlError(pdev, req);
      ret = USBD_FAIL;
      break;
    }
    break;

  default:
    USBD_CtlError(pdev, req);
    ret = USBD_FAIL;
    break;
  }
  return (uint8_t)ret;
}


uint8_t USBD_CUSTOM_HID_EP0_RxReady(USBD_HandleTypeDef *pdev)
{
  USBD_CUSTOM_HID_HandleTypeDef *hhid = &((struct composite_CDC_HID_Handle*) pdev->pClassData)->chid;

  if (hhid->IsReportAvailable == 1U)
  {
//    hhid->IF->OutEvent(hhid->Report_buf[0], hhid->Report_buf[1]);
    /* Resume USB Out process */
    (void)USBD_LL_PrepareReceive(pdev, CUSTOM_HID_EPOUT_ADDR, hhid->Report_buf, USBD_CUSTOMHID_OUTREPORT_BUF_SIZE);
    hhid->IsReportAvailable = 0U;
    return (uint8_t)USBD_BUSY;
  }

  return (uint8_t)USBD_OK;
}

uint8_t USBD_CUSTOM_HID_DataIn(USBD_HandleTypeDef *pdev, uint8_t epnum)
{
  UNUSED(epnum);

  /* Ensure that the FIFO is empty before a new transfer, this condition could
  be caused by  a new transfer before the end of the previous transfer */
  ((struct composite_CDC_HID_Handle*) pdev->pClassData)->chid.state = CUSTOM_HID_IDLE;

  return (uint8_t)USBD_OK;
}

uint8_t USBD_CUSTOM_HID_DataOut(USBD_HandleTypeDef *pdev, uint8_t epnum)
{
  UNUSED(epnum);

  if (pdev->pClassData == NULL)
  {
    return (uint8_t)USBD_FAIL;
  }

  USBD_CUSTOM_HID_HandleTypeDef *hhid = &((struct composite_CDC_HID_Handle*) pdev->pClassData)->chid;

  /* USB data will be immediately processed, this allow next USB traffic being
  NAKed till the end of the application processing */
  CUSTOM_HID_OutEvent_FS(hhid->Report_buf[0], hhid->Report_buf[1]);

  return (uint8_t)USBD_OK;
}

uint8_t USBD_CUSTOM_HID_SendReport(USBD_HandleTypeDef *pdev,  uint8_t *report, uint16_t len)
{

  if (pdev->pClassData == NULL)
  {
    return (uint8_t)USBD_FAIL;
  }

  USBD_CUSTOM_HID_HandleTypeDef *hhid = &((struct composite_CDC_HID_Handle*) pdev->pClassData)->chid;

  if (pdev->dev_state == USBD_STATE_CONFIGURED)
  {
    if (hhid->state == CUSTOM_HID_IDLE)
    {
      hhid->state = CUSTOM_HID_BUSY;
      (void)USBD_LL_Transmit(pdev, CUSTOM_HID_EPIN_ADDR, report, len);
    }
    else
    {
      return (uint8_t)USBD_BUSY;
    }
  }
  return (uint8_t)USBD_OK;
}

uint8_t USBD_CUSTOM_HID_ReceivePacket(USBD_HandleTypeDef *pdev)
{

  if (pdev->pClassData == NULL)
  {
    return (uint8_t)USBD_FAIL;
  }

  USBD_CUSTOM_HID_HandleTypeDef *hhid = &((struct composite_CDC_HID_Handle*) pdev->pClassData)->chid;

  /* Resume USB Out process */
  (void)USBD_LL_PrepareReceive(pdev, CUSTOM_HID_EPOUT_ADDR, hhid->Report_buf,
                               USBD_CUSTOMHID_OUTREPORT_BUF_SIZE);

  return (uint8_t)USBD_OK;
}

