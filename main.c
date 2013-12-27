/***************************************************************************
                          main.c  -  description
                             -------------------
    begin                : Fri Dec 12 17:57:05 MSK 2003
    copyright            : (C) 2003 by Rumata
    email                : rumata@gmx.co.uk
 ***************************************************************************/


#include "main.h"
   
int main(int argc, char *argv[]) {
  short param;
  int stat;
  pid_t pid, t;
  char str[MAXLINE + 1],*files[3];

  if (argc > 1) {
    printf("Assuming commands to be in \"%s\"\n",argv[1]);
    redirect(argv[1], STDIN_FILENO, O_RDONLY);
  }
  

  while (sh_gets(str) != NULL)
    switch(pid = fork()) {
      case 0:
        normalize(str, &param, files);
        if (R_IN & param) {
          redirect(files[0], STDIN_FILENO, O_RDONLY);
        }
        if (R_OUT & param) {
          redirect(files[1], STDOUT_FILENO, O_CREAT | O_TRUNC | O_WRONLY);
        } else if (R_OUTA & param){
          redirect(files[2], STDOUT_FILENO, O_APPEND | O_WRONLY);
        }
        if (BGR & param)
          switch(fork()){
            case 0: break;
            case -1: print_err("Can't fork!\n");
            default: exit(0);
          }
        do_work(str);        
        exit(0);
      case -1:
        print_err("Can't fork!\n");
      default:
        while((t = wait(&stat))==-1);
        if (stat != 0)
          printf("Warning, end status: %d\n", stat);
    }

  printf("\nThe End!\n");    
  return EXIT_SUCCESS;
}

