#include<stdio.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<pwd.h> //getpwuid()
#include<grp.h> //getgrgid()
#include<unistd.h>//getcwd()
#include<string.h>//strlen()
#include<time.h>//ctime()
#include<dirent.h>

int total;

void printLmode(char *, struct stat*);

void strmode(mode_t mode, char *buf){
	const char chars[] = "-rwxrwxrwx";

    switch(mode & S_IFMT){
		case S_IFREG:
			buf[0] = '-';
			break;
		case S_IFDIR:
			buf[0] = 'd';
			break;
		case S_IFCHR:
			buf[0] = 'c';
			break;
		case S_IFBLK:
			buf[0] = 'b';
			break;
		case S_IFIFO:
			buf[0] = 'p';
		case S_IFLNK:
			buf[0] = '|';
			break;
		case S_IFSOCK:
			buf[0] = 's';
		default:
			buf[0] = '?';
	}
	for(size_t i =1; i<10; i++)
		buf[i] = (mode & (1 << (9-i))) ? chars[i] : '-';
	
	buf[10] = '\0';
}

int main(int argc, char **argv){
    struct stat info;
    char mode_str[11];
	DIR *dp;
	char *dir;
	struct dirent *d;
	char path[1024];


	if(argc == 1)
		dir = ".";
	else
		dir  = argv[1];
	//dir
	if((dp = opendir(dir)) != NULL){
		while(( d = readdir(dp)) != NULL){
			sprintf(path, "%s/%s", dir, d->d_name);
			if(d->d_name[0] == '.')
				continue;	
			if(lstat( path, &info) < 0){
				printf("error\n");
				exit(1);
			}
		
			printLmode(d->d_name, &info);
		}
	//file
	}else{
		if(lstat( dir, &info) < 0){
				printf("error\n");
				exit(1);
		}
		printLmode(dir, &info);
	}
	printf("total : %d\n", total/2);

        printf("%ld  ", info.st_ino);
}

void printLmode(char *name, struct stat *info){
	char str[11];
	
	strmode(info->st_mode, str);
	printf("%s", str);
	printf("%2ld", info->st_nlink);
	printf("%10s ", getpwuid(info->st_uid)->pw_name);
	printf("%s ", getgrgid(info->st_gid)->gr_name);
	printf("%5ld ", info->st_size);
	printf("%.12s ", ctime(&info->st_mtime)+4);
	printf("%s\n", name);
	//if(str[0] != 'd')
		total += info->st_blocks;

}