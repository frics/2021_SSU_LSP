#include<stdio.h>
#include<stdlib.h> //exit()
#include<sys/stat.h> //chmod()
#include<string.h> //strchr()
#include<errno.h> //errno
#include<unistd.h> //getopt)()



int main(int argc , char *argv[]){

  
    int mode = 0;
    char *ptr;
    //옵션이 들어왔을 경우 무시하고 처리
    while(getopt(argc, argv, "cfvR") != -1);

    //setuid(0);
    //setgid(0);

    if(((ptr =strchr(argv[optind], '+')) != NULL) || ((ptr =strchr(argv[optind], '-')) != NULL)){
        char op = ptr[0];
        char perm[2][4] = {{'u', 'g', 'o', 'a'},
                    {'r', 'w', 'x'}};
        char *ugo = strtok(argv[optind], &op);
        char *rwx = strtok(NULL, &op);
        struct stat info;
        char flag[3];
        int to_change;

        if(lstat( argv[optind+1] , &info) < 0){
             fprintf(stderr, "%s : chmod error : %s\n", argv[optind+1], strerror(errno));
             exit(1);
        }
        if(strcmp(ugo, "a") == 0)
            ugo = "ugo";
        
        mode = info.st_mode;
        mode%=010000;
       
        for(int i=0; i<3; i++){
            int check= 0;
            int div  = 1;
            //printf("checking %c\n", perm[0][i]);
            if(strchr(ugo, perm[0][i]) != NULL){
                for(int j=0; j<3; j++){
                    if(strchr(rwx, perm[1][j]) != NULL){
                        check += (4/div);
                    }
                    div *= 2;
                }
                
            }
            flag[i] = check+'0';
        }
        sscanf(flag, "%o", &to_change);
        printf("to chage : %o\n", to_change);
        switch(op){
            case '+':
                mode |= to_change;
                break;
            case '-':
                mode &= ~to_change;
                break;
            case '=':
                mode = to_change;
                break;
            default:
                fprintf(stderr, "%s : chmod error : invalid operator\n", argv[optind+1]);
                break;
        }
        printf("mode in string : %o\n", mode);
     
    }else{
        printf("%s\n", argv[optind]);
        sscanf(argv[optind], "%o", &mode); 
        printf("%o\n", mode);
    }
    
    printf("ccc :%0o\n", mode);

    if(mode > 07777 || mode < 0){
        fprintf(stderr, "%s : chmod error : invalid permission\n", argv[optind+1]);
        exit(1);
    }
    //printf("%o\n", mode);
    if(chmod(argv[optind+1], mode) < 0)
        fprintf(stderr, "%s : chmod error : %s\n", argv[optind+1], strerror(errno));
    exit(0);
}

