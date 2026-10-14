#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "ecriture_entete.h"



bool ecrire_SOI(FILE *f) {

    if (f == NULL)
    {
        return false;
    }

    uint8_t buf[2];
    buf[0] = DEBUT_MARQUEUR;
    buf[1] = MARQUEUR_SOI;

    if (fwrite(buf, 1, 2, f) != 2)
    {
        return false;
    }

    return true;
}

bool ecrire_EOI(FILE *f) {

    if (f == NULL)
    {
        return false;
    }

    uint8_t buf[2];
    buf[0] = DEBUT_MARQUEUR;
    buf[1] = MARQUEUR_EOI;

    if (fwrite(buf, 1, 2, f) != 2)
    {
        return false;
    }

    return true;
}

bool ecrire_appx(FILE *f)
{
    if (f == NULL)
    {
        return false;
    }

    uint8_t buf[18];
    buf[0] = DEBUT_MARQUEUR;
    buf[1] = MARQUEUR_APP0;

    buf[2] = 0;
    buf[3] = 16;

    memcpy(&buf[4], "JFIF\0", 5);

    buf[9] = 1;
    buf[10] = 1;

    for (int i = 11; i < 18; i++)
    {
        buf[i] = 0;
    }

    if (fwrite(buf, 1, 18, f) != 18)
    {
        return false;
    }

    return true;
}

bool ecrire_DQT(FILE *f, const uint8_t tab_quantif[64], bool tab_est_CbCr)
{

    if (f == NULL)
    {
        return false;
    }

    uint8_t buf[5];
    buf[0] = DEBUT_MARQUEUR;
    buf[1] = MARQEUR_DQT;
    buf[2] = 0;
    buf[3] = 67;

    if (tab_est_CbCr)
    {
        buf[4] = ID_QUANTIFICATION_CBCR;
    }
    else
    {
        buf[4] = ID_QUANTIFICATION_Y;
    }

    if (fwrite(buf, 1, 5, f) != 5)
    {
        return false;
    }
    if (fwrite(tab_quantif, 1, 64, f) != 64)
    {
        return false;
    }
    return true;
}

bool ecrire_SOFx(FILE *f, uint16_t hauteur_image, uint16_t largeur_image, bool couleur, Facteurs_echantillonnage facteurs)
{

    if (f == NULL)
    {
        return false;
    }

    uint8_t nombre_de_composantes = couleur ? 3 : 1;
    uint8_t taille_marqueur = 8 + 3 * nombre_de_composantes;
    uint8_t buf[19];

    buf[0] = DEBUT_MARQUEUR;
    buf[1] = MARQUEUR_SOF0;
    buf[2] = 0;
    buf[3] = taille_marqueur;
    buf[4] = 8;
    buf[5] = (hauteur_image >> 8) & 0xFF;
    buf[6] = hauteur_image & 0xFF;
    buf[7] = (largeur_image >> 8) & 0xFF;
    buf[8] = largeur_image & 0xFF;
    buf[9] = nombre_de_composantes;

    buf[10] = ID_COMPOSANTE_Y;
    buf[11] = facteurs.h1 << 4 | facteurs.v1;
    buf[12] = ID_QUANTIFICATION_Y;

    if (couleur)
    {
        buf[13] = ID_COMPOSANTE_CB;
        buf[14] = facteurs.h2 << 4 | facteurs.v2;
        buf[15] = ID_QUANTIFICATION_CBCR;
        buf[16] = ID_COMPOSANTE_CR;
        buf[17] = facteurs.h3 << 4 | facteurs.v3;
        buf[18] = ID_QUANTIFICATION_CBCR;
    }

    if (fwrite(buf, 1, (taille_marqueur + 2), f) != ((size_t)taille_marqueur + 2))
    {
        return false;
    }

    return true;
}

bool ecrire_DHT(FILE *f, bool ac, const uint8_t longueurs_huffman[16], const uint8_t *table_symboles_huffman, const uint8_t taille_table_huffman, uint8_t id_composante)
{
    if (f == NULL)
    {
        return false;
    }

    uint8_t buf[5];
    uint16_t taille_marqueur = 19 + taille_table_huffman;

    buf[0] = DEBUT_MARQUEUR;
    buf[1] = MARQUEUR_DHT;
    buf[2] = (taille_marqueur >> 8) & 0xFF;
    buf[3] = taille_marqueur & 0xFF;
    buf[4] = (uint8_t)ac << 4 | id_composante;

    if (fwrite(buf, 1, 5, f) != 5)
    {
        return false;
    }

    if (fwrite(longueurs_huffman, 1, 16, f) != 16)
    {
        return false;
    }

    if (fwrite(table_symboles_huffman, 1, taille_table_huffman, f) != taille_table_huffman)
    {
        return false;
    }

    return true;
}

