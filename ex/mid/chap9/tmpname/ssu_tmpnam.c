#include<stdio.h>
#include<stdlib.h>
#define MAX_LINE 4096

int main(int argc, char *argv[]){
    
    char *arg_dir = NULL;
    char *arg_prefix = NULL;

    if(argc != 3)
        exit(1);
        
    arg_dir = argv[1][0]!='\0'? argv[1]:NULL;
    arg_prefix = argv[2][0] != '\0'? argv[2]:NULL;
    printf("created : %s\n", tempnam(arg_dir, arg_prefix));
    getchar();
    exit(0);
}