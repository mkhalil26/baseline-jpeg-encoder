#ifndef _HTABLES_H_
#define _HTABLES_H_

#include <stdint.h>

// Définitions de constantes données pour que ca compile au début.
// A vous de faire mieux si ca vous arrange  !
enum { NB_SAMPLE_TYPES = 2 };
enum { NB_COLOR_COMPONENTS = 3 };

/*
    Table des nombres de symboles par longueur, indexée par le type
    d'échantillon (DC ou AC), la composante de couleur (Y, Cb ou Cr) et la
    longueur du symbole (comprise entre 0 et 15).
*/
extern const uint8_t htables_nb_symb_per_lengths[NB_SAMPLE_TYPES][NB_COLOR_COMPONENTS][16];

/*
    Table des symboles, indexée par le type d'échantillon (DC ou AC) et la
    composante de couleur (Y, Cb ou Cr).
*/
extern const uint8_t *htables_symbols[NB_SAMPLE_TYPES][NB_COLOR_COMPONENTS];

/*
    Table des longueurs des tableaux de symboles, indexée par le type
    d'échantillon (DC ou AC) et la composante de couleur (Y, Cb ou Cr).
*/
extern const uint8_t htables_nb_symbols[NB_SAMPLE_TYPES][NB_COLOR_COMPONENTS];

#endif /* _HTABLES_H_ */
