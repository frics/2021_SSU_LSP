#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/time.h>
#include<pthread.h>
#include<errno.h>
#include<sys/ipc.h>
#include<sys/shm.h>
//matrix 최대 input인 10000*1000을 읽기 위함
#define BUF_SIZE 20002

char **input_matrix;
char **next_matrix;
int *rows;

int gen;
long **used_proc_thread;
int row, col; //row, col의 default 크기는 1이다.

void copy_matrix(){
   for(int i=0; i<row; i++){
        for(int j=0; j<col; j++){
            input_matrix[i][j] = next_matrix[0][i*col+j];
        }
    }
}

char **alloc_matrix(int isShm, int *shmid){
    char **matrix;

    if(isShm != 1){ 
        matrix = (char **)malloc(sizeof(char*)*row);
        for(int i=0; i<row; i++)
            matrix[i] = (char *)malloc(sizeof(char)*col);

    }else{
        matrix = (char **)malloc(sizeof(char *));
        if((shmid[0] = shmget(0, sizeof(char)*row*col, IPC_CREAT|0666)) == -1){
            fprintf(stderr, "shmget failed(%s)\n",strerror(errno));
            exit(0);
        }
        if((void *)(matrix[0] = shmat(shmid[0], NULL, 0)) == (void *)-1){
            fprintf(stderr, "shmat failed(%s)\n", strerror(errno));
            exit(0);
        }
    }
    return matrix;

}

void free_matrix(int isShm, char **matrix, int *shmid){
    if(isShm != 1){
        for(int i=0; i<row; i++) 
            free(matrix[i]);
    }else{
        if(shmctl(shmid[0], IPC_RMID, 0) == -1){
            fprintf(stderr, "shm remove fail(%s)\n", strerror(errno));
            exit(1);
        }
        free(shmid);
    }
    free(matrix);
}

void read_matrix(char *filename){ 
    FILE *fp;
    char *buf = (char *)malloc(sizeof(char)*BUF_SIZE);

    if((fp = fopen(filename, "r")) < 0 ){
        fprintf(stderr, "open error for %s\n", "input.matrix");
        exit(1);
    }

    fseek(fp, 0, SEEK_SET);   
    row = 0, col = 0;
    //파일을 읽어서 matrix의 크기를 파악한다.
    while(!feof(fp)){
       fgets(buf, BUF_SIZE, fp);
       row++;
    }
    for(int i=0; i<strlen(buf); i++){
        if(buf[i] == '1' || buf[i] == '0')
            col++;
    }
    //row, col max 사이즈 확인
    if(row > 10000 || col > 10000){
        fprintf(stderr, "Invalid matrix size(%d, %d)\n", row, col);
        exit(1);
    }
    //읽어온 크기만큼의 배열을 동적할당한다.
    input_matrix = alloc_matrix(0, NULL);
    fseek(fp, 0, SEEK_SET);
    int cur=0;
    while(!feof(fp)){ 
        fgets(buf, BUF_SIZE, fp);
        for(int i=0; i<strlen(buf); i++){
            if(buf[i] == '1' || buf[i] == '0')
               input_matrix[cur][i/2] = buf[i];
        }
        memset(buf, '\0', strlen(buf));
        cur++;
    }
    free(buf);
    fclose(fp);
}

void write_matrix(int cur_gen, char **matrix){ 
    FILE *fp;
    char filename[30];
    int row_size = col*2+2;
    char *rowbuf = malloc(sizeof(char)*row_size);
    
    char *dir = "./result/";
    if(cur_gen != gen)
        sprintf(filename, "%sgen_%d.matrix", dir, cur_gen);
    else
        sprintf(filename, "%soutput.matrix", dir);

    if((fp = fopen(filename, "w")) < 0 ){
        fprintf(stderr, "open error for %s\n", filename);
        exit(1);
    }
    memset(rowbuf, '\0', row_size);
    for(int i=0; i<row; i++){
        for(int j=0; j<col; j++)
            sprintf(rowbuf+j*2, "%c ", matrix[i][j]);
        if(i != row-1)
            rowbuf[col*2] = '\n';
        fputs(rowbuf, fp);
        memset(rowbuf, '\0', row_size);
    }
    free(rowbuf);
    fclose(fp);
}

