#include<stdio.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<unistd.h>//getcwd()
#include<string.h>//strlen()
#include<time.h>//ctime()
#include<dirent.h>


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
/*
	char name[100];
	getcwd(name, 100);
	
//	name[strlen(name)] = '/';
// 	name[strlen(name)] = '*';
	printf("pwd : %s\n", name);*/
	DIR *dir;
	struct dirent *ent;
	dir = opendir("./");

	while((ent = readdir(dir)) != NULL){
		printf("%s\n", ent->d_name);
	}
	closedir(dir);

    if(lstat(argv[1], &info) < 0){
        printf("err\n");
        exit(1);
    }
	char *modify_time = ctime(&info.st_mtime);
	modify_time[strlen(modify_time)-1] = '\0';
    strmode(info.st_mode, mode_str);
	
	if(argc > 2){
		if(strcmp(argv[2], "-i") == 0){
			printf("%ld  ", info.st_ino);
		}else if(strcmp(argv[2], "-l") == 0){
			printf("%s  ", mode_str);
			printf("%ld  ", info.st_nlink);
			printf("%d  ", info.st_uid);
			printf("%d  ", info.st_gid);
			printf("%ld  ", info.st_size);
			printf("%s  ", modify_time);
		}
	}
	printf("%s\n", argv[1]);
    //printf("%s\t%d\t%d\t%s\n", mode_str,info.st_uid, info.st_gid, ctime(&info.st_mtime));


        
}
