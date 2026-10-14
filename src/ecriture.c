#include <math.h>
#include "ecriture.h"
#include "ecriture_entete.h"



bool ouvrir_fichier_sortie(const char *chemin_sortie, Flux_Ecriture *flux) {

    FILE *fichier = fopen(chemin_sortie, "wb");
    if (fichier == NULL) return false;

    flux->fichier_sortie = fichier;
    flux->buffer_bits.taille = 0;
    flux->buffer_octets.taille = 0;

    if (!ecrire_SOI(fichier)) return false;

    return true;
}

// écrit le contenu du buffer d'octets dans le fichier de sortie puis le vide
void ecrire_donnees(Flux_Ecriture *flux) {

    fwrite(flux->buffer_octets.buffer, sizeof(uint8_t), flux->buffer_octets.taille, flux->fichier_sortie);

    flux->buffer_octets.taille = 0;
}

// ajoute l'octet eu buffer d'octets et effectue le byte stuffing si nécéssaire
void ecrire_octet_buffer(uint8_t octet_a_ecrire, Flux_Ecriture *flux) {

    flux->buffer_octets.buffer[flux->buffer_octets.taille] = octet_a_ecrire;
    flux->buffer_octets.taille += 1;

    if (octet_a_ecrire == OCTET_BYTE_STUFFING) {
        flux->buffer_octets.buffer[flux->buffer_octets.taille] = OCTET_AJOUT_BYTE_STUFFING;
        flux->buffer_octets.taille += 1;
    }

    if (flux->buffer_octets.taille >= LIMITE_ECRITURE) {
        ecrire_donnees(flux);
    }
}

// ajoute au buffer d'octets les paquets de 8 bits disponibles dans le buffer de bits
void vider_buffer_bits(Flux_Ecriture *flux) {

    while (flux->buffer_bits.taille >= 8) {

        uint8_t octet = (flux->buffer_bits.buffer >> (flux->buffer_bits.taille - 8));
        flux->buffer_bits.taille -= 8;

        ecrire_octet_buffer(octet, flux);
    }
}

void ajouter_bits(uint16_t bits, uint8_t taille_bits, Flux_Ecriture *flux) {

    if (taille_bits == 0) return;

    flux->buffer_bits.buffer = (flux->buffer_bits.buffer << taille_bits) | bits;
    flux->buffer_bits.taille += taille_bits;

    vider_buffer_bits(flux);
}

void completer_derniers_bits(Flux_Ecriture *flux) {
    
    if (flux->buffer_bits.taille == 0) return;

    if ((flux->buffer_bits.taille % 8 ) == 0) {
        
        vider_buffer_bits(flux);
        return;
    }

    uint8_t taille_complement = 8 - (flux->buffer_bits.taille % 8);
    uint16_t complement = (1 << taille_complement) - 1;
    ajouter_bits(complement, taille_complement, flux);

    flux->buffer_bits.taille = 0;
}

void fermer_fichier_sortie(Flux_Ecriture *flux) {

    completer_derniers_bits(flux);
    ecrire_donnees(flux);
    ecrire_EOI(flux->fichier_sortie);

    fclose(flux->fichier_sortie);
}

void ajouter_donnees_compressees(const AC_DC *coefficients_ac_dc, Flux_Ecriture *flux) {

    ajouter_bits(coefficients_ac_dc->DC.code, coefficients_ac_dc->DC.nb_bits, flux);
    ajouter_bits(coefficients_ac_dc->DC.magnitude.indice, coefficients_ac_dc->DC.magnitude.classe, flux);

    for (uint8_t i = 0; i < coefficients_ac_dc->taille; i++) {

        ajouter_bits(coefficients_ac_dc->AC[i].code, coefficients_ac_dc->AC[i].nb_bits, flux);
        ajouter_bits(coefficients_ac_dc->AC[i].magnitude.indice, coefficients_ac_dc->AC[i].magnitude.classe, flux);
    }
}