#ifndef __COMPOSITE_INTERNAL_H__
#define __COMPOSITE_INTERNAL_H__

#include "usbd_cdc_if.h"
#include "usbd_custom_hid_if.h"



#if 0
#define CUSTOM_HID_EPIN_ADDR                         0x83U
#define CUSTOM_HID_EPOUT_ADDR                        0x03U
#define CDC_IN_EP                                   0x81U  /* EP1 for data IN */
#define CDC_OUT_EP                                  0x01U  /* EP1 for data OUT */
#define CDC_CMD_EP                                  0x82U  /* EP2 for CDC commands */
#else
#define CUSTOM_HID_EPIN_ADDR                         0x81U
#define CUSTOM_HID_EPOUT_ADDR                        0x01U
#define CDC_IN_EP                                   0x82U  /* EP1 for data IN */
#define CDC_OUT_EP                                  0x02U  /* EP1 for data OUT */
#define CDC_CMD_EP                                  0x83U  /* EP2 for CDC commands */
#endif

#define CUSTOM_HID_EPIN_SIZE                         64
#define CUSTOM_HID_EPOUT_SIZE                        64

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

#define CDC_DATA_HS_IN_PACKET_SIZE                  CDC_DATA_HS_MAX_PACKET_SIZE
#define CDC_DATA_HS_OUT_PACKET_SIZE                 CDC_DATA_HS_MAX_PACKET_SIZE

#define CDC_DATA_FS_IN_PACKET_SIZE                  CDC_DATA_FS_MAX_PACKET_SIZE
#define CDC_DATA_FS_OUT_PACKET_SIZE                 CDC_DATA_FS_MAX_PACKET_SIZE


#define USB_COMPOSITE_CONFIG_DESC_SIZ       107U


struct composite_CDC_HID_Handle
{
	USBD_CDC_HandleTypeDef cdc;
	USBD_CUSTOM_HID_HandleTypeDef chid;
};
#endif //__COMPOSITE_INTERNAL_H__
