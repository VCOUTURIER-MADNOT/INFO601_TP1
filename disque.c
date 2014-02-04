#include "disque.h"

disque_t* disque_ouvrir(char* nomDisque)
{
	int disqueExiste;
	disque_t* disque = NULL;
	table_t* tablePrinc = NULL;

	disque = (disque_t*) malloc(sizeof(disque_t));

	disqueExiste = fichierExiste(nomDisque);

	if((disque->fd = open(nomDisque, O_RDWR | O_CREAT, S_IWUSR | S_IRUSR)) == -1)
	{
		fprintf(stderr, "Erreur lors de l'ouverture du fichier disque.bin \n");
		exit(EXIT_FAILURE);
	}

	if(!disqueExiste)
	{
		tablePrinc = table_creer();
		table_sauvegarder(disque->fd, tablePrinc);
	}
	
	table_supprimer(&tablePrinc);
	return disque;
}

void disque_fermer(disque_t** disque)
{
	*disque = NULL;
	free(*disque);
}

void disque_afficherTables(disque_t* disque)
{
	table_t* table;

	/* On se place au début du fichier pour lire la table d'adresse principale */
	deplacerDebut(disque->fd);
	printf("**********\n");
	do
	{
		/* On charge la table au pointeur */
		table = table_charger(disque->fd);
		table_afficher(table);

		/* Si il existe une autre table, on déplace le pointeur du fd à son adresse */
		if(table->prochTable != 0)
		{
			if(lseek(disque->fd, table->prochTable, SEEK_SET) == -1)
			{
				fprintf(stderr, "Erreur lors du déplacement vers la table à l'adresse : %ld \n", table->prochTable);
				exit(EXIT_FAILURE);
			}
		}
	}while(table->prochTable != 0);
	printf("**********\n");
	deplacerDebut(disque->fd);
	table_supprimer(&table);
}

void disque_afficherFichiers(disque_t* disque)
{
	int i;
	table_t* table;
	inode_t* inode;

	/* On se place au début du fichier pour lire la table d'adresse principale */
	deplacerDebut(disque->fd);
	printf("**********\n");
	printf("Affichage des fichiers : \n");
	do
	{
		/* On charge la table au pointeur */
		table = table_charger(disque->fd);

		/* On affiche les inodes */
		for(i = 0; i < _NB_ADR_TABLE_; i++)
		{
			if(table->adresses[i] != 0)
			{
				if(lseek(disque->fd, table->adresses[i], SEEK_SET) == -1)
				{
					fprintf(stderr, "Erreur lors du déplacement vers l'inode à l'adesse : %ld\n", table->adresses[i]);
					exit(EXIT_FAILURE);
				}

				inode = inode_charger(disque->fd);
				inode_afficher(inode);
			}
		}

		/* Si il existe une autre table, on déplace le pointeur du fd à son adresse */
		if(table->prochTable != 0)
		{
			if(lseek(disque->fd, table->prochTable, SEEK_SET) == -1)
			{
				fprintf(stderr, "Erreur lors du déplacement vers la table à l'adresse : %ld \n", table->prochTable);
				exit(EXIT_FAILURE);
			}
		}
	}while(table->prochTable != 0);
	printf("**********\n");
	table_supprimer(&table);
	inode_supprimer(&inode);
}

void disque_stocker(disque_t* disque, char* nomFichierIn, char* nomFichierOut)
{
	off_t adrVide, res;


	adrVide = disque_rechercherVide(disque);

	/* On se place à la fin pour stocker le fichier et en profiter pour récupérer l'adresse à laquelle sera stocké le fichier */
	if((res = lseek(disque->fd, 0L, SEEK_END)) == -1)
	{
		fprintf(stderr, "Erreur lors de la récupération de l'adresse à laquelle sera stocké le fichier %s\n", nomFichierIn);
		exit(EXIT_FAILURE);
	}
	inode_stockerFichier(disque->fd, nomFichierIn, nomFichierOut);


	if(lseek(disque->fd, res, SEEK_SET) == -1)
	{
		fprintf(stderr, "Erreur lors du déplacement à la première adresse vide : %ld \n", adrVide);
		exit(EXIT_FAILURE);
	}


	/* On rentre l'adresse du fichier à l'emplacement vide dans la table */
	if(lseek(disque->fd, adrVide, SEEK_SET) == -1)
	{
		fprintf(stderr, "Erreur lors du déplacement à la première adresse vide : %ld \n", adrVide);
		exit(EXIT_FAILURE);
	}
	sauverOffT(disque->fd, res);
}

