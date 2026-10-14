
#ifndef _MAGNITUDE_RLE_HUFFMAN_H_
#define _MAGNITUDE_RLE_HUFFMAN_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "magnitude.h"
#include "htables.h"
#include "downsampler.h"

#define ZRL 0xf0
#define EOB 0x00 



typedef struct codageHuffman // coefficient compressé avec Huffman
{
    Magnitude magnitude; // magnitude associée
    uint16_t code; // code de Huffman
    uint8_t nb_bits; // longueur du code en bits

} CodageHuffman;

typedef struct AC_DC // bloc de compressé avec Huffman
{
    CodageHuffman AC[63]; // les encodages Huffman des 63 coefficients AC
    CodageHuffman DC; // l'encodage Huffman de DC
    uint8_t taille; // le nombre de coefficients AC encodés

} AC_DC;

typedef struct NoeudHuffman // noeud de l'abre d'Huffman
{
    uint16_t code; // code binaire du noeud
    uint8_t nb_bits; // nombre de bits de ce code binaire
    bool valide; // indique si l'entrée est valide ou non

} Huffman;



// initialise les 4 tableaux contenant la convertion octet à code huffman
void initialise_huffman();

// encode les AC et le DC avec la magnitude, l'encodage RLE puis celui d'Huffman, renvoie false si on utilise une entrée invalide dans la table d'Huffman
bool magnitude_rle_huffman(int16_t *dc_prec, const int16_t vecteur[64], Composante composante, AC_DC *resultat);

// fonction similaire à magnitude_rle_huffman, déclarée pour les tests
bool magnitude_rle_huffman_composante(int16_t *dc_prec, const int16_t vecteur[64], const Huffman tab_dc[256], const Huffman tab_ac[256], AC_DC *resultat);

// construit un tableau à 256 entrées qui pour chaque entrée valide donne l'encodage Huffman correspondant, déclarée pour les tests
void construction_arbre_huffman(Huffman tab[256], const uint8_t *lengths, const uint8_t *symbols);

// fonctions déclarées pour les tests
bool magnitude_rle_huffman_y(int16_t *dc_prec, const int16_t vecteur[64], AC_DC *resultat);

bool magnitude_rle_huffman_cbcr(int16_t *dc_prec, const int16_t vecteur[64], AC_DC *resultat);



#endif /* _MAGNITUDE_RLE_HUFFMAN_H_ */