#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

#define TABLE_SIZE (sizeof(table)/sizeof(*table))

int main(int argc, char *argv[]){

    struct{
        char *text;
        int mode;
    } table[] = {
        {"exist", F_OK},
        {"execute", X_OK},
        {"write", W_OK},
        {"read", R_OK}
    };

    int i;
    if(argc<2){
        fprintf(stderr, "Usage : %s <file1> <file2> ... <fileN>\n", argv[0]);
        exit(1);
    }

    for(i = 1; i<TABLE_SIZE; i++){
        
        if(access(argv[1], table[i].mode) != -1)
            printf("%s -ok\n", table[i].text);
        else
            printf("%s\n", table[i].text);
    
    }
}