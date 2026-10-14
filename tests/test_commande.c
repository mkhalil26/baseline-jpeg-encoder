#include <stdlib.h>
#include "unity.h"
#include "commande.h"



void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}

void test_commande_simple_me(bool messages_erreur) {

    int argc = 2;
    char *argv[] = {"./ppm2jpeg","images/etu/invader.pgm"};

    Parametres_commande parametres;

    bool res = initialiser_parametres_commande(argc, argv, &parametres, messages_erreur);

    TEST_ASSERT_EQUAL(true,res);
    TEST_ASSERT_EQUAL_STRING("images/etu/invader.pgm", parametres.chemin_entree);
    TEST_ASSERT_EQUAL_STRING("out/invader.jpg",parametres.chemin_sortie);
    TEST_ASSERT_EQUAL(false,parametres.help);

    TEST_ASSERT_EQUAL_UINT8(1, parametres.facteurs.h1);
    TEST_ASSERT_EQUAL_UINT8(1, parametres.facteurs.v1);
    TEST_ASSERT_EQUAL_UINT8(1, parametres.facteurs.h2);
    TEST_ASSERT_EQUAL_UINT8(1, parametres.facteurs.v2);
    TEST_ASSERT_EQUAL_UINT8(1, parametres.facteurs.h3);
    TEST_ASSERT_EQUAL_UINT8(1, parametres.facteurs.v3);

    bool help = help_demande(&parametres);
    TEST_ASSERT_EQUAL(false, help);

    liberer_parametres_commande(&parametres);
}

void test_commande_simple_me_v(void) {

    test_commande_simple_me(true);
}

void test_commande_simple_me_f(void) {

    test_commande_simple_me(false);
}

void test_commande_outfile_arguments_inverses_me(bool messages_erreur) {

    int argc = 3;
    char *argv[] = {"./ppm2jpeg","--outfile=images/etu/truc.jpg","./images/etu/gris.pgm"};

    Parametres_commande parametres;

    bool res = initialiser_parametres_commande(argc, argv, &parametres, messages_erreur);

    TEST_ASSERT_EQUAL(true,res);
    TEST_ASSERT_EQUAL_STRING("./images/etu/gris.pgm", parametres.chemin_entree);
    TEST_ASSERT_EQUAL_STRING("images/etu/truc.jpg", parametres.chemin_sortie);
    TEST_ASSERT_EQUAL(false,parametres.help);

    bool help = help_demande(&parametres);
    TEST_ASSERT_EQUAL(false, help);

    liberer_parametres_commande(&parametres);
}

void test_commande_outfile_arguments_inverses_me_v(void) {

    test_commande_outfile_arguments_inverses_me(true);
}

void test_commande_outfile_arguments_inverses_me_f(void) {

    test_commande_outfile_arguments_inverses_me(false);
}

void test_commande_help_entree_me(bool messages_erreur) {

    int argc = 3;
    char *argv[] = {"./ppm2jpeg","--help","test_lignes.pgm"};

    Parametres_commande parametres;

    bool res = initialiser_parametres_commande(argc, argv, &parametres, messages_erreur);

    TEST_ASSERT_EQUAL(true,res);
    TEST_ASSERT_EQUAL(true,parametres.help);

    bool help = help_demande(&parametres);
    TEST_ASSERT_EQUAL(true, help);

    liberer_parametres_commande(&parametres);
} 

void test_commande_help_entree_me_v(void) {

    test_commande_help_entree_me(true);
}

void test_commande_help_entree_me_f(void) {

    test_commande_help_entree_me(false);
}

void test_commande_help_me(bool messages_erreur) {

    int argc = 2;
    char *argv[] = {"./ppm2jpeg","--help"};

    Parametres_commande parametres;

    bool res = initialiser_parametres_commande(argc, argv, &parametres, messages_erreur);

    TEST_ASSERT_EQUAL(true,res);
    TEST_ASSERT_EQUAL(true,parametres.help);

    bool help = help_demande(&parametres);
    TEST_ASSERT_EQUAL(true, help);

    liberer_parametres_commande(&parametres);
}

