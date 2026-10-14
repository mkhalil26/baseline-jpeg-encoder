#include "unity.h"
#include "rgb_to_ycbcr.h"



void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}

void test_valeurs_min_y(void) {

    Couleur_rgb couleur = {0,0,0};

    TEST_ASSERT_UINT8_WITHIN(1, 0, calculer_y(couleur));
    TEST_ASSERT_UINT8_WITHIN(1, 128, calculer_cb(couleur));
    TEST_ASSERT_UINT8_WITHIN(1, 128, calculer_cr(couleur));
}

void test_valeurs_max_y(void) {

    Couleur_rgb couleur = {255,255,255};

    TEST_ASSERT_UINT8_WITHIN(1, 255, calculer_y(couleur));
    TEST_ASSERT_UINT8_WITHIN(1, 128, calculer_cb(couleur));
    TEST_ASSERT_UINT8_WITHIN(1, 128, calculer_cr(couleur));
}

void test_valeurs_min_cb(void) {

    Couleur_rgb couleur = {255,255,0};

    TEST_ASSERT_UINT8_WITHIN(1, 226, calculer_y(couleur));
    TEST_ASSERT_UINT8_WITHIN(1, 0, calculer_cb(couleur));
    TEST_ASSERT_UINT8_WITHIN(1, 149, calculer_cr(couleur));
}

void test_valeurs_max_cb(void) {

    Couleur_rgb couleur = {0,0,255};

    TEST_ASSERT_UINT8_WITHIN(1, 29, calculer_y(couleur));
    TEST_ASSERT_UINT8_WITHIN(1, 255, calculer_cb(couleur));
    TEST_ASSERT_UINT8_WITHIN(1, 107, calculer_cr(couleur));
}

void test_valeurs_min_cr(void) {

    Couleur_rgb couleur = {0,255,255};

    TEST_ASSERT_UINT8_WITHIN(1, 179, calculer_y(couleur));
    TEST_ASSERT_UINT8_WITHIN(1, 171, calculer_cb(couleur));
    TEST_ASSERT_UINT8_WITHIN(1, 0, calculer_cr(couleur));
}

void test_valeurs_max_cr(void) {

    Couleur_rgb couleur = {255,0,0};

    TEST_ASSERT_UINT8_WITHIN(1, 76, calculer_y(couleur));
    TEST_ASSERT_UINT8_WITHIN(1, 85, calculer_cb(couleur));
    TEST_ASSERT_UINT8_WITHIN(1, 255, calculer_cr(couleur));
}


int main(void) {

    UNITY_BEGIN();

    RUN_TEST(test_valeurs_min_y);
    RUN_TEST(test_valeurs_max_y);
    RUN_TEST(test_valeurs_min_cb);
    RUN_TEST(test_valeurs_max_cb);
    RUN_TEST(test_valeurs_min_cr);
    RUN_TEST(test_valeurs_max_cr);
    
    return UNITY_END();
}
