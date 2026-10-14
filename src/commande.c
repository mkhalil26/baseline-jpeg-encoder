#include <unistd.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <libgen.h>
#include "commande.h"



// définitions des options utilisables
static struct option options[] = {
	{"outfile", required_argument, NULL, 0},
	{"sample", 	required_argument, NULL, 1},
	{"help", 	no_argument,       NULL, 2},
	{0, 0, 0, 0}
};



// teste si un chemin est un dossier
bool chemin_est_dossier(const char *chemin) {

	if (chemin == NULL) {
		return false;
	}

	struct stat stat_;
    return (stat(chemin, &stat_) == 0 && S_ISDIR(stat_.st_mode));
}

// teste si un chemin est un fichier
bool chemin_est_fichier(const char *chemin) {

	if (chemin == NULL) {
		return false;
	}

	struct stat stat_;
	return (stat(chemin, &stat_) == 0 && S_ISREG(stat_.st_mode));
}

// fonction similaire à strdup qui alloue la chaine en paramètre dans le tas
char *dupliquer_chaine(const char *chaine) {

	if (chaine == NULL) {
		return NULL;
	}

	char *nouvelle_chaine = malloc(strlen(chaine) + 1);
	
	if (nouvelle_chaine == NULL) {
		return NULL;
	}

	strcpy(nouvelle_chaine, chaine);

	return nouvelle_chaine;
}

// vérifie si le dossier parent du chemin existe
bool dossier_chemin_existe(const char *chemin) {

	if (chemin == NULL) {
		return false;
	}

	char *chemin_dup = dupliquer_chaine(chemin);
	if (chemin_dup == NULL) {
		return false;
	}

	char *chemin_dossier = dirname(chemin_dup);

    bool res = chemin_est_dossier(chemin_dossier);

	free(chemin_dup);

	return res;
}

// teste si l'on peut ouvrir le fichier depuis le chemin
bool chemin_accessible(const char *chemin) {

	if (chemin == NULL) {
		return false;
	}

	FILE *fichier = fopen(chemin, "rb");
	
	if (fichier == NULL) {
		return false;
	}

	fclose(fichier);

	return true;
}

// vérifie si les fecteurs d'échantillonnages sont valides
bool verifier_facteurs_echantillonnage(Facteurs_echantillonnage facteurs) {

	if (
		facteurs.h1 < 1 || facteurs.h1 > 4 || 
		facteurs.v1 < 1 || facteurs.v1 > 4 || 
		facteurs.h2 < 1 || facteurs.h2 > 4 ||
		facteurs.v2 < 1 || facteurs.v2 > 4 || 
		facteurs.h3 < 1 || facteurs.h3 > 4 ||
		facteurs.v3 < 1 || facteurs.v3 > 4
	) {
		return false;
	}

	if ((facteurs.h1 * facteurs.v1 + facteurs.h2 * facteurs.v2 + facteurs.h3 * facteurs.v3) > 10) {
		return false;
	}

	if (
		facteurs.h1 % facteurs.h2 != 0 || 
		facteurs.h1 % facteurs.h3 != 0 ||
		facteurs.v1 % facteurs.v2 != 0 || 
		facteurs.v1 % facteurs.v3 != 0
	) {
		return false;
	}

	return true;
}



// récupère les valeurs des paramètres dans une ligne de commande
bool recuperer_parametres_commande(int argc, char **argv, Parametres_commande *parametres, bool *facteurs_initialises, bool messages_erreur) {

	parametres->chemin_sortie = NULL;

	if (argc < 2) {
		if (messages_erreur) {
			fprintf(stderr, "Erreur : moins de deux arguments\n");
		}
		return false;
	}

	parametres->chemin_entree = NULL;
	parametres->help = false;
	*facteurs_initialises = false;

	int opt;
	optind = 1;
	
	while ((opt = getopt_long(argc, argv, "", options, NULL)) != -1) {
		
		switch (opt) {
			
			case 0: // code de l'option outfile

				if (parametres->chemin_sortie != NULL) {
					free(parametres->chemin_sortie);
				}

				parametres->chemin_sortie = dupliquer_chaine(optarg);
				break;

			case 1: // code de l'option sample
				if (optarg == NULL) {
					if (messages_erreur) {
						fprintf(stderr, "Erreur : optarg NULL\n");
					}
					return false;
				}
				int args_corrects = sscanf(
					optarg,
					"%hhux%hhu,%hhux%hhu,%hhux%hhu",
					&(parametres->facteurs.h1),
					&(parametres->facteurs.v1),
					&(parametres->facteurs.h2),
					&(parametres->facteurs.v2),
					&(parametres->facteurs.h3),
					&(parametres->facteurs.v3)
				);

				if (args_corrects != 6) {
					if (messages_erreur) {
						fprintf(stderr, "Erreur dans la récupération des facteurs d'échantillonnage\n");
					}
					return false;
				}

				*facteurs_initialises = true;
				break;

			case 2: // code de l'option help
				parametres->help = true;
				return true;
			
			default:
				if (messages_erreur) {
					fprintf(stderr, "Erreur : cas option par défaut\n");
				}
				return false;
			}
	}

	if (optind >= argc) {
		if (messages_erreur) {
			fprintf(stderr, "Erreur : option hors des arguments\n");
		}
		return false;
	}

	parametres->chemin_entree = argv[optind];

	return true;
}

