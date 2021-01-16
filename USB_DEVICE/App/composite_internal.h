#ifndef __COMPOSITE_INTERNAL_H__
#define __COMPOSITE_INTERNAL_H__

#include "usbd_cdc_if.h"
#include "usbd_custom_hid_if.h"

struct composite_CDC_HID_Handle
{
	USBD_CDC_HandleTypeDef cdc;
	USBD_CUSTOM_HID_HandleTypeDef chid;
};
#endif //__COMPOSITE_INTERNAL_H__
