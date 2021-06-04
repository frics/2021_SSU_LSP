#include<stdio.h>
#include<fcntl.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<time.h>


char **matrix;

void make_matrix(int row, int col){
    srand((unsigned int) time(NULL));
    matrix = (char **)malloc(sizeof(char*)*row);
    for(int i=0; i<row; i++)
        matrix[i] = (char *)malloc(sizeof(char)*col);

    for(int i=0; i<row; i++){
        for(int j=0; j<col; j++){
            if(rand()%3 == 0)
                matrix[i][j] = '0';
            else
                matrix[i][j] = '1';
        }
    }

}

void write_matrix(int row, int col){
   
    int fd;
    int row_size = col*2;
    char *rowbuf = malloc(sizeof(char)*row_size);


    if((fd = open("input.matrix", O_WRONLY|O_CREAT|O_TRUNC, 0644)) < 0 ){
        fprintf(stderr, "open error for %s\n", "input.matrix");
        exit(1);
    }
    
    for(int i=0; i<row; i++){
        for(int j=0; j<col; j++){
            sprintf(rowbuf+j*2, "%c ", matrix[i][j]);
        }
        if(i != row-1){
            rowbuf[row_size-1] = '\n';
            write(fd, rowbuf, row_size);    
        }else{
            write(fd, rowbuf, row_size-1);
        }
        //printf("%s", rowbuf);
        memset(rowbuf, '\0', row_size);
    }
    close(fd);
}


int main(int argc, char *argv[]){
   
   if(argc != 3){
       fprintf(stderr, "Invalid input\n");
    }
    int row = atoi(argv[1]);
    int col = atoi(argv[2]);

    make_matrix(row, col);

    write_matrix(row, col);
    printf("%d by %d matrix is created.\n", row, col);

}