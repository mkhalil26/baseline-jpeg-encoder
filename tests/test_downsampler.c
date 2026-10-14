#include "unity.h"
#include "downsampler.h"
#include "lecture.h"



void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}

void test_simple_2x2(void) {
    
    Facteurs_echantillonnage facteurs = {2,2,1,1,1,1};

    Dimensions_cbcr dim_cb_cr = determiner_dimensions_cb_cr(facteurs);

    uint8_t hauteur_mcu, largeur_mcu;
    determiner_facteurs_mcu(facteurs, &largeur_mcu, &hauteur_mcu);

    Couleur_rgb matrice[MCU_MAX][MCU_MAX];
    int val = 0;

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            
            for (int k = 0; k < 8; k++) {
                for (int l = 0; l < 8; l++) {

                    Couleur_rgb couleur = {val, val * 2, val * 3};
                    matrice[i * 8 + k][j * 8 + l] = couleur;
                }
            }

            val++;
        }
    }

    Vecteurs_ycbcr vecteurs_sortie;
    decouper_matrices_ycbcr(matrice, largeur_mcu, hauteur_mcu, dim_cb_cr, &vecteurs_sortie);

    TEST_ASSERT_EQUAL_UINT8(6,vecteurs_sortie.nb_vecteurs);

    for (uint8_t i = 0; i < vecteurs_sortie.nb_vecteurs; i++) {
        
        if (i < 4) {

            for (uint8_t j = 0; j < 64; j++) {

                Couleur_rgb couleur = {i, 2*i, 3*i};
                int16_t valeur_attendue = (int16_t)calculer_y(couleur) - 128;
                int16_t valeur_trouvee = (int16_t)vecteurs_sortie.vecteurs[i].valeur[j];

                TEST_ASSERT_EQUAL_INT16(valeur_attendue, valeur_trouvee);
            }

            TEST_ASSERT_EQUAL_UINT8(Y,vecteurs_sortie.vecteurs[i].composante);
        
        } else if (i == 4) {

            uint8_t valeurs_attendues[64] = {
                0,0,0,0,2,2,2,2,
                0,0,0,0,2,2,2,2,
                0,0,0,0,2,2,2,2,
                0,0,0,0,2,2,2,2,
                4,4,4,4,6,6,6,6,
                4,4,4,4,6,6,6,6,
                4,4,4,4,6,6,6,6,
                4,4,4,4,6,6,6,6,
            };

            for (uint8_t j = 0; j < 64; j++) {
                
                Couleur_rgb couleur = {valeurs_attendues[j] / 2, valeurs_attendues[j], valeurs_attendues[j] * 3 / 2};
                int16_t valeur_attendue = (int16_t)calculer_cb(couleur) - 128;
                int16_t valeur_trouvee = (int16_t)vecteurs_sortie.vecteurs[i].valeur[j];

                TEST_ASSERT_EQUAL_INT16(valeur_attendue, valeur_trouvee);
            }

            TEST_ASSERT_EQUAL_UINT8(CB,vecteurs_sortie.vecteurs[i].composante);

        } else  if (i == 5) {
            
            uint8_t valeurs_attendues[64] = {
                0,0,0,0,3,3,3,3,
                0,0,0,0,3,3,3,3,
                0,0,0,0,3,3,3,3,
                0,0,0,0,3,3,3,3,
                6,6,6,6,9,9,9,9,
                6,6,6,6,9,9,9,9,
                6,6,6,6,9,9,9,9,
                6,6,6,6,9,9,9,9,
            };
            for (uint8_t j = 0; j < 64; j++) {

                Couleur_rgb couleur = {valeurs_attendues[j] / 3, valeurs_attendues[j] / 3 * 2, valeurs_attendues[j]};
                int16_t valeur_attendue = (int16_t)calculer_cr(couleur) - 128;
                int16_t valeur_trouvee = (int16_t)vecteurs_sortie.vecteurs[i].valeur[j];

                TEST_ASSERT_EQUAL_INT16(valeur_attendue, valeur_trouvee);
            }

            TEST_ASSERT_EQUAL_UINT8(CR,vecteurs_sortie.vecteurs[i].composante);
        }
    }
}
 
