#include <stdlib.h>
#include <stdbool.h>
#include "unity.h"
#include "ecriture.h"
#include "ecriture_entete.h"
#include "qtables.h"
#include "htables.h"



void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}

void test_ecriture_SOI(void) {

    bool res = ecrire_SOI(NULL);

    TEST_ASSERT_EQUAL(false, res);

    FILE *fichier = fopen("test.jpg","wb");
    TEST_ASSERT_NOT_NULL(fichier);
    fclose(fichier);

    fichier = fopen("test.jpg","rb");
    TEST_ASSERT_NOT_NULL(fichier);

    res = ecrire_SOI(fichier);
    TEST_ASSERT_EQUAL(false, res);
    fclose(fichier);

    fichier = fopen("test.jpg","wb+");
    TEST_ASSERT_NOT_NULL(fichier);

    res = ecrire_SOI(fichier);
    uint8_t buf[2];
    fseek(fichier, 0, SEEK_SET);

    uint8_t lus = fread(buf, 1, 2, fichier);

    TEST_ASSERT_EQUAL_UINT8(2, lus);

    TEST_ASSERT_EQUAL_UINT8(DEBUT_MARQUEUR, buf[0]);
    TEST_ASSERT_EQUAL_UINT8(MARQUEUR_SOI, buf[1]);
    TEST_ASSERT_EQUAL(true, res);

    fclose(fichier);
    remove("test.jpg");
}

void test_ecriture_EOI(void) {

    bool res = ecrire_EOI(NULL);

    TEST_ASSERT_EQUAL(false, res);

    FILE *fichier = fopen("test.jpg","wb");
    TEST_ASSERT_NOT_NULL(fichier);
    fclose(fichier);

    fichier = fopen("test.jpg","rb");
    TEST_ASSERT_NOT_NULL(fichier);

    res = ecrire_EOI(fichier);
    TEST_ASSERT_EQUAL(false, res);
    fclose(fichier);

    fichier = fopen("test.jpg","wb+");
    TEST_ASSERT_NOT_NULL(fichier);

    res = ecrire_EOI(fichier);
    uint8_t buf[2];
    fseek(fichier, 0, SEEK_SET);

    uint8_t lus = fread(buf, 1, 2, fichier);

    TEST_ASSERT_EQUAL_UINT8(2, lus);

    TEST_ASSERT_EQUAL_UINT8(DEBUT_MARQUEUR, buf[0]);
    TEST_ASSERT_EQUAL_UINT8(MARQUEUR_EOI, buf[1]);
    TEST_ASSERT_EQUAL(true, res);

    fclose(fichier);
    remove("test.jpg");
}

void test_ecriture_appx(void) {

    bool res = ecrire_appx(NULL);

    TEST_ASSERT_EQUAL(false, res);

    FILE *fichier = fopen("test.jpg","wb");
    TEST_ASSERT_NOT_NULL(fichier);
    fclose(fichier);

    fichier = fopen("test.jpg","rb");
    TEST_ASSERT_NOT_NULL(fichier);

    res = ecrire_appx(fichier);
    TEST_ASSERT_EQUAL(false, res);
    fclose(fichier);

    fichier = fopen("test.jpg","wb+");
    TEST_ASSERT_NOT_NULL(fichier);

    res = ecrire_appx(fichier);
    uint8_t buf[18];
    fseek(fichier, 0, SEEK_SET);

    uint8_t lus = fread(buf, 1, 18, fichier);

    TEST_ASSERT_EQUAL_UINT8(18, lus);

    TEST_ASSERT_EQUAL_UINT8(DEBUT_MARQUEUR, buf[0]);
    TEST_ASSERT_EQUAL_UINT8(MARQUEUR_APP0, buf[1]);
    TEST_ASSERT_EQUAL_UINT8(0, buf[2]);
    TEST_ASSERT_EQUAL_UINT8(16, buf[3]);
    TEST_ASSERT_EQUAL_UINT8('J', buf[4]);
    TEST_ASSERT_EQUAL_UINT8('F', buf[5]);
    TEST_ASSERT_EQUAL_UINT8('I', buf[6]);
    TEST_ASSERT_EQUAL_UINT8('F', buf[7]);
    TEST_ASSERT_EQUAL_UINT8('\0', buf[8]);
    TEST_ASSERT_EQUAL_UINT8(1, buf[9]);
    TEST_ASSERT_EQUAL_UINT8(1, buf[10]);

    for (int i = 11; i < 18; i++)
    {
        TEST_ASSERT_EQUAL_UINT8(0, buf[i]);
    }

    TEST_ASSERT_EQUAL(true, res);

    fclose(fichier);
    remove("test.jpg");
}

