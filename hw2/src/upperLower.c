#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<unistd.h>
#define MAX_NAME_SIZE 20
#define MAX_BUF_SIZE 100

void abcChange(char *str){
  
    for(int i=0; i<strlen(str); i++){
        if(str[i] >= 'a' && str[i] <= 'z')
            str[i]-=32;
        else if(str[i] >= 'A' && str[i] <= 'Z')
            str[i]+=32;
    }
}


int main(){
    int fd1;
    int fd2;
    int read_size;
    char buffer[MAX_BUF_SIZE] = {0, };
    char name[MAX_NAME_SIZE];
   
    printf("input :");
    scanf("%s", name);

    if((fd1 = open(name, O_RDONLY)) <0){
        fprintf(stderr, "open error for %s\n", name);
        exit(1);
    }
    abcChange(name);
    if((fd2 = open(name, O_WRONLY | O_CREAT | O_TRUNC, 0666)) <0){
        fprintf(stderr, "open error for %s\n", name);
        exit(1);
    }
    
    while((read_size = read(fd1, buffer, MAX_BUF_SIZE)) > 0 ){
        abcChange(buffer);
        write(fd2, buffer, read_size);
    }

    close(fd1);
    close(fd2);
    return 0;
}