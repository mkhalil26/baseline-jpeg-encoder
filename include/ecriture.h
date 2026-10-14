#ifndef _ECRITURE_H_
#define _ECRITURE_H_

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "magnitude_rle_huffman.h"

#define TAILLE_BUFFER_ECRITURE 10000
#define LIMITE_ECRITURE 8192

#define OCTET_BYTE_STUFFING (uint8_t)0xff
#define OCTET_AJOUT_BYTE_STUFFING (uint8_t)0x00



typedef struct buffer_octets { // structure gérant l'ajout d'octets dans le fichier
	uint8_t buffer[TAILLE_BUFFER_ECRITURE];
	uint32_t taille;

} Buffer_Octets;

typedef struct buffer_bits { // structure gérant l'ajout de bits dans le fichier
	uint64_t buffer;
	uint8_t taille;

} Buffer_Bits;

typedef struct flux_ecriture { // structure gérant l'écriture dans le fichier de sortie
	Buffer_Octets buffer_octets;
	Buffer_Bits buffer_bits;
	FILE *fichier_sortie;

} Flux_Ecriture;


// ouvre le fichier ou sera écrit le contenu du JPEG
bool ouvrir_fichier_sortie(const char *chemin_sortie, Flux_Ecriture *flux);

// ajoute des bits dans le fichier de sortie
void ajouter_bits(uint16_t bits, uint8_t taille_bits, Flux_Ecriture *flux);

// aligne les bits dans le fichier en les complétant avec des bits à 1
void completer_derniers_bits(Flux_Ecriture *flux);

// ferme le fichier de sortie
void fermer_fichier_sortie(Flux_Ecriture *flux);

// ajoute les coefficients AC et DC compressés avec Huffman au flux d'écriture
void ajouter_donnees_compressees(const AC_DC *coefficients_ac_dc, Flux_Ecriture *flux);



#endif /* _ECRITURE_H_ */
