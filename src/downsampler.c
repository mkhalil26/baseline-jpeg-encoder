#include <math.h>
#include <string.h>
#include "downsampler.h"
#include "rgb_to_ycbcr.h"
#include "commande.h"



Dimensions_cbcr determiner_dimensions_cb_cr(Facteurs_echantillonnage facteurs) {

	uint8_t largeur_mcu = 8 * facteurs.h1;
	uint8_t hauteur_mcu = 8 * facteurs.v1;

	Dimensions_cbcr dimensions = {
		largeur_mcu / (facteurs.h1 / facteurs.h2),
		hauteur_mcu / (facteurs.v1 / facteurs.v2),
		largeur_mcu / (facteurs.h1 / facteurs.h3),
		hauteur_mcu / (facteurs.v1 / facteurs.v3)
	};
	
	return dimensions;
}

float moyenne_micro_matrice(const Couleur_rgb matrice[MCU_MAX][MCU_MAX], uint8_t hauteur, uint8_t largeur, uint8_t i, uint8_t j, bool cb) {

	uint32_t somme = 0;
	uint16_t i_hauteur = i * hauteur;
	uint16_t j_largeur = j * largeur;

	if (cb) {

		for (uint8_t y = 0; y < hauteur; y++) {
			for (uint8_t x = 0; x < largeur; x++) {

				somme += calculer_cb(matrice[i_hauteur + y][j_largeur + x]) ;
			}
		}

	} else {

		for (uint8_t y = 0; y < hauteur; y++) {
			for (uint8_t x = 0; x < largeur; x++) {
				somme += calculer_cr(matrice[i_hauteur + y][j_largeur + x]);
			}
		}
	}

	return (float)somme / (largeur * hauteur);

}

void decouper_matrices_ycbcr(const Couleur_rgb matrice[MCU_MAX][MCU_MAX], uint8_t largeur_mcu, uint8_t hauteur_mcu, Dimensions_cbcr dimensions_sortie, Vecteurs_ycbcr *vecteurs_sortie) {

	uint8_t nb_vecteurs = 0;

	// calcul des vecteurs de la composante Y
	uint8_t nb_blocs_hauteur_y = hauteur_mcu / 8;
	uint8_t nb_blocs_largeur_y = largeur_mcu / 8;

	for (uint8_t i = 0; i < nb_blocs_hauteur_y; i++) {
		for (uint8_t j = 0; j < nb_blocs_largeur_y; j++) {

			Vecteur *vec = &(vecteurs_sortie->vecteurs[nb_vecteurs]);
			vec->composante = Y;

			for (uint8_t x = 0; x < 8; x++) {
				for (uint8_t y = 0; y < 8; y++) {
					
					vec->valeur[x * 8 + y] = calculer_y(matrice[8 * i + x][8 * j + y]) - 128.0; // -128 pour ne pas avoir à l'étape suivante de DCT
				}
			}

			nb_vecteurs++;
 		}
	}

	// calcul des vecteurs de la composante CB
	uint8_t largeur_micro_matrices_cb = largeur_mcu / dimensions_sortie.largeur_mcu_cb;
	uint8_t hauteur_micro_matrices_cb = hauteur_mcu / dimensions_sortie.hauteur_mcu_cb;
	uint8_t nb_blocs_largeur_cb = dimensions_sortie.largeur_mcu_cb / 8;
	uint8_t nb_blocs_hauteur_cb = dimensions_sortie.hauteur_mcu_cb / 8;

	for (uint8_t i = 0; i < nb_blocs_hauteur_cb; i++) {
		for (uint8_t j = 0; j < nb_blocs_largeur_cb; j++) {

			Vecteur *vec = &(vecteurs_sortie->vecteurs[nb_vecteurs]);
			vec->composante = CB;

			for (uint8_t x = 0; x < 8; x++) {
				for (uint8_t y = 0; y < 8; y++) {

					vec->valeur[x * 8 + y] = moyenne_micro_matrice(matrice, hauteur_micro_matrices_cb, largeur_micro_matrices_cb, i*8+x, j*8+y, true) - 128.0; // -128 pour ne pas avoir à l'étape suivante de DCT
				}
			}

			nb_vecteurs++;
		}
	}

	// calcul des vecteurs de la composante CR
	uint8_t largeur_micro_matrices_cr = largeur_mcu / dimensions_sortie.largeur_mcu_cr;
	uint8_t hauteur_micro_matrices_cr = hauteur_mcu / dimensions_sortie.hauteur_mcu_cr;
	uint8_t nb_blocs_largeur_cr = dimensions_sortie.largeur_mcu_cr / 8;
	uint8_t nb_blocs_hauteur_cr = dimensions_sortie.hauteur_mcu_cr / 8;

	for (uint8_t i = 0; i < nb_blocs_hauteur_cr; i++) {
		for (uint8_t j = 0; j < nb_blocs_largeur_cr; j++) {

			Vecteur *vec = &(vecteurs_sortie->vecteurs[nb_vecteurs]);
			vec->composante = CR;

			for (uint8_t x = 0; x < 8; x++) {
				for (uint8_t y = 0; y < 8; y++) {

					vec->valeur[x * 8 + y] = moyenne_micro_matrice(matrice, hauteur_micro_matrices_cr, largeur_micro_matrices_cr, i*8+x, j*8+y, false) - 128.0; // -128 pour ne pas avoir à l'étape suivante de DCT
				}
			}

			nb_vecteurs++;
		}
	}

	vecteurs_sortie->nb_vecteurs = nb_vecteurs;
}

void decouper_matrice_gris(const uint8_t matrice[8][8], Vecteur *vecteur) {

	vecteur->composante = Y;

	for (uint8_t x = 0; x < 8; x++) {
		for (uint8_t y = 0; y < 8; y++) {

			vecteur->valeur[x * 8 + y] = matrice[x][y] - 128.0; // -128 pour ne pas avoir à l'étape suivante de DCT
		}
	}
}