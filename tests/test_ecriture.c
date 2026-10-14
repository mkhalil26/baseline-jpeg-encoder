#include <stdlib.h>
#include <stdbool.h>
#include "unity.h"
#include "ecriture.h"
#include "ecriture_entete.h"



void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}

void test_ecriture_vide(void) {

    char *chemin_sortie = "test.jpg";

    Flux_Ecriture flux;
    ouvrir_fichier_sortie(chemin_sortie, &flux);
    TEST_ASSERT_NOT_NULL(flux.fichier_sortie);

    fermer_fichier_sortie(&flux);
    
    FILE *fichier = fopen(chemin_sortie, "rb");
    TEST_ASSERT_NOT_NULL(fichier);

    uint8_t octets[4];
    int octets_lus = fread(octets, sizeof(uint8_t), 4, fichier);
    
    TEST_ASSERT_EQUAL_UINT8(4, octets_lus);
    TEST_ASSERT_EQUAL_UINT8(DEBUT_MARQUEUR, octets[0]);
    TEST_ASSERT_EQUAL_UINT8(MARQUEUR_SOI, octets[1]);
    TEST_ASSERT_EQUAL_UINT8(DEBUT_MARQUEUR, octets[2]);
    TEST_ASSERT_EQUAL_UINT8(MARQUEUR_EOI, octets[3]);

    remove(chemin_sortie);
}

void test_ecriture_octets_bits(void) {

    char *chemin_sortie = "test.jpg";

    Flux_Ecriture flux;
    ouvrir_fichier_sortie(chemin_sortie, &flux);
    TEST_ASSERT_NOT_NULL(flux.fichier_sortie);

    uint16_t bits_1 = OCTET_BYTE_STUFFING;
    ajouter_bits(bits_1, 8, &flux);

    uint16_t bits_2 = 0b0000001101110101;
    uint16_t bits_3 = 0b0000000000001000;
    ajouter_bits(bits_2, 10, &flux);
    ajouter_bits(bits_3, 4, &flux);

    fermer_fichier_sortie(&flux);
    


    FILE *fichier = fopen(chemin_sortie, "rb");
    TEST_ASSERT_NOT_NULL(fichier);

    uint8_t octets[8];
    int octets_lus = fread(octets, sizeof(uint8_t), 8, fichier);
    
    TEST_ASSERT_EQUAL_UINT8(8, octets_lus);
    TEST_ASSERT_EQUAL_UINT8(DEBUT_MARQUEUR, octets[0]);
    TEST_ASSERT_EQUAL_UINT8(MARQUEUR_SOI, octets[1]);

    TEST_ASSERT_EQUAL_UINT8(OCTET_BYTE_STUFFING, octets[2]);
    TEST_ASSERT_EQUAL_UINT8(OCTET_AJOUT_BYTE_STUFFING, octets[3]);
    TEST_ASSERT_EQUAL_UINT8(0b11011101, octets[4]);
    TEST_ASSERT_EQUAL_UINT8(0b01100011, octets[5]);
    
    TEST_ASSERT_EQUAL_UINT8(DEBUT_MARQUEUR, octets[6]);
    TEST_ASSERT_EQUAL_UINT8(MARQUEUR_EOI, octets[7]);

    remove(chemin_sortie);
}

void test_ecriture_ac_dc(void) {

    char *chemin_sortie = "test.jpg";

    Flux_Ecriture flux;
    ouvrir_fichier_sortie(chemin_sortie, &flux);
    TEST_ASSERT_NOT_NULL(flux.fichier_sortie);

    AC_DC ac_dc;
    CodageHuffman dc =    {.magnitude.indice = 0b10,   .magnitude.classe = 2,    .code = 0b0110000,   .nb_bits = 7};
    CodageHuffman ac_1 =  {.magnitude.indice = 0,      .magnitude.classe = 0,    .code = 0b001,       .nb_bits = 3};
    CodageHuffman ac_2 =  {.magnitude.indice = 0b1011, .magnitude.classe = 5,    .code = 0b10,        .nb_bits = 9};
    ac_dc.DC = dc;
    ac_dc.AC[0] = ac_1;
    ac_dc.AC[1] = ac_2;
    ac_dc.taille = 2;

    ajouter_donnees_compressees(&ac_dc, &flux);

    fermer_fichier_sortie(&flux);
    


    FILE *fichier = fopen(chemin_sortie, "rb");
    TEST_ASSERT_NOT_NULL(fichier);

    uint8_t octets[9];
    int octets_lus = fread(octets, sizeof(uint8_t), 9, fichier);
    
    TEST_ASSERT_EQUAL_UINT8(9, octets_lus);
    TEST_ASSERT_EQUAL_UINT8(DEBUT_MARQUEUR, octets[0]);
    TEST_ASSERT_EQUAL_UINT8(MARQUEUR_SOI, octets[1]);

    TEST_ASSERT_EQUAL_UINT8(0b01100001, octets[2]);
    TEST_ASSERT_EQUAL_UINT8(0b00010000, octets[3]);
    TEST_ASSERT_EQUAL_UINT8(0b00010010, octets[4]);
    TEST_ASSERT_EQUAL_UINT8(OCTET_BYTE_STUFFING, octets[5]);
    TEST_ASSERT_EQUAL_UINT8(OCTET_AJOUT_BYTE_STUFFING, octets[6]);

    TEST_ASSERT_EQUAL_UINT8(DEBUT_MARQUEUR, octets[7]);
    TEST_ASSERT_EQUAL_UINT8(MARQUEUR_EOI, octets[8]);

    remove(chemin_sortie);
}

int main(void) {

    UNITY_BEGIN();
    RUN_TEST(test_ecriture_vide);
    RUN_TEST(test_ecriture_octets_bits);
    RUN_TEST(test_ecriture_ac_dc);

    return UNITY_END();
}