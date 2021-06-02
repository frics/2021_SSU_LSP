#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<time.h>
#include<pthread.h>
#include<errno.h>
#include<sys/ipc.h>
#include<sys/shm.h>

#define BUF_SIZE 20000

typedef struct _args {
    int start;
    int end;
}Args;
Args *input;
int **input_matrix;
int **next_matrix;

int row, col; //row, col의 default 크기는 1이다.

//매번 프로세스 죽이고 살리고 ㅇ안할라면 signal 사용해라
//input.matrix로부터 matrix를 읽어온다.
void read_matrix(int);
//생성된 다음 세대 matrix를 쓴다.
void write_matrix(int, int**);
//row, col 크기만큼의 2차원 배열을 할당
int **alloc_matrix();
//동적할당된 2차원 배열 해제
void free_matrix(int **);
//해당 세포가 다음 세대에 살아있는지 확인
int check_live(int, int, int **);
void seq_process();

int *rows_per_parallel(int);

int **alloc_shmem_matrix(int *);
void free_shmem_matrix(int *);

void proc_parallel(int, int*);
void *run_thread(void *);
void thread_parallel(int, int*);


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

void copy_matrix(){
    for(int i=0; i<row; i++){
        for(int j=0; j<col; j++)
            input_matrix[i][j] = next_matrix[i][j];
    }
}

int main(int argc, char *argv[]){
    int fd;
    clock_t start, end;
    long long elapsed;
    int gen, op, proc_thread;
    int *rows;

    while(1){     
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
        
        //row, col의 default 크기는 1이다.
        row = 1;
        col = 1;
        //시간 측정 시작
        start = clock();
        read_matrix(fd);
     
        switch(op){
            case 1:
                printf("프로그램이 종료됩니다.\n");
                exit(0);
            case 2:
                for(int i=1; i<=gen; i++){
                    seq_process();
                    write_matrix(i, next_matrix);
                    free_matrix(input_matrix);
                    input_matrix = next_matrix;
                }
                end = clock();
                elapsed = (long long)end-start;
                printf("elapsed_time: %lld ms\n", elapsed);
                free_matrix(input_matrix);
                break;
            case 3:
                printf("생성할 Process의 개수를 입력하시오 : ");
                scanf("%d", &proc_thread);
                getchar();
                //process별 연산 행을 할당
                rows = rows_per_parallel(proc_thread);

                 //공유 메모리 할당
                int *shmid = (int *)malloc(sizeof(int)*row);
                void **shm_matrix = (void **)malloc(sizeof(void*)*row);
               // next_matrix = (int **)malloc(sizeof(int *)*row);
                for(int i=0; i<row; i++){
                    if((shmid[i] = shmget((key_t)i, sizeof(int)*col, IPC_CREAT | 0666)) == -1){
                        fprintf(stderr, "%dth shmget failed(%s)\n", i, strerror(errno));
                        exit(0);
                    }
                    if((shm_matrix[i] = shmat(shmid[i], NULL, 0)) == (int *)-1){
                        fprintf(stderr, "%dth shmat failed(%s)\n", i, strerror(errno));
                        exit(0);
                    }
                }
                next_matrix = (int **)shm_matrix;
               
                for(int i=1; i<=gen; i++){
                    proc_parallel(proc_thread, rows);
                    write_matrix(i, next_matrix);
                    copy_matrix();
                }

                end = clock();
                elapsed = (long long)end-start;
                printf("elapsed_time: %lld ms\n", elapsed);
                free(rows);

                for(int i=0; i<row; i++){
                     if(shmdt(shm_matrix[i]) == -1) {
                        fprintf(stderr, "%dth detach fail(%s)\n", i,  strerror(errno));
                        exit(1);
                    }
                    if(shmctl(shmid[i], IPC_RMID, 0) == -1){
                        fprintf(stderr, "%dth remove fail(%s)\n", i,  strerror(errno));
                        exit(1);
                    }
                }
                free(shm_matrix);
                free(shmid);
                //공유 메모리 해제
                
                break;
            case 4:
                printf("생성할 Thread의 개수를 입력하시오 : ");
                scanf("%d", &proc_thread);
                getchar();
                 //process별 연산 행을 할당
                rows = rows_per_parallel(proc_thread);
                printf("where seg\n");
                //입력된 세대 수만큼 연산
                for(int i = 1; i <= gen; i++){
                    thread_parallel(proc_thread, rows);
                  //  write_matrix(i, next_matrix);
                   // free_matrix(input_matrix);
                    //input_matrix = next_matrix;
                }

                end = clock();
                elapsed = (long long)end-start;
                printf("elapsed_time: %lld ms\n", elapsed);
                free_matrix(input_matrix);
                free(rows);
                break;
            default:
                fprintf(stderr, "invalid input please enter again.\n");
                getchar();
        }
    }  
}

