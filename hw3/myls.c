#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>//getopt()
#include<sys/stat.h>
#include<pwd.h> //getpwuid()
#include<grp.h> //getgrgid()
#include<time.h> //ctime()
#include<dirent.h> //for read directory

#define MAX_FILE_CNT 100
#define MAX_FILE_PATH 1024
#define MAX_FILE_NAME 50

struct option{
	int opt_i;
	int opt_l;
	int opt_t;
}option;

int total;

void mode_to_str(mode_t , char *);
void printLmode(char *, struct stat *);
void sortStat(struct stat *, char **, int, int);
int is_dir(char *);
void error(char *);

int main(int argc, char *argv[]){
	struct stat *info = malloc(sizeof(struct stat)*MAX_FILE_CNT);
	char *statName[MAX_FILE_CNT];
    char opt;
	
    while((opt = getopt(argc, argv, "ilt")) != -1){
        switch(opt){
            case 'i':
                option.opt_i = 1;
                break;
            case 'l':
				option.opt_l = 1;
                break;
            case 't':
				option.opt_t = 1;
                break;
        }
    }

	//지정 디렉터리나 파일이 없을 경우 현재 디렉터리를 인자로 넣어준다.
	//ls명령어의 기본 현재 디렉터리 확인
	if(optind == argc)
		argv[argc++] = ".";

	if(is_dir(argv[optind])){
		DIR *dp;
		struct dirent *dFile;
		
		char fPath[MAX_FILE_PATH];
		
		if((dp = opendir(argv[optind])) != NULL){
			int cnt = 0;
			while(( dFile = readdir(dp)) != NULL){
				
				sprintf(fPath, "%s/%s", argv[optind], dFile->d_name);
				if(dFile->d_name[0] == '.')
					continue;	
				if(lstat( fPath, info+cnt) < 0)
					error(dFile->d_name);
				statName[cnt] = dFile->d_name;
				total += (info+cnt)->st_blocks;
				cnt++;
			}
			//알파벳 순으로 정렬인지 생성 시간순으로 정렬인지 check
			//t 옵션이 있을 경우 생성 시간 순으로 정렬
			int flag =0;
			if(option.opt_t == 1)
				flag =1;
			sortStat(info, statName, cnt, flag);

			if(option.opt_l == 1){
				printf("total  %d\n", total/2);
				//sortStat(info, statName, cnt);
				for(int i=0; i<cnt; i++){
					if(option.opt_i == 1)
						printf("%ld ",info[i].st_ino);
					printLmode(statName[i], info+i);
				}
			}else{
				
				for(int i =0; i<cnt; i++){
					if(i%10 == 0 && i != 0)
						putchar('\n');
					if(option.opt_i ==1)
						printf("%ld ",info[i].st_ino);
					printf("%s   ", statName[i]);
				}
				putchar('\n');
			}
		}

	}else{
		if(lstat( argv[optind], info) < 0)
			error(argv[optind]);
		
		printLmode(argv[optind], info);
	}
	
	
	return 0;

}
void error(char *name){
	printf("myls: can't access file'%s'\n", name);
	exit(1);
}
int is_dir(char* fname){
    struct stat buff;
    if(lstat(fname, &buff) != 0)
        error(fname);
    return S_ISDIR(buff.st_mode);
}

void mode_to_str(mode_t mode, char *buf){
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

void printLmode(char *name, struct stat *info){
	char str[11];
	mode_to_str(info->st_mode, str);
	printf("%s", str);
	printf("%2ld", info->st_nlink);
	printf("%10s ", getpwuid(info->st_uid)->pw_name);
	printf("%s ", getgrgid(info->st_gid)->gr_name);
	printf("%5ld ", info->st_size);
	printf("%.12s ", ctime(&info->st_mtime)+4);
	printf("%s\n", name);
}

void sortStat(struct stat *info, char **name, int cnt, int flag){
	struct stat tmpStat;
	char *tmpName;
	//flag가 1이면 st_mtime(수정시간)기준으로 정렬

	for(int i=cnt-1; i>0; i--){
		for(int j=0; j<i; j++){
			//flag == 1일때 st_mtime비교, flag == 0일때 이름 알파벳 비교 
			if((flag == 1 && !(info[j].st_mtime < info[j+1].st_mtime)) || (flag == 0 && !(name[j][0] > name[j+1][0])))
				continue;	
			tmpStat = info[j];
			tmpName = name[j];
			info[j]= info[j+1];
			name[j] = name[j+1];
			info[j+1] = tmpStat;
			name[j+1] = tmpName;
		}
	}
}
