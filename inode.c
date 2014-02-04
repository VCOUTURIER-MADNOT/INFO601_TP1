#include "inode.h"

inode_t* inode_creer()
{
	inode_t* inode;
	int i;
	
	inode = (inode_t*)malloc(sizeof(inode_t));
	inode->nom = (char*)malloc(sizeof(char)* _TAILLE_NOM_FICHIER_ );
	inode->taille = 0;
	inode->adresses = (off_t*)malloc(sizeof(off_t)*_NB_ADR_TABLE_);
	
	for(i = 0; i < _NB_ADR_TABLE_; i++)
	{
		inode->adresses[i] = (off_t)0;
	}

	return inode;
}

void inode_supprimer(inode_t **i)
{
	(*i)->adresses = NULL;
	free((*i)->adresses);
	(*i)->nom = NULL;
	free((*i)->nom);
	*i = NULL;
	free(*i);
}

void inode_afficher(inode_t *i)
{
	int n;

	printf("Affichage de l'i-node :\n");
	printf("\tNom du fichier : %s\n", i->nom);
	printf("\tTaille du fichier : %d\n", i->taille);
	printf("\tAdresses :\n");
	for(n = 0; n < _NB_ADR_TABLE_; n++)
	{
		printf("\t\tn°%3d : %ld\n",n, i->adresses[n]);
	}
}

void inode_sauvegarder(int fd, inode_t *i)
{
	int n;

	/* Enregistrement du nom du fichier */
	sauverChaine(fd, i->nom);

	/* Enregistrement de la taille du fichier */
	sauverInt(fd, i->taille);

	/* Enregistrement des adresses */
	for(n = 0; n < _NB_ADR_TABLE_; n++)
	{
		sauverOffT(fd, i->adresses[n]);
	}
}

inode_t* inode_charger(int fd)
{
	int n;
	char* temp = NULL;
	inode_t* inode;

	/* On crée un inode initialisé */
	inode = inode_creer();
	/* On charge le nom du fichier */
	temp = chargerChaine(fd);
	strcpy(inode->nom,temp);

	/* On charge la taille du fichier */
	inode->taille = chargerInt(fd);
	/* On charge les adresses */
	for(n = 0; n < _NB_ADR_TABLE_; n++)
	{
		inode->adresses[n] = chargerOffT(fd);
	}

	temp = NULL;
	free(temp);

	return inode;
}

void inode_stockerFichier(int fd, char *nomFichierIn, char *nomFichierOut)
{
	inode_t* inode;
	int fdIn, res = _TAILLE_BLOC_, i = 0;
	off_t taille, adrInode;
	char* chaine;

	/* On crée l'inode à l'endroit pointé par fd */
	inode = inode_creer();

	/* On met le nom du fichier out dans l'inode */
	strcpy(inode->nom, nomFichierOut);

	/* On ouvre le fichier in en lecture */
	if((fdIn = open(nomFichierOut, O_RDONLY)) == -1)
	{
		fprintf(stderr, "Erreur lors de l'ouvertur du fichier %s\n", nomFichierIn);
		exit(EXIT_FAILURE);
	}

	/* On sauvegarde l'Inode */
	adrInode = recupererAdresse(fd);

	inode_sauvegarder(fd, inode);

	/* Tant que read ne renvoie pas d'erreur (-1)*/
	chaine = (char*)malloc(sizeof(char)*_TAILLE_BLOC_);
	
	/* On lit la taille d'un bloc dans le fichierin */
	if((res = read(fdIn, chaine, _TAILLE_BLOC_ * sizeof(char))) == -1)
	{
		fprintf(stderr, "Erreur lors de la récupération du contenu du fichier %s pour le bloc n°%d\n", nomFichierIn, i);
		exit(EXIT_FAILURE);
	}
	
	while(res != 0 && i < _NB_ADR_TABLE_)
	{
		inode->taille += res;
		/* On rentre l'adresse du fichier dans la table d'adresses de l'inode */
		inode->adresses[i] = recupererAdresse(fd);
		i++;

		/* On ecrit dans le fichier out */
		if(write(fd, chaine, sizeof(char) * res) == -1)
		{
			perror("Erreur lors de l'ecriture dans le fichier");
			exit(EXIT_FAILURE);
		}

		/* On lit la taille d'un bloc dans le fichierin */
		if((res = read(fdIn, chaine, _TAILLE_BLOC_ * sizeof(char))) == -1)
		{
			fprintf(stderr, "Erreur lors de la récupération du contenu du fichier %s pour le bloc n°%d\n", nomFichierIn, i);
			exit(EXIT_FAILURE);
		}
	}

	/* On sauvegarde à nouveau l'inode */
	if((taille = lseek(fd, adrInode, SEEK_SET)) == -1)
	{
		fprintf(stderr, "Erreur lors du déplacement à l'adresse de l'inode\n");
		exit(EXIT_FAILURE);
	}
	inode_sauvegarder(fd, inode);

	/* On ferme les deux fichiers */
	if(close(fdIn) == -1)
	{
		fprintf(stderr, "Erreur lors de la fermeture du fichier %s\n", nomFichierIn);
		exit(EXIT_FAILURE);
	}
	inode_supprimer(&inode);
	chaine = NULL;
	free(chaine);
}

void inode_extraireFichier(int fd, char* nomFichierOut)
{
	inode_t* inode;
	int fdOut, i, nbCar, tailleRest, tailleLect;
	off_t adresseTemp = 0;
	char* temp;

	/* On récupère l'inode */
	inode = inode_charger(fd);
	tailleRest = inode->taille;
	/* On crée un fichier du nom du fichier sortant */
	if((fdOut = open(nomFichierOut, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR)) == -1)
	{
		perror("Erreur lors de l'ouverture du fichier");
		exit(EXIT_FAILURE);
	}
	temp = (char*)malloc(sizeof(char) * _TAILLE_BLOC_);

	/* Pour chaque adresse de l'inode */
	inode_afficher(inode);
	for(i = 0; i < _NB_ADR_TABLE_; i++)
	{
		/* On lit à l'adresse de la tables de la taille d'un bloc */
		adresseTemp = inode->adresses[i];
		tailleLect = MIN(tailleRest, _TAILLE_BLOC_);
		if(adresseTemp != 0)
		{
			if(lseek(fd, adresseTemp, SEEK_SET) == -1)
			{
				fprintf(stderr, "Erreur lors du déplacement du pointeur à l'adresse : %ld\n", adresseTemp );
				exit(EXIT_FAILURE);
			}
			if((nbCar = read(fd, temp, sizeof(char) * tailleLect)) == -1)
			{
				fprintf(stderr, "Erreur lors de la lecture du fichier %s\n", inode->nom);
				exit(EXIT_FAILURE);
			}

			/* On ecrit dans le fichier sortant */
			if(write(fdOut, temp, sizeof(char) * tailleLect) == -1)
			{
				fprintf(stderr,"Erreur lors de l'ecriture dans le fichier %s\n", nomFichierOut);
				exit(EXIT_FAILURE);
			}
		}
		tailleRest -= tailleLect;

	}
	/* On ferme le fichier */

	if(close(fdOut) == -1)
	{
		perror("Erreur lors de fermeture du fichier");
		exit(EXIT_FAILURE);
	}

	/* On libère les varibles dynamiques */
	inode_supprimer(&inode);
	free(temp);	
}