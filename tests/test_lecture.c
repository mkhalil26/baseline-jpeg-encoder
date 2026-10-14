#include <stdlib.h>
#include <stdbool.h>
#include "unity.h"
#include "lecture.h"



void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}
void test_recupEntete_P5(void) {

    char* filename = "images/etu/gris.pgm";
 
    FILE *f_test = fopen(filename, "rb");
    TEST_ASSERT_NOT_NULL(f_test);
    Image* img = recupEntete(f_test);
    
    TEST_ASSERT_NOT_NULL(img);
    TEST_ASSERT_EQUAL_INT(P5, img->type);
    TEST_ASSERT_EQUAL_UINT32(320, img->largeur);
    TEST_ASSERT_EQUAL_UINT32(320, img->hauteur);
    TEST_ASSERT_EQUAL_INT(15, img->debut_pixels);
    fclose(f_test);

    free(img); 
}

void test_recupEntete_fichier_type_invalide_1(void) {

    FILE *fichier = fopen("test.jpg", "wb");
    TEST_ASSERT_NOT_NULL(fichier);
    fprintf(fichier, "P");

    fclose(fichier);

    fichier = fopen("test.jpg", "rb");
    Image* img = recupEntete(fichier);
    TEST_ASSERT_NULL(img);

    fclose(fichier);

    remove("test.jpg");
}

void test_recupEntete_fichier_type_invalide_2(void) {

    FILE *fichier = fopen("test.jpg", "wb");
    TEST_ASSERT_NOT_NULL(fichier);
    fprintf(fichier, "P4\n");

    fclose(fichier);

    fichier = fopen("test.jpg", "rb");
    Image* img = recupEntete(fichier);
    TEST_ASSERT_NULL(img);

    fclose(fichier);

    remove("test.jpg");
}

void test_recupEntete_fichier_dimensions_invalide(void) {

    FILE *fichier = fopen("test.jpg", "wb");
    TEST_ASSERT_NOT_NULL(fichier);
    fprintf(fichier, "P5\n8 a\n");

    fclose(fichier);

    fichier = fopen("test.jpg", "rb");
    Image* img = recupEntete(fichier);
    TEST_ASSERT_NULL(img);

    fclose(fichier);

    remove("test.jpg");
}

void test_recupEntete_fichier_profondeur_invalide(void) {

    FILE *fichier = fopen("test.jpg", "wb");
    TEST_ASSERT_NOT_NULL(fichier);
    fprintf(fichier, "P5\n8 8\ntruc\n");

    fclose(fichier);

    fichier = fopen("test.jpg", "rb");
    Image* img = recupEntete(fichier);
    TEST_ASSERT_NULL(img);

    fclose(fichier);

    remove("test.jpg");
}

void test_recupEntete_fichier_taille_invalide(void) {

    FILE *fichier = fopen("test.jpg", "wb");
    TEST_ASSERT_NOT_NULL(fichier);
    fprintf(fichier, "P5\n8 8\n255\n");

    fclose(fichier);

    fichier = fopen("test.jpg", "rb");
    Image* img = recupEntete(fichier);
    TEST_ASSERT_NULL(img);

    fclose(fichier);

    remove("test.jpg");
}

void test_recupEntete(void) {

    FILE *fichier = fopen("test.jpg", "wb");
    TEST_ASSERT_NOT_NULL(fichier);
    fprintf(fichier, "P5\n0 0\n255\n");

    fclose(fichier);

    fichier = fopen("test.jpg", "rb");
    Image* img = recupEntete(fichier);
    
    TEST_ASSERT_NOT_NULL(img);
    TEST_ASSERT_EQUAL_INT(P5, img->type);
    TEST_ASSERT_EQUAL_UINT32(0, img->largeur);
    TEST_ASSERT_EQUAL_UINT32(0, img->hauteur);
    TEST_ASSERT_EQUAL_INT(11, img->debut_pixels);

    fclose(fichier);

    remove("test.jpg");
}

void test_lire_image(void) {
    
    char* filename = "images/etu/gris.pgm";
    
    uint32_t L_bloc = 8;
    uint32_t H_super = 8;
    uint32_t nb_blocs = 64;

    Image* img = lectureImage(filename);
    img = allouer_image(img, L_bloc, H_super, nb_blocs);

    TEST_ASSERT_NOT_NULL(img);
    TEST_ASSERT_NOT_NULL(img->fichier);
    TEST_ASSERT_EQUAL_INT(P5, img->type);
    TEST_ASSERT_EQUAL_UINT32(320, img->largeur);
    TEST_ASSERT_EQUAL_UINT32(320, img->hauteur);

    liberer_image(img, H_super);
}

