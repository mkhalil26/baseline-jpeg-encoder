
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "lecture.h"


         
#define NB_BLOCS_SUPERBLOC 64
#define TAILLE_ENTETE_PPM_MIN 9



// lit les potentiels commentaires dans les lignes du fichier ppm ou pgm
void ignore_commentaires(FILE *fichier)
{
    int c;
    while ((c = fgetc(fichier)) != EOF)
    {

        if (c == '#')
        {

            while ((c = fgetc(fichier)) != '\n' && c != EOF)
            {
            }
        }
        else
        {

            ungetc(c, fichier);
            break;
        }
    }
}

Image *recupEntete(FILE *fichier) {

    struct Image *image = (struct Image *)malloc(sizeof(Image));
    if (image == NULL)
        return NULL;

    image->tab = NULL;

    char c1, c2;
    unsigned int l, h, profondeur;

    ignore_commentaires(fichier);

    if (fscanf(fichier, "%c%c\n", &c1, &c2) != 2) // type d'image P5 ou P6
    {
        free(image);
        return NULL;
    }
    else
    {

        if (c1 == 'P' && c2 == '5')
        {
            image->type = P5;
        }
        else if (c1 == 'P' && c2 == '6')
        {
            image->type = P6;
        }
        else
        {
            free(image);
            return NULL;
        }
    }

    ignore_commentaires(fichier);

    if (fscanf(fichier, "%u %u\n", &l, &h) != 2) // largeur et hauteur
    { 
        free(image);
        return NULL;
    }

    image->largeur = l;
    image->hauteur = h;

    ignore_commentaires(fichier);

    if (fscanf(fichier, "%u\n", &profondeur) != 1) // profondeur, ignorée ici
    {
        free(image);
        return NULL;
    }

    ignore_commentaires(fichier);

    image->debut_pixels = ftell(fichier);

    fseek(fichier, 0, SEEK_END);
    size_t taille_fichier = ftell(fichier);
    fseek(fichier, image->debut_pixels, SEEK_SET);

    if (taille_fichier < (image->largeur * image->hauteur * ((image->type == P6) ? 3 : 1) + image->debut_pixels)) { // vérifie qu'il y a bien assez de données dans le fichier

        free(image);
        return NULL;
    }

    image->fichier = fichier;
    return image;
}

// positionne le curseur du fichier aux coordonnées (x,y) en pixels
void positionner_curseur(const Image *img, uint32_t x, uint32_t y)
{
    int multiplicateur = (img->type == P6) ? 3 : 1;
    long position = img->debut_pixels + (((long)y * img->largeur) + (long)x) * multiplicateur;
    fseek(img->fichier, position, SEEK_SET);
}

Image *allouer_image(Image *image, uint32_t largeur_bloc_en_pixels, uint32_t nb_lignes_superbloc,uint32_t nb_blocs) {

    int multiplicateur = (image->type == P6) ? 3 : 1;

    image->tab = (uint8_t **)malloc(nb_lignes_superbloc * sizeof(uint8_t *)); // tableau de pointeurs
    if (image->tab == NULL) {
        return NULL;
    }

    for (uint32_t i = 0; i < nb_lignes_superbloc; i++) {
        image->tab[i] = (uint8_t *)malloc(largeur_bloc_en_pixels * nb_blocs * multiplicateur * sizeof(uint8_t)); // alloue une ligne du superbloc
        if (image->tab == NULL) {
            return NULL;
        }
    }

    return image;
}

Image *lectureImage(const char *nom_fichier)
{

    FILE *fichier = fopen(nom_fichier, "rb");
    if (fichier == NULL)
    {
        perror("[ERREUR] ouverture du fichier");
        return NULL;
    }
    
    fseek(fichier, 0, SEEK_END);
    long taille_fichier = ftell(fichier);
    
    if (taille_fichier < TAILLE_ENTETE_PPM_MIN)
    {
        printf("[ERREUR] taille de fichier inferieur a l'entete");
        fclose(fichier);
        return NULL;
    }

    rewind(fichier);
    
    Image * image = recupEntete(fichier); 
    
    if (image == NULL) {

        printf("[ERREUR] lecture de l'entete");
        fclose(fichier);
        return NULL;
    }

    return image;
}

Image *lireEblocs(Image *image_ppm, uint32_t x, uint32_t y, uint32_t largeur_bloc_en_pixels, uint32_t nb_lignes_superbloc,uint32_t nb_blocs)
{
    uint32_t nb_pixels_a_lire = largeur_bloc_en_pixels * nb_blocs;

    if (x + nb_pixels_a_lire > image_ppm->largeur) {
        nb_pixels_a_lire = image_ppm->largeur - x;
    }

    uint32_t unite = (image_ppm->type == P6) ? 3 : 1;
    
    for (size_t i = 0; i < nb_lignes_superbloc; i++) {
        
        positionner_curseur(image_ppm, x, y + i);

        size_t lus = fread(image_ppm->tab[i], unite, nb_pixels_a_lire, image_ppm->fichier);
        
        if (i == 0) {
            image_ppm->taille_ligne = lus;
        }

        if (lus < nb_pixels_a_lire) {
            break;
        }

        image_ppm->nb_lignes = i + 1;
    }

    return image_ppm;
}

void liberer_image(Image *image, uint32_t nb_lignes_superbloc) {

    if (image == NULL) {
        return;
    }

    if (image->tab != NULL) {
        
        for (uint32_t i = 0; i < nb_lignes_superbloc; i++) {
            
            free(image->tab[i]);
        }

        free(image->tab);
        fclose(image->fichier);
        free(image);
    }
}



