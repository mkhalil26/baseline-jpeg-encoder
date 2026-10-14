#ifndef _COMMANDE_H_
#define _COMMANDE_H_

#include <stdint.h>
#include <stdbool.h>



typedef struct facteurs_echantillonnage {
	uint8_t h1,v1,h2,v2,h3,v3; // h1xv1,h2xv2,h3v3
	
} Facteurs_echantillonnage;

typedef struct parametres_commande {
	char *chemin_entree; // le chemin du fichier en entrée accessible
	char *chemin_sortie; // le chemin de sortie accessible
	bool help; // vrai si on a mis --help, on s'arrète, les autres champs ne sont pas forcément initialisés
	Facteurs_echantillonnage facteurs; // les facteurs récupérés

} Parametres_commande;



/*
récupère les valeurs des paramètres dans une ligne de commande
et renvoir un booléen qui est à vrai si la commande contient des arguments corrects
parametres est une structure non initialisée qui sera remplie à la fin de l'appel de fonction
*/
bool initialiser_parametres_commande(int argc, char **argv, Parametres_commande *parametres, bool messages_erreur);

// affiche l'aide si l'argument help à été détecté
bool help_demande(const Parametres_commande *parametres);

// libère les paramètres récupérés
void liberer_parametres_commande(Parametres_commande *parametres);

// fonctions auxiliaires déclarées pour les tests
bool chemin_est_dossier(const char *chemin);

bool chemin_est_fichier(const char *chemin);

char *dupliquer_chaine(const char *chaine);

bool dossier_chemin_existe(const char *chemin);

bool chemin_accessible(const char *chemin);

char *chemin_par_defaut(const char *chemin);


#endif /* _COMMANDE_H_ */