int **alloc_matrix(){
    int **matrix = (int **)malloc(sizeof(int*)*row);

    for(int i=0; i<row; i++)
        matrix[i] = (int *)malloc(sizeof(int)*col);

    return matrix;
}

void free_matrix(int **matrix){
    for(int i=0; i<row; i++) 
        free(matrix[i]);
    free(matrix);
}

int **alloc_shmem_matrix(int *shmid){

    shmid = (int *)malloc(sizeof(int)*row);
    int **shm_matrix = (int **)malloc(sizeof(int *)*row);

    for(int i=0; i<row; i++){
        if((shmid[i] = shmget(0, sizeof(int)*row, IPC_CREAT|0666)) == -1){
            fprintf(stderr, "%dth shmget failed(%s)\n", i, strerror(errno));
            exit(0);
        }
        if((shm_matrix[i] = shmat(shmid[i], NULL, 0)) == (int*)-1){
            fprintf(stderr, "%dth shmat failed(%s)\n", i, strerror(errno));
            exit(0);
        }
    }
    return shm_matrix;
}

void free_shmem_matrix(int *shmid){
    for(int i=0; i<row; i++){
        if(shmctl(shmid[i], IPC_RMID, 0) == -1){
            fprintf(stderr, "%dth remove fail(%s)\n", i,  strerror(errno));
            exit(1);
        }
    }
    free(next_matrix);
    free(shmid);
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
    printf("row : %d, col %d\n", row, col);
    //row, col max 사이즈 확인
    if(row > 10000 || col > 10000){
        fprintf(stderr, "Invalid matrix size(%d, %d)\n", row, col);
        exit(1);
    }

    //읽어온 크기만큼의 배열을 동적할당한다.
    input_matrix = alloc_matrix();


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
    int live_neighbor = 0;
    printf("seg? in check \n");
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
    if(matrix[cur_row][cur_col] == 1 && (live_neighbor >= 3 && live_neighbor <= 6 ))
        return 1;
    if(matrix[cur_row][cur_col] == 0 && live_neighbor == 4 )
         return 1;

    return 0;
}

void seq_process(){
    //next matrix 할당
    next_matrix = alloc_matrix();
    //다음 세대 연산
    for(int i=0; i<row; i++){
       for(int j=0; j<col; j++)
           next_matrix[i][j] = check_live(i,j, input_matrix);
    }
     
}
//process나 thread별 행을 할당한다.
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

void proc_parallel(int proc_thread, int *rows) {


    for(int p=0, index=0; p<proc_thread; p++){
        
        switch(fork()){
            case 1:
                printf("Fork fail\n");
                break;
            case 0:
                
                for(int i = index; i < index+rows[p]; i++){
                    for(int j=0; j<col; j++){
                        next_matrix[i][j] = check_live(i,j, input_matrix);
                    }
                }
                //printf("%d process finished\n", getpid());
                exit(1);
            default:
                //printf("Fork NEXT Child Process\n");
                break;
        }
        index += rows[p];
    }
    int ret;
    while((ret = wait(NULL)) > 0);
  
}
void *run_thread(void *data){

    //Args input = *((Args *)data);
    int id = *((int *)data);
    //print(input.next_matrix);
    printf("id : %d\n", id);
    // printf("%d, %d\n", input[id].start, input[id].end);
    //for(int i = input[id].start; i<input[id].end; i++){
     //   for(int j=0; j<col; j++)
     //       next_matrix[i][j] = check_live(i, j, input_matrix);
   // }
   
    return 0;
}

void thread_parallel(int proc_thread, int *rows){
    pthread_t *thread;
     //next matrix 동적 할당
    next_matrix = alloc_matrix();
    //thread 할당
    thread = (pthread_t *)malloc(sizeof(pthread_t)*proc_thread);
    //각 thread별로 인자를 넘길 구조체 동적할당
    input = (Args *)malloc(sizeof(Args)*proc_thread);
   
    for(int i=0; i<proc_thread; i++){
        if(i != 0){
            input[i].start = input[i-1].end;
            input[i].end = input[i-1].end + rows[i];
        }else{
            input[i].start = 0;
            input[i].end = rows[i];
        }
    }
     
    for(int i=0; i<proc_thread; i++)
        pthread_create(&thread[i], NULL, run_thread, (void *)&i);
    
    printf("in here\n");
    for(int i=0; i<proc_thread; i++)
        pthread_join(thread[i], NULL);

    free(thread);
    free(input); 
}
