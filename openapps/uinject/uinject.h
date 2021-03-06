#ifndef __UINJECT_H
#define __UINJECT_H

/**
\addtogroup AppUdp
\{
\addtogroup uinject
\{
*/

#include "opentimers.h"
#include "sixtop.h"

//=========================== define ==========================================

#define UINJECT_PERIOD_MS 1000

//=========================== typedef =========================================

BEGIN_PACK
typedef struct {                                 // always written big endian, i.e. MSB in addr[0]
   l2_ht     l2_hdr;
   uint16_t  l3_src;
   uint16_t  l3_dst;
   uint16_t  counter;
   uint8_t   asn0;
   uint8_t   asn1;
   uint8_t   asn2;
   uint8_t   asn3;   
} uinject_ht;
END_PACK

//=========================== variables =======================================

typedef struct {
   opentimer_id_t       timerId;  // periodic timer which triggers transmission
   uint16_t             counter;  // incrementing counter which is written into the packet
} uinject_vars_t;

//=========================== prototypes ======================================

void uinject_init(void);
void uinject_sendDone(OpenQueueEntry_t* msg, owerror_t error);
void uinject_receive(OpenQueueEntry_t* msg);

/**
\}
\}
*/

#endif
