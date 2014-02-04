#ifndef _UTILS_
#define _UTILS_
	
	#include <stdio.h>
	#include <fcntl.h>
	#include <unistd.h>
	#include <stdlib.h>
	#include <string.h>
	#include <errno.h>

	void sauverChaine(int, char*);
	void sauverInt(int, int);
	void sauverOffT(int, off_t);
	char* chargerChaine(int);
	int chargerInt(int);
	off_t chargerOffT(int);
	off_t recupererAdresse(int);
	off_t recupererAdresseFin(int, int);
	off_t deplacerDebut(int);
	int fichierExiste(char*);

#endif