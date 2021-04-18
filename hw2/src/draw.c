#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>

int main(){
    int fd;
    int side;
    char ch;
    char *name;
    printf("input :");
    scanf("%d %c %s", &side, &ch, name);

    if((fd = open(name, O_WRONLY | O_CREAT | O_TRUNC, 0666)) <0 ){
        fprintf(stderr, "open error for %s\n", name);
    }
    for(int i= 0; i<abs(side); i++){
        if(side > 0){
            for(int j = 0; j<i+1; j++)
                write(fd, (char *)&ch, sizeof(ch));
            
        }else{
            for(int j=abs(side)-i; j>0; j--)
                write(fd, (char *)&ch, sizeof(ch));
        }
        write(fd, "\n", sizeof(char));
    }
    close(fd);
    return 0;
}