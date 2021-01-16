/**
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                      www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include <composite_cdc_hid.h>
#include "usbd_cdc_if.h"
#include "usbd_custom_hid_if.h"
#include "usbd_ctlreq.h"

#include "composite_internal.h"


#define IFC //static
IFC int8_t CDC_Init_FS(void);
IFC int8_t CDC_DeInit_FS(void);
IFC int8_t CDC_Control_FS(uint8_t cmd, uint8_t* pbuf, uint16_t length);
IFC int8_t CDC_Receive_FS(uint8_t* pbuf, uint32_t *Len);
IFC int8_t CDC_TransmitCplt_FS(uint8_t *pbuf, uint32_t *Len, uint8_t epnum);

/* USB Standard Device Descriptor */
__ALIGN_BEGIN static uint8_t COMPOSITE_CDC_HID_DeviceQualifierDesc[USB_LEN_DEV_QUALIFIER_DESC] __ALIGN_END =
{
  USB_LEN_DEV_QUALIFIER_DESC,
  USB_DESC_TYPE_DEVICE_QUALIFIER,
  0x00,
  0x02,
  0x00,
  0x00,
  0x00,
  0x40,
  0x01,
  0x00,
};




#define CUSTOM_HID_EPIN_ADDR                         0x81U
#define CUSTOM_HID_EPOUT_ADDR                        0x01U
#define CDC_IN_EP                                   0x82U  /* EP1 for data IN */
#define CDC_OUT_EP                                  0x02U  /* EP1 for data OUT */
#define CDC_CMD_EP                                  0x83U  /* EP2 for CDC commands */

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

#define HID_INTERFACE_IDX 0
#define CDC_INTERFACE_IDX 1


//#define CDC_IN_EP                                   0x82U  /* EP1 for data IN */
//#define CDC_OUT_EP                                  0x02U  /* EP1 for data OUT */
//#define CDC_CMD_EP                                  0x83U  /* EP2 for CDC commands */

#ifndef CDC_HS_BINTERVAL
#define CDC_HS_BINTERVAL                            0x10U
#endif /* CDC_HS_BINTERVAL */

#ifndef CDC_FS_BINTERVAL
#define CDC_FS_BINTERVAL                            0x10U
#endif /* CDC_FS_BINTERVAL */

/* CDC Endpoints parameters: you can fine tune these values depending on the needed baudrates and performance. */
#define CDC_DATA_HS_MAX_PACKET_SIZE                 512U  /* Endpoint IN & OUT Packet size */
#define CDC_DATA_FS_MAX_PACKET_SIZE                 64U  /* Endpoint IN & OUT Packet size */
#define CDC_CMD_PACKET_SIZE                         8U  /* Control Endpoint Packet size */

#define USB_CDC_CONFIG_DESC_SIZ                     67U
#define CDC_DATA_HS_IN_PACKET_SIZE                  CDC_DATA_HS_MAX_PACKET_SIZE
#define CDC_DATA_HS_OUT_PACKET_SIZE                 CDC_DATA_HS_MAX_PACKET_SIZE

#define CDC_DATA_FS_IN_PACKET_SIZE                  CDC_DATA_FS_MAX_PACKET_SIZE
#define CDC_DATA_FS_OUT_PACKET_SIZE                 CDC_DATA_FS_MAX_PACKET_SIZE


#define USB_COMPOSITE_CONFIG_DESC_SIZ       107U

