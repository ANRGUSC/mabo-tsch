#include "opendefs.h"
#include "openqueue.h"
#include "openserial.h"
#include "packetfunctions.h"
#include "sixtop.h"
#include "IEEE802154E.h"

//=========================== variables =======================================

openqueue_vars_t openqueue_vars;

//=========================== prototypes ======================================

void openqueue_reset_entry(OpenQueueEntry_t* entry);

//=========================== public ==========================================

//======= admin

/**
\brief Initialize this module.
*/
void openqueue_init() {
   uint8_t i;
   for (i=0;i<QUEUELENGTH;i++){
      openqueue_reset_entry(&(openqueue_vars.queue[i]));
   }
}

//======= called by any component

/**
\brief Request a new (free) packet buffer.

Component throughout the protocol stack can call this function is they want to
get a new packet buffer to start creating a new packet.

\note Once a packet has been allocated, it is up to the creator of the packet
      to free it using the openqueue_freePacketBuffer() function.

\returns A pointer to the queue entry when it could be allocated, or NULL when
         it could not be allocated (buffer full or not synchronized).
*/
OpenQueueEntry_t* openqueue_getFreePacketBuffer(uint8_t creator) {
   uint8_t i;
   INTERRUPT_DECLARATION();
   DISABLE_INTERRUPTS();
   
   // refuse to allocate if we're not in sync
   if (ieee154e_isSynch()==FALSE && creator > COMPONENT_IEEE802154E){
     ENABLE_INTERRUPTS();
     return NULL;
   }
   
   // if you get here, I will try to allocate a buffer for you
   
   // walk through queue and find free entry
   for (i=0;i<QUEUELENGTH;i++) {
      if (openqueue_vars.queue[i].owner==COMPONENT_NULL) {
         openqueue_vars.queue[i].creator=creator;
         openqueue_vars.queue[i].owner=COMPONENT_OPENQUEUE;
         ENABLE_INTERRUPTS(); 
         return &openqueue_vars.queue[i];
      }
   }
   ENABLE_INTERRUPTS();
   return NULL;
}


/**
\brief Free a previously-allocated packet buffer.

\param pkt A pointer to the previsouly-allocated packet buffer.

\returns E_SUCCESS when the freeing was succeful.
\returns E_FAIL when the module could not find the specified packet buffer.
*/
owerror_t openqueue_freePacketBuffer(OpenQueueEntry_t* pkt) {
   uint8_t i;
   INTERRUPT_DECLARATION();
   DISABLE_INTERRUPTS();
   for (i=0;i<QUEUELENGTH;i++) {
      if (&openqueue_vars.queue[i]==pkt) {
         if (openqueue_vars.queue[i].owner==COMPONENT_NULL) {
            // log the error
            openserial_printCritical(COMPONENT_OPENQUEUE,ERR_FREEING_UNUSED,
                                  (errorparameter_t)0,
                                  (errorparameter_t)0);
         }
         openqueue_reset_entry(&(openqueue_vars.queue[i]));
         ENABLE_INTERRUPTS();
         return E_SUCCESS;
      }
   }
   // log the error
   openserial_printCritical(COMPONENT_OPENQUEUE,ERR_FREEING_ERROR,
                         (errorparameter_t)0,
                         (errorparameter_t)0);
   ENABLE_INTERRUPTS();
   return E_FAIL;
}

/**
\brief Free all the packet buffers created by a specific module.

\param creator The identifier of the component, taken in COMPONENT_*.
*/
void openqueue_removeAllCreatedBy(uint8_t creator) {
   uint8_t i;
   INTERRUPT_DECLARATION();
   DISABLE_INTERRUPTS();
   for (i=0;i<QUEUELENGTH;i++){
      if (openqueue_vars.queue[i].creator==creator) {
         openqueue_reset_entry(&(openqueue_vars.queue[i]));
      }
   }
   ENABLE_INTERRUPTS();
}

/**
\brief Free all the packet buffers owned by a specific module.

\param owner The identifier of the component, taken in COMPONENT_*.
*/
void openqueue_removeAllOwnedBy(uint8_t owner) {
   uint8_t i;
   INTERRUPT_DECLARATION();
   DISABLE_INTERRUPTS();
   for (i=0;i<QUEUELENGTH;i++){
      if (openqueue_vars.queue[i].owner==owner) {
         openqueue_reset_entry(&(openqueue_vars.queue[i]));
      }
   }
   ENABLE_INTERRUPTS();
}

