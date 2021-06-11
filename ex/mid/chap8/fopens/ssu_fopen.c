#include<stdio.h>
#include<stdlib.h>

int main(void){
    char *fname = "../ssu_test.txt";
    char *mode = "r";

    if(fopen(fname, mode)  == NULL){
        frpintf(stderr, "fopen error for %s\n", fname);
        exit(1);
    }else{
        printf("Success!\n Filename : <%s>, mode : <%s>\n", fname, mode);

    }
    exit(0);
}