void test_aucune_perte_1x1(void) {
    
    Facteurs_echantillonnage facteurs = {1,1,1,1,1,1};

    Dimensions_cbcr dim_cb_cr = determiner_dimensions_cb_cr(facteurs);

    uint8_t hauteur_mcu, largeur_mcu;
    determiner_facteurs_mcu(facteurs, &largeur_mcu, &hauteur_mcu);

    Couleur_rgb matrice[MCU_MAX][MCU_MAX];
    int val = 0;

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            
            Couleur_rgb couleur = {val, val * 2, val * 3};
            matrice[i][j] = couleur;
            val++;
        }
    }

    Vecteurs_ycbcr vecteurs_sortie;
    decouper_matrices_ycbcr(matrice, largeur_mcu, hauteur_mcu, dim_cb_cr, &vecteurs_sortie);

    TEST_ASSERT_EQUAL_UINT8(3,vecteurs_sortie.nb_vecteurs);

    for (uint8_t i = 0; i < vecteurs_sortie.nb_vecteurs; i++) { // test valeurs
        
        Composante c;
        switch (i) {
            
            case 0:

                {
                    c = Y;
                    for (uint8_t j = 0; j < 64; j++) {
                        
                        Couleur_rgb couleur = {j, 2*j, 3*j};
                        int16_t valeur_attendue = calculer_y(couleur) - 128;
                        int16_t valeur_trouvee = (int16_t)vecteurs_sortie.vecteurs[i].valeur[j];

                        TEST_ASSERT_EQUAL_INT16(valeur_attendue, valeur_trouvee);
                    }                    
                    break;
                }

            case 1:

                {
                    c = CB;
                    for (uint8_t j = 0; j < 64; j++) {
                        
                        Couleur_rgb couleur = {j, 2*j, 3*j};
                        int16_t valeur_attendue = calculer_cb(couleur) - 128;
                        int16_t valeur_trouvee = (int16_t)vecteurs_sortie.vecteurs[i].valeur[j];

                        TEST_ASSERT_EQUAL_INT16(valeur_attendue, valeur_trouvee);
                    } 
                    break;
                }

            default:

                {
                    c = CR;
                    for (uint8_t j = 0; j < 64; j++) {

                        Couleur_rgb couleur = {j, 2*j, 3*j};
                        int16_t valeur_attendue = calculer_cr(couleur) - 128;
                        int16_t valeur_trouvee = (int16_t)vecteurs_sortie.vecteurs[i].valeur[j];

                        TEST_ASSERT_EQUAL_INT16(valeur_attendue, valeur_trouvee);
                    } 
                    break;
                }
        }
        
        TEST_ASSERT_EQUAL_UINT8(c, vecteurs_sortie.vecteurs[i].composante);
    }
}