void test_ecriture_DQT(void) {

    uint8_t tab_quantif[64];

    bool res = ecrire_DQT(NULL, NULL, false);

    TEST_ASSERT_EQUAL(false, res);

    FILE *fichier = fopen("test.jpg","wb");
    TEST_ASSERT_NOT_NULL(fichier);
    fclose(fichier);

    fichier = fopen("test.jpg","rb");
    TEST_ASSERT_NOT_NULL(fichier);

    res = ecrire_DQT(fichier, tab_quantif, false);
    TEST_ASSERT_EQUAL(false, res);
    fclose(fichier);

    fichier = fopen("test.jpg","wb+");
    TEST_ASSERT_NOT_NULL(fichier);

    res = ecrire_DQT(fichier, tab_quantif, false);
    uint8_t buf[69];
    fseek(fichier, 0, SEEK_SET);

    uint8_t lus = fread(buf, 1, 69, fichier);

    TEST_ASSERT_EQUAL_UINT8(69, lus);

    TEST_ASSERT_EQUAL_UINT8(DEBUT_MARQUEUR, buf[0]);
    TEST_ASSERT_EQUAL_UINT8(MARQEUR_DQT, buf[1]);
    TEST_ASSERT_EQUAL_UINT8(0, buf[2]);
    TEST_ASSERT_EQUAL_UINT8(67, buf[3]);
    TEST_ASSERT_EQUAL_UINT8(ID_QUANTIFICATION_Y, buf[4]);

    for (int i = 0; i < 64; i++)
    {
        TEST_ASSERT_EQUAL_UINT8(tab_quantif[i], buf[5 + i]);
    }

    TEST_ASSERT_EQUAL(true, res);

    fclose(fichier);


    fichier = fopen("test.jpg","wb+");
    TEST_ASSERT_NOT_NULL(fichier);

    res = ecrire_DQT(fichier, tab_quantif, true);
    fseek(fichier, 0, SEEK_SET);

    lus = fread(buf, 1, 69, fichier);

    TEST_ASSERT_EQUAL_UINT8(69, lus);

    TEST_ASSERT_EQUAL_UINT8(DEBUT_MARQUEUR, buf[0]);
    TEST_ASSERT_EQUAL_UINT8(MARQEUR_DQT, buf[1]);
    TEST_ASSERT_EQUAL_UINT8(0, buf[2]);
    TEST_ASSERT_EQUAL_UINT8(67, buf[3]);
    TEST_ASSERT_EQUAL_UINT8(ID_QUANTIFICATION_CBCR, buf[4]);

    for (int i = 0; i < 64; i++)
    {
        TEST_ASSERT_EQUAL_UINT8(tab_quantif[i], buf[5 + i]);
    }

    TEST_ASSERT_EQUAL(true, res);

    fclose(fichier);

    remove("test.jpg");
}

