#ifndef __OPENQUEUE_H
#define __OPENQUEUE_H

/**
\addtogroup cross-layers
\{
\addtogroup OpenQueue
\{
*/

#include "opendefs.h"

//=========================== define ==========================================

#define QUEUELENGTH  15

//=========================== typedef =========================================

typedef struct {
   uint8_t  creator;
   uint8_t  owner;
} debugOpenQueueEntry_t;

//=========================== module variables ================================

typedef struct {
   OpenQueueEntry_t queue[QUEUELENGTH];
} openqueue_vars_t;

//=========================== prototypes ======================================

// admin
void               openqueue_init(void);
bool               debugPrint_queue(void);
// called by any component
OpenQueueEntry_t*  openqueue_getFreePacketBuffer(uint8_t creator);
owerror_t          openqueue_freePacketBuffer(OpenQueueEntry_t* pkt);
uint8_t            openqueue_getFreeSpace(void);
void               openqueue_removeAllCreatedBy(uint8_t creator);
void               openqueue_removeAllOwnedBy(uint8_t owner);
void               openqueue_removeAll(void);
// called by res
OpenQueueEntry_t*  openqueue_sixtopGetSentPacket(void);
OpenQueueEntry_t*  openqueue_sixtopGetReceivedPacket(void);
// called by IEEE80215E
OpenQueueEntry_t*  openqueue_macGetDataPacket(void);
OpenQueueEntry_t*  openqueue_macGetDataPacketDestination(uint16_t dst_l2);
OpenQueueEntry_t*  openqueue_macGetDataPacketDestinationSource(uint16_t dst_l3, uint16_t src_l3);
OpenQueueEntry_t*  openqueue_macGetEBPacket(void);

/**
\}
\}
*/

#endif
