#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "magnitude.h"



inline Magnitude encoder_magnitude(int16_t val){

    if (val==0)
    {
        return (Magnitude){0,0};
    }
    int16_t tmp=abs(val);
    uint8_t magnitude=0;
   
    Magnitude val_enc;

    while ( tmp > 0)
    { 
      magnitude++;
      tmp=tmp >> 1;
    }
   
    uint16_t puissance_2 = 1 << magnitude;
    val_enc.classe=magnitude;
    int16_t sup_int=puissance_2-1;
    int16_t masque_signe = val >> 15;
    val_enc.indice = val + (masque_signe & sup_int);

    return val_enc;
}