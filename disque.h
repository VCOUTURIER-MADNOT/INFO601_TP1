#ifndef _DISQUE_
#define _DISQUE_
	
	#include "table.h"
	#include "inode.h"
	#include "utils.h"
	#include <stdlib.h>

	typedef struct disque_type
	{
		int fd;
	}disque_t;

	disque_t* disque_ouvrir();
	void disque_fermer(disque_t**);
	void disque_afficherTables(disque_t*);
	void disque_afficherFichiers(disque_t*);
	void disque_stocker(disque_t*, char*, char*);
	void disque_extraire(disque_t*, char*, char*);
	off_t disque_rechercherFichier(disque_t*, char*);
	off_t disque_rechercherVide(disque_t*);
#endif