__ALIGN_BEGIN static uint8_t COMPOSITE_CDC_HID_CfgFSDesc[USB_COMPOSITE_CONFIG_DESC_SIZ] __ALIGN_END =
{
  0x09,                                               /* bLength: Configuration Descriptor size */
  USB_DESC_TYPE_CONFIGURATION,                        /* bDescriptorType: Configuration */
  USB_COMPOSITE_CONFIG_DESC_SIZ,
                                                      /* wTotalLength: Bytes returned */
  0x00,
  0x03,                                               /* bNumInterfaces: 1 interface */
  0x01,                                               /* bConfigurationValue: Configuration value */
  0x00,                                               /* iConfiguration: Index of string descriptor describing the configuration */
  0xC0,                                               /* bmAttributes: bus powered */
  0x32,                                               /* MaxPower 100 mA: this current is used for detecting Vbus */

  /************** Descriptor of CUSTOM HID interface ****************/
  /* 09 */
  0x09,                                               /* bLength: Interface Descriptor size*/
  USB_DESC_TYPE_INTERFACE,                            /* bDescriptorType: Interface descriptor type */
  HID_INTERFACE_IDX,                                               /* bInterfaceNumber: Number of Interface */
  0x00,                                               /* bAlternateSetting: Alternate setting */
  0x02,                                               /* bNumEndpoints*/
  0x03,                                               /* bInterfaceClass: CUSTOM_HID */
  0x00,                                               /* bInterfaceSubClass : 1=BOOT, 0=no boot */
  0x00,                                               /* nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse */
  0x00,                                               /* iInterface: Index of string descriptor */
  /******************** Descriptor of CUSTOM_HID *************************/
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
  /******************** Descriptor of Custom HID endpoints ********************/
  /* 27 */
  0x07,                                               /* bLength: Endpoint Descriptor size */
  USB_DESC_TYPE_ENDPOINT,                             /* bDescriptorType: */

  CUSTOM_HID_EPIN_ADDR,                               /* bEndpointAddress: Endpoint Address (IN) */
  0x03,                                               /* bmAttributes: Interrupt endpoint */
  CUSTOM_HID_EPIN_SIZE,                               /* wMaxPacketSize: 2 Byte max */
  0x00,
  CUSTOM_HID_FS_BINTERVAL,                            /* bInterval: Polling Interval */
  /* 34 */

  0x07,                                               /* bLength: Endpoint Descriptor size */
  USB_DESC_TYPE_ENDPOINT,                             /* bDescriptorType: */
  CUSTOM_HID_EPOUT_ADDR,                              /* bEndpointAddress: Endpoint Address (OUT) */
  0x03,                                               /* bmAttributes: Interrupt endpoint */
  CUSTOM_HID_EPOUT_SIZE,                              /* wMaxPacketSize: 2 Bytes max  */
  0x00,
  CUSTOM_HID_FS_BINTERVAL,                            /* bInterval: Polling Interval */
  /* 41 */

	/******** IAD should be positioned just before the CDC interfaces ******
			 IAD to associate the two CDC interfaces */

	0x08, /* bLength */
	0x0B, /* bDescriptorType */
	CDC_INTERFACE_IDX, /* bFirstInterface */
	0x02, /* bInterfaceCount */
	0x02, /* bFunctionClass */
	0x02, /* bFunctionSubClass */
	0x01, /* bFunctionProtocol */
	0x00, /* iFunction (Index of string descriptor describing this function) */
	/* 08 bytes */

	/********************  CDC interfaces ********************/

	/*Interface Descriptor */
	0x09,   /* bLength: Interface Descriptor size */
	USB_DESC_TYPE_INTERFACE,  /* bDescriptorType: Interface */
	                          /* Interface descriptor type */
	CDC_INTERFACE_IDX,   /* bInterfaceNumber: Number of Interface */
	0x00,   /* bAlternateSetting: Alternate setting */
	0x01,   /* bNumEndpoints: One endpoints used */
	0x02,   /* bInterfaceClass: Communication Interface Class */
	0x02,   /* bInterfaceSubClass: Abstract Control Model */
	0x01,   /* bInterfaceProtocol: Common AT commands */
	0x01,   /* iInterface: */
	/* 09 bytes */

	/*Header Functional Descriptor*/
	0x05,   /* bLength: Endpoint Descriptor size */
	0x24,   /* bDescriptorType: CS_INTERFACE */
	0x00,   /* bDescriptorSubtype: Header Func Desc */
	0x10,   /* bcdCDC: spec release number */
	0x01,
	/* 05 bytes */

	/*Call Management Functional Descriptor*/
	0x05,   /* bFunctionLength */
	0x24,   /* bDescriptorType: CS_INTERFACE */
	0x01,   /* bDescriptorSubtype: Call Management Func Desc */
	0x00,   /* bmCapabilities: D0+D1 */
	CDC_INTERFACE_IDX + 1,   /* bDataInterface: 2 */
	/* 05 bytes */

	/*ACM Functional Descriptor*/
	0x04,   /* bFunctionLength */
	0x24,   /* bDescriptorType: CS_INTERFACE */
	0x02,   /* bDescriptorSubtype: Abstract Control Management desc */
	0x02,   /* bmCapabilities */
	/* 04 bytes */

	/*Union Functional Descriptor*/
	0x05,   /* bFunctionLength */
	0x24,   /* bDescriptorType: CS_INTERFACE */
	0x06,   /* bDescriptorSubtype: Union func desc */
	CDC_INTERFACE_IDX,   /* bMasterInterface: Communication class interface */
	CDC_INTERFACE_IDX + 1,   /* bSlaveInterface0: Data Class Interface */
	/* 05 bytes */

	/*Endpoint 2 Descriptor*/
	0x07,                          /* bLength: Endpoint Descriptor size */
	USB_DESC_TYPE_ENDPOINT,        /* bDescriptorType: Endpoint */
	CDC_CMD_EP,                    /* bEndpointAddress */
	0x03,                          /* bmAttributes: Interrupt */
	LOBYTE(CDC_CMD_PACKET_SIZE),   /* wMaxPacketSize: */
	HIBYTE(CDC_CMD_PACKET_SIZE),
	0x10,                          /* bInterval: */
	/* 07 bytes */

	/*Data class interface descriptor*/
	0x09,   /* bLength: Endpoint Descriptor size */
	USB_DESC_TYPE_INTERFACE,       /* bDescriptorType: */
	CDC_INTERFACE_IDX + 1,         /* bInterfaceNumber: Number of Interface */
	0x00,                          /* bAlternateSetting: Alternate setting */
	0x02,                          /* bNumEndpoints: Two endpoints used */
	0x0A,                          /* bInterfaceClass: CDC */
	0x00,                          /* bInterfaceSubClass: */
	0x00,                          /* bInterfaceProtocol: */
	0x00,                          /* iInterface: */
	/* 09 bytes */

	/*Endpoint OUT Descriptor*/
	0x07,   /* bLength: Endpoint Descriptor size */
	USB_DESC_TYPE_ENDPOINT,        /* bDescriptorType: Endpoint */
	CDC_OUT_EP,                    /* bEndpointAddress */
	0x02,                          /* bmAttributes: Bulk */
	LOBYTE(CDC_DATA_FS_MAX_PACKET_SIZE),  /* wMaxPacketSize: */
	HIBYTE(CDC_DATA_FS_MAX_PACKET_SIZE),
	0x00,                          /* bInterval: ignore for Bulk transfer */
	/* 07 bytes */

	/*Endpoint IN Descriptor*/
	0x07,   /* bLength: Endpoint Descriptor size */
	USB_DESC_TYPE_ENDPOINT,        /* bDescriptorType: Endpoint */
	CDC_IN_EP,                     /* bEndpointAddress */
	0x02,                          /* bmAttributes: Bulk */
	LOBYTE(CDC_DATA_FS_MAX_PACKET_SIZE),  /* wMaxPacketSize: */
	HIBYTE(CDC_DATA_FS_MAX_PACKET_SIZE),
	0x00,                          /* bInterval */
	/* 07 bytes */
};

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

