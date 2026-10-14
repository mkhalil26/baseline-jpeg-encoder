#include <stdlib.h>
#include <inttypes.h>
#include <time.h>
#include "commande.h"
#include "lecture.h"
#include "rgb_to_ycbcr.h"
#include "downsampler.h"
#include "dct.h"
#include "ecriture.h"
#include "ecriture_entete.h"
#include "dct.h"
#include "zigzag_quantification.h"



int main(int argc, char **argv) {
    
    // commande
    Parametres_commande parametres;

    bool res = initialiser_parametres_commande(argc, argv, &parametres, true);
    if (!res) {
        return EXIT_FAILURE;
    }

    bool help = help_demande(&parametres);
    if (help) {
        liberer_parametres_commande(&parametres);
        return EXIT_SUCCESS;
    }

    // lecture
    IterateurMCU iterateur;
    bool init = initialiser_iterateur_mcu(&iterateur, parametres.chemin_entree, parametres.facteurs);
    if (!init) {
        fprintf(stderr, "erreur d'initialisation de l'itérateur de lecture\n");
        liberer_parametres_commande(&parametres);
        return EXIT_FAILURE;
    }

    // ecriture 
    Flux_Ecriture flux;
    ouvrir_fichier_sortie(parametres.chemin_sortie, &flux);
    
    bool entete = ecrire_entete(
        flux.fichier_sortie, iterateur.image->hauteur, iterateur.image->largeur, iterateur.image->type == P6, parametres.facteurs, quantification_table_Y, quantification_table_CbCr,
        htables_nb_symb_per_lengths, htables_symbols, htables_nb_symbols
    );

    if (!entete) {
        fprintf(stderr, "erreur d'initialisation de l'entete dans le fichier de sortie\n");
        fermer_fichier_sortie(&flux);
        remove(parametres.chemin_sortie);
        liberer_parametres_commande(&parametres);
        return EXIT_FAILURE;
    }

    bool reste_mcu;
    AC_DC ac_dc;
    Vecteur vecteur;
    int16_t vecteur_entiers[64];

    initialise_huffman();

    if (image_couleur(&iterateur)) { // image couleur

        int16_t dc_prec_y_cb_cr[3] = {0, 0, 0};
        Couleur_rgb mcu_couleur[MCU_MAX][MCU_MAX];
        Vecteurs_ycbcr vecteurs;
        Dimensions_cbcr dim_cbcr = determiner_dimensions_cb_cr(parametres.facteurs);

        while (true) {

            reste_mcu = mcu_couleur_suivant(&iterateur, mcu_couleur);
            if (!reste_mcu) {
                break;
            }
            
            decouper_matrices_ycbcr(mcu_couleur, iterateur.largeur_mcu, iterateur.hauteur_mcu, dim_cbcr, &vecteurs);

            for (uint8_t i = 0; i < vecteurs.nb_vecteurs; i++) {

                vecteur = vecteurs.vecteurs[i];
                
                applique_dct(vecteur.valeur);
                applique_zigzag_quantification(vecteur.valeur, vecteur.composante, vecteur_entiers);

                bool res = magnitude_rle_huffman(&(dc_prec_y_cb_cr[vecteur.composante]), vecteur_entiers, vecteur.composante, &ac_dc);
                if (!res) {
                    fprintf(stderr, "Un symbole dans la table d'Huffman est invalide\n");
                    fermer_fichier_sortie(&flux);
                    remove(parametres.chemin_sortie);
                    liberer_iterateur_mcu(&iterateur);
                    liberer_parametres_commande(&parametres);
                    return EXIT_FAILURE;

                }

                ajouter_donnees_compressees(&ac_dc, &flux);
            }   
        }

    } else { // image en niveaux de gris 
        
        int16_t dc_prec = 0;
        uint8_t mcu_gris[8][8];

        while (true) {

            reste_mcu = mcu_gris_suivant(&iterateur, mcu_gris);
            if (!reste_mcu) {
                break;
            }
            
            decouper_matrice_gris(mcu_gris, &vecteur);    
            
            applique_dct(vecteur.valeur);
            applique_zigzag_quantification(vecteur.valeur, vecteur.composante, vecteur_entiers);

            magnitude_rle_huffman(&dc_prec, vecteur_entiers, Y, &ac_dc);

            ajouter_donnees_compressees(&ac_dc, &flux);
        }

    }

    fermer_fichier_sortie(&flux);

    // lecture
    liberer_iterateur_mcu(&iterateur);

    // commande
    liberer_parametres_commande(&parametres);

    return EXIT_SUCCESS;
}
