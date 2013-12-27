/***************************************************************************
                          do_work.h  -  description
                             -------------------
    begin                : Fri Dec 12 2003
    copyright            : (C) 2003 by Rumata
    email                : rumata@gmx.co.uk
 ***************************************************************************/
#ifndef _do_work_h
#define _do_work_h

#include <ctype.h>
#include <sys/wait.h>
#include <sys/types.h>

#include "commons.h"

void
do_work (char*);

void
_arrkill(int*);

void
_arrcp(int*, int*);

void
normalize(char*, short*, char**);


#endif


 

