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



__ALIGN_BEGIN static uint8_t COMPOSITE_CDC_HID_CfgFSDesc[USB_COMPOSITE_CONFIG_DESC_SIZ] __ALIGN_END =
{
  0x09,                                               /* bLength: Configuration Descriptor size */
  USB_DESC_TYPE_CONFIGURATION,                        /* bDescriptorType: Configuration */
  USB_COMPOSITE_CONFIG_DESC_SIZ,
                                                      /* wTotalLength: Bytes returned */
  0x00,
  0x03,                                             /* bNumInterfaces:  */
  0x01,                                               /* bConfigurationValue: Configuration value */
  0x00,                                               /* iConfiguration: Index of string descriptor describing the configuration */
  0xC0,                                               /* bmAttributes: bus powered */
  0x32,                                               /* MaxPower 100 mA: this current is used for detecting Vbus */

  /************** Descriptor of CUSTOM HID interface ****************/
  /* 09 */
  0x09,                                               /* bLength: Interface Descriptor size*/
  USB_DESC_TYPE_INTERFACE,                            /* bDescriptorType: Interface descriptor type */
  HID_INTERFACE_IDX,                                  /* bInterfaceNumber: Number of Interface */
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
	 2 , /* bInterfaceCount */
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
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

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
	USBD_CDC_Init(pdev,cfgidx);
	USBD_CUSTOM_HID_Init(pdev,cfgidx);
	return USBD_OK;
}

static uint8_t COMPOSITE_CDC_HID_DeInit(USBD_HandleTypeDef *pdev, uint8_t cfgidx)
{
	USBD_CUSTOM_HID_DeInit(pdev,cfgidx);
	USBD_CDC_DeInit(pdev,cfgidx);
	USBD_free(pdev->pClassData);
    pdev->pClassData = NULL;
	return USBD_OK;
}

static uint8_t COMPOSITE_CDC_HID_Setup(USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req)
{
	if (req->wIndex == HID_INTERFACE_IDX)
		return (USBD_CUSTOM_HID_Setup (pdev, req));
	else
		return (USBD_CDC_Setup(pdev, req));
}

static uint8_t COMPOSITE_CDC_HID_DataIn(USBD_HandleTypeDef *pdev, uint8_t epnum)
{
	/*DataIN can be for CDC or HID */
	if (epnum == (CUSTOM_HID_EPIN_ADDR&~0x80))
		return (USBD_CUSTOM_HID_DataIn(pdev, epnum));
	else
		return (USBD_CDC_DataIn(pdev, epnum));
//	return USBD_CDC_DataIn(pdev, epnum);
}


static uint8_t COMPOSITE_CDC_HID_DataOut(USBD_HandleTypeDef *pdev, uint8_t epnum)
{
	/*DataOUT can be for CDC or HID */
	if (epnum == (CUSTOM_HID_EPOUT_ADDR&~0x80))
		return (USBD_CUSTOM_HID_DataOut(pdev, epnum));
	else
		return (USBD_CDC_DataOut(pdev, epnum));
//	return USBD_CDC_DataOut(pdev, epnum);
}


static uint8_t COMPOSITE_CDC_HID_EP0_RxReady(USBD_HandleTypeDef *pdev)
{
	USBD_CUSTOM_HID_EP0_RxReady(pdev);
	USBD_CDC_EP0_RxReady(pdev);

	return USBD_OK;
//	return USBD_CDC_EP0_RxReady(pdev);
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
