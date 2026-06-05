/* 
 * File:   main.c
 * Author: Sneha
 *
 * Created on 6 March, 2026, 8:33 AM
 */
#include <xc.h>
#include "can.h"
#include "clcd.h"
#include "msgid.h"
#include "main.h"


#define LEFT 1
#define RIGHT 2
#define HAZ 3

volatile int blink = 0;

void init_config() {
    TRISB0 = 0;
    TRISB7 = 0;
    RB0 = 0;
    RB7 = 0;
    PEIE = 1;
    GIE = 1;

    init_timer0();

    init_clcd();
    init_can();

}

void __interrupt() isr(void) {
    static int count = 0;
    static int c = 0;
    if (TMR0IF == 1) {
        TMR0 = TMR0 + 8;

        if (count++ >= 10000) {
            count = 0;
            blink ^= 1;
        }


        TMR0IF = 0;
    }
}

void main(void) {

    init_config();
    CLEAR_DISP_SCREEN;


    uint16_t msg_id;
    uint8_t data;
    uint8_t len;
    uint8_t * gear[] = {"GN", "G1", "G2", "G3", "G4", "G5", "GR ", "_C"};


    while (1) {


        clcd_print("SPD RPM  GR IND", LINE1(0));
        while (can_receive(&msg_id, &data, &len)) {



            if (msg_id == RPM_MSG_ID)
            {

                uint16_t rpm_rec = (uint16_t) data * 60;
                
                clcd_putch((rpm_rec / 1000) % 10 + '0', LINE2(4));
                clcd_putch((rpm_rec / 100) % 10 + '0', LINE2(5));
                clcd_putch((rpm_rec / 10) % 10 + '0', LINE2(6));
                clcd_putch((rpm_rec % 10) + '0', LINE2(7));
                
            } else if (msg_id == INDICATOR_MSG_ID) {

                if (blink) 
                {
                    if (data == 0) 
                    {
                        clcd_print("<-", LINE2(12));
                        RB0 = 1;

                    } 
                    else if (data == 1) 
                    {
                        clcd_print("->", LINE2(14));
                        RB7 = 1;
                    }
                    else if (data == 2) 
                    {
                        clcd_print("<-->", LINE2(12));
                        RB0 = 1;
                        RB7 = 1;
                    } 
                    else if (data == 3) 
                    {
                        RB0 = 0;
                        RB7 = 0;
                        clcd_print("    ", LINE2(12));
                    }
                }
                else
                {
                    clcd_print("    ", LINE2(12));
                    RB0 = 0;
                    RB7 = 0;
                }
            }
            else if (msg_id == GEAR_MSG_ID) 
            {

                clcd_print(gear[data], LINE2(9));

            } 
            else if (msg_id == SPEED_MSG_ID) {
                clcd_putch(((data / 100) % 10) + '0', LINE2(0));
                clcd_putch(((data / 10) % 10) + '0', LINE2(1));
                clcd_putch((data % 10) + '0', LINE2(2));

            }

        }
    }
}