void test_lectureImage_null(void) {

    Image* img = lectureImage(NULL);
    TEST_ASSERT_NULL(img);
}

void test_lectureImage_invalide(void) {

    Image* img = lectureImage("truc");
    TEST_ASSERT_NULL(img);
}

void test_taille_invalide(void) {

    FILE *fichier = fopen("test.jpg", "wb");
    TEST_ASSERT_NOT_NULL(fichier);
    fprintf(fichier, "a");
    fclose(fichier);

    Image* img = lectureImage("test.jpg");
    TEST_ASSERT_NULL(img);

    remove("test.jpg");
}

void test_lectureEblocs() {
    
    uint8_t motif_attendu[64] = {
        0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00,
        0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00,
        0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00,
        0xff, 0xff, 0x00, 0xff, 0xff, 0x00, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00,
        0x00, 0xff, 0x00, 0xff, 0xff, 0x00, 0xff, 0x00,
        0xff, 0x00, 0xff, 0x00, 0x00, 0xff, 0x00, 0xff
    };

    char* filename = "images/etu/invader.pgm";
    
    uint32_t L_bloc = 8;
    uint32_t H_super = 8;
    uint32_t nb_blocs = 64;

    Image* img = lectureImage(filename);
    img = allouer_image(img, L_bloc, H_super, nb_blocs);

    lireEblocs(img, 0, 0, L_bloc, H_super, nb_blocs);
    
    for (int i = 0; i < 8; i++) {
        char msg[50];
        snprintf(msg, 50, "Erreur de lecture à la ligne %d", i);
        TEST_ASSERT_EQUAL_HEX8_ARRAY_MESSAGE(motif_attendu + (i * 8), img->tab[i], 8, msg);
    }

    liberer_image(img, H_super);
}

void test_lireEblocs_grande_image(void) {

    uint32_t L = 1024;
    uint32_t H = 1024;
    char* filename = "test_large.pgm";
    
    uint32_t L_bloc = 8;
    uint32_t H_super = 8;
    uint32_t nb_blocs = 64;

    FILE *f = fopen(filename, "wb");
    fprintf(f, "P5\n%u %u\n255\n", L, H);
    for (uint32_t i = 0; i < L * H; i++) {
        fputc((uint8_t)(i % 256), f);
    }
    fclose(f);

    Image *img = lectureImage(filename);
    img = allouer_image(img, L_bloc, H_super, nb_blocs);

    TEST_ASSERT_NOT_NULL(img);

    uint32_t tx = 128;
    uint32_t ty = 200;
    lireEblocs(img, tx, ty, L_bloc, H_super, nb_blocs);

    for (uint32_t i = 0; i < 8; i++) {
        uint8_t attendu = (uint8_t)(((ty + i) * L + tx) % 256);
        char msg[50];
        snprintf(msg, 50, "Erreur à la ligne locale %u", i);
        TEST_ASSERT_EQUAL_HEX8_MESSAGE(attendu, img->tab[i][0], msg);
    }

    liberer_image(img, H_super);
    remove(filename);
}

