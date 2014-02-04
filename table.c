#include "table.h"


table_t* table_creer()
{
	int i;
	table_t* table = (table_t*)malloc(sizeof(table_t));
	for(i = 0; i < _NB_ADR_TABLE_; i++)
	{
		table->adresses[i] = (off_t)0;
	}
	table->prochTable = (off_t)0;
	return table;
}

void table_supprimer(table_t** table)
{
	*table = NULL;
	free(*table);
}

void table_afficher(table_t* table)
{
	int i;

	printf("Affichage de la table :\n");
	if(table != NULL)
	{
		for(i = 0; i < _NB_ADR_TABLE_; i++)
		{
			printf("\t- Adresse de l'inode n°%3d : %ld\n",i, table->adresses[i]);	
		}
		printf("\n\t- adresse de la prochaine table : %ld \n", table->prochTable);
	}
	else
	{
		printf("Erreur, la table à afficher est nulle\n");
	}
}

table_t* table_charger(int fd)
{
	int i;

	/* On crée la table qui sera retournée */
	table_t* table = table_creer();

	/* On récupère toutes les adresses */
	for(i = 0; i < _NB_ADR_TABLE_; i++)
	{
		table->adresses[i] = chargerOffT(fd);
	}

	/* On récupère l'adresse de la prochaine table */
	table->prochTable = chargerOffT(fd);

	return table;
}

void table_sauvegarder(int fd, table_t* table)
{
	int i;
	/* On sauvegarde toutes les adresses dans le fichier */
	for(i = 0; i < _NB_ADR_TABLE_; i++)
	{
		sauverOffT(fd, table->adresses[i]);
	}

	/* On sauvegarde l'adresse de la prochaine table dans le fichier */
	sauverOffT(fd, table->prochTable);
}