void test_commande_help_me_v(void) {

    test_commande_help_me(true);
}

void test_commande_help_me_f(void) {

    test_commande_help_me(false);
}

void test_commande_vide_me(bool messages_erreur) {

    int argc = 1;
    char *argv[] = {"./ppm2jpeg"};

    Parametres_commande parametres;

    bool res = initialiser_parametres_commande(argc, argv, &parametres, messages_erreur);

    TEST_ASSERT_EQUAL(false,res);

    liberer_parametres_commande(&parametres);
}

void test_commande_vide_me_v(void) {

    test_commande_vide_me(true);
}

void test_commande_vide_me_f(void) {

    test_commande_vide_me(false);
}

void test_commande_invalide_me(bool messages_erreur) {

    int argc = 2;
    char *argv[] = {"./ppm2jpeg", "--truc"};

    Parametres_commande parametres;

    bool res = initialiser_parametres_commande(argc, argv, &parametres, messages_erreur);

    TEST_ASSERT_EQUAL(false,res);

    liberer_parametres_commande(&parametres);
}

void test_commande_invalide_me_v(void) {

    test_commande_invalide_me(true);
}

void test_commande_invalide_me_f(void) {

    test_commande_invalide_me(false);
}

void test_commande_sample_me(bool messages_erreur) {

    int argc = 3;
    char *argv[] = {"./ppm2jpeg","--sample=2x2,2x1,1x2","images/etu/invader.pgm"};
    
    Parametres_commande parametres;

    bool res = initialiser_parametres_commande(argc, argv, &parametres, messages_erreur);

    TEST_ASSERT_EQUAL(true,res);
    TEST_ASSERT_EQUAL_STRING("images/etu/invader.pgm", parametres.chemin_entree);
    TEST_ASSERT_EQUAL_STRING("out/invader.jpg", parametres.chemin_sortie);
    TEST_ASSERT_EQUAL(false,parametres.help);

    TEST_ASSERT_EQUAL_UINT8(2, parametres.facteurs.h1);
    TEST_ASSERT_EQUAL_UINT8(2, parametres.facteurs.v1);
    TEST_ASSERT_EQUAL_UINT8(2, parametres.facteurs.h2);
    TEST_ASSERT_EQUAL_UINT8(1, parametres.facteurs.v2);
    TEST_ASSERT_EQUAL_UINT8(1, parametres.facteurs.h3);
    TEST_ASSERT_EQUAL_UINT8(2, parametres.facteurs.v3);

    bool help = help_demande(&parametres);
    TEST_ASSERT_EQUAL(false, help);
    
    liberer_parametres_commande(&parametres);
}

void test_commande_sample_me_v(void) {

    test_commande_sample_me(true);
}

void test_commande_sample_me_f(void) {

    test_commande_sample_me(false);
}

void test_commande_samples_incorrect_me(bool messages_erreur) {
    
    int argc = 3;
    char *argv[] = {"./ppm2jpeg","--sample=2x2,2x1,1xb","images/etu/invader.pgm"};
    
    Parametres_commande parametres;

    bool res = initialiser_parametres_commande(argc, argv, &parametres, messages_erreur);

    TEST_ASSERT_EQUAL(false,res);
    liberer_parametres_commande(&parametres);
}

void test_commande_samples_incorrect_me_v(void) {

    test_commande_samples_incorrect_me(true);
}

void test_commande_samples_incorrect_me_f(void) {

    test_commande_samples_incorrect_me(false);
}

void test_commande_samples_null_me(bool messages_erreur) {
    
    int argc = 3;
    char *argv[] = {"./ppm2jpeg","--sample=0", "images/etu/invader.pgm"};
    
    Parametres_commande parametres;

    bool res = initialiser_parametres_commande(argc, argv, &parametres, messages_erreur);

    TEST_ASSERT_EQUAL(false,res);
    liberer_parametres_commande(&parametres);
}

