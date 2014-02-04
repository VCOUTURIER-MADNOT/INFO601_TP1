#ifndef _INODE_
#define _INODE_

	#include <stdio.h>
	#include <stdlib.h>
	#include <fcntl.h>
	#include <sys/stat.h>
	#include <unistd.h>
	#include <string.h>
	#include "virtualFS.h"
	#include "utils.h"

	typedef struct
	{
		char* nom;
		int taille;
		off_t* adresses;
	} inode_t;

	/**
	 * Creation d'un inode alloue et initialise.
	 * @return un pointeur vers l'inode
	 **/
	inode_t* inode_creer();
	 
	/**
	 * Supprimer un inode.
	 * @param i pointeur vers l'inode
	 **/
	void inode_supprimer(inode_t **i);
	 
	/**
	 * Afficher un inode a l'ecran.
	 * @param i l'inode a afficher
	 **/
	void inode_afficher(inode_t *i);
	 
	/**
	 * Sauvegarder un inode dans un disque virtuel a la position courante.
	 * @param fd le descripteur de fichier du disque virtuel
	 * @param i l'inode a sauvegarder
	 **/
	void inode_sauvegarder(int fd, inode_t *i);
	 
	/**
	 * Charger un inode a la position courante depuis un disque virtuel.
	 * @param fd le descripteur de fichier du disque virtuel
	 * @return l'inode lu
	 **/
	inode_t *inode_charger(int fd);
	 
	/**
	 * Stocker un fichier a la position courante dans le disque virtuel.
	 * @param fd le descripteur de fichier du disque virtuel
	 * @param nomFichierIn le nom du fichier a charger
	 * @param nomFichierOut le nom du fichier dans le disque virtuel
	 **/
	void inode_stockerFichier(int fd, char *nomFichierIn, char *nomFichierOut);
	 
	/**
	 * Extraire le fichier dont l'inode est situe a la position courante.
	 * @param fd le descripteur de fichier du disque virtuel
	 * @param nomFichierOut le nom du fichier dans lequel sauvegarder le fichier
	 **/
	void inode_extraireFichier(int fd, char* nomFichierOut);
	#endif