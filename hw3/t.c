#include<stdio.h>
#include<unistd.h>

static char *__tty_color[] = {
      "\033[0;40;30m",       /* 0   black on black */
      "\033[0;40;31m",       /* 1   red */
      "\033[0;40;32m",       /* 2   green */
      "\033[0;40;33m",       /* 3   brown */
      "\033[0;40;34m",       /* 4   blue */
      "\033[0;40;35m",       /* 5   magenta */
      "\033[0;40;36m",       /* 6   cyan */
      "\033[0;40;37m",       /* 7   light gray */
      "\033[1;40;30m",       /* 0   gray */
      "\033[1;40;31m",       /* 1   brightred */
      "\033[1;40;32m",       /* 2   brightgreen */
      "\033[1;40;33m",       /* 3   yellow */
      "\033[1;40;34m",       /* 4   brightblue */
      "\033[1;40;35m",       /* 5   brighmagenta */
      "\033[1;40;36m",       /* 6   brightcyan */
      "\033[1;40;37m",       /* 7   white */
};
int main(int argc, char *argv[]){


     for(int i=80; i<300; i++){
          printf("\x1b[%dm", i);
          printf(" %d check\n", i);
          printf("\x1b[0m");
     }
     
}