void test_aucune_perte_2x2(void) {
    
    Facteurs_echantillonnage facteurs = {2,2,2,2,2,2};

    Dimensions_cbcr dim_cb_cr = determiner_dimensions_cb_cr(facteurs);

    uint8_t hauteur_mcu, largeur_mcu;
    determiner_facteurs_mcu(facteurs, &largeur_mcu, &hauteur_mcu);

    Couleur_rgb matrice[MCU_MAX][MCU_MAX];
    int val = 0;

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            
            for (int k = 0; k < 8; k++) {
                for (int l = 0; l < 8; l++) {

                    Couleur_rgb couleur = {val, val * 2, val * 3};
                    matrice[i * 8 + k][j * 8 + l] = couleur;
                }
            }

            val++;
        }
    }

    Vecteurs_ycbcr vecteurs_sortie;
    decouper_matrices_ycbcr(matrice, largeur_mcu, hauteur_mcu, dim_cb_cr, &vecteurs_sortie);

    TEST_ASSERT_EQUAL_UINT8(12,vecteurs_sortie.nb_vecteurs);

    uint8_t l = 0;
    for (uint8_t i = 0; i < 3; i++) { // test valeurs

        for (uint8_t k = 0; k < 4; k++) {

            for (uint8_t j = 0; j < 64; j++) {
                
                Couleur_rgb couleur = {k, k*2, k*3};
                int16_t valeur_attendue;
                int16_t valeur_trouvee = (int16_t)vecteurs_sortie.vecteurs[l].valeur[j];
                
                if (i == 0) {
                    valeur_attendue = (int16_t)calculer_y(couleur) - 128;
                }
                else if (i == 1) {
                    valeur_attendue = (int16_t)calculer_cb(couleur) - 128;
                }
                else {
                    valeur_attendue = (int16_t)calculer_cr(couleur) - 128;
                }

                TEST_ASSERT_EQUAL_INT16(valeur_attendue, valeur_trouvee);
            }

            TEST_ASSERT_EQUAL_UINT8(i,vecteurs_sortie.vecteurs[l].composante);

            l++;
        }
    }
}
 
