#include<xc.h>
#include "main.h"
#include "can.h"
#include "clcd.h"


void  can_receive()
{
    unsigned char data[8]; // +1 for null
    uint8_t len ; // FIXED
    uint16_t id;


 
        can_transmit(RPM_MSG_ID, buff, strlen(buff));
        delay(1);
        can_transmit(INDICATOR_MSG_ID, indicator[ind_val], strlen(indicator[ind_val]));
        delay(1);
    
     if(can_receive(&id, data, &len))
     {  
         if(id == SPEED_MSG_ID)
        {
            clcd_print("SPEED", LINE1(0));
            clcd_print(data, LINE2(0));
        }
        else if(id == GEAR_MSG_ID)
        {
            clcd_print("G", LINE1(11));
            clcd_print(data, LINE2(11));
        }
        else if (id == RPM_MSG_ID) {
            data[len] = '\0';

            clcd_print("RPM", LINE1(6));
            clcd_print(data, LINE2(6));
            //delay(500);
        }
        else if(id == INDICATOR_MSG_ID) {

            data[len] = '\0';
            clcd_print("IND", LINE1(13));
            if(blink)
            clcd_print(data, LINE2(13));
            else
              clcd_print("         ", LINE2(13));  
            
        }
        
    }
}