void test_commande_samples_null_me_v(void) {

    test_commande_samples_null_me(true);
}

void test_commande_samples_null_me_f(void) {

    test_commande_samples_null_me(false);
}

void test_commande_option_invalide_me(bool messages_erreur) {
    
    int argc = 3;
    char *argv[] = {"./ppm2jpeg","--truc=abc", "images/etu/invader.pgm"};
    
    Parametres_commande parametres;

    bool res = initialiser_parametres_commande(argc, argv, &parametres, messages_erreur);

    TEST_ASSERT_EQUAL(false,res);
    liberer_parametres_commande(&parametres);
}

void test_commande_option_invalide_me_v(void) {

    test_commande_option_invalide_me(true);
}

void test_commande_option_invalide_me_f(void) {

    test_commande_option_invalide_me(false);
}

void test_commande_sortie_dossier_me(bool messages_erreur) {
    
    int argc = 3;
    char *argv[] = {"./ppm2jpeg","--outfile=images", "images/etu/invader.pgm"};
    
    Parametres_commande parametres;

    bool res = initialiser_parametres_commande(argc, argv, &parametres, messages_erreur);

    TEST_ASSERT_EQUAL(false,res);
    liberer_parametres_commande(&parametres);
}

void test_commande_sortie_dossier_me_v(void) {

    test_commande_sortie_dossier_me(true);
}

void test_commande_sortie_dossier_me_f(void) {

    test_commande_sortie_dossier_me(false);
}

void test_commande_2_chemins_sortie_me(bool messages_erreur) {
    
    int argc = 3;
    char *argv[] = {"./ppm2jpeg","--outfile=images", "--outfile=images", "images/etu/invader.pgm"};
    
    Parametres_commande parametres;

    bool res = initialiser_parametres_commande(argc, argv, &parametres, messages_erreur);

    TEST_ASSERT_EQUAL(false,res);
    liberer_parametres_commande(&parametres);
}

void test_commande_2_chemins_sortie_me_v(void) {

    test_commande_2_chemins_sortie_me(true);
}

void test_commande_2_chemins_sortie_me_f(void) {

    test_commande_2_chemins_sortie_me(false);
}

void test_commande_sortie_dossier_2_me(bool messages_erreur) {
    
    int argc = 3;
    char *argv[] = {"./ppm2jpeg","--outfile=images/etu", "images/etu/invader.pgm"};
    
    Parametres_commande parametres;

    bool res = initialiser_parametres_commande(argc, argv, &parametres, messages_erreur);

    TEST_ASSERT_EQUAL(false,res);
    liberer_parametres_commande(&parametres);
}

void test_commande_sortie_dossier_2_me_v(void) {

    test_commande_sortie_dossier_2_me(true);
}

void test_commande_sortie_dossier_2_me_f(void) {

    test_commande_sortie_dossier_2_me(false);
}

void test_commande_sortie_inexistant_me(bool messages_erreur) {
    
    int argc = 3;
    char *argv[] = {"./ppm2jpeg","--outfile=truc/truc.ppm", "images/etu/invader.pgm"};
    
    Parametres_commande parametres;

    bool res = initialiser_parametres_commande(argc, argv, &parametres, messages_erreur);

    TEST_ASSERT_EQUAL(false,res);
    liberer_parametres_commande(&parametres);
}

void test_commande_sortie_inexistant_me_v(void) {

    test_commande_sortie_inexistant_me(true);
}

void test_commande_sortie_inexistant_me_f(void) {

    test_commande_sortie_inexistant_me(false);
}

void test_commande_samples_limite_me(bool messages_erreur) {
    
    int argc = 3;
    char *argv[] = {"./ppm2jpeg","--sample=2x5,2x1,1x1","images/etu/invader.pgm"};
    
    Parametres_commande parametres;

    bool res = initialiser_parametres_commande(argc, argv, &parametres, messages_erreur);

    TEST_ASSERT_EQUAL(false,res);
    liberer_parametres_commande(&parametres);
}