void test_3x3(void) {
    
    Facteurs_echantillonnage facteurs = {3,3,3,1,1,1};

    Dimensions_cbcr dim_cb_cr = determiner_dimensions_cb_cr(facteurs);

    uint8_t hauteur_mcu, largeur_mcu;
    determiner_facteurs_mcu(facteurs, &largeur_mcu, &hauteur_mcu);

    Couleur_rgb matrice[MCU_MAX][MCU_MAX];
    int val = 0;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            
            for (int k = 0; k < 8; k++) {
                for (int l = 0; l < 8; l++) {

                    Couleur_rgb couleur = {val, val * 2, val * 3};
                    matrice[i * 8 + k][j * 8 + l] = couleur;
                }
            }
            val++;
        }
    }

    Vecteurs_ycbcr vecteurs_sortie;
    decouper_matrices_ycbcr(matrice, largeur_mcu, hauteur_mcu, dim_cb_cr, &vecteurs_sortie);

    TEST_ASSERT_EQUAL_UINT8(13, vecteurs_sortie.nb_vecteurs);

    for (uint8_t i = 0; i < 9; i++) { // tests Y
        
        for (uint8_t j = 0; j < 64; j++) {

            Couleur_rgb couleur = {i, 2*i, 3*i};
            int16_t valeur_attendue = calculer_y(couleur) - 128;
            int16_t valeur_trouvee = vecteurs_sortie.vecteurs[i].valeur[j];

            TEST_ASSERT_EQUAL_INT16(valeur_attendue, valeur_trouvee);
        }

        TEST_ASSERT_EQUAL_UINT8(Y, vecteurs_sortie.vecteurs[i].composante);
    }

    uint8_t valeurs_attendues_cb_1[64] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        2, 2, 2, 2, 2, 2, 2, 2,
        6, 6, 6, 6, 6, 6, 6, 6,
        6, 6, 6, 6, 6, 6, 6, 6,
        10, 10, 10, 10, 10, 10, 10, 10,
        12, 12, 12, 12, 12, 12, 12, 12,
        12, 12, 12, 12, 12, 12, 12, 12
    };
    
    uint8_t valeurs_attendues_cb_2[64] = {
        2, 2, 2, 2, 2, 2, 2, 2,
        2, 2, 2, 2, 2, 2, 2, 2,
        4, 4, 4, 4, 4, 4, 4, 4,
        8, 8, 8, 8, 8, 8, 8, 8,
        8, 8, 8, 8, 8, 8, 8, 8,
        12, 12, 12, 12, 12, 12, 12, 12,
        14, 14, 14, 14, 14, 14, 14, 14,
        14, 14, 14, 14, 14, 14, 14, 14
    };

    uint8_t valeurs_attendues_cb_3[64] = {
        4, 4, 4, 4, 4, 4, 4, 4,
        4, 4, 4, 4, 4, 4, 4, 4,
        6, 6, 6, 6, 6, 6, 6, 6,
        10, 10, 10, 10, 10, 10, 10, 10,
        10, 10, 10, 10, 10, 10, 10, 10,
        14, 14, 14, 14, 14, 14, 14, 14,
        16, 16, 16, 16, 16, 16, 16, 16,
        16, 16, 16, 16, 16, 16, 16, 16
    };

    for (uint8_t j = 0; j < 64; j++) {
        
        Couleur_rgb couleur_1 = {valeurs_attendues_cb_1[j] / 2, valeurs_attendues_cb_1[j], valeurs_attendues_cb_1[j] * 3 / 2};
        int16_t valeur_attendue_cb_1 = (int16_t)calculer_cb(couleur_1) - 128;
        int16_t valeur_trouvee_cb_1 = vecteurs_sortie.vecteurs[9].valeur[j];

        TEST_ASSERT_EQUAL_INT16(valeur_attendue_cb_1, valeur_trouvee_cb_1);

        Couleur_rgb couleur_2 = {valeurs_attendues_cb_2[j] / 2, valeurs_attendues_cb_2[j], valeurs_attendues_cb_2[j] * 3 / 2};
        int16_t valeur_attendue_cb_2 = (int16_t)calculer_cb(couleur_2) - 128;
        int16_t valeur_trouvee_cb_2 = vecteurs_sortie.vecteurs[10].valeur[j];

        TEST_ASSERT_INT16_WITHIN(3, valeur_attendue_cb_2, valeur_trouvee_cb_2);

        Couleur_rgb couleur_3 = {valeurs_attendues_cb_3[j] / 2, valeurs_attendues_cb_3[j], valeurs_attendues_cb_3[j] * 3 / 2};
        int16_t valeur_attendue_cb_3 = (int16_t)calculer_cb(couleur_3) - 128;
        int16_t valeur_trouvee_cb_3 = vecteurs_sortie.vecteurs[11].valeur[j];

        TEST_ASSERT_INT16_WITHIN(2, valeur_attendue_cb_3, valeur_trouvee_cb_3);
    }

    TEST_ASSERT_EQUAL_UINT8(CB, vecteurs_sortie.vecteurs[9].composante);
    TEST_ASSERT_EQUAL_UINT8(CB, vecteurs_sortie.vecteurs[10].composante);
    TEST_ASSERT_EQUAL_UINT8(CB, vecteurs_sortie.vecteurs[11].composante);

    uint8_t valeurs_attendues_cr[64] = {
        0, 0, 1, 3, 3, 5, 6, 6,
        0, 0, 1, 3, 3, 5, 6, 6,
        3, 3, 4, 6, 6, 8, 9, 9,
        9, 9, 10, 12, 12, 14, 15, 15,
        9, 9, 10, 12, 12, 14, 15, 15,
        15, 15, 16, 18, 18, 20, 21, 21,
        18, 18, 19, 21, 21, 23, 24, 24,
        18, 18, 19, 21, 21, 23, 24, 24
    };

    for (uint8_t j = 0; j < 64; j++) {

        Couleur_rgb couleur = {valeurs_attendues_cr[j] / 2, valeurs_attendues_cr[j], valeurs_attendues_cr[j] * 3 / 2};
        int16_t valeur_attendue_cr = calculer_cr(couleur) - 128;
        int16_t valeur_trouvee = vecteurs_sortie.vecteurs[12].valeur[j];

        TEST_ASSERT_INT16_WITHIN(3, valeur_attendue_cr, valeur_trouvee);
    }

    TEST_ASSERT_EQUAL_UINT8(CR, vecteurs_sortie.vecteurs[12].composante);
}   
 

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_simple_2x2);
    RUN_TEST(test_aucune_perte_1x1);
    RUN_TEST(test_aucune_perte_2x2);
    RUN_TEST(test_3x3);
    return UNITY_END();
}