void disque_extraire(disque_t* disque, char* nomFichierIn, char* nomFichierOut)
{
	off_t adrFich;

	adrFich = disque_rechercherFichier(disque, nomFichierIn);
	printf("Adresse du fichier recherché : %ld\n", adrFich);
	if(lseek(disque->fd, adrFich, SEEK_SET) == -1)
	{
		fprintf(stderr, "Erreur lors du déplacement du pointeur sur le fichier %s du disque.\n", nomFichierIn);
	}

	inode_extraireFichier(disque->fd, nomFichierOut);
}

off_t disque_rechercherFichier(disque_t* disque, char* nomFichier)
{

	int i;
	off_t adrInode = (off_t)0;
	table_t* table;
	inode_t* inode;

	/* On se place au début du fichier pour lire la table d'adresse principale */
	deplacerDebut(disque->fd);

	do
	{
		/* On charge la table au pointeur */
		table = table_charger(disque->fd);

		/* On recherche l'inode correspondant au fichier recherché */
		for(i = 0; i < _NB_ADR_TABLE_; i++)
		{
			if(table->adresses[i] != 0)
			{
				if((adrInode = lseek(disque->fd, table->adresses[i], SEEK_SET)) == -1)
				{
					fprintf(stderr, "Erreur lors du déplacement vers l'inode à l'adesse : %ld\n", table->adresses[i]);
					exit(EXIT_FAILURE);
				}

				inode = inode_charger(disque->fd);

				if(!strcmp(inode->nom, nomFichier))
				{
					printf("Le fichier \"%s\" a été trouvé à l'adresse : %ld\n", nomFichier, adrInode);
					return adrInode;
				}
			}
		}

		/* Si il existe une autre table, on déplace le pointeur du fd à son adresse */
		if(table->prochTable != 0)
		{
			if(lseek(disque->fd, table->prochTable, SEEK_SET) == -1)
			{
				fprintf(stderr, "Erreur lors du déplacement vers la table à l'adresse : %ld \n", table->prochTable);
				exit(EXIT_FAILURE);
			}
		}
	}while(table->prochTable != 0);

	table_supprimer(&table);
	inode_supprimer(&inode);
	return 0;
}

off_t disque_rechercherVide(disque_t* disque)
{
	int i;
	table_t *table1, *table2;
	off_t adrRet = 0, adrTable = 0;

	/* On se place au début du fichier pour lire la table d'adresse principale */
	deplacerDebut(disque->fd);

	do
	{
		/* On charge la table au pointeur */
		table1 = table_charger(disque->fd);
		/* Si il existe une autre table, on déplace le pointeur du fd à son adresse */
		if(table1->prochTable != 0)
		{
			adrTable = table1->prochTable;
			if((adrRet = lseek(disque->fd, table1->prochTable, SEEK_SET)) == -1)
			{
				fprintf(stderr, "Erreur lors du déplacement vers la table à l'adresse : %ld \n", table1->prochTable);
				exit(EXIT_FAILURE);
			}
		}
		else
		{
			/* On parcours les adresses */
			for(i = 0; i < _NB_ADR_TABLE_; i++)
			{
				if(table1->adresses[i] == 0)
				{
					return adrRet;
				}
				else
				{
					adrRet += sizeof(off_t);
				}
			}
		}

	}while(table1->prochTable != 0);

	/* On sauvegarde la nouvelle table à la fin du fichier et on récupère l'adresse */
	if((table1->prochTable = lseek(disque->fd, 0L, SEEK_END)) == -1)
	{
		fprintf(stderr, "Erreur lors du déplacement à la fin du fichier pour placer la nouvelle table\n");
	}
	table2 = table_creer();
	table_sauvegarder(disque->fd, table2);

	/* On sauvegarde la table modifiée ci-dessus */
	if(lseek(disque->fd, adrTable, SEEK_SET) == -1)
	{
		fprintf(stderr, "Erreur lors du déplacement vers la table d'adresses à l'adresse : %ld \n", adrTable);
		exit(EXIT_FAILURE);
	}
	table_sauvegarder(disque->fd, table1);	

	table_supprimer(&table1);
	table_supprimer(&table2);
	return adrTable;
}