/***************************************************************************
                          commons.c  -  description
                             -------------------
    begin                : Fri Dec 12 2003
    copyright            : (C) 2003 by Rumata
    email                : rumata@gmx.co.uk
 ***************************************************************************/
#include "commons.h"


void
print_err(char*str) {
  fprintf(stderr,"\nErr: %s Errno: %d.\n", str, errno);
  exit(1);
}

char*
sh_gets(char* str) {
  printf(PS);
  return fgets(str, MAXLINE, stdin);  
}

void
redirect(char* file, int d, int mode) {
  int newd;                
  newd = open(file, mode, 0666);
  if (newd == -1) print_err("Problems with file!");
  dup2(newd,d);
  close(newd);
}

