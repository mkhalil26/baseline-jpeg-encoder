#ifndef _LECTURE_ECRITURE_H_
#define _LECTURE_ECRITURE_H_



#include <stdio.h>
#include "rgb_to_ycbcr.h"



typedef enum{ // P6 : image couleur, P5 : image en niveaux de gris
    P5,
    P6,

} TypeImage;

typedef struct Image { // les propriétés de l'image ouverte

    uint32_t largeur;
    uint32_t hauteur;
    uint32_t debut_pixels; // taille de l'entête

    TypeImage type;
    uint8_t ** tab; // buffer ou sont stockés les superblocs
    FILE * fichier; // le fichier d'entrée
    uint32_t taille_ligne; // largeur du dernier superbloc lu en pixels
    uint32_t nb_lignes; // hauteur du dernier superbloc lu en pixels

} Image;

typedef struct IterateurMCU { // itérateur sur la MCU en cours

    Image *image;
    uint8_t largeur_mcu, hauteur_mcu; // dimensions du MCU en pixels
    uint32_t largeur_image_mcu, hauteur_image_mcu; // dimensions de l'image en MCU
    uint32_t nb_mcu_lus;
    uint32_t x, y, i_mcu;

} IterateurMCU;



// permet d'obtenir les dimensions du MCU en pixels à partir des facteurs
void determiner_facteurs_mcu(Facteurs_echantillonnage facteurs, uint8_t *largeur_mcu, uint8_t *hauteur_mcu); 

// crée un itérateur d'un fichier avec son nom
bool initialiser_iterateur_mcu(IterateurMCU *iterateur, const char *nom_fichier, Facteurs_echantillonnage facteurs);

// retourne la prochaine MCU en couleur de l'itérateur d'image, retourne false si il n'y en a plus
bool mcu_couleur_suivant(IterateurMCU *iterateur, Couleur_rgb mcu[MCU_MAX][MCU_MAX]);

// retourne la prochaine MCU en niveaux de gris de l'itérateur d'image, retourne false si il n'y en a plus
bool mcu_gris_suivant(IterateurMCU *iterateur, uint8_t mcu[8][8]);

// libère l'itérateur, la structure Image et ferme le fichier
void liberer_iterateur_mcu(IterateurMCU *iterateur);

// détermine si l'image actuelle est en couleur ou en niveaux de gris
bool image_couleur(const IterateurMCU *iterateur);

// crée une structure image en lisant les informations de l'entête du fichier
Image* recupEntete(FILE *fichier);

// ouvre le fichier et récupère l'entête a partir de son nom
Image *lectureImage(const char *nom_fichier);

// alloue un superbloc pour commencer à lire les données
Image *allouer_image(Image *image, uint32_t largeur_bloc_en_pixels, uint32_t nb_lignes_superbloc,uint32_t nb_blocs);

// lis un superbloc en fonction des coordonnées de l'image fournies
Image *lireEblocs(Image *image_ppm, uint32_t x, uint32_t y,uint32_t largeur_bloc_en_pixels, uint32_t nb_lignes_superbloc,uint32_t nb_blocs);
    
// libère l'image et le superbloc alloué
void liberer_image(Image *image,uint32_t nb_lignes_superbloc);



#endif // _LECTURE_ECRITURE_H_