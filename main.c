#include <msp430.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "peripherals.h"

// Function Prototypes
void swDelay(char numLoops);
int intToAscii(int number);
void runtimerA2(void);
void stoptimerA2(int reset);
void displayTime(unsigned int inTme);
void playNote(int duration, int pitch);
int tallyPoints(void);

// Declare globals here
long unsigned int timer_cnt=0;
char tdir = 1;
int s = 2; // game speed multiplier
enum GAME_STATE {DISP_WELCOME = 0, PLAY_SEQUENCE = 1, WIN_SCREEN = 2, LOSE_SCREEN = 3};
int p;
int score = 0;
// left
int A = 440;
int Bf = 466;
int B = 494;
//mid left
int Cn = 523;
int Cs = 554;
int D = 587;
//mid right
int Eb = 622;
int E = 659;
int F = 698;
//right
int Fs = 740;
int G = 784;
int Af = 831;

int song[28];
int durr[28];
int playerPoints[28];
int playerTotal = 0;

char score_arr[2];

#pragma vector=TIMER2_A0_VECTOR
__interrupt void TimerA2_ISR (void)
{
        timer_cnt++;
        if (timer_cnt == 60000)
            timer_cnt = 0;
        if (timer_cnt%200==0) // blink LEDs once a second
        {

        }
}
// Main
void main(void)