/**
  * @brief  USBD_CDC_Init
  *         Initialize the CDC interface
  * @param  pdev: device instance
  * @param  cfgidx: Configuration index
  * @retval status
  */
static uint8_t USBD_CDC_Init(USBD_HandleTypeDef *pdev, uint8_t cfgidx)
{
  UNUSED(cfgidx);

  if (pdev->pClassData == NULL)
  {
    return (uint8_t)USBD_EMEM;
  }

  USBD_CDC_HandleTypeDef *hcdc = &((struct composite_CDC_HID_Handle*) pdev->pClassData)->cdc;

  if (pdev->dev_speed == USBD_SPEED_HIGH)
  {
    /* Open EP IN */
    (void)USBD_LL_OpenEP(pdev, CDC_IN_EP, USBD_EP_TYPE_BULK,
                         CDC_DATA_HS_IN_PACKET_SIZE);

     pdev->ep_in[CDC_IN_EP & 0xFU].is_used = 1U;

     /* Open EP OUT */
     (void)USBD_LL_OpenEP(pdev, CDC_OUT_EP, USBD_EP_TYPE_BULK,
                          CDC_DATA_HS_OUT_PACKET_SIZE);

      pdev->ep_out[CDC_OUT_EP & 0xFU].is_used = 1U;

      /* Set bInterval for CDC CMD Endpoint */
      pdev->ep_in[CDC_CMD_EP & 0xFU].bInterval = CDC_HS_BINTERVAL;
  }
  else
  {
    /* Open EP IN */
    (void)USBD_LL_OpenEP(pdev, CDC_IN_EP, USBD_EP_TYPE_BULK,
                         CDC_DATA_FS_IN_PACKET_SIZE);

     pdev->ep_in[CDC_IN_EP & 0xFU].is_used = 1U;

     /* Open EP OUT */
     (void)USBD_LL_OpenEP(pdev, CDC_OUT_EP, USBD_EP_TYPE_BULK,
                          CDC_DATA_FS_OUT_PACKET_SIZE);

      pdev->ep_out[CDC_OUT_EP & 0xFU].is_used = 1U;

      /* Set bInterval for CMD Endpoint */
      pdev->ep_in[CDC_CMD_EP & 0xFU].bInterval = CDC_FS_BINTERVAL;
  }

  /* Open Command IN EP */
  (void)USBD_LL_OpenEP(pdev, CDC_CMD_EP, USBD_EP_TYPE_INTR, CDC_CMD_PACKET_SIZE);
  pdev->ep_in[CDC_CMD_EP & 0xFU].is_used = 1U;

  /* Init  physical Interface components */
  CDC_Init_FS(); //((USBD_CDC_ItfTypeDef *)pdev->pUserData)->Init();

  /* Init Xfer states */
  hcdc->TxState = 0U;
  hcdc->RxState = 0U;

  if (pdev->dev_speed == USBD_SPEED_HIGH)
  {
    /* Prepare Out endpoint to receive next packet */
    (void)USBD_LL_PrepareReceive(pdev, CDC_OUT_EP, hcdc->RxBuffer,
                                 CDC_DATA_HS_OUT_PACKET_SIZE);
  }
  else
  {
    /* Prepare Out endpoint to receive next packet */
    (void)USBD_LL_PrepareReceive(pdev, CDC_OUT_EP, hcdc->RxBuffer,
                                 CDC_DATA_FS_OUT_PACKET_SIZE);
  }

  return (uint8_t)USBD_OK;
}