void test_ecriture_SOFx(void) {

    Facteurs_echantillonnage facteurs;

    bool res = ecrire_SOFx(NULL, 0, 0, false, facteurs);
    TEST_ASSERT_EQUAL(false, res);

    FILE *fichier = fopen("test.jpg","wb");
    TEST_ASSERT_NOT_NULL(fichier);
    fclose(fichier);

    fichier = fopen("test.jpg","rb");
    TEST_ASSERT_NOT_NULL(fichier);

    res = ecrire_SOFx(fichier, 0, 0, false, facteurs);
    TEST_ASSERT_EQUAL(false, res);
    fclose(fichier);

    fichier = fopen("test.jpg","wb+");
    TEST_ASSERT_NOT_NULL(fichier);

    res = ecrire_SOFx(fichier, 0, 0, false, facteurs);
    uint8_t buf[19];
    fseek(fichier, 0, SEEK_SET);

    uint8_t lus = fread(buf, 1, 13, fichier);

    TEST_ASSERT_EQUAL_UINT8(13, lus);

    TEST_ASSERT_EQUAL_UINT8(DEBUT_MARQUEUR, buf[0]);
    TEST_ASSERT_EQUAL_UINT8(MARQUEUR_SOF0, buf[1]);
    TEST_ASSERT_EQUAL_UINT8(0, buf[2]);
    TEST_ASSERT_EQUAL_UINT8(8, buf[4]);
    TEST_ASSERT_EQUAL_UINT8(1, buf[9]);
    TEST_ASSERT_EQUAL_UINT8(ID_COMPOSANTE_Y, buf[10]);
    TEST_ASSERT_EQUAL_UINT8(ID_QUANTIFICATION_Y, buf[12]);

    TEST_ASSERT_EQUAL(true, res);


    fclose(fichier);

    fichier = fopen("test.jpg","wb+");
    TEST_ASSERT_NOT_NULL(fichier);

    res = ecrire_SOFx(fichier, 0, 0, true, facteurs);
    fseek(fichier, 0, SEEK_SET);

    lus = fread(buf, 1, 19, fichier);

    TEST_ASSERT_EQUAL_UINT8(19, lus);

    TEST_ASSERT_EQUAL_UINT8(DEBUT_MARQUEUR, buf[0]);
    TEST_ASSERT_EQUAL_UINT8(MARQUEUR_SOF0, buf[1]);
    TEST_ASSERT_EQUAL_UINT8(0, buf[2]);
    TEST_ASSERT_EQUAL_UINT8(8, buf[4]);
    TEST_ASSERT_EQUAL_UINT8(3, buf[9]);
    TEST_ASSERT_EQUAL_UINT8(ID_COMPOSANTE_Y, buf[10]);
    TEST_ASSERT_EQUAL_UINT8(ID_QUANTIFICATION_Y, buf[12]);
    TEST_ASSERT_EQUAL_UINT8(ID_COMPOSANTE_CB, buf[13]);
    TEST_ASSERT_EQUAL_UINT8(ID_QUANTIFICATION_CBCR, buf[15]);
    TEST_ASSERT_EQUAL_UINT8(ID_COMPOSANTE_CR, buf[16]);
    TEST_ASSERT_EQUAL_UINT8(ID_QUANTIFICATION_CBCR, buf[18]);

    TEST_ASSERT_EQUAL(true, res);

    fclose(fichier);

    remove("test.jpg");
}

void test_ecriture_DHT(void) {

    uint8_t longueurs_huffman[16];

    for (uint8_t i = 0; i < 16; i++) {
        longueurs_huffman[i] = i;
    }

    uint8_t table_symboles_huffman[16];

    for (uint8_t i = 0; i < 16; i++) {
        table_symboles_huffman[i] = 2*i;
    }

    bool res = ecrire_DHT(NULL, false, longueurs_huffman, table_symboles_huffman, 16, ID_QUANTIFICATION_Y);
    TEST_ASSERT_EQUAL(false, res);

    FILE *fichier = fopen("test.jpg","wb");
    TEST_ASSERT_NOT_NULL(fichier);
    fclose(fichier);

    fichier = fopen("test.jpg","rb");
    TEST_ASSERT_NOT_NULL(fichier);

    res = ecrire_DHT(fichier, false, longueurs_huffman, table_symboles_huffman, 16, ID_QUANTIFICATION_Y);
    TEST_ASSERT_EQUAL(false, res);
    fclose(fichier);

    fichier = fopen("test.jpg","wb+");
    TEST_ASSERT_NOT_NULL(fichier);

    res = ecrire_DHT(fichier, false, longueurs_huffman, table_symboles_huffman, 16, ID_QUANTIFICATION_Y);
    uint8_t buf[37];
    fseek(fichier, 0, SEEK_SET);

    uint8_t lus = fread(buf, 1, 37, fichier);

    TEST_ASSERT_EQUAL_UINT8(37, lus);

    TEST_ASSERT_EQUAL_UINT8(DEBUT_MARQUEUR, buf[0]);
    TEST_ASSERT_EQUAL_UINT8(MARQUEUR_DHT, buf[1]);
    
    for (int i = 0; i < 16; i++)
    {
        TEST_ASSERT_EQUAL_UINT8(longueurs_huffman[i], buf[5 + i]);
    }

    for (int i = 0; i < 16; i++)
    {
        TEST_ASSERT_EQUAL_UINT8(table_symboles_huffman[i], buf[21 + i]);
    }

    TEST_ASSERT_EQUAL(true, res);


    fclose(fichier);

    remove("test.jpg");
}