void test_commande_samples_limite_me_v(void) {

    test_commande_samples_limite_me(true);
}

void test_commande_samples_limite_me_f(void) {

    test_commande_samples_limite_me(false);
}

void test_commande_samples_limite_2_me(bool messages_erreur) {
    
    int argc = 3;
    char *argv[] = {"./ppm2jpeg","--sample=2x2,2x1,0x1","images/etu/invader.pgm"};
    
    Parametres_commande parametres;

    bool res = initialiser_parametres_commande(argc, argv, &parametres, messages_erreur);

    TEST_ASSERT_EQUAL(false,res);
    liberer_parametres_commande(&parametres);
}

void test_commande_samples_limite_2_me_v(void) {

    test_commande_samples_limite_2_me(true);
}

void test_commande_samples_limite_2_me_f(void) {

    test_commande_samples_limite_2_me(false);
}

void test_commande_samples_superieurs_a_10_me(bool messages_erreur) {
    
    int argc = 3;
    char *argv[] = {"./ppm2jpeg","--sample=2x4,2x2,2x2","images/etu/invader.pgm"};
    
    Parametres_commande parametres;

    bool res = initialiser_parametres_commande(argc, argv, &parametres, messages_erreur);

    TEST_ASSERT_EQUAL(false,res);
    liberer_parametres_commande(&parametres);
}

void test_commande_samples_superieurs_a_10_me_v(void) {

    test_commande_samples_superieurs_a_10_me(true);
}

void test_commande_samples_superieurs_a_10_me_f(void) {

    test_commande_samples_superieurs_a_10_me(false);
}

void test_commande_samples_indivisibles_me(bool messages_erreur) {
    
    int argc = 3;
    char *argv[] = {"./ppm2jpeg","--sample=1x3,1x2,1x1","images/etu/invader.pgm"};
    
    Parametres_commande parametres;

    bool res = initialiser_parametres_commande(argc, argv, &parametres, messages_erreur);

    TEST_ASSERT_EQUAL(false,res);
    liberer_parametres_commande(&parametres);
}

void test_commande_samples_indivisibles_me_v(void) {

    test_commande_samples_indivisibles_me(true);
}

void test_commande_samples_indivisibles_me_f(void) {

    test_commande_samples_indivisibles_me(false);
}

void test_commande_pas_chemin_entree_me(bool messages_erreur) {
    
    int argc = 2;
    char *argv[] = {"./ppm2jpeg","--sample=1x1,1x1,1x1"};
    
    Parametres_commande parametres;

    bool res = initialiser_parametres_commande(argc, argv, &parametres, messages_erreur);

    TEST_ASSERT_EQUAL(false,res);
    liberer_parametres_commande(&parametres);
}

void test_commande_pas_chemin_entree_me_v(void) {

    test_commande_pas_chemin_entree_me(true);
}

void test_commande_pas_chemin_entree_me_f(void) {

    test_commande_pas_chemin_entree_me(false);
}

void test_dossier_null(void) {

    bool res = chemin_est_dossier(NULL);
    TEST_ASSERT_EQUAL(false, res);

    res = chemin_est_dossier("ppm2jpeg");
    TEST_ASSERT_EQUAL(false, res);

    res = chemin_est_dossier("images");
    TEST_ASSERT_EQUAL(true, res);
}

void test_fichier_null(void) {

    bool res = chemin_est_fichier(NULL);
    TEST_ASSERT_EQUAL(false, res);

    res = chemin_est_fichier("images");
    TEST_ASSERT_EQUAL(false, res);

    res = chemin_est_fichier("ppm2jpeg");
    TEST_ASSERT_EQUAL(true, res);
}

void test_dupliquer_chaine(void) {

    char *chaine = dupliquer_chaine(NULL);
    TEST_ASSERT_NULL(chaine);

    chaine = dupliquer_chaine("truc");
    TEST_ASSERT_EQUAL_STRING("truc", chaine);
    free(chaine);
}