{
    unsigned char currKey=0;
    //unsigned char dispThree[3];
    WDTCTL = WDTPW | WDTHOLD;    // Stop watch dog timer. Always need to stop this!!

    initLeds();
    configDisplay();
    configKeypad();
    configButtons();
    configUserLED();

    _enable_interrupts();
    //_BIS_SR(GIE);

    song[0] = G;
    song[1] = Af;
    song[2] = G;
    song[3] = E;
    song[4] = E;
    song[5] = E;
    song[6] = D;
    song[7] = E;
    song[8] = F;
    song[9] = E;
    song[10] = F;
    song[11] = D;
    song[12] = G;
    song[13] = E;
    song[14] = Cn;
    song[15] = A;
    song[16] = D;
    song[17] = A;
    song[18] = G;
    song[19] = E;
    song[20] = Cs;
    song[21] = Bf;
    song[22] = F;
    song[23] = E;
    song[24] = G;
    song[25] = Af;
    song[26] = G;
    song[27] = F;


    durr[0] = 60*s;
    durr[1] = 30*s;
    durr[2] = 10*s;
    durr[3] = 60*s;
    durr[4] = 60*s;
    durr[5] = 30*s;
    durr[6] = 10*s;
    durr[7] = 30*s;
    durr[8] = 10*s;
    durr[9] = 120*s;
    durr[10] = 60*s;
    durr[11] = 30*s;
    durr[12] = 10*s;
    durr[13] = 120*s;
    durr[14] = 60*s;
    durr[15] = 30*s;
    durr[16] = 10*s;
    durr[17] = 200*s;
    durr[18] = 30*s;
    durr[19] = 30*s;
    durr[20] = 30*s;
    durr[21] = 30*s;
    durr[22] = 30*s;
    durr[23] = 30*s;
    durr[24] = 30*s;
    durr[25] = 30*s;
    durr[26] = 30*s;
    durr[27] = 30*s;

    //dispThree[0] = ' ';
    //dispThree[2] = ' ';
    //VARIABLES

    char bits[2] = {'0', '0'};
    setUserLED(bits);
    int i=0;
    int buttonHold = 0;
    enum GAME_STATE state = DISP_WELCOME;
    while (1)    // Forever loop
    {
        switch(state)
        {
                   case DISP_WELCOME: //Begins WELCOME case.
                   Graphics_drawStringCentered(&g_sContext, "WELCOME TO HERO", AUTO_STRING_LENGTH, 48, 35, TRANSPARENT_TEXT); //Displays startup screen.
                   Graphics_drawStringCentered(&g_sContext, "Press *", AUTO_STRING_LENGTH, 48, 45, TRANSPARENT_TEXT);
                   Graphics_flushBuffer(&g_sContext); //Resets screen.
                   currKey = getKey(); //Checks button press.
                   if (currKey == '*') //When '*' is pressed, the display resets, and the PLAY case starts.
                           {
                               runtimerA2();
                               int save_time=timer_cnt;
                               Graphics_clearDisplay(&g_sContext);
                               while (timer_cnt<=(save_time + 600))
                               {
                                   int counting = timer_cnt-save_time;

                                   if (counting < 10)
                                   {

                                       Graphics_drawStringCentered(&g_sContext, "3", AUTO_STRING_LENGTH, 48, 45, OPAQUE_TEXT);
                                       Graphics_flushBuffer(&g_sContext);
                                       setLeds(8);
                                   }
                                   if (counting > 200 & counting < 400)
                                   {

                                       Graphics_drawStringCentered(&g_sContext, "2", AUTO_STRING_LENGTH, 48, 45, OPAQUE_TEXT);
                                       Graphics_flushBuffer(&g_sContext);
                                       setLeds(4);
                                   }
                                   if (counting > 400)
                                   {

                                       Graphics_drawStringCentered(&g_sContext, "1", AUTO_STRING_LENGTH, 48, 45, OPAQUE_TEXT);
                                       Graphics_flushBuffer(&g_sContext);
                                       setLeds(2);
                                   }
                               }

                               setLeds(0);
                               state = PLAY_SEQUENCE;
                           }
                   break;
                   case PLAY_SEQUENCE: //Begins PLAY case
                       Graphics_clearDisplay(&g_sContext); // Clear the display
                       for (i = 0; i < 28; i++)
                       {
                           int holdTime = timer_cnt;
                           //int noteDurr = durr[i];
                           char off[2] = {'0', '0'};
                           setUserLED(off);
                           while (timer_cnt<=(holdTime + durr[i]))
                           {
                               BuzzerOn(song[i]);
                               int currLED = 1;
                               if (440 <= song[i] && song[i] <= 494)
                               { // led 1
                                   setLeds(8);
                                   currLED = 8;
                               }
                               if (523 <= song[i] && song[i] <= 587)
                               { // led 2
                                   setLeds(4);
                                   currLED = 4;
                               }
                               if (622 <= song[i] && song[i] <= 698)
                               { // led 3
                                   setLeds(2);
                                   currLED = 2;
                               }
                               if (740 <= song[i] && song[i] <= 831)
                               { // led 4
                                   setLeds(1);
                                   currLED = 1;
                               }
                               currKey = getKey();
                               if (currKey == '#') {
                                                         state = DISP_WELCOME;
                                                         i = 29;
                                                         break;
                                                     }


                  // Button part **************************************************************************
                               int ret = getButton();
                               if (ret == currLED)
                               {
                                   if (timer_cnt % 5 == 0) { // every 5 ms check if button is down
                                          buttonHold++;
                                   }
                                   char b0[2] = {'1', '0'};
                                   setUserLED(b0);
                               } else {
                                   char b0[2] = {'0', '1'};
                                   setUserLED(b0);
                               }
               // Button part **************************************************************************

                           }
                           Graphics_clearDisplay(&g_sContext); // Clear the display
                           Graphics_flushBuffer(&g_sContext);
                           int holdTime2 = timer_cnt;
                           while (timer_cnt<=(holdTime2 + 60))
                           {
                               BuzzerOff();
                               setLeds(0000);
                               char b[2] = {'0', '0'};
                               setUserLED(b);
                           }
                           Graphics_clearDisplay(&g_sContext); // Clear the display
                           Graphics_flushBuffer(&g_sContext);
                           //buttonHold;

                           if (durr[i] >= 60*s)
                           {
                               if (buttonHold >= (durr[i] - 20*s))
                               {
                                   Graphics_drawStringCentered(&g_sContext, "EXCELLENT",AUTO_STRING_LENGTH,51,32,OPAQUE_TEXT);
                                   Graphics_drawStringCentered(&g_sContext, "+3",AUTO_STRING_LENGTH,51,50,OPAQUE_TEXT);
                                   Graphics_flushBuffer(&g_sContext);
                                   playerPoints[i] = 3;
                               }
                               else if ( buttonHold >= (durr[i] - 30*s))
                               {
                                   Graphics_drawStringCentered(&g_sContext, "GOOD",AUTO_STRING_LENGTH,51,32,OPAQUE_TEXT);
                                   Graphics_drawStringCentered(&g_sContext, "+2",AUTO_STRING_LENGTH,51,50,OPAQUE_TEXT);
                                   Graphics_flushBuffer(&g_sContext);
                                   playerPoints[i] = 2;
                               }
                               else if (buttonHold >= (durr[i] - 40*s))
                               {
                                   Graphics_drawStringCentered(&g_sContext, "OK",AUTO_STRING_LENGTH,51,32,OPAQUE_TEXT);
                                   Graphics_drawStringCentered(&g_sContext, "+1",AUTO_STRING_LENGTH,51,50,OPAQUE_TEXT);
                                   Graphics_flushBuffer(&g_sContext);
                                   playerPoints[i] = 1;
                               }
                               else
                               {
                                   Graphics_drawStringCentered(&g_sContext, "BOOO!!",AUTO_STRING_LENGTH,51,32,OPAQUE_TEXT);
                                   Graphics_flushBuffer(&g_sContext);
                                   playerPoints[i] = 0;
                               }
                           }
                           else
                           {
                               if (buttonHold >= (durr[i] - 4*s))
                               {
                                   Graphics_drawStringCentered(&g_sContext, "EXCELLENT",AUTO_STRING_LENGTH,51,32,OPAQUE_TEXT);
                                   Graphics_drawStringCentered(&g_sContext, "+3",AUTO_STRING_LENGTH,51,50,OPAQUE_TEXT);
                                   Graphics_flushBuffer(&g_sContext);
                                   playerPoints[i] = 3;
                               }
                               else if ( buttonHold >= (durr[i] - 7*s))
                               {
                                   Graphics_drawStringCentered(&g_sContext, "GOOD",AUTO_STRING_LENGTH,51,32,OPAQUE_TEXT);
                                   Graphics_drawStringCentered(&g_sContext, "+2",AUTO_STRING_LENGTH,51,50,OPAQUE_TEXT);
                                   Graphics_flushBuffer(&g_sContext);
                                   playerPoints[i] = 2;
                               }
                               else if (buttonHold >= (durr[i] - 8*s))
                               {
                                   Graphics_drawStringCentered(&g_sContext, "OK",AUTO_STRING_LENGTH,51,32,OPAQUE_TEXT);
                                   Graphics_drawStringCentered(&g_sContext, "+1",AUTO_STRING_LENGTH,51,50,OPAQUE_TEXT);
                                   Graphics_flushBuffer(&g_sContext);
                                   playerPoints[i] = 1;
                               }
                           else
                           {
                                Graphics_drawStringCentered(&g_sContext, "BOOO!!",AUTO_STRING_LENGTH,51,32,OPAQUE_TEXT);
                                 Graphics_flushBuffer(&g_sContext);
                                 playerPoints[i] = 0;
                           }
                     }
                           buttonHold = 0;
                       }
                       Graphics_clearDisplay(&g_sContext); // Clear the display
                       Graphics_flushBuffer(&g_sContext);

                       for (p = 0; p < 28; p++)
                       {
                           score = score + playerPoints[p];
                       }
                       if (score > 20)
                       {
                           state = WIN_SCREEN;
                       }
                       else
                       {
                           state = LOSE_SCREEN;
                       }

                       break;
                   case WIN_SCREEN: //Displays win screen and sends to welcome screen.
                       if (score >= 10)
                       {
                          score_arr[0] = intToAscii(score/10);
                          score_arr[1] = intToAscii(score-((score/10)*10));

                       }
                       Graphics_drawStringCentered(&g_sContext, score_arr,2,48,55,OPAQUE_TEXT);


                       Graphics_drawStringCentered(&g_sContext, "YOU", AUTO_STRING_LENGTH, 48, 15, TRANSPARENT_TEXT);
                       Graphics_drawStringCentered(&g_sContext, "WIN", AUTO_STRING_LENGTH, 48, 25, TRANSPARENT_TEXT);
                       Graphics_flushBuffer(&g_sContext);
                       currKey = getKey(); //Checks button press.
                       if (currKey == '#') //When '*' is pressed, the display resets, and the PLAY case starts.
                       {
                           Graphics_clearDisplay(&g_sContext);
                           state = DISP_WELCOME;
                       }
                   break;
                   case LOSE_SCREEN: //Displays lose screen and sends to welcome screen.
                        Graphics_drawStringCentered(&g_sContext, "YOU", AUTO_STRING_LENGTH, 48, 15, TRANSPARENT_TEXT);
                        Graphics_drawStringCentered(&g_sContext, "LOSE", AUTO_STRING_LENGTH, 48, 25, TRANSPARENT_TEXT);
                        Graphics_flushBuffer(&g_sContext);
                        currKey = getKey(); //Checks button press.
                        if (currKey == '#') //When '*' is pressed, the display resets, and the PLAY case starts.
                        {
                             Graphics_clearDisplay(&g_sContext);
                             state = DISP_WELCOME;
                        }
                   break;
               }//End switch state
    }  // end while (1)
}

