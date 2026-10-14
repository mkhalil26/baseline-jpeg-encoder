#ifndef _MAGNITUDE_H_
#define _MAGNITUDE_H_

#include <stdint.h>



typedef struct Magnitude // définition de la magnitude 
{
   uint16_t indice; // indice associé à la classe de magnitude
   uint8_t classe; // classe de magnitude

} Magnitude;



// transforme une valeur en magnitude
Magnitude encoder_magnitude(int16_t val);



#endif 