static uint8_t COMPOSITE_CDC_HID_Init(USBD_HandleTypeDef *pdev, uint8_t cfgidx)
{
	struct composite_HID_VCP_Handle *pCtxt;

	pCtxt = USBD_malloc(sizeof(struct composite_CDC_HID_Handle));
	if (pCtxt == NULL)
	{
	    pdev->pClassData = NULL;
	    return (uint8_t)USBD_EMEM;
	}

	pdev->pClassData = (void *)pCtxt;
	return USBD_CDC_Init(pdev,cfgidx);

//	USBD_CDC_Init(pdev,cfgidx);
//	USBD_CUSTOM_HID_Init(pdev,cfgidx);
//	return USBD_OK;

}

/**
  * @brief  USBD_CDC_Init
  *         DeInitialize the CDC layer
  * @param  pdev: device instance
  * @param  cfgidx: Configuration index
  * @retval status
  */
static uint8_t USBD_CDC_DeInit(USBD_HandleTypeDef *pdev, uint8_t cfgidx)
{
  UNUSED(cfgidx);
  uint8_t ret = 0U;

  /* Close EP IN */
  (void)USBD_LL_CloseEP(pdev, CDC_IN_EP);
  pdev->ep_in[CDC_IN_EP & 0xFU].is_used = 0U;

  /* Close EP OUT */
  (void)USBD_LL_CloseEP(pdev, CDC_OUT_EP);
  pdev->ep_out[CDC_OUT_EP & 0xFU].is_used = 0U;

  /* Close Command IN EP */
  (void)USBD_LL_CloseEP(pdev, CDC_CMD_EP);
  pdev->ep_in[CDC_CMD_EP & 0xFU].is_used = 0U;
  pdev->ep_in[CDC_CMD_EP & 0xFU].bInterval = 0U;

  /* DeInit  physical Interface components */
  CDC_DeInit_FS();//((USBD_CDC_ItfTypeDef *)pdev->pUserData)->DeInit();

  return ret;
}

