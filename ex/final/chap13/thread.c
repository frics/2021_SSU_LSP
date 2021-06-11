#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<sys/time.h>


#define NTHREAD 50000

void *do_nothing(void *null){
    int i;
    i = 0;
    return 0;
}


int main(int argc, char *argv[]){
    int rc, i, j, detachstate;

    struct timeval start, end;
    double delay_time;

    pthread_t tid;
    pthread_attr_t attr;
    pthread_attr_init(&attr);

    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    gettimeofday(&start,NULL);

    for(j=0; j<NTHREAD; j++){
       
        rc = pthread_create(&tid, &attr, do_nothing, NULL);
        
        if(rc){
            fprintf(stderr, "ERROR create thread %d\n", rc);
            exit(-1);
        }

        rc = pthread_join(tid, NULL);
        if(rc){
            fprintf(stderr, "join error\n");
            exit(-1);
        }
    }

    gettimeofday(&end, NULL);
    delay_time = (end.tv_sec - start.tv_sec) + ((end.tv_usec - start.tv_usec) /1000000);
    printf("%f seconds\n", delay_time);
    
    pthread_attr_destroy(&attr);
    pthread_exit(NULL);
}