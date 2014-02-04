#ifndef _TABLE_
#define _TABLE_
	

	#include <sys/types.h>
	#include <stdlib.h>
	#include <stdio.h>
	#include "virtualFS.h"
	#include "utils.h"

	typedef struct table_type
	{
		off_t adresses[ _NB_ADR_TABLE_ ];
		off_t prochTable;
	}table_t;

	table_t* table_creer();
	void table_supprimer(table_t**);
	void table_afficher(table_t*);
	table_t* table_charger(int);
	void table_sauvegarder(int, table_t*);

#endif