//======= called by RES

OpenQueueEntry_t* openqueue_sixtopGetSentPacket() {
   uint8_t i;
   INTERRUPT_DECLARATION();
   DISABLE_INTERRUPTS();
   for (i=0;i<QUEUELENGTH;i++) {
      if (openqueue_vars.queue[i].owner==COMPONENT_IEEE802154E_TO_SIXTOP &&
          openqueue_vars.queue[i].creator!=COMPONENT_IEEE802154E) {
         ENABLE_INTERRUPTS();
         return &openqueue_vars.queue[i];
      }
   }
   ENABLE_INTERRUPTS();
   return NULL;
}

OpenQueueEntry_t* openqueue_sixtopGetReceivedPacket() {
   uint8_t i;
   INTERRUPT_DECLARATION();
   DISABLE_INTERRUPTS();
   for (i=0;i<QUEUELENGTH;i++) {
      if (openqueue_vars.queue[i].owner==COMPONENT_IEEE802154E_TO_SIXTOP &&
          openqueue_vars.queue[i].creator==COMPONENT_IEEE802154E) {
         ENABLE_INTERRUPTS();
         return &openqueue_vars.queue[i];
      }
   }
   ENABLE_INTERRUPTS();
   return NULL;
}

//======= called by IEEE80215E

OpenQueueEntry_t* openqueue_macGetDataPacket(void) {
   uint8_t i;
   INTERRUPT_DECLARATION();
   DISABLE_INTERRUPTS();
   for (i=0;i<QUEUELENGTH;i++) {
      if (openqueue_vars.queue[i].owner==COMPONENT_SIXTOP_TO_IEEE802154E &&
          openqueue_vars.queue[i].creator!=COMPONENT_SIXTOP) {
         ENABLE_INTERRUPTS();
         return &openqueue_vars.queue[i];
      }
   }
   ENABLE_INTERRUPTS();
   return NULL;
}

OpenQueueEntry_t* openqueue_macGetDataPacketDestination(uint16_t dst) {
   uint8_t i;
   INTERRUPT_DECLARATION();
   DISABLE_INTERRUPTS();
   for (i=0;i<QUEUELENGTH;i++) {
      if (openqueue_vars.queue[i].owner==COMPONENT_SIXTOP_TO_IEEE802154E &&
          openqueue_vars.queue[i].creator!=COMPONENT_SIXTOP) {
         // we have to read the destion addr
         l2_ht *payload = (l2_ht *)(openqueue_vars.queue[i].payload);
         if (payload->dst == dst) {
            ENABLE_INTERRUPTS();
            return &openqueue_vars.queue[i];
         }
      }
   }
   ENABLE_INTERRUPTS();
   return NULL;
}

OpenQueueEntry_t* openqueue_macGetEBPacket() {
   uint8_t i;
   INTERRUPT_DECLARATION();
   DISABLE_INTERRUPTS();
   for (i=0;i<QUEUELENGTH;i++) {
      if (openqueue_vars.queue[i].owner==COMPONENT_SIXTOP_TO_IEEE802154E &&
          openqueue_vars.queue[i].creator==COMPONENT_SIXTOP) {
         ENABLE_INTERRUPTS();
         return &openqueue_vars.queue[i];
      }
   }
   ENABLE_INTERRUPTS();
   return NULL;
}

//=========================== private =========================================

void openqueue_reset_entry(OpenQueueEntry_t* entry) {
   //admin
   entry->creator                      = COMPONENT_NULL;
   entry->owner                        = COMPONENT_NULL;
   entry->payload                      = &(entry->packet[127]); // Footer is longer if security is used
   entry->length                       = 0;
   //l4
   entry->l4_protocol                  = IANA_UNDEFINED;
   //l3
   entry->l3_destinationAdd.type       = ADDR_NONE;
   entry->l3_sourceAdd.type            = ADDR_NONE;
   //l2
   entry->l2_nextORpreviousHop.type    = ADDR_NONE;
   entry->l2_frameType                 = SHORTTYPE_UNDEFINED;
   entry->l2_retriesLeft               = 0;
   entry->l2_IEListPresent             = 0;
   entry->l2_payloadIEpresent          = 0;
}
