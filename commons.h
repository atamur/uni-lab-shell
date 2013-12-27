/***************************************************************************
                          commons.h  -  description
                             -------------------
    begin                : Fri Dec 12 2003
    copyright            : (C) 2003 by Rumata
    email                : rumata@gmx.co.uk
 ***************************************************************************/
#ifndef _commons_h
#define _commons_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/file.h>
#include <errno.h>

// some consts
#define PS "prog4> "
#define MAXLINE 256
#define R_IN 1
#define R_OUT 2
#define R_OUTA 4
#define BGR 8


void
print_err(char*);

char*
sh_gets(char*);

void
redirect(char*,int,int);

#endif



