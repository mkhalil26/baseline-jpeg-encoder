#include <magnitude.h>
#include "unity.h"



void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}

void test_encoder_val(){

    Magnitude res=encoder_magnitude(7);
    TEST_ASSERT_EQUAL_UINT8(3, res.classe);
    TEST_ASSERT_EQUAL_UINT16(0b111,res.indice);
}

void test_encoder_val_complet(void) {
    Magnitude res;


    res = encoder_magnitude(0);
    TEST_ASSERT_EQUAL_UINT8_MESSAGE(0, res.classe, "Erreur Taille pour 0");
    TEST_ASSERT_EQUAL_UINT16_MESSAGE(0, res.indice, "Erreur Indice pour 0");


    res = encoder_magnitude(7);
    TEST_ASSERT_EQUAL_UINT8_MESSAGE(3, res.classe, "Erreur Taille pour 7");
    TEST_ASSERT_EQUAL_UINT16_MESSAGE(7, res.indice, "Erreur Indice pour 7");

    res = encoder_magnitude(12); 
    TEST_ASSERT_EQUAL_UINT8_MESSAGE(4, res.classe, "Erreur Taille pour 12");
    TEST_ASSERT_EQUAL_UINT16_MESSAGE(12, res.indice, "Erreur Indice pour 12");


    res = encoder_magnitude(-7);
    TEST_ASSERT_EQUAL_UINT8_MESSAGE(3, res.classe, "Erreur Taille pour -7");
    TEST_ASSERT_EQUAL_UINT16_MESSAGE(0, res.indice, "Erreur Indice pour -7"); 

    res = encoder_magnitude(-12);
 

    TEST_ASSERT_EQUAL_UINT8_MESSAGE(4, res.classe, "Erreur Taille pour -12");
    TEST_ASSERT_EQUAL_UINT16_MESSAGE(3, res.indice, "Erreur Indice pour -12");
}

int main(void) {

    
    UNITY_BEGIN();
    RUN_TEST(test_encoder_val);
    RUN_TEST(test_encoder_val_complet);

    return UNITY_END();
}