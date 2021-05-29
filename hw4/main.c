#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/time.h>
#include<pthread.h>

#define BUF_SIZE 512

typedef struct _args {
    int start;
    int end;
    int **input_matrix;
    int **next_matrix;
}Args;

int **input_matrix;

int row=1, col = 1; //row, col의 default 크기는 1이다.

void read_matrix(int);
void write_matrix(int, int**);
void free_matrix(int **);
int **seq_process(int **);
int **proc_parallel(int, int*, int **);
int *rows_per_parallel(int);
int ** thread_parallel(int, int*, int **);
int check_live(int, int, int **);


void print(int **matrix){
    printf("\n\nPRINT MATRIX\n");
    for(int i=0; i<row; i++){
        for(int j=0; j<col; j++){
            printf("%d ", matrix[i][j]);
        }
        putchar('\n');
    }
    putchar('\n');
}

int main(int argc, char *argv[]){
    int fd;
    struct timeval start, end;
    long long elapsed;
    int **next_matrix;

    while(1){
        int op;
        int gen;
        int proc_thread;
        int *rows;
             
        printf("(1) 프로그램 종료\t(2) 순차처리\t(3) Process 병렬처리\t(4) Thread 병렬처리\n");
        printf("input : ");
        scanf("%d", &op);
        getchar();
        printf("gen : ");
        scanf("%d", &gen);

        if((fd = open(argv[1], O_RDONLY)) < 0 ){
            fprintf(stderr, "open error for %s\n", "input.matrix");
            exit(1);
        }
        gettimeofday(&start, NULL);
        row = 1;
        col = 1;
        read_matrix(fd);
    
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
                gettimeofday(&end, NULL);
                elapsed = (end.tv_sec - start.tv_sec) * 1000000 + end.tv_usec - start.tv_usec;;
		        printf("elapsed_time: %lld us\n", elapsed);
                break;
            case 3:
                printf("생성할 Process의 개수를 입력하시오 : ");
                scanf("%d", &proc_thread);
                getchar();
                rows = rows_per_parallel(proc_thread);

                for(int i=1; i<=gen; i++){
                    next_matrix = proc_parallel(proc_thread, rows, input_matrix);
                    print(next_matrix);
                    write_matrix(i, next_matrix);
                    
                    input_matrix = next_matrix;
                }
                gettimeofday(&end, NULL);
                elapsed = (end.tv_sec - start.tv_sec) * 1000000 + end.tv_usec - start.tv_usec;;
		        printf("elapsed_time: %lld us\n", elapsed);
                free(rows);
                break;
            case 4:
                printf("생성할 Thread의 개수를 입력하시오 : ");
                scanf("%d", &proc_thread);
                getchar();

                rows = rows_per_parallel(proc_thread);

                for(int i = 1; i <= gen; i++){
                   // printf("GEN : %d\n", i);
                    next_matrix = thread_parallel(proc_thread, rows, input_matrix);
                    //print(next_matrix);
                    write_matrix(i, next_matrix);
                    free(input_matrix);
                    input_matrix = next_matrix;
                }
                gettimeofday(&end, NULL);
                elapsed = (end.tv_sec - start.tv_sec) * 1000000 + end.tv_usec - start.tv_usec;;
		        printf("elapsed_time: %lld us\n", elapsed);
                free(rows);
                break;
            default:
                fprintf(stderr, "invalid input please enter again.\n");
                getchar();
        }
        free_matrix(input_matrix);
        //free_matrix(next_matrix);
        
    }  
}
void free_matrix(int **matrix){
    for(int i=0; i<row; i++) 
        free(matrix[i]);
    free(matrix);
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
    printf("alloc nex input\n");
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
}
void write_matrix(int gen, int **matrix){
   
    int fd_w;
    char filename[30];
    int row_size = col*2+1;
    char *rowbuf = malloc(sizeof(char)*row_size);
    
    sprintf(filename, "gen_%d.matrix", gen);

    if((fd_w = open(filename, O_WRONLY|O_CREAT|O_TRUNC, 0644)) < 0 ){
        fprintf(stderr, "open error for %s\n", filename);
        exit(1);
    }
    
    for(int i=0; i<row; i++){
        for(int j=0; j<col; j++){
            sprintf(rowbuf+j*2, "%c ", matrix[i][j]+'0');
        }
        if(i != row-1){
            rowbuf[row_size-1] = '\n';
            write(fd_w, rowbuf, row_size);    
        }else{
            write(fd_w, rowbuf, row_size-1);
        }
       
        //printf("%s", rowbuf);
        memset(rowbuf, '\0', row_size);
    }
    close(fd_w);
}
int check_live(int cur_row, int cur_col, int **matrix){
    int cell = matrix[cur_row][cur_col];
    int live_neighbor = 0;
  
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

int **seq_process(int **input_matrix){
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
int *rows_per_parallel(int proc_thread){
    //process/thread 별 row갯수를 저장할 int형 배열 동적할당
    int *rows = malloc(sizeof(int)*proc_thread);
   //process/thread 별 담당 row 갯수 저장
    for(int i=0, pos = 0; i<row; i++){
        rows[pos++]++;
        if(pos == proc_thread)
            pos = 0;
    }
    return rows;
}

int **proc_parallel(int proc_thread, int *rows, int **input_matrix){

    int **next_matrix;
    //next matrix 동적 할당
    next_matrix = (int**)malloc(sizeof(int*)*row);
    for(int i=0; i<row; i++)
        next_matrix[i] = (int *)malloc(sizeof(int)*col);
   
    int cnt = 0;
    for(int p=0, index=0; p<proc_thread; p++){
        
        switch(vfork()){
            case 1:
                printf("Fork fail\n");
                break;
            case 0:
                for(int i = index; i < index+rows[p]; i++){
                    for(int j=0; j<col; j++){
                        cnt++;
                        next_matrix[i][j] = check_live(i,j, input_matrix);
                    }
                }
                _exit(1);
            default:
                //printf("Fork %dth Child Process\n", p);
                break;
        }
        index += rows[p];
    }
    int ret;
    while((ret = wait(NULL)) > 0);
       // printf("ret : %d\n", ret);
    //printf("cnt : %d\n", cnt);
    
  // printf("all process exited\n");
   return next_matrix;
}
void *run_thread(void *data){
    Args input = *((Args *)data);

    for(int i = input.start; i<input.end; i++){
        for(int j=0; j<col; j++)
            input.next_matrix[i][j] = check_live(i, j, input.input_matrix);
    }
    return 0;
}
int **thread_parallel(int proc_thread, int *rows, int **input_matrix){
    printf("???????\n");
    pthread_t *thread;
    int **next_matrix;
    //thread 할당
    thread = (pthread_t *)malloc(sizeof(pthread_t)*proc_thread);
     //next matrix 동적 할당
    next_matrix = (int**)malloc(sizeof(int*)*row);
    for(int i=0; i<row; i++)
        next_matrix[i] = (int *)malloc(sizeof(int)*col);

    Args *input = (Args *)malloc(sizeof(Args)*proc_thread);
    // Args input;
    // input.start = 0;
    // input.end = 0;
    // input.input_matrix = input_matrix;
    // input.next_matrix = next_matrix;
  // print(input_matrix);
    for(int i=0; i<proc_thread; i++){
        if(i != 0){
            input[i].start = input[i-1].end;
            input[i].end = input[i-1].end + rows[i];
        }else{
            input[i].start = 0;
            input[i].end = rows[i];
        }
        input[i].input_matrix = input_matrix;
        input[i].next_matrix = next_matrix;
    }

    for(int i=0; i<proc_thread; i++){
        printf("non start : %d\nend : %d\n", input[i].start, input[i].end);
        pthread_create(&thread[i], NULL, run_thread, (void *)&input[i]);
    }
    for(int i=0; i<proc_thread; i++){
        pthread_join(thread[i], NULL);
        printf("%d thread joined\n", i);
    }
    free(thread);
    free(input);
    return next_matrix;
    
}