//void lightLEDs() {

//}





//Functions
void swDelay(char numLoops)
{
	volatile unsigned int i,j;	// volatile to prevent removal in optimization by compiler.
	for (j=0; j<numLoops; j++)
    {
    	i = 50000 ;					// SW Delay
   	    while (i > 0)				// could also have used while (i)
	       i--;
    }
}

int intToAscii(int number)
{
   return '0' + number;
}

//void displayTime(unsigned int inTime)
//{
//char asc_arr[6];
//unsigned int sec;
//char s
//sec = inTime*0.005;
//sec = intToAscii(inTime);
//asc_arr[1] = sec;
//Graphics_drawStringCentered(&g_sContext, time,6,51,32,OPAQUE_TEXT);
//Graphics_flushBuffer(&g_sContext);
//}



void runtimerA2(void)
{
// This function configures and starts Timer A2
// Timer is counting ~0.01 seconds
//
// Input: none, Output: none
//
// smj, ECE2049, 17 Sep 2013
//
// Use ACLK, 16 Bit, up mode, 1 divider
    TA2CTL = TASSEL_1 + MC_1 + ID_0;
    //TA2CCR0 = 327; // 327+1 = 328 ACLK tics = ~1/100 seconds
    TA2CCR0 = 163; // 162.84 ~ 163 ACLK tics = 0.005 seconds resolution
    TA2CCTL0 = CCIE; // TA2CCR0 interrupt enabled
}

void stoptimerA2(int reset)
{
// This function stops Timer A2 andresets the global time variable
// if input reset = 1
//
// Input: reset, Output: none
//
// smj, ECE2049, 17 Sep 2013
//
    TA2CTL = MC_0; // stop timer
    TA2CCTL0 &= ~CCIE; // TA2CCR0 interrupt disabled
    if(reset)
            timer_cnt=0;
}

int tallyPoints(void) {
    int p = 0;
    int total = 0;
    for (p = 0; p < 28; p++) {
        total = total + playerPoints[p];
    }
    return total;
}