// construit le chemin de sortie par défaut a partir du chemin d'entrée
char *chemin_par_defaut(const char *chemin) {

	char *chemin_dup = dupliquer_chaine(chemin);
	if (chemin_dup == NULL) {
		return NULL;
	}

	char *chemin_fichier = basename(chemin_dup);

	if (!chemin_est_dossier("out")) {
		if (mkdir("out", 0777) != 0) {
			free(chemin_dup);
            return NULL;
        }
	}

	char *dernier_point = strrchr(chemin_fichier,'.');
	int len = strlen(chemin_fichier);

	if (dernier_point == NULL) { // si le nom de fichier n'a pas d'extension

		char *chaine = (char *)malloc(len + 9);
		if (chaine == NULL) {
			free(chemin_dup);
			return NULL;
		}
		snprintf(chaine ,len + 9, "out/%s.jpg", chemin_fichier);
		free(chemin_dup);
		return chaine;
	}
	
	int len_avant_point = (int)(dernier_point - chemin_fichier);
	char *chaine = (char *)malloc(len_avant_point + 9);
	if (chaine == NULL) {
		free(chemin_dup);
		return NULL;
	}

	snprintf(chaine, len_avant_point + 9, "out/%.*s.jpg", len_avant_point, chemin_fichier);
	free(chemin_dup);
	
	return chaine;
}

bool initialiser_parametres_commande(int argc, char **argv, Parametres_commande *parametres, bool messages_erreur) {

	bool facteurs_initialises;
	bool res = recuperer_parametres_commande(argc, argv, parametres, &facteurs_initialises, messages_erreur);

	if (!res) {
		return false;
	}

	if (parametres->help) {
		return true;
	}

	if (parametres->chemin_entree == NULL) {
		if (messages_erreur) fprintf(stderr, "Erreur : moins de deux arguments\n");
		return false;
	}

	if (parametres->chemin_sortie == NULL) {
	
		char *chemin_sortie = chemin_par_defaut(parametres->chemin_entree);

		if (chemin_sortie == NULL) {
			if (messages_erreur) {
				fprintf(stderr, "Erreur : chemin de sortie NULL\n");
			}
			return false;
		}

		parametres->chemin_sortie = chemin_sortie;
	}


	if (!chemin_accessible(parametres->chemin_entree)) {
		if (messages_erreur) {
			fprintf(stderr, "Erreur : fichier en entree inaccessible\n");
		}
		return false;
	}

	if (!dossier_chemin_existe(parametres->chemin_entree)) {
		if (messages_erreur) {
			fprintf(stderr, "Erreur : dossier du du fichier d'entree inaccessible\n");
		}
		return false;
	}

	if (!chemin_est_fichier(parametres->chemin_entree)) {
		if (messages_erreur) {
			fprintf(stderr, "Erreur : le chemin donne en entree n'est pas un fichier\n");
		}
		return false;
	}
	
	if (chemin_accessible(parametres->chemin_sortie)) {

		if (!chemin_est_fichier(parametres->chemin_sortie)) {
			liberer_parametres_commande(parametres);
			if (messages_erreur) {
				fprintf(stderr, "Erreur : le chemin de sortie n'est pas un fichier\n");
			}
			return false;
		}
	}

	if (!dossier_chemin_existe(parametres->chemin_sortie)) {
		liberer_parametres_commande(parametres);
		if (messages_erreur) {
			fprintf(stderr, "Erreur : le dossier du fichier de sortie est inaccessible\n");
		}
		return false;
	}

	
	if (!facteurs_initialises) {

		Facteurs_echantillonnage facteurs = {1, 1, 1, 1, 1, 1}; // facteurs d'échantillonnage par défaut
		parametres->facteurs = facteurs;
	
	} else {
		
		if (!verifier_facteurs_echantillonnage(parametres->facteurs)) {
			liberer_parametres_commande(parametres);
			if (messages_erreur) {
				fprintf(stderr, "Erreur : facteurs d'echantillonnage incorrects\n");
			}
			return false;
		} 
	}

	return true;
}

bool help_demande(const Parametres_commande *parametres) {

	if (!parametres->help) {
		return false;
	}

	printf("Usage : ./ppm2jpeg <chemin du fichier en entrée> <options>\n");
	printf("--help : affiche cette page d'aide\n");
	printf("--outfile <chemin de sortie> : spécifie le chemin de sortie du fichier JPEG\n");
	printf("--sample <AxB,CxD,ExF>: permet de définir avec les variables A,B,C,D,E,F les facteurs d'échantillonnage des composantes Y, Cb et Cr\n");

	return true;
}

void liberer_parametres_commande(Parametres_commande *parametres) {

	if (parametres->chemin_sortie == NULL) {
		return;
	}
	
	free(parametres->chemin_sortie);
	parametres->chemin_sortie = NULL;
}