void test_lireEblocs_grande_image_P6(void) {
    uint32_t L = 1024;
    uint32_t H = 1024;
    char* filename = "test_large_p6.ppm";
    
    uint32_t L_bloc = 8;
    uint32_t H_super = 8;
    uint32_t nb_blocs = 64;

    FILE *f = fopen(filename, "wb");
    fprintf(f, "P6\n%u %u\n255\n", L, H);
    for (uint32_t y = 0; y < H; y++) {
        for (uint32_t x = 0; x < L; x++) {
            fputc((uint8_t)(x % 256), f);
            fputc((uint8_t)(y % 256), f);
            fputc((uint8_t)((x + y) % 256), f);
        }
    }
    fclose(f);

    Image *img = lectureImage(filename);
    img = allouer_image(img, L_bloc, H_super, nb_blocs);

    TEST_ASSERT_NOT_NULL(img);
    TEST_ASSERT_EQUAL_INT(P6, img->type);

    uint32_t tx = 100;
    uint32_t ty = 50;
    lireEblocs(img, tx, ty, L_bloc, H_super, nb_blocs);

    uint8_t attendu_R = (uint8_t)(tx % 256);
    uint8_t attendu_G = (uint8_t)(ty % 256);
    uint8_t attendu_B = (uint8_t)((tx + ty) % 256);

    TEST_ASSERT_EQUAL_HEX8_MESSAGE(attendu_R, img->tab[0][0], "Erreur Canal Rouge");
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(attendu_G, img->tab[0][1], "Erreur Canal Vert");
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(attendu_B, img->tab[0][2], "Erreur Canal Bleu");

    uint8_t attendu_R1 = (uint8_t)((tx + 1) % 256);
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(attendu_R1, img->tab[0][3], "Erreur Canal Rouge Pixel suivant");

    for (uint32_t i = 0; i < 8; i++) {
        for (uint32_t j = 0; j < 3; j++) {
            
            uint32_t x_actuel = tx + j;
            uint32_t y_actuel = ty + i;

            uint8_t attendu_R = (uint8_t)(x_actuel % 256);
            uint8_t attendu_G = (uint8_t)(y_actuel % 256);
            uint8_t attendu_B = (uint8_t)((x_actuel + y_actuel) % 256);

            uint32_t base_index = j * 3;

            char msg[100];
            snprintf(msg, 100, "Erreur R: Ligne %u, Pixel %u", i, j);
            TEST_ASSERT_EQUAL_HEX8_MESSAGE(attendu_R, img->tab[i][base_index], msg);

            snprintf(msg, 100, "Erreur G: Ligne %u, Pixel %u", i, j);
            TEST_ASSERT_EQUAL_HEX8_MESSAGE(attendu_G, img->tab[i][base_index + 1], msg);

            snprintf(msg, 100, "Erreur B: Ligne %u, Pixel %u", i, j);
            TEST_ASSERT_EQUAL_HEX8_MESSAGE(attendu_B, img->tab[i][base_index + 2], msg);
        }
    }

    liberer_image(img, H_super);
    remove(filename);
}

void test_iterateur_mcu_invader(void) {

    uint8_t mcu_attendu[64] = {
        0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00,
        0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00,
        0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00,
        0xff, 0xff, 0x00, 0xff, 0xff, 0x00, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00,
        0x00, 0xff, 0x00, 0xff, 0xff, 0x00, 0xff, 0x00,
        0xff, 0x00, 0xff, 0x00, 0x00, 0xff, 0x00, 0xff
    };

    char* chemin = "images/etu/invader.pgm";

    Facteurs_echantillonnage facteurs = {1, 1, 1, 1, 1, 1};
    IterateurMCU iterateur;
    Couleur_rgb mcu[MCU_MAX][MCU_MAX];

    bool init = initialiser_iterateur_mcu(&iterateur, chemin, facteurs);

    TEST_ASSERT_EQUAL(true, init);

    bool reste_mcu = mcu_couleur_suivant(&iterateur, mcu);
    
    TEST_ASSERT_EQUAL(true, reste_mcu);
    
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {

            TEST_ASSERT_EQUAL_UINT8(mcu_attendu[i*8 + j], mcu[i][j].r);
            TEST_ASSERT_EQUAL_UINT8(mcu_attendu[i*8 + j], mcu[i][j].g);
            TEST_ASSERT_EQUAL_UINT8(mcu_attendu[i*8 + j], mcu[i][j].b);
        }
    }

    reste_mcu = mcu_couleur_suivant(&iterateur, mcu);
    TEST_ASSERT_EQUAL(false, reste_mcu);

    liberer_iterateur_mcu(&iterateur);
}

