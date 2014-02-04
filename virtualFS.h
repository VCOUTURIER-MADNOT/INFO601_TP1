#ifndef _VIRTUALFS_
#define _VIRTUALFS_

	/* Constantes générales */
	#define _TAILLE_NOM_FICHIER_ 50
	#define _NB_ADR_TABLE_ 20
	#define _TAILLE_BLOC_ 1024
	
	#include "inode.h"
	#include "table.h"
	#include "disque.h"
	#include <stdlib.h>

	/* Macros */

	#define MIN(x, y) (((x) < (y)) ? (x) : (y))

	#define _INODE_TEST_

#endif