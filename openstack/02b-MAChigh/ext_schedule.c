#include "opendefs.h"
#include "schedule.h"

void getExtSchedule(uint16_t addr, uint8_t timeslot_offset, extScheduleEntry_t *extScheduleEntry)
{
   switch (addr) {
      case 0x5a53:
         switch(timeslot_offset) {
            case 0:
               extScheduleEntry->type = CELLTYPE_RX;
               extScheduleEntry->channelMask = 0x00ff;
               extScheduleEntry->neighbor = 0x89a5;
               break;
            case 1:
               extScheduleEntry->type = CELLTYPE_OFF;
               extScheduleEntry->channelMask = 0;
               extScheduleEntry->neighbor = 0;
               break;
            case 2:
               extScheduleEntry->type = CELLTYPE_RX;
               extScheduleEntry->channelMask = 0x00ff;
               extScheduleEntry->neighbor = 0x89a5;
               break;
            case 3:
               extScheduleEntry->type = CELLTYPE_OFF;
               extScheduleEntry->channelMask = 0;
               extScheduleEntry->neighbor = 0;
               break;
            default:
               break;
         }
         break;
      case 0x89a5:
         switch(timeslot_offset) {
            case 0:
               extScheduleEntry->type = CELLTYPE_TX;
               extScheduleEntry->channelMask = 0x00ff;
               extScheduleEntry->neighbor = 0x5a53;
               break;
            case 1:
               extScheduleEntry->type = CELLTYPE_RX;
               extScheduleEntry->channelMask = 0x00ff;
               extScheduleEntry->neighbor = 0x6e29;
               break;
            case 2:
               extScheduleEntry->type = CELLTYPE_TX;
               extScheduleEntry->channelMask = 0x00ff;
               extScheduleEntry->neighbor = 0x5a53;
               break;
            case 3:
               extScheduleEntry->type = CELLTYPE_OFF;
               extScheduleEntry->channelMask = 0;
               extScheduleEntry->neighbor = 0;
               break;
            default:
               break;
         }
         break;
      case 0x6e29:
         switch(timeslot_offset) {
            case 0:
               extScheduleEntry->type = CELLTYPE_OFF;
               extScheduleEntry->channelMask = 0;
               extScheduleEntry->neighbor = 0;
               break;
            case 1:
               extScheduleEntry->type = CELLTYPE_TX;
               extScheduleEntry->channelMask = 0x00ff;
               extScheduleEntry->neighbor = 0x89a5;
               break;
            case 2:
               extScheduleEntry->type = CELLTYPE_OFF;
               extScheduleEntry->channelMask = 0;
               extScheduleEntry->neighbor = 0;
               break;
            case 3:
               extScheduleEntry->type = CELLTYPE_OFF;
               extScheduleEntry->channelMask = 0;
               extScheduleEntry->neighbor = 0;
               break;
            default:
               break;
         }
         break;
      default:
         break;
   }
}
