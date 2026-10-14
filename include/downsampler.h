#ifndef _DOWNSAMPLER_H_
#define _DOWNSAMPLER_H_

#define MCU_MAX 32

#include "rgb_to_ycbcr.h"
#include "commande.h"



typedef enum composante { // les 3 composantes d'une couleur yCbCr
    Y,
    CB,
    CR
} Composante;

typedef struct vecteur { // c'est un bloc de 8x8 applati avec sa composante correspondante
    
    float valeur[64];
    Composante composante;
} Vecteur;

typedef struct vecteurs_ycbcr { // vecteur de vecteurs qui contient tous les blocs 8x8 découpés
    
    uint8_t nb_vecteurs;
    Vecteur vecteurs[48];
} Vecteurs_ycbcr;

typedef struct dimensions_cbcr { // permet d'avoir les dimensions des sous-matrices moyennes des composantes Cb et Cr
    
    uint8_t largeur_mcu_cb, hauteur_mcu_cb, largeur_mcu_cr, hauteur_mcu_cr;
} Dimensions_cbcr;



// permet de calculer les dimensions des composantes Cb et Cr une seule fois
Dimensions_cbcr determiner_dimensions_cb_cr(Facteurs_echantillonnage facteurs); 

// decoupe la matrice de dimensions du MCU en un vecteur de vecteurs avec les 3 composantes
void decouper_matrices_ycbcr(const Couleur_rgb matrice[MCU_MAX][MCU_MAX], uint8_t largeur_mcu, uint8_t hauteur_mcu, Dimensions_cbcr dimensions_sortie, Vecteurs_ycbcr *vecteurs_sortie);

// decoupe la matrice de dimensions 8x8 pixels en un vecteur de pixels ayant la composante Y
void decouper_matrice_gris(const uint8_t matrice[8][8], Vecteur *vecteur);



#endif // _DOWNSAMPLER_H_