static uint8_t COMPOSITE_CDC_HID_DeInit(USBD_HandleTypeDef *pdev, uint8_t cfgidx)
{
//	USBD_CUSTOM_HID_DeInit(pdev,cfgidx);
	USBD_CDC_DeInit(pdev,cfgidx);
	USBD_free(pdev->pClassData);
    pdev->pClassData = NULL;
	return USBD_OK;
}
/**
  * @brief  USBD_CDC_Setup
  *         Handle the CDC specific requests
  * @param  pdev: instance
  * @param  req: usb requests
  * @retval status
  */
static uint8_t USBD_CDC_Setup(USBD_HandleTypeDef *pdev,
                              USBD_SetupReqTypedef *req)
{
  USBD_CDC_HandleTypeDef *hcdc = &((struct composite_CDC_HID_Handle*) pdev->pClassData)->cdc;
  uint8_t ifalt = 0U;
  uint16_t status_info = 0U;
  USBD_StatusTypeDef ret = USBD_OK;

  switch (req->bmRequest & USB_REQ_TYPE_MASK)
  {
  case USB_REQ_TYPE_CLASS:
    if (req->wLength != 0U)
    {
      if ((req->bmRequest & 0x80U) != 0U)
      {
    	  CDC_Control_FS/*((USBD_CDC_ItfTypeDef *)pdev->pUserData)->Control*/(req->bRequest, (uint8_t *)hcdc->data, req->wLength);

          (void)USBD_CtlSendData(pdev, (uint8_t *)hcdc->data, req->wLength);
      }
      else
      {
        hcdc->CmdOpCode = req->bRequest;
        hcdc->CmdLength = (uint8_t)req->wLength;

        (void)USBD_CtlPrepareRx(pdev, (uint8_t *)hcdc->data, req->wLength);
      }
    }
    else
    {
    	CDC_Control_FS/*((USBD_CDC_ItfTypeDef *)pdev->pUserData)->Control*/(req->bRequest, (uint8_t *)req, 0U);
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

    case USB_REQ_GET_INTERFACE:
      if (pdev->dev_state == USBD_STATE_CONFIGURED)
      {
        (void)USBD_CtlSendData(pdev, &ifalt, 1U);
      }
      else
      {
        USBD_CtlError(pdev, req);
        ret = USBD_FAIL;
      }
      break;

    case USB_REQ_SET_INTERFACE:
      if (pdev->dev_state != USBD_STATE_CONFIGURED)
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

static uint8_t COMPOSITE_CDC_HID_Setup(USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req)
{
	return USBD_CDC_Setup(pdev, req);
}

/**
  * @brief  USBD_CDC_DataIn
  *         Data sent on non-control IN endpoint
  * @param  pdev: device instance
  * @param  epnum: endpoint number
  * @retval status
  */
static uint8_t USBD_CDC_DataIn(USBD_HandleTypeDef *pdev, uint8_t epnum)
{
  PCD_HandleTypeDef *hpcd = pdev->pData;

  if (pdev->pClassData == NULL)
  {
    return (uint8_t)USBD_FAIL;
  }

  USBD_CDC_HandleTypeDef *hcdc = &((struct composite_CDC_HID_Handle*) pdev->pClassData)->cdc;

  if ((pdev->ep_in[epnum].total_length > 0U) &&
      ((pdev->ep_in[epnum].total_length % hpcd->IN_ep[epnum].maxpacket) == 0U))
  {
    /* Update the packet total length */
    pdev->ep_in[epnum].total_length = 0U;

    /* Send ZLP */
    (void)USBD_LL_Transmit(pdev, epnum, NULL, 0U);
  }
  else
  {
    hcdc->TxState = 0U;
    CDC_TransmitCplt_FS/*((USBD_CDC_ItfTypeDef *)pdev->pUserData)->TransmitCplt*/(hcdc->TxBuffer, &hcdc->TxLength, epnum);
  }

  return (uint8_t)USBD_OK;
}

static uint8_t COMPOSITE_CDC_HID_DataIn(USBD_HandleTypeDef *pdev, uint8_t epnum)
{
	return USBD_CDC_DataIn(pdev, epnum);
}

/**
  * @brief  USBD_CDC_DataOut
  *         Data received on non-control Out endpoint
  * @param  pdev: device instance
  * @param  epnum: endpoint number
  * @retval status
  */
static uint8_t USBD_CDC_DataOut(USBD_HandleTypeDef *pdev, uint8_t epnum)
{
  if (pdev->pClassData == NULL)
  {
    return (uint8_t)USBD_FAIL;
  }

  USBD_CDC_HandleTypeDef *hcdc = &((struct composite_CDC_HID_Handle*) pdev->pClassData)->cdc;

  /* Get the received data length */
  hcdc->RxLength = USBD_LL_GetRxDataSize(pdev, epnum);

  /* USB data will be immediately processed, this allow next USB traffic being
  NAKed till the end of the application Xfer */

  CDC_Receive_FS/*((USBD_CDC_ItfTypeDef *)pdev->pUserData)->Receive*/(hcdc->RxBuffer, &hcdc->RxLength);

  return (uint8_t)USBD_OK;
}

static uint8_t COMPOSITE_CDC_HID_DataOut(USBD_HandleTypeDef *pdev, uint8_t epnum)
{
	return USBD_CDC_DataOut(pdev, epnum);
}

/**
  * @brief  USBD_CDC_EP0_RxReady
  *         Handle EP0 Rx Ready event
  * @param  pdev: device instance
  * @retval status
  */
static uint8_t USBD_CDC_EP0_RxReady(USBD_HandleTypeDef *pdev)
{
  if (pdev->pClassData == NULL)
  {
    return (uint8_t)USBD_FAIL;
  }

  USBD_CDC_HandleTypeDef *hcdc = &((struct composite_CDC_HID_Handle*) pdev->pClassData)->cdc;

  if (hcdc->CmdOpCode != 0xFFU)
  {
	  CDC_Control_FS/*((USBD_CDC_ItfTypeDef *)pdev->pUserData)->Control*/(hcdc->CmdOpCode, (uint8_t *)hcdc->data,(uint16_t)hcdc->CmdLength);
    hcdc->CmdOpCode = 0xFFU;

  }

  return (uint8_t)USBD_OK;
}

static uint8_t COMPOSITE_CDC_HID_EP0_RxReady(USBD_HandleTypeDef *pdev)
{
	return USBD_CDC_EP0_RxReady(pdev);
}

/**
  * @brief  USBD_CDC_GetFSCfgDesc
  *         Return configuration descriptor
  * @param  speed : current device speed
  * @param  length : pointer data length
  * @retval pointer to descriptor buffer
  */
static uint8_t *COMPOSITE_CDC_HID_GetFSCfgDesc(uint16_t *length)
{
  *length = (uint16_t)sizeof(COMPOSITE_CDC_HID_CfgFSDesc);

  return COMPOSITE_CDC_HID_CfgFSDesc;
}

/**
* @brief  DeviceQualifierDescriptor
*         return Device Qualifier descriptor
* @param  length : pointer data length
* @retval pointer to descriptor buffer
*/
uint8_t *COMPOSITE_CDC_HID_GetDeviceQualifierDescriptor(uint16_t *length)
{
  *length = (uint16_t)sizeof(COMPOSITE_CDC_HID_DeviceQualifierDesc);

  return COMPOSITE_CDC_HID_DeviceQualifierDesc;
}


/**
  * @brief  USBD_CDC_SetTxBuffer
  * @param  pdev: device instance
  * @param  pbuff: Tx Buffer
  * @retval status
  */
uint8_t USBD_CDC_SetTxBuffer(USBD_HandleTypeDef *pdev,
                             uint8_t *pbuff, uint32_t length)
{
  if (pdev->pClassData == NULL)
  {
    return (uint8_t)USBD_FAIL;
  }

  USBD_CDC_HandleTypeDef *hcdc = &((struct composite_CDC_HID_Handle*) pdev->pClassData)->cdc;

  hcdc->TxBuffer = pbuff;
  hcdc->TxLength = length;

  return (uint8_t)USBD_OK;
}


/**
  * @brief  USBD_CDC_SetRxBuffer
  * @param  pdev: device instance
  * @param  pbuff: Rx Buffer
  * @retval status
  */
uint8_t USBD_CDC_SetRxBuffer(USBD_HandleTypeDef *pdev, uint8_t *pbuff)
{
  if (pdev->pClassData == NULL)
  {
    return (uint8_t)USBD_FAIL;
  }

  USBD_CDC_HandleTypeDef *hcdc = &((struct composite_CDC_HID_Handle*) pdev->pClassData)->cdc;

  hcdc->RxBuffer = pbuff;

  return (uint8_t)USBD_OK;
}

/**
  * @brief  USBD_CDC_TransmitPacket
  *         Transmit packet on IN endpoint
  * @param  pdev: device instance
  * @retval status
  */
uint8_t USBD_CDC_TransmitPacket(USBD_HandleTypeDef *pdev)
{
  if (pdev->pClassData == NULL)
  {
    return (uint8_t)USBD_FAIL;
  }

  USBD_CDC_HandleTypeDef *hcdc = &((struct composite_CDC_HID_Handle*) pdev->pClassData)->cdc;
  USBD_StatusTypeDef ret = USBD_BUSY;


  if (hcdc->TxState == 0U)
  {
    /* Tx Transfer in progress */
    hcdc->TxState = 1U;

    /* Update the packet total length */
    pdev->ep_in[CDC_IN_EP & 0xFU].total_length = hcdc->TxLength;

    /* Transmit next packet */
    (void)USBD_LL_Transmit(pdev, CDC_IN_EP, hcdc->TxBuffer, hcdc->TxLength);

    ret = USBD_OK;
  }

  return (uint8_t)ret;
}


/**
  * @brief  USBD_CDC_ReceivePacket
  *         prepare OUT Endpoint for reception
  * @param  pdev: device instance
  * @retval status
  */
uint8_t USBD_CDC_ReceivePacket(USBD_HandleTypeDef *pdev)
{
  if (pdev->pClassData == NULL)
  {
    return (uint8_t)USBD_FAIL;
  }
  USBD_CDC_HandleTypeDef *hcdc = &((struct composite_CDC_HID_Handle*) pdev->pClassData)->cdc;

  if (pdev->dev_speed == USBD_SPEED_HIGH)
  {
    /* Prepare Out endpoint to receive next packet */
    (void)USBD_LL_PrepareReceive(pdev, CDC_OUT_EP, hcdc->RxBuffer,
                                 CDC_DATA_HS_OUT_PACKET_SIZE);
  }
  else
  {
    /* Prepare Out endpoint to receive next packet */
    (void)USBD_LL_PrepareReceive(pdev, CDC_OUT_EP, hcdc->RxBuffer,
                                 CDC_DATA_FS_OUT_PACKET_SIZE);
  }

  return (uint8_t)USBD_OK;
}



/* CDC interface class callbacks structure */
USBD_ClassTypeDef  USBD_COMPOSITE_CDC_HID =
{
  COMPOSITE_CDC_HID_Init,
  COMPOSITE_CDC_HID_DeInit,
  COMPOSITE_CDC_HID_Setup,
  NULL,                 /* EP0_TxSent, */
  COMPOSITE_CDC_HID_EP0_RxReady,
  COMPOSITE_CDC_HID_DataIn,
  COMPOSITE_CDC_HID_DataOut,
  NULL,
  NULL,
  NULL,
  COMPOSITE_CDC_HID_GetFSCfgDesc,//USBD_CDC_GetHSCfgDesc,
  COMPOSITE_CDC_HID_GetFSCfgDesc,//USBD_CDC_GetFSCfgDesc,
  COMPOSITE_CDC_HID_GetFSCfgDesc,//USBD_CDC_GetOtherSpeedCfgDesc,
  COMPOSITE_CDC_HID_GetDeviceQualifierDescriptor,
};