void test_ecriture_SOS(void) {

    bool res = ecrire_SOS(NULL, false);
    TEST_ASSERT_EQUAL(false, res);

    FILE *fichier = fopen("test.jpg","wb");
    TEST_ASSERT_NOT_NULL(fichier);
    fclose(fichier);

    fichier = fopen("test.jpg","rb");
    TEST_ASSERT_NOT_NULL(fichier);

    res = ecrire_SOS(fichier, false);
    TEST_ASSERT_EQUAL(false, res);
    fclose(fichier);

    fichier = fopen("test.jpg","wb+");
    TEST_ASSERT_NOT_NULL(fichier);

    res = ecrire_SOS(fichier, false);
    uint8_t buf[14];
    fseek(fichier, 0, SEEK_SET);

    uint8_t lus = fread(buf, 1, 10, fichier);

    TEST_ASSERT_EQUAL_UINT8(10, lus);

    TEST_ASSERT_EQUAL_UINT8(DEBUT_MARQUEUR, buf[0]);
    TEST_ASSERT_EQUAL_UINT8(MARQUEUR_SOS, buf[1]);
    TEST_ASSERT_EQUAL_UINT8(0, buf[2]);
    TEST_ASSERT_EQUAL_UINT8(1, buf[4]);
    TEST_ASSERT_EQUAL_UINT8(ID_COMPOSANTE_Y, buf[5]);
    TEST_ASSERT_EQUAL_UINT8(0, buf[7]);
    TEST_ASSERT_EQUAL_UINT8(63, buf[8]);
    TEST_ASSERT_EQUAL_UINT8(0, buf[9]);

    TEST_ASSERT_EQUAL(true, res);

    fclose(fichier);


    fichier = fopen("test.jpg","wb+");
    TEST_ASSERT_NOT_NULL(fichier);

    res = ecrire_SOS(fichier, true);
    fseek(fichier, 0, SEEK_SET);

    lus = fread(buf, 1, 14, fichier);

    TEST_ASSERT_EQUAL_UINT8(14, lus);

    TEST_ASSERT_EQUAL_UINT8(DEBUT_MARQUEUR, buf[0]);
    TEST_ASSERT_EQUAL_UINT8(MARQUEUR_SOS, buf[1]);
    TEST_ASSERT_EQUAL_UINT8(0, buf[2]);
    TEST_ASSERT_EQUAL_UINT8(3, buf[4]);
    TEST_ASSERT_EQUAL_UINT8(ID_COMPOSANTE_Y, buf[5]);
    TEST_ASSERT_EQUAL_UINT8(ID_COMPOSANTE_CB, buf[7]);
    TEST_ASSERT_EQUAL_UINT8(ID_COMPOSANTE_CR, buf[9]);
    TEST_ASSERT_EQUAL_UINT8(0, buf[11]);
    TEST_ASSERT_EQUAL_UINT8(63, buf[12]);
    TEST_ASSERT_EQUAL_UINT8(0, buf[13]);

    TEST_ASSERT_EQUAL(true, res);

    fclose(fichier);


    remove("test.jpg");
}

void test_ecriture_entete(void) {

    Facteurs_echantillonnage facteurs = {2, 2, 1, 1, 1, 1};
    bool res = ecrire_entete(NULL, 0, 0, false, facteurs, NULL, NULL, NULL , NULL, NULL);
    TEST_ASSERT_EQUAL(false, res);

    FILE *fichier = fopen("test.jpg","wb+");
    TEST_ASSERT_NOT_NULL(fichier);

    res = ecrire_entete(fichier, 0, 0, false, facteurs, quantification_table_Y, quantification_table_CbCr, htables_nb_symb_per_lengths, htables_symbols, htables_nb_symbols);
    TEST_ASSERT_EQUAL(true, res);

    fclose(fichier);

    fichier = fopen("test.jpg","wb+");
    TEST_ASSERT_NOT_NULL(fichier);

    res = ecrire_entete(fichier, 0, 0, true, facteurs, quantification_table_Y, quantification_table_CbCr, htables_nb_symb_per_lengths, htables_symbols, htables_nb_symbols);
    TEST_ASSERT_EQUAL(true, res);

    fclose(fichier);

    remove("test.jpg");
}


int main(void) {

    UNITY_BEGIN();
    RUN_TEST(test_ecriture_SOI);
    RUN_TEST(test_ecriture_EOI);
    RUN_TEST(test_ecriture_appx);
    RUN_TEST(test_ecriture_DQT);
    RUN_TEST(test_ecriture_SOFx);
    RUN_TEST(test_ecriture_DHT);
    RUN_TEST(test_ecriture_SOS);
    RUN_TEST(test_ecriture_entete);

    return UNITY_END();
}