bool ecrire_SOS(FILE *f, bool couleur)
{
    if (f == NULL)
    {
        return false;
    }

    uint8_t nombre_de_composantes = couleur ? 3 : 1;
    uint8_t taille_marqueur = 6 + 2 * nombre_de_composantes;
    uint8_t buf[14];

    buf[0] = DEBUT_MARQUEUR;
    buf[1] = MARQUEUR_SOS;
    buf[2] = 0;
    buf[3] = taille_marqueur;
    buf[4] = nombre_de_composantes;

    buf[5] = ID_COMPOSANTE_Y;
    buf[6] = 0 << 4 | 0;

    if (couleur)
    {
        buf[7] = ID_COMPOSANTE_CB;
        buf[8] = 1 << 4 | 1;
        buf[9] = ID_COMPOSANTE_CR;
        buf[10] = 1 << 4 | 1;
    }

    buf[taille_marqueur - 1] = 0;
    buf[taille_marqueur] = 63;
    buf[taille_marqueur + 1] = 0;

    if (fwrite(buf, 1, (taille_marqueur + 2), f) != ((size_t)taille_marqueur + 2))
    {
        return false;
    }

    return true;
}

bool ecrire_entete(
    FILE *f, uint16_t hauteur_image, uint16_t largeur_image, bool couleur, Facteurs_echantillonnage facteurs,
    const uint8_t *table_q_y, const uint8_t *table_q_cbcr,
    const uint8_t longueurs_huffman[NB_SAMPLE_TYPES][NB_COLOR_COMPONENTS][16],
    const uint8_t *table_symboles_huffman[NB_SAMPLE_TYPES][NB_COLOR_COMPONENTS],
    const uint8_t taille_table_huffman[NB_SAMPLE_TYPES][NB_COLOR_COMPONENTS]
) {

    if (!ecrire_appx(f)) {
        return false;
    }

    if (!ecrire_DQT(f, table_q_y, false)) {
        return false;
    }

    if (couleur) {
        if (!ecrire_DQT(f, table_q_cbcr, true)) {
            return false;
        }
    }

    if (!ecrire_SOFx(f, hauteur_image, largeur_image, couleur, facteurs)) {
        return false;
    }
    
    if (!ecrire_DHT(f, false,   longueurs_huffman[ID_DC][ID_QUANTIFICATION_Y],     table_symboles_huffman[ID_DC][ID_QUANTIFICATION_Y],    taille_table_huffman[ID_DC][ID_QUANTIFICATION_Y],      ID_QUANTIFICATION_Y)) {
        return false;
    }

    if (!ecrire_DHT(f, true,    longueurs_huffman[ID_AC][ID_QUANTIFICATION_Y],     table_symboles_huffman[ID_AC][ID_QUANTIFICATION_Y],    taille_table_huffman[ID_AC][ID_QUANTIFICATION_Y],      ID_QUANTIFICATION_Y)) {
        return false;
    }

    if (couleur) {

        if (!ecrire_DHT(f, false,   longueurs_huffman[ID_DC][ID_QUANTIFICATION_CBCR],  table_symboles_huffman[ID_DC][ID_QUANTIFICATION_CBCR], taille_table_huffman[ID_DC][ID_QUANTIFICATION_CBCR],   ID_QUANTIFICATION_CBCR)) {
            return false;
        }

        if (!ecrire_DHT(f, true,    longueurs_huffman[ID_AC][ID_QUANTIFICATION_CBCR],  table_symboles_huffman[ID_AC][ID_QUANTIFICATION_CBCR], taille_table_huffman[ID_AC][ID_QUANTIFICATION_CBCR],   ID_QUANTIFICATION_CBCR)) {
            return false;
        }
    }
    
    if (!ecrire_SOS(f, couleur)) {
        return false;
    }

    return true;
}