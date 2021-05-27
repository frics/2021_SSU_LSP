#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/stat.h>
#include<sys/types.h>

#define BUF_SIZE 512

int **input_matrix;

int row=1, col = 1; //row, col의 default 크기는 1이다.
void read_matrix(int);
void write_matrix(int, int**);
int **seq_process(int **);
void proc_parallel();
void thread_parallel();
int check_live(int, int, int **);


int main(int argc, char *argv[]){
    int fd;
    int **next_matrix;

    if((fd = open(argv[1], O_RDONLY)) < 0 ){
        fprintf(stderr, "open error for %s\n", "input.matrix");
        exit(1);
    }
    read_matrix(fd);
  
    while(1){
        int op;
        int gen;
        int proc_thread;
             
        printf("(1) 프로그램 종료\t(2) 순차처리\t(3) Process 병렬처리\t(4) Thread 병렬처리\n");
        printf("input : ");
        scanf("%d", &op);
        getchar();
        printf("gen : ");
        scanf("%d", &gen);

        switch(op){
            case 1:
                printf("프로그램이 종료됩니다.\n");
                exit(0);
            case 2:
                for(int i=1; i<=gen; i++){
                     printf("GEN %d\n", i);
                     next_matrix = seq_process(input_matrix);
                     write_matrix(i, next_matrix);
                     input_matrix = next_matrix;
                }
               
                break;
            case 3:
                printf("생성할 Process의 개수를 입력하시오 : ");
                scanf("%d", &proc_thread);
                getchar();
                // proc_parallel();
                break;
            case 4:
                printf("생성할 Thread의 개수를 입력하시오 : ");
                scanf("%d", &proc_thread);
                getchar();
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
    //다음 세대 matrix를 미리 할당한다.
    input_matrix = (int**)malloc(sizeof(int*)*row);
    
   
    for(int i=0; i<row; i++)
        input_matrix[i] = (int *)malloc(sizeof(int)*col);
       
    
    
    //matrix의 값을 2차원 배열에 쓴다.
    lseek(fd, 0, SEEK_SET); //다시 파일을 읽기 전 offset 초기화
    memset(buf, '\0', BUF_SIZE);
    for(int i=0; i<row; i++){
        //input matrix의 1줄씩 읽어서 matrix 배열에 저장한다.
        read(fd, buf, col*2+1);
        for(int j=0; j<col*2; j+=2){
            if(buf[j]=='1')
                input_matrix[i][j/2] = 1;
            else
                input_matrix[i][j/2] = 0;
        }
        memset(buf, '\0', BUF_SIZE);
    }

    // printf("print matrix\n");
    // for(int i=0; i<row; i++){
    //     for(int j=0; j<col; j++)
    //         printf("%d ", input_matrix[i][j]);
    //     putchar('\n');
    // }   

    printf("row : %d, col : %d\n", row, col);
}
void write_matrix(int gen, int **matrix){
   
    int fd;
    char filename[30];
    int row_size = col*2+1;
    char *rowbuf = malloc(sizeof(char)*row_size);
    
    sprintf(filename, "gen_%d.matrix", gen);

    if((fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644)) < 0 ){
        fprintf(stderr, "open error for %sㄴㄴㄴ\n", filename);
        exit(1);
    }
    
    for(int i=0; i<row; i++){
        for(int j=0; j<col; j++){
            sprintf(rowbuf+j*2, "%c ", matrix[i][j]+'0');
        }
        if(i != row-1){
            rowbuf[row_size-1] = '\n';
            write(fd, rowbuf, row_size);    
        }else{
            write(fd, rowbuf, row_size-1);
        }
       
        printf("%s", rowbuf);
        memset(rowbuf, '\0', row_size);
    }
    close(fd);
}

int check_live(int cur_row, int cur_col, int **matrix){

    int cell = matrix[cur_row][cur_col];
    int live_neighbor = 0;
    int cnt = 0;

    for(int i=cur_row-1; i <= cur_row+1; i++){
        //matrix 가장자리는 0으로 가정하므로 확인하지 않는다.
        if(i < 0 || i >= row)
            continue;    
        for(int j = cur_col -1; j <= cur_col+1; j++){
             //matrix 가장자리는 0으로 가정하므로 확인하지 않는다.
            if(j < 0 || j >= col || ((i == cur_row) && (j == cur_col)))
                continue;
          
            if(matrix[i][j] == 1)
                live_neighbor++;
            
        }
        
    }
    if( cell == 1 && (live_neighbor >= 3 && live_neighbor <= 6 ))
        return 1;
    
    if( cell == 0 && live_neighbor == 4 )
         return 1;

    return 0;
}
int  **seq_process(int **input_matrix){
    int **next_matrix;

    next_matrix = (int**)malloc(sizeof(int*)*row);

    for(int i=0; i<row; i++)
        next_matrix[i] = (int *)malloc(sizeof(int)*col);
    
    for(int i=0; i<row; i++){
       for(int j=0; j<col; j++)
           next_matrix[i][j] = check_live(i,j, input_matrix);
    }
    return next_matrix;
     
}