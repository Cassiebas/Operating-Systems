#include "eos.h"

void blink()
{
    UI32 clrRecord[] = {17, 22, 23, 24, 25, 27}; //Set GPCLR0/1 bits to pass into setCLR Function 
    //int buttonCounter = 0;

    char *temp;
    UI32 gpset0, gpclr0, gplev0, i;

//Knopje
    setFSEL(EOS_GPFSEL1, INPUT, 1); //gpio 10 Knopje INPUT

//stoplicht 1 
    setFSEL(EOS_GPFSEL1, OUTPUT, 7); //gpio 17 rood OUTPUT ..
    setFSEL(EOS_GPFSEL2, OUTPUT, 7); //gpio 27 oranje
    setFSEL(EOS_GPFSEL2, OUTPUT, 2); //gpio 22 groen
    
//stoplicht 2
    setFSEL(EOS_GPFSEL2, OUTPUT, 5); //gpio 25 groen
    setFSEL(EOS_GPFSEL2, OUTPUT, 4); //gpio 24 oranje
    setFSEL(EOS_GPFSEL2, OUTPUT, 3); //gpio 23 rood

    while (1) 
    {
        //gpio 10 aanzetten voor knopje
        setGPSET(EOS_GPSET0, BIT, 10);

        gplev0 = get32(EOS_GPLEV0);
        for (int i = 0; i <= 31; i++)
        {

            if (getBit(gplev0, 10) == 1)
            {
                
                //stoplight 1 op rood 2 op groen
                setGPSET(EOS_GPSET0, BIT, 17);
                setGPSET(EOS_GPSET0, BIT, 25);
                
                printsln("1");
                HOLD_1S // Wait functie
                
                for (int i = 0; i <= 5; i++)
                {
                    setGPCLR(EOS_GPCLR0, BIT, clrRecord[i]);
                }
                
                //stoplicht 1 op rood 2 op oranje
                setGPSET(EOS_GPSET0, BIT, 17);
                setGPSET(EOS_GPSET0, BIT, 24);
        
                printsln("2");
                HOLD_1S
                
                for (int i = 0; i <= 5; i++)
                {
                    setGPCLR(EOS_GPCLR0, BIT, clrRecord[i]);
                }
                
                //stoplicht 1 op rood 2 op rood
                setGPSET(EOS_GPSET0, BIT, 17);
                setGPSET(EOS_GPSET0, BIT, 23);
                
                printsln("3");
                HOLD_1S
                
                for (int i = 0; i <= 5; i++)
                {
                    setGPCLR(EOS_GPCLR0, BIT, clrRecord[i]);
                }
                
                //stoplicht 1 op groen 2 op rood
                setGPSET(EOS_GPSET0, BIT, 22);
                setGPSET(EOS_GPSET0, BIT, 23);
                
                printsln("4");
                HOLD_1S

                for (int i = 0; i <= 5; i++)
                {
                    setGPCLR(EOS_GPCLR0, BIT, clrRecord[i]);
                }
                
                //stoplicht 1 op oranje 2 op rood
                setGPSET(EOS_GPSET0, BIT, 27);
                setGPSET(EOS_GPSET0, BIT, 23);
                
                printsln("5");
                HOLD_1S
                
                for (int i = 0; i <= 5; i++)
                {
                    setGPCLR(EOS_GPCLR0, BIT, clrRecord[i]);
                }
                
                //stoplicht 1 op rood 2 op rood
                setGPSET(EOS_GPSET0, BIT, 17);
                setGPSET(EOS_GPSET0, BIT, 23);
                
                printsln("6");
                HOLD_1S
                
                for (int i = 0; i <= 5; i++)
                {
                    setGPCLR(EOS_GPCLR0, BIT, clrRecord[i]);
                }
            }
            else
            {
                for(i = 0; i <= 31;i++)
                {
                    //reset GPSET0 register
                    setGPSET(EOS_GPSET0, BIT, i); 
                }
            }
        
        }
        
        //printBin(gplev0);        
    }

}