char check_live(int cur_row, int cur_col, char **matrix){
    int live_neighbor = 0;
    for(int i=cur_row-1; i <= cur_row+1; i++){
        //matrix 가장자리는 0으로 가정하므로 확인하지 않는다.
        if(i < 0 || i >= row)
            continue;    
        for(int j = cur_col -1; j <= cur_col+1; j++){
             //matrix 가장자리는 0으로 가정하므로 확인하지 않는다.
            if(j < 0 || j >= col || ((i == cur_row) && (j == cur_col)))
                continue;
            if(matrix[i][j] == '1')
                live_neighbor++;
        }
    }
    if(matrix[cur_row][cur_col] == '1' && ((live_neighbor >= 3) && (live_neighbor <= 6) ))
        return '1';
    if(matrix[cur_row][cur_col] == '0' && (live_neighbor == 4 ))
        return '1';

    return '0';
}

void seq_process(){
    //next matrix 할당
    next_matrix = alloc_matrix(0, NULL);
    //다음 세대 연산
    for(int i=0; i<row; i++){
       for(int j=0; j<col; j++)
           next_matrix[i][j] = check_live(i,j, input_matrix);
    }
     
}
//process나 thread별 행을 할당한다.
int *rows_per_parallel(int proc_thread){
    //process/thread 별 row갯수를 저장할 int형 배열 동적할당
    int *_rows = malloc(sizeof(int)*proc_thread);
    //process/thread 별 담당 row 갯수 저장
    memset(_rows, '\0', sizeof(int)*proc_thread);

    for(int i=0, pos = 0; i<row; i++){
        _rows[pos++]++;
        if(pos == proc_thread)
            pos = 0;
    }
    return _rows;
}

void proc_parallel(int cur_gen, int proc_thread) {

    for(int p=0, index=0; p<proc_thread; p++){  
        used_proc_thread[cur_gen][p] = fork();
        switch(used_proc_thread[cur_gen][p]){
            case 1:
                printf("Fork fail\n");
                break;
            case 0:
               // printf("run %d process\n", getpid());
                for(int i = index; i < index+rows[p]; i++){
                    for(int j=0; j<col; j++)
                        next_matrix[0][i*col+j] = check_live(i,j, input_matrix);
                }
                exit(1);
            default:
                break;
        }
        index += rows[p];
    }

    while(wait(NULL)>0);
}


void *run_thread(void *data){
    int id = *((int *)data);
    int start =0;
    used_proc_thread[0][id] = pthread_self();

    for(int i=0; i<id; i++)
        start+=rows[i];
    for(int i = start; i<start+rows[id]; i++){
        for(int j=0; j<col; j++)
            next_matrix[i][j] = check_live(i, j, input_matrix);
    }
    return 0;
}

void thread_parallel(int proc_thread, pthread_t *thread, int *thread_id){
     //next matrix 동적 할당
    next_matrix = alloc_matrix(0, NULL);
    //thread 할당
    for(int i=0; i<proc_thread; i++)
        pthread_create(&thread[i], NULL, run_thread, (void *)&thread_id[i]);
    for(int i=0; i<proc_thread; i++)
        pthread_join(thread[i], NULL);

}

void print_used(long **used, int proc_thread){
    printf("\nUsed Process ID\n");
    for(int i=0; i<gen ; i++){
        printf("gen[%d] : ", i+1);
        for(int j=0; j<proc_thread; j++){
            printf("%ld", used[i][j]);
            if(j != proc_thread-1)
                printf(", ");
        }
        putchar('\n');
    }
    printf("total : %d\n", gen*proc_thread);

    for(int i=0; i<gen; i++)
        free(used[i]);
    free(used);
}


