#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<unistd.h>

#define BUF_SIZE 512

int **matrix;
int row=1, col = 1; //row, col의 default 크기는 1이다.
void read_matrix(int);
void seq_process();
void proc_parallel();
void thread_parallel();



int main(){
    int fd;

    if((fd = open("input.matrix", O_RDONLY)) < 0 ){
        fprintf(stderr, "open error for %s\n", "input.matrix");
        exit(1);
    }
    read_matrix(fd);
    while(1){
        int op;
        int gen;
        printf("(1) 프로그램 종료\t(2) 순차처리\t(3) Process 병렬처리\t(4) Thread 병렬처리\n");
        printf("input : ");
        scanf("%d", &op);
        getchar();

        switch(op){
            case 1:
                printf("프로그램이 종료됩니다.\n");
                exit(0);
            case 2:
             //   seq_process();
                break;
            case 3:
               // proc_parallel();
                break;
            case 4:
                //thread_parallel();
                break;
            default:
                fprintf(stderr, "invalid input please enter again.\n");
                getchar();
        }
    }  
}

void read_matrix(int fd){
    int read_size;
    char *buf = malloc(sizeof(char)*BUF_SIZE);
    //파일을 읽어서 matrix의 크기를 파악한다.
    while((read_size = read(fd, buf, BUF_SIZE)) > 0){
        for(int i=0; i<read_size; i++){
            if(buf[i] == ' ' && row == 1)
                col++;
            else if(buf[i] == '\n')
                row++;
        }
    }
    //읽어온 크기만큼의 배열을 동적할당한다.
    matrix = (int**)malloc(sizeof(int*)*row);
    for(int i=0; i<row; i++){
        matrix[i] = (int *)malloc(sizeof(int)*col);
    }
   
    //matrix의 값을 2차원 배열에 쓴다.
    lseek(fd, 0, SEEK_SET); //다시 파일을 읽기 전 offset 초기화
    memset(buf, '\0', BUF_SIZE);
    for(int i=0; i<row; i++){
        //input matrix의 1줄씩 읽어서 matrix 배열에 저장한다.
        read(fd, buf, col*2+1);
        for(int j=0; j<col*2; j+=2){
            if(buf[j]=='1')
                matrix[i][j/2] = 1;
            else
                matrix[i][j/2] = 0;
        }
        memset(buf, '\0', BUF_SIZE);
    }
    printf("print matrix\n");
    for(int i=0; i<row; i++){
        for(int j=0; j<col; j++)
            printf("%d ", matrix[i][j]);
        putchar('\n');
    }

    printf("row : %d, col : %d\n", row, col);
}

int check_neighbor(int row, int col){

}