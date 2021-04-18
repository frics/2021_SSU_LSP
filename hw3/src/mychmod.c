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

    if(((ptr =strchr(argv[optind], '+')) != NULL) || 
    ((ptr =strchr(argv[optind], '-')) != NULL) || 
    ((ptr =strchr(argv[optind], '=')) != NULL)){
       
        //ptr에 담긴 연산자를 op에 저장
        char op = ptr[0];
        //string으로 들어온 권한 설정을 확인하기 위해 
        //미리 확인할 값을 저장
        const char perm[2][3] = {{'u', 'g', 'o'},
                    {'r', 'w', 'x'}};
        //+,-, = 기준으로 토큰을 분리
        char *ugo = strtok(argv[optind], &op);
        char *rwx = strtok(NULL, &op);
        struct stat info;
        char flag[4] = { '0', };
        int to_change;
        //입력된 파일의 mode값을 받는다.
        if(lstat( argv[optind+1] , &info) < 0){
             fprintf(stderr, "%s : chmod error : %s\n", argv[optind+1], strerror(errno));
             exit(1);
        }
        //ugo 전체에 해당하는 a가 들어왔을 때 1번째 토큰에 ugo를 넣어준다.
        if(strcmp(ugo, "a") == 0)
            ugo = "ugo";
        
        mode = info.st_mode;
        //파일 종류를 확인하는 bit를 없앤다.
        mode%=010000;
        int check;
        //들어온 인자를 확인하여 flag에 저장
        for(int i=0, check =0; i<3; i++){
            check = 0;
           // div  = 1;
            if(strchr(ugo, perm[0][i]) != NULL){
                for(int j=0; j<3; j++){
                    if(strchr(rwx, perm[1][j]) != NULL){
                            check += 4/(j+1);
                    }
                    //div *= 2;
                } 
            }
            flag[i+1] = check+'0';
        }

        for(int i=0, check =0; i<3; i++){
            if(strchr(ugo, perm[0][i]) != NULL){ 
                char ch = i<2 ? 's' : 't';
                if((strchr(rwx, ch) != NULL)){
                    check += (4/(i+1));
                    flag[0] = check+'0';
                }
            }
        }
        
        //flag에 저장되어 있는 값을 8진수 형태로 to_change 변수에 담아준다.
        sscanf(flag, "%o", &to_change);
        //op에 들어와 있는 연산자 별로 올바른 연산을 수행한다.
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
       
    }else{
        //숫자 입력 모드일때 해당 값을 8진수 형태로 mode에 담아준다.
        sscanf(argv[optind], "%o", &mode);   
    }
    //mode에 저장된 값이 8진수 07777보다 크거나 0보다 작을 경우 미리 오류 처리를 해준다.
    if(mode > 07777 || mode < 0){
        fprintf(stderr, "%s : chmod error : invalid permission\n", argv[optind+1]);
        exit(1);
    }
    //권한 변경 실행
    if(chmod(argv[optind+1], mode) < 0){
        fprintf(stderr, "%s : chmod error : %s\n", argv[optind+1], strerror(errno));
    }
    exit(0);
}