int main(int argc, char *argv[]){
    FILE *fp;
    struct timeval start, end;
    long long elapsed;
    int op, proc_thread;
  

    while(1){
        if(argc < 1){
            fprintf(stderr, "Parameter Error\n");
            exit(1);
        }
        printf("(1) 프로그램 종료\t(2) 순차처리\t(3) Process 병렬처리\t(4) Thread 병렬처리\n");
        printf("Input Option : ");
        scanf("%d", &op);

        if(op==1){
            printf("프로그램을 종료합니다.\n");
            exit(0);
        }

        printf("Input Goal generation : ");
        scanf("%d", &gen);
       
        switch(op){
            case 2:
                printf("일반 모드 실행\n");
                gettimeofday(&start, NULL);
                read_matrix(argv[1]);
                for(int i=1; i<=gen; i++){
                    seq_process();
                    write_matrix(i, next_matrix);
                    free_matrix(0, input_matrix, NULL);
                    input_matrix = next_matrix;
                }
                gettimeofday(&end, NULL);
                elapsed = (end.tv_sec - start.tv_sec)*1000 + (end.tv_usec - start.tv_usec)/1000;
                printf("elapsed_time: %lld ms\n", elapsed);
                free_matrix(0, input_matrix, NULL);
                break;

            case 3:
                printf("병렬 프로세스 모드 실행\n");
                printf("생성할 Process의 개수를 입력하시오 : ");

                scanf("%d", &proc_thread);
                getchar();

                gettimeofday(&start, NULL);
                read_matrix(argv[1]);
                //process별 연산 행을 할당
                rows = rows_per_parallel(proc_thread);
                
                used_proc_thread = (long **)malloc(sizeof(long *)*gen);
                for(int i=0; i<gen; i++)
                    used_proc_thread[i]=(long *)malloc(sizeof(long)*proc_thread);
                 //공유 메모리 할당
                int *shmid = (int *)malloc(sizeof(int));
                next_matrix = alloc_matrix(1, shmid);

                for(int i=1; i<=gen; i++){            
                    proc_parallel(i-1, proc_thread);
                    copy_matrix();
                    write_matrix(i, input_matrix);
                }
                gettimeofday(&end, NULL);
                elapsed = (end.tv_sec - start.tv_sec)*1000 + (end.tv_usec - start.tv_usec)/1000;
               
                print_used(used_proc_thread, proc_thread);
                printf("elapsed_time: %lld ms\n", elapsed);
                free(rows);
                free_matrix(1, next_matrix, shmid);
                break;
            case 4:
                printf("생성할 Thread의 개수를 입력하시오 : ");
                scanf("%d", &proc_thread);
                getchar();

                gettimeofday(&start, NULL);
                read_matrix(argv[1]);
                //process별 연산 행을 할당
                rows = rows_per_parallel(proc_thread);

                used_proc_thread = (long **)malloc(sizeof(long *));
                used_proc_thread[0] = (long *)malloc(sizeof(long)*proc_thread);
                //thread 생성
                pthread_t *thread = (pthread_t *)malloc(sizeof(pthread_t)*proc_thread);
                int *thread_id = (int *)malloc(sizeof(int)*proc_thread);
                for(int i=0; i<proc_thread; i++)  
                    thread_id[i] = i;
                
                //입력된 세대 수만큼 연산
                for(int i = 1; i <= gen; i++){
                    thread_parallel(proc_thread, thread, thread_id);
                    write_matrix(i, next_matrix);
                    free_matrix(0, input_matrix, NULL);
                    input_matrix = next_matrix;
                }
                gettimeofday(&end, NULL);
                elapsed = (end.tv_sec - start.tv_sec)*1000 + (end.tv_usec - start.tv_usec)/1000;
                printf("Used Thread Id\n");
                for(int i=0; i<proc_thread; i++){
                    printf("%ld", used_proc_thread[0][i]);
                    if(i!=proc_thread-1)
                        printf(", ");
                    else
                        printf("\ntotal : %d\n", proc_thread);
                }
                printf("elapsed_time: %lld ms\n", elapsed);
                free_matrix(0, input_matrix, NULL);
                free(rows);
                free(thread);
                free(thread_id);
                break;

            default:
                fprintf(stderr, "invalid input please enter again.\n");
                getchar();
        }
    }  
}
