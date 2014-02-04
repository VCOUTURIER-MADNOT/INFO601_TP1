#include "utils.h"

void sauverChaine( int _fd, char* _string)
{
	int taille;

	taille = strlen(_string);

	if(write(_fd, &taille, sizeof(int)) == -1)
	{
		perror("Erreur lors de l'ecriture de la taille dans le fichier");
		exit(EXIT_FAILURE);
	}

	if(write(_fd, _string, sizeof(char)*taille) == -1)
	{
		perror("Erreur lors de l'ecriture de la chaine dans le fichier");
		exit(EXIT_FAILURE);	
	}
}

void sauverInt(int _fd, int _nombre)
{
	if(write(_fd, &_nombre, sizeof(int)) == -1)
	{
		perror("Erreur lors de l'ecriture de l'entier dans le fichier");
		exit(EXIT_FAILURE);
	}
}

void sauverOffT(int _fd, off_t _nombre)
{
	if(write(_fd, &_nombre, sizeof(off_t)) == -1)
	{
		perror("Erreur lors de l'ecriture de l'entier dans le fichier");
		exit(EXIT_FAILURE);
	}
}

char* chargerChaine(int _fd)
{
	int taille = 0;
	char* res = NULL;
	if(read(_fd, &taille, sizeof(int)) == -1)
	{
		perror("Erreur lors de la lecture de la taille dans le fichier");
		exit(EXIT_FAILURE);
	}
	res = (char*) malloc(sizeof(char) * taille);
	if(read(_fd, res, taille * sizeof(char)) == -1)
	{
		perror("Erreur lors de la lecture de la chaine dans le fichier");
		exit(EXIT_FAILURE);
	}
	return res;
}


int chargerInt(int _fd)
{
	int nombre;

	if(read(_fd, &nombre, sizeof(int)) == -1)
	{
		perror("Erreur lors de la lecture de l'entier dans le fichier");
		exit(EXIT_FAILURE);
	}
	return nombre;
}

off_t chargerOffT(int _fd)
{
	off_t nombre;

	if(read(_fd, &nombre, sizeof(off_t)) == -1)
	{
		perror("Erreur lors de la lecture de l'entier dans le fichier");
		exit(EXIT_FAILURE);
	}

	return nombre;
}

off_t recupererAdresse(int fd)
{
	off_t res;

	if((res=lseek(fd, 0, SEEK_CUR))==-1)
	{
		perror("Erreur lors de la récupération de l'adresse");
		exit(EXIT_FAILURE);
	}

	return res;
}

off_t recupererAdresseFin(int fd, int depl)
{
	off_t res, cur;

	if((cur=lseek(fd, 0, SEEK_CUR))==-1)
	{
		perror("Erreur lors de la récupération de l'adresse");
		exit(EXIT_FAILURE);
	}

	if((res=lseek(fd, 0, SEEK_END))==-1)
	{
		perror("Erreur lors de la récupération de l'adresse");
		exit(EXIT_FAILURE);
	}

	if(!depl)
	{
		if(lseek(fd, cur, SEEK_SET)==-1)
		{
			perror("Erreur lors de la récupération de l'adresse");
			exit(EXIT_FAILURE);
		}
	}

	return res;
}

off_t deplacerDebut(int fd)
{
	off_t res;

	if((res=lseek(fd, 0, SEEK_SET))==-1)
	{
		perror("Erreur lors de la récupération de l'adresse");
		exit(EXIT_FAILURE);
	}

	return res;
}

int fichierExiste(char* nomFichier)
{
	if(open(nomFichier, O_RDONLY) == -1)
	{
		/* Si le fichier est introuvable */
		if(errno == ENOENT)
		{
			return 0;
		}

		fprintf(stderr, "Erreur lors de l'ouverture du fichier\n");
		exit(EXIT_FAILURE);
	}
	return 1;
	
}