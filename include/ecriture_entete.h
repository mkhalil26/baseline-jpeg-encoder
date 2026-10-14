#ifndef _ECRITURE_ENTETE_H_
#define _ECRITURE_ENTETE_H_

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "commande.h"
#include "qtables.h"
#include "htables.h"

#define DEBUT_MARQUEUR 0xff
#define MARQUEUR_APP0 0xe0
#define MARQEUR_DQT 0xdb
#define MARQUEUR_SOF0 0xc0
#define MARQUEUR_DHT 0xc4
#define MARQUEUR_SOS 0xda
#define MARQUEUR_SOI 0xd8
#define MARQUEUR_EOI 0xd9

#define ID_QUANTIFICATION_Y 0
#define ID_QUANTIFICATION_CBCR 1

#define ID_COMPOSANTE_Y 1
#define ID_COMPOSANTE_CB 2
#define ID_COMPOSANTE_CR 3

#define ID_DC 0
#define ID_AC 1



// écris le marqueur SOI de début de fichier dans le fichier de sortie
bool ecrire_SOI(FILE *f);

// écris le marqueur EOI de fin de fichier dans le fichier de sortie
bool ecrire_EOI(FILE *f);

// écris l'entête du fichier de sortie en fonction des spécifications de l'image
bool ecrire_entete(
    FILE *f, uint16_t hauteur_image, uint16_t largeur_image, bool couleur, Facteurs_echantillonnage facteurs,
    const uint8_t *table_q_y, const uint8_t *table_q_cbcr,
    const uint8_t longueurs_huffman[NB_SAMPLE_TYPES][NB_COLOR_COMPONENTS][16],
    const uint8_t *table_symboles_huffman[NB_SAMPLE_TYPES][NB_COLOR_COMPONENTS],
    const uint8_t taille_table_huffman[NB_SAMPLE_TYPES][NB_COLOR_COMPONENTS]
);

// marqueurs écrits dans la fonction ecrire_entete qui sont déclarés pour les tests
bool ecrire_appx(FILE *f);

bool ecrire_DQT(FILE *f, const uint8_t tab_quantif[64], bool tab_est_CbCr);

bool ecrire_SOFx(FILE *f, uint16_t hauteur_image, uint16_t largeur_image, bool couleur, Facteurs_echantillonnage facteurs);

bool ecrire_DHT(FILE *f, bool ac, const uint8_t longueurs_huffman[16], const uint8_t *table_symboles_huffman, const uint8_t taille_table_huffman, uint8_t id_composante);

bool ecrire_SOS(FILE *f, bool couleur);



#endif /* _ECRITURE_ENTETE_H_ */
