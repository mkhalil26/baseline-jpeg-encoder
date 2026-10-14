#include <stdio.h>
#include <math.h>
#include "zigzag_quantification.h"



void applique_zigzag_quantification(const float vecteur_frequentiel[64], Composante composante, int16_t vecteur_sortie[64])
{
    const uint8_t *table_quantification = (composante == Y) ? quantification_table_Y : quantification_table_CbCr;

    for (int i = 0; i < 64; i++) {
                
        vecteur_sortie[i] = (int16_t)round(vecteur_frequentiel[indices_zigzag[i]] / table_quantification[i]);
    }
}