void test_iterateur_mcu_complexe(void) {

    uint32_t largeur = 15;
    uint32_t hauteur = 6;
    char* chemin = "complexe.ppm";
    
    FILE *f = fopen(chemin, "wb");
    fprintf(f, "P6\n%u %u\n255\n", largeur, hauteur);

    for (uint32_t i = 0; i < largeur * hauteur; i++) {
        fputc((uint8_t)(i % 256), f);
        fputc((uint8_t)((i + 1) % 256), f);
        fputc((uint8_t)((i + 2) % 256), f);
    }
    fclose(f);

    Facteurs_echantillonnage facteurs = {1, 1, 1, 1, 1, 1};
    IterateurMCU iterateur;
    Couleur_rgb mcu[MCU_MAX][MCU_MAX];

    bool init = initialiser_iterateur_mcu(&iterateur, chemin, facteurs);
    TEST_ASSERT_EQUAL(true, init);
    TEST_ASSERT_EQUAL_UINT32(2, iterateur.largeur_image_mcu);
    TEST_ASSERT_EQUAL_UINT32(1, iterateur.hauteur_image_mcu);
    
    bool reste_mcu = mcu_couleur_suivant(&iterateur, mcu);
    TEST_ASSERT_EQUAL(true, reste_mcu);

    for (uint32_t y = 0; y < 6; y++) {
        for (uint32_t x = 0; x < 8; x++) {

            uint32_t val_pixel = y * largeur + x;
            TEST_ASSERT_EQUAL_UINT8(val_pixel % 256, mcu[y][x].r);
            TEST_ASSERT_EQUAL_UINT8((val_pixel + 1) % 256, mcu[y][x].g);
            TEST_ASSERT_EQUAL_UINT8((val_pixel + 2) % 256, mcu[y][x].b);
        }
    }
    for (uint32_t y = 6; y < 8; y++) {
        for (uint32_t x = 0; x < 8; x++) {

            uint32_t val_pixel = 5 * largeur + x;
            TEST_ASSERT_EQUAL_UINT8(val_pixel % 256, mcu[y][x].r);
            TEST_ASSERT_EQUAL_UINT8((val_pixel + 1) % 256, mcu[y][x].g);
            TEST_ASSERT_EQUAL_UINT8((val_pixel + 2) % 256, mcu[y][x].b);
        }
    }

    reste_mcu = mcu_couleur_suivant(&iterateur, mcu);
    TEST_ASSERT_EQUAL(true, reste_mcu);

    for (uint32_t y = 0; y < 6; y++) {
        for (uint32_t x = 0; x < 7; x++) {

            uint32_t val_pixel = y * largeur + 8 + x;
            TEST_ASSERT_EQUAL_UINT8(val_pixel % 256, mcu[y][x].r);
            TEST_ASSERT_EQUAL_UINT8((val_pixel + 1) % 256, mcu[y][x].g);
            TEST_ASSERT_EQUAL_UINT8((val_pixel + 2) % 256, mcu[y][x].b);
        }

        uint32_t val_pixel = y * largeur + 14;
        TEST_ASSERT_EQUAL_UINT8(val_pixel % 256, mcu[y][7].r);
        TEST_ASSERT_EQUAL_UINT8((val_pixel + 1) % 256, mcu[y][7].g);
        TEST_ASSERT_EQUAL_UINT8((val_pixel + 2) % 256, mcu[y][7].b);
    }

    for (uint32_t y = 6; y < 8; y++) {
        for (uint32_t x = 0; x < 7; x++) {

            uint32_t val_pixel = 5 * largeur + 8 + x;
            TEST_ASSERT_EQUAL_UINT8(val_pixel % 256, mcu[y][x].r);
            TEST_ASSERT_EQUAL_UINT8((val_pixel + 1) % 256, mcu[y][x].g);
            TEST_ASSERT_EQUAL_UINT8((val_pixel + 2) % 256, mcu[y][x].b);
        }

        uint32_t val_pixel = 5 * largeur + 14;
        TEST_ASSERT_EQUAL_UINT8(val_pixel % 256, mcu[y][7].r);
        TEST_ASSERT_EQUAL_UINT8((val_pixel + 1) % 256, mcu[y][7].g);
        TEST_ASSERT_EQUAL_UINT8((val_pixel + 2) % 256, mcu[y][7].b);
    }

    reste_mcu = mcu_couleur_suivant(&iterateur, mcu);
    TEST_ASSERT_EQUAL(false, reste_mcu);

    liberer_iterateur_mcu(&iterateur);
    
    remove(chemin);
}




int main(void) {

    UNITY_BEGIN();
    RUN_TEST(test_lire_image);
    RUN_TEST(test_recupEntete_P5);
    RUN_TEST(test_recupEntete_fichier_type_invalide_1);
    RUN_TEST(test_recupEntete_fichier_type_invalide_2);
    RUN_TEST(test_recupEntete_fichier_dimensions_invalide);
    RUN_TEST(test_recupEntete_fichier_profondeur_invalide);
    RUN_TEST(test_recupEntete_fichier_taille_invalide);
    RUN_TEST(test_lectureEblocs);
    RUN_TEST(test_lireEblocs_grande_image);
    RUN_TEST(test_lireEblocs_grande_image_P6);
    RUN_TEST(test_iterateur_mcu_complexe);
    RUN_TEST(test_lectureImage_null);
    RUN_TEST(test_lectureImage_invalide);
    RUN_TEST(test_taille_invalide);

    return UNITY_END();
}