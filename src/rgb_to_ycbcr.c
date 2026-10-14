#include "rgb_to_ycbcr.h"



uint8_t calculer_y(Couleur_rgb couleur) {

    return tab_Y_R[couleur.r]  + tab_Y_G[couleur.g]  + tab_Y_B[couleur.b];
}

uint8_t calculer_cb(Couleur_rgb couleur) {

    return tab_Cb_R[couleur.r] + tab_Cb_G[couleur.g] + (couleur.b >> 1) + 128;
}

uint8_t calculer_cr(Couleur_rgb couleur) {

    return (couleur.r >> 1)    + tab_Cr_G[couleur.g] + tab_Cr_B[couleur.b] + 128;
}