void determiner_facteurs_mcu(Facteurs_echantillonnage facteurs, uint8_t *largeur_mcu, uint8_t *hauteur_mcu) {

	*largeur_mcu = 8 * facteurs.h1;
	*hauteur_mcu = 8 * facteurs.v1;
}

bool initialiser_iterateur_mcu(IterateurMCU *iterateur, const char *nom_fichier, Facteurs_echantillonnage facteurs) {

    iterateur->i_mcu = 0;
    iterateur->x = 0;
    iterateur->y = 0;
    iterateur->nb_mcu_lus = 0;

    Image *image = lectureImage(nom_fichier);
    if (image == NULL) {
        return false;
    }

    determiner_facteurs_mcu(facteurs, &(iterateur->largeur_mcu), &(iterateur->hauteur_mcu));

    image = allouer_image(image, iterateur->largeur_mcu, iterateur->hauteur_mcu, NB_BLOCS_SUPERBLOC);
    if (image == NULL) {
        return false;
    }

    iterateur->largeur_image_mcu = ceil((double)image->largeur / (iterateur->largeur_mcu));
    iterateur->hauteur_image_mcu = ceil((double)image->hauteur / (iterateur->hauteur_mcu));

    iterateur->image = image;
    return true;
}

bool mcu_suivant(IterateurMCU *iterateur, bool couleur, uint8_t mcu_gris[8][8], Couleur_rgb mcu_couleur[MCU_MAX][MCU_MAX]) {

    if ((iterateur->y) >= (iterateur->hauteur_image_mcu)) {
        return false;
    }

    if (iterateur->i_mcu == iterateur->nb_mcu_lus) {
        iterateur->i_mcu = 0;
    }

    uint8_t octets_par_pixel = couleur ? 3 : 1;
    uint8_t largeur_mcu = couleur ? iterateur->largeur_mcu : 8;
    uint8_t hauteur_mcu = couleur ? iterateur->hauteur_mcu : 8;

    if (iterateur->i_mcu == 0) { // quand on doit lire un nouveau superbloc

        lireEblocs(iterateur->image, iterateur->x * largeur_mcu, iterateur->y * hauteur_mcu, largeur_mcu, hauteur_mcu, NB_BLOCS_SUPERBLOC);
        iterateur->nb_mcu_lus = ceil((double)iterateur->image->taille_ligne / largeur_mcu);

        if ((iterateur->image->taille_ligne % largeur_mcu) != 0) { // répéter la dernière colonne
            uint32_t indice_derniere_colonne = iterateur->image->taille_ligne - 1;

            for (uint8_t i = 0; i < hauteur_mcu; i++) {
                for (uint32_t j = iterateur->image->taille_ligne; j < (iterateur->nb_mcu_lus * largeur_mcu); j++) {
                    for (uint8_t k = 0; k < octets_par_pixel; k++) {

                        iterateur->image->tab[i][j * octets_par_pixel + k] = iterateur->image->tab[i][indice_derniere_colonne * octets_par_pixel + k];
                    }
                }
            }
        }

        if (iterateur->image->nb_lignes != hauteur_mcu) { // répéter la dernière ligne

            uint32_t indice_derniere_ligne = iterateur->image->nb_lignes-1;
            uint8_t nb_lignes_vides = hauteur_mcu - iterateur->image->nb_lignes;
            uint32_t largeur_octets = NB_BLOCS_SUPERBLOC * largeur_mcu * octets_par_pixel;

            for (uint8_t i = 1; i < (nb_lignes_vides + 1); i++) {
                memcpy(iterateur->image->tab[indice_derniere_ligne + i], iterateur->image->tab[indice_derniere_ligne], largeur_octets);
            }
        }
    }
    
    // remplissage de la MCU
    uint32_t debut_mcu_dans_ligne = (iterateur->i_mcu * largeur_mcu) * octets_par_pixel;

    if (couleur) {

        for (uint8_t i = 0; i < hauteur_mcu; i++) {
            memcpy(mcu_couleur[i], &(iterateur->image->tab[i][debut_mcu_dans_ligne]), largeur_mcu * 3);
        }

    } else {

        for (uint8_t i = 0; i < hauteur_mcu; i++) {
            for (uint8_t j = 0; j < largeur_mcu; j++) {
            
                uint8_t niveau_gris = iterateur->image->tab[i][debut_mcu_dans_ligne + j];
                mcu_gris[i][j] = niveau_gris;
            }
        }
    }

    if ((iterateur->x) == (uint32_t)(iterateur->largeur_image_mcu -1)) {
        
        iterateur->y +=1;
        iterateur->x = 0;
        iterateur->i_mcu = 0;

    } else {

        iterateur->x += 1;
        iterateur->i_mcu += 1;
    }

    if (iterateur->i_mcu == NB_BLOCS_SUPERBLOC) {
        iterateur->i_mcu = 0;
    }

    return true;
}

bool mcu_couleur_suivant(IterateurMCU *iterateur, Couleur_rgb mcu[MCU_MAX][MCU_MAX]) {

    return mcu_suivant(iterateur, true, NULL, mcu);
}

bool mcu_gris_suivant(IterateurMCU *iterateur, uint8_t mcu[8][8]) {

    return mcu_suivant(iterateur, false, mcu, NULL);
}

void liberer_iterateur_mcu(IterateurMCU *iterateur) {

    liberer_image(iterateur->image, iterateur->hauteur_mcu);
}

bool image_couleur(const IterateurMCU *iterateur) {

    return iterateur->image->type == P6;
}