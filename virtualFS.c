#include "virtualFS.h"	

int main(int argc, char* argv[])
{
	disque_t* disque;

	if(argc < 2)
	{
		fprintf(stderr,"[Erreur] Pas assez d'arguments.\n");
		exit(EXIT_FAILURE);
	}
	else if(strlen(argv[1]) != 1)
	{
		fprintf(stderr,"[Erreur] 2ème argument non valide.\n");
		exit(EXIT_FAILURE);
	}
	else
	{
		switch(argv[1][0]){
			case 'S':
				if(argc < 5)
				{
					fprintf(stderr,"[Erreur] Pas assez d'arguments.\n");
					printf("[INFO] Prototype: ./virtualFS S nom_disque_virtuel nom_fichier_entrant nom_fichier_dans_disque\n");
					exit(EXIT_FAILURE);
				}
				else
				{
					disque = disque_ouvrir(argv[2]);
					disque_stocker(disque, argv[3], argv[4]);
					printf("[SUCCESS] Le fichier %s a bien été sauvegardé dans le disque %s.\n", argv[3], argv[2]);
				}
				break;
			case 'X':
				if(argc < 5)
				{
					fprintf(stderr,"[Erreur] Pas assez d'arguments.\n");
					printf("[INFO] Prototype: ./virtualFS X nom_disque_virtuel nom_fichier_dans_disque nom_fichier_sortant\n");
					exit(EXIT_FAILURE);
				}
				else
				{
					disque = disque_ouvrir(argv[2]);
					disque_extraire(disque, argv[3], argv[4]);
					printf("[SUCCESS] Le fichier %s a bien été extrait du disque %s sous le nom %s.\n", argv[3], argv[2], argv[4]);
				}
				break;
			case 'L':
				if(argc < 3)
				{
					fprintf(stderr,"[Erreur] Pas assez d'arguments.\n");
					printf("[INFO] Prototype: ./virtualFS L nom_disque_virtuel\n");
					exit(EXIT_FAILURE);
				}
				else
				{
					disque = disque_ouvrir(argv[2]);
					disque_afficherTables(disque);
				}
				break;
			case 'F':
				if(argc < 3)
				{
					fprintf(stderr,"[Erreur] Pas assez d'arguments.\n");
					printf("[INFO] Prototype: ./virtualFS F nom_disque_virtuel\n");
					exit(EXIT_FAILURE);
				}
				else
				{
					disque = disque_ouvrir(argv[2]);
					disque_afficherFichiers(disque);
				}
				break;
			default:
				fprintf(stderr,"[Erreur] Commande non valide.\n");
				exit(EXIT_FAILURE);
				break;
		}
	}

	disque_fermer(&disque);
	exit(EXIT_SUCCESS);
}