#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<unistd.h>
#include<time.h>


void write_rand(int num, int fd){

    int cnt, random_number;
    char buffer[10];

    for(int i=0; i<num; i++){
        random_number = rand()%100;
        cnt = sprintf(buffer, "%d", random_number);
        buffer[cnt] = ' ';
        write(fd, buffer, cnt+1);
        memset(buffer, '\0', 10);
    }
}

void write_sort(int fd, int* array, int num){
    int cnt;
    char buffer[10];
    for(int i=0; i<num; i++){
        cnt = sprintf(buffer, "%d", array[i]);
        buffer[cnt] = ' ';
        write(fd, buffer, cnt+1);
        memset(buffer, '\0', 10);
    }
}
int get_file_size(int fd){
    int size = lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);
    return size;
}

void save_array(char *str, int* array){
    char *cut = strtok(str, " ");
    int i=0;
    while(cut != NULL){
        array[i] = atoi(cut);
        cut = strtok(NULL, " ");
        i++;
    }

}

void sort(int* array, int cnt){
    int tmp;
    for(int i=0; i<cnt; i++){
        for(int j=0; j<cnt-1; j++){
            if(array[j]<array[j+1]){
                tmp = array[j];
                array[j] = array[j+1];
                array[j+1] = tmp;
            }
        }
    }
}

int main(){
    //rand()함수에서 매번 다른 숫자를 출력할 수 있게 srand 사용
    srand((unsigned int) time(NULL));

    int fd1;
    int fd2;
   
    int number;
    printf("INPUT : ");
    scanf("%d", &number);

    if(number >= 10){
        if((fd1 = open("number.txt",O_RDWR | O_CREAT | O_TRUNC, 0666)) <0){
                fprintf(stderr, "open error for %s\n", "number.txt");
                exit(1);
        }

        write_rand(number, fd1);

        int size =  get_file_size(fd1);
    
        char *to_read = (char *)malloc(sizeof(char)*size);
        int *array = (int *)malloc(sizeof(int)*number);

        read(fd1, to_read, size);
        
        save_array(to_read, array);
        sort(array, number);

        if((fd2 = open("sort.txt",O_WRONLY | O_CREAT | O_TRUNC, 0666)) <0){
                fprintf(stderr, "open error for %s\n", "sort.txt");
                exit(1);
        }
        write_sort(fd2, array, number);

        close(fd1);
        close(fd2);
    }else{
        printf("INPUT SHOULD OVER 10\n");
    }
}