void test_dossier_chemin_existe(void) {

    bool res = dossier_chemin_existe(NULL);
    TEST_ASSERT_EQUAL(false, res);

    res = dossier_chemin_existe("ppm2jpeg");
    TEST_ASSERT_EQUAL(true, res);
}

void test_chemin_accessible(void) {

    bool res = chemin_accessible(NULL);
    TEST_ASSERT_EQUAL(false, res);

    res = chemin_accessible("truc");
    TEST_ASSERT_EQUAL(false, res);

    res = chemin_accessible("ppm2jpeg");
    TEST_ASSERT_EQUAL(true, res);
}

void test_chemin_par_defaut(void) {

    char *chaine = chemin_par_defaut(NULL);
    TEST_ASSERT_NULL(chaine);

    chaine = chemin_par_defaut("truc");
    TEST_ASSERT_EQUAL_STRING("out/truc.jpg", chaine);
    free(chaine);

    chaine = chemin_par_defaut("truc.ppm"); 
    TEST_ASSERT_EQUAL_STRING("out/truc.jpg", chaine);
    free(chaine);
}

int main(void) {

    UNITY_BEGIN();

    RUN_TEST(test_commande_simple_me_v);
    RUN_TEST(test_commande_simple_me_f);
    RUN_TEST(test_commande_outfile_arguments_inverses_me_v);
    RUN_TEST(test_commande_outfile_arguments_inverses_me_f);
    RUN_TEST(test_commande_help_entree_me_v);
    RUN_TEST(test_commande_help_entree_me_f);
    RUN_TEST(test_commande_help_me_v);
    RUN_TEST(test_commande_help_me_f);
    RUN_TEST(test_commande_vide_me_v);
    RUN_TEST(test_commande_vide_me_f);
    RUN_TEST(test_commande_invalide_me_v);
    RUN_TEST(test_commande_invalide_me_f);
    RUN_TEST(test_commande_sample_me_v);
    RUN_TEST(test_commande_sample_me_f);
    RUN_TEST(test_commande_samples_incorrect_me_v);
    RUN_TEST(test_commande_samples_incorrect_me_f);
    RUN_TEST(test_commande_samples_null_me_v);
    RUN_TEST(test_commande_samples_null_me_f);
    RUN_TEST(test_commande_option_invalide_me_v);
    RUN_TEST(test_commande_option_invalide_me_f);
    RUN_TEST(test_commande_sortie_dossier_me_v);
    RUN_TEST(test_commande_sortie_dossier_me_f);
    RUN_TEST(test_commande_sortie_dossier_2_me_v);
    RUN_TEST(test_commande_sortie_dossier_2_me_f);
    RUN_TEST(test_commande_2_chemins_sortie_me_v);
    RUN_TEST(test_commande_2_chemins_sortie_me_f);
    RUN_TEST(test_commande_sortie_inexistant_me_v);
    RUN_TEST(test_commande_sortie_inexistant_me_f);
    RUN_TEST(test_commande_samples_limite_me_v);
    RUN_TEST(test_commande_samples_limite_me_f);
    RUN_TEST(test_commande_samples_limite_2_me_v);
    RUN_TEST(test_commande_samples_limite_2_me_f);
    RUN_TEST(test_commande_samples_superieurs_a_10_me_v);
    RUN_TEST(test_commande_samples_superieurs_a_10_me_f);
    RUN_TEST(test_commande_samples_indivisibles_me_v);
    RUN_TEST(test_commande_samples_indivisibles_me_f);
    RUN_TEST(test_commande_pas_chemin_entree_me_v);
    RUN_TEST(test_commande_pas_chemin_entree_me_f);
    RUN_TEST(test_dossier_null);
    RUN_TEST(test_fichier_null);
    RUN_TEST(test_dupliquer_chaine);
    RUN_TEST(test_dossier_chemin_existe);
    RUN_TEST(test_chemin_accessible);
    RUN_TEST(test_chemin_par_defaut);

    return UNITY_END();
}
