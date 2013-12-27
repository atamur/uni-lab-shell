/***************************************************************************
                          do_work.c  -  description
                             -------------------
    begin                : Fri Dec 12 2003
    copyright             : (C) 2003 by Rumata
    email                : rumata@gmx.co.uk
 ***************************************************************************/
#include "do_work.h"

void
do_work (char *str) {
  char *p = str, *start = p, **argv = NULL;
  short is_pipe = 0, was_pipe = 0, in_call = 0, end = 0, str_end = 0;
  int pd_old[2],pd_new[2], pid, stat, argc = 0, maxc = 1, pcounter = 0;
  
  while ((end == 0)) {
    if (*p == '\0' || (p-str) >= MAXLINE)
      str_end = end = 1;

    if (in_call == 1) {
      if (*p == '|') {
        str_end = 1;
        in_call = 0;
        if (start == p) {
          str_end = 0;
          continue;
        }
      }

      if (isspace(*p)) str_end = 1;
      if ((str_end == 1) && (start == p)) {
        in_call = 0;
        str_end = 0;
      }
      if (str_end == 1) {
        str_end = 0;
        
        if (argc + 1 >= maxc) {
          maxc *= 2;
          argv = (char**)realloc(argv,maxc*sizeof(char*));
        }
        argv[argc] = (char*)malloc(sizeof(char)*(p - start + 1));
        strncpy(argv[argc],start,p-start);
        argv[argc][p-start] = '\0';
        argv[++argc] = NULL;

        if (end != 1) {
          while(isspace(*p)) p++;
          start = p;
          continue;
        }
      }
    } else if (isspace(*p)) ;
    else if (*p == '|') {
      is_pipe = 1;
      if (pipe(pd_new) < 0) print_err("Can't create pipe!");
      switch(pid = fork()) {
        case 0:
          dup2(pd_new[0],STDOUT_FILENO);
          _arrkill(pd_new);
          if (was_pipe > 0) {
            dup2(pd_old[1],STDIN_FILENO);
            _arrkill(pd_old);
          }          
          execvp(argv[0],argv);
          print_err("Cant't exec!");
        case -1:  print_err("Can't fork!");
      }
      pcounter++;
      for (;argc > 0;argc--)
        free(argv[argc-1]);
      _arrkill(pd_old);
      _arrcp(pd_old,pd_new);
      was_pipe = is_pipe;
      is_pipe = 0;
    } else {
       in_call = 1;
       start = p;
       continue;
    }

    p++;
    
    if (end == 1) {
      switch(pid = fork()) {
        case 0:
          if (was_pipe > 0) {
            dup2(pd_old[1],STDIN_FILENO);
            _arrkill(pd_old);
          }   
          execvp(argv[0],argv);              
          print_err("Cant't exec!");
        case -1:  print_err("Can't fork!");
      }
      for (;argc > 0;argc--)
        free(argv[argc-1]);
      if (was_pipe == 1) {
        _arrkill(pd_old);
        _arrcp(pd_old,pd_new);
        was_pipe = is_pipe;
        is_pipe = 0;
      }
      pcounter++;
    }
  }
  for (; pcounter > 0; pcounter--) {
    while((pid = wait(&stat)) == -1);
    fprintf(stderr,"proc %d ended: [%d]\n",pid, stat);
  }
}

void
normalize(char* str, short *param, char *files[3]){
  char *p = str, *fstart, *fend;
  int k;
  *param = 0;
  str[strlen(str)-1] = '\0'; //eliminate \n

  if ((p = strchr(p,'<')) != NULL) {
    *p = ' ';
    while (isspace(*p) && (*p != '\0') && (p-str <= MAXLINE)) p++;
    fstart = p;
    while (!isspace(*p) && (*p != '|') && (*p != '>') && (*p != '\0') && (p-str <= MAXLINE)) p++;
    fend = p;
    files[0] = (char*)malloc(sizeof(char)*(fend - fstart + 1));
    strncpy(files[0], fstart, fend - fstart);
    files[0][fend - fstart + 1] = '\0';
    strcpy(fstart, fend);
    p = fstart;
    *param |= R_IN;
  } else {
    *param &= ~R_IN;
    fstart = p = str;
  }
  
  if ((p = strchr(p,'>')) != NULL) {
    if (*(p+1) == '>') {
      *p = ' ';
      *(p+1) = ' ';
      k = 2;
      *param &= ~R_OUT;
      *param |= R_OUTA;
    } else {
      *p = ' ';
      k = 1;
      *param &= ~R_OUTA;
      *param |= R_OUT;
    }
    while (isspace(*p) && (*p != '\0') && (p-str <= MAXLINE)) p++;
    fstart = p;
    while (!isspace(*p) && (*p != '\0') && (p-str <= MAXLINE)) p++;
    fend = p;
    files[k] = (char*)malloc(sizeof(char)*(fend - fstart + 1));
    strncpy(files[k], fstart, fend - fstart);
    files[k][fend - fstart + 1] = '\0';
    strcpy(fstart, fend);
    p = fstart;
  } else {
    *param &= ~R_OUTA & ~R_OUT;
    p = fstart;
  }

  if ((p = strchr(p, '&')) != NULL) {
    *p = '\0';
    *param |= BGR;
  } else {
    *param &= ~BGR;
  }
}

void
_arrkill(int *arr){
  close(arr[0]);
  close(arr[1]);
}

void
_arrcp(int *to, int *from){
  to[0] = from[0];
  to[1] = from[1];
}
