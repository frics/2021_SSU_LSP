#include<stdio.h>
#include<stdlib.h>
#include<string.h> //strcmp()
#include<unistd.h>//getopt()
#include<sys/stat.h> //struct stat, lstat()
#include<pwd.h> //getpwuid()
#include<grp.h> //getgrgid()
#include<time.h> //ctime()
#include<dirent.h> //for read directory

#define MAX_FILE_CNT 100
#define MAX_FILE_PATH 1024
#define MAX_FILE_NAME 50
#define ANSI_COLOR_GREEN    "\x1b[92m"
#define ANSI_COLOR_BLUE     "\x1b[94m"
#define ANSI_COLOR_SKY 	    "\x1b[96m"
#define ANSI_COLOR_BLACK    "\x1b[30m"
#define ANSI_COLOR_RED_BACK "\x1b[41m"
#define ANSI_COLOR_YEL_BACK "\x1b[43m"

#define ANSI_COLOR_RESET    "\x1b[0m"


struct option{
	int opt_i;
	int opt_l;
	int opt_t;
	int opt_a;
}option;

int total_blk = 0;

int getFileStat(struct stat *i, char **, char *);
void mode_to_str(mode_t , char *);
void printLmode(char *, struct stat *);
void printNormal(char *, struct stat *);
void sortStat(struct stat *, char **, int, int);
void check_color(mode_t);
int is_dir(char *);
void error(char *);

int main(int argc, char *argv[]){
	
	struct stat *info = malloc(sizeof(struct stat)*MAX_FILE_CNT);
	char *statName[MAX_FILE_CNT];
    char opt;
	//list할 파일의 개수를 저장
	int cnt = 0;
	//입력에 i,l,t옵션이 존재하는 지 확인
	//존재하면 해당 옵션변수를 1로 변경
    while((opt = getopt(argc, argv, "ilta")) != -1){
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
			case 'a':
				option.opt_a = 1;
        }
    }
	//지정 디렉터리나 파일이 없을 경우 현재 디렉터리를 인자로 넣어준다.
	//ls명령어의 기본 현재 디렉터리 확인
	if(optind == argc)
		argv[argc++] = ".";
	
	cnt = getFileStat(info, statName, argv[optind]);

	//option.opt_t의 값을 확인하여 알파벳순 정렬(default)할지, 수정 시간순으로 정렬할지 인자를 넘겨준다.
	//option.opt_t의 default값은 0이고 t옵션이 입력 되었을 경우 1
	sortStat(info, statName, cnt, option.opt_t);
	//ls file이 아니고 -l옵션이 들어왔을때 total block 개수를 출력해준다.
	if(cnt > 1 && option.opt_l)
		printf("total  %d\n", total_blk/2);
	//getFileStat()을 통해서 가져온 모든 stat 구조체 정보를 출력한다.
	for(int i=0; i<cnt; i++){
		//-i옵션이 있을 경우 앞에 inode번호를 출력한다.
		if(option.opt_i)
			printf("%-7ld ",info[i].st_ino);
			
		//-l 옵션이 있을 경우 printLmode함수를 호출하여 stat구조체의 모든 맴버의 상세 정보를 출력한다.
		if(option.opt_l){
			printLmode(statName[i], info+i);
		}else{
			//-l 옵션이 아닌 경우 단순 파일 이름 들만 출력한다.
			printNormal(statName[i], info+i);
			//10번째 파일 항목마다 개행을 수행, 마지막 항목일 경우에도 개행
			if((i%9 == 0 && i != 0) || i == (cnt-1))
				putchar('\n');
		}
	}
	free(info);
//	printf("max : %d\n", max_file_len);
	return 0;

}

int getFileStat(struct stat *info, char **statName, char *input){
	int cnt=0;
	//ls directory
	if(is_dir(input)){
		DIR *dp;
		struct dirent *dFile;
		char fPath[MAX_FILE_PATH];
		//해당 dir을 open하고 dirent를 하나씩 lstat으로 확인하여 stat구조체를 가져온다.
		if((dp = opendir(input)) != NULL){
			//디렉터리 엔트리 전체 확인
			while(( dFile = readdir(dp)) != NULL){
				//절대 경로를 생성해준다.
				sprintf(fPath, "%s/%s", input, dFile->d_name);
				//숨김파일의 경우 list하지 않는다. 
				if(dFile->d_name[0] == '.' && option.opt_a != 1)
					continue;	
				//해당 파일의 stat구조체 확득
				if(lstat( fPath, info+cnt) < 0)
					error(dFile->d_name);
				//statName에 현재 이름을 저장
				statName[cnt] = dFile->d_name;
				total_blk += (info+cnt)->st_blocks;
				cnt++;
			}	
		}
	//ls file
	}else{
		cnt++;
		statName[0] = input;
		//인자로 들어온 파일 이름의 stat구조체를 획득
		if(lstat( statName[0], info) < 0)
			error(input);
	}
	return cnt;
}
void error(char *name){
	//파일 엑세스에 오류가 발생할 경우 출력문을 출력하고 프로그램 종료
	printf("myls: can't access file'%s'\n", name);
	exit(1);
}
int is_dir(char* fname){
	//fname이 디렉터리 파일인지 일반 파일인지 확인
    struct stat buff;
    if(lstat(fname, &buff) != 0)
        error(fname);
    return S_ISDIR(buff.st_mode);
}

void mode_to_str(mode_t mode, char *buf){
	const char chars[] = "-rwxrwxrwx";
	//해당 파일이 어떤 종류인지 확인
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
			buf[0] = 'l';
			break;
		case S_IFSOCK:
			buf[0] = 's';
		default:
			buf[0] = '?';
	}
	//권한을 확인하여 저장
	for(size_t i =1; i<10; i++)
		buf[i] = (mode & (1 << (9-i))) ? chars[i] : '-';
	
	const char ch[2][3] = {{'s', 's', 't'},
				{'S', 'S', 'T'}};
	for(int i=0; i<3; i++){
		if(mode & (S_ISUID>>i)){
			if(buf[(i+1)*3] == 'x')
				buf[(i+1)*3] = ch[0][i];
			else	
				buf[(i+1)*3] = ch[1][i];
		}
	}
	buf[10] = '\0';
}

void printLmode(char *name, struct stat *info){
	//-l 옵션 항목 출력
	char str[11];
	mode_to_str(info->st_mode, str);

	printf("%s ", str);
	printf("%2ld", info->st_nlink);
	printf(" %-9s ", getpwuid(info->st_uid)->pw_name);
	printf("%-9s ", getgrgid(info->st_gid)->gr_name);
	printf("%5ld  ", info->st_size);
	printf("%.12s ", ctime(&info->st_mtime)+4);
	check_color(info->st_mode);
	printf( "%s" ANSI_COLOR_RESET, name);
	if(str[0] == 'l'){
		char *path = malloc(sizeof(char)*MAX_FILE_NAME);
		memset(path, '\0', MAX_FILE_NAME);
		printf(" -> ");
		if(readlink(name, path, MAX_FILE_NAME) < 0)
			path = "can't find path";
		else{
			if(lstat( path, info) < 0)
				error(path);
			check_color(info->st_mode);
		}
		printf("%s"ANSI_COLOR_RESET, path);
	}
	putchar('\n');
}
void printNormal(char *name, struct stat *info){

	char str[11];
	mode_to_str(info->st_mode, str);
	check_color(info->st_mode);
	printf("%.30s  "ANSI_COLOR_RESET, name);
	//printf("%.30s  "ANSI_COLOR_RESET, name);
}
void check_color(mode_t mode){
	for(int i=0; i<3; i++){
		//user, group, other 중에 파일 실행 권한이 있을 경우
		//초록색
		if(mode &(S_IXUSR >> i*3))
			printf(ANSI_COLOR_GREEN);
	}
	//디렉터리일 경우 파란색 -> 디렉터리의 경우 실행권한이 있어도 파란색으로 출력이 된다.
	if(S_ISDIR(mode))
		printf(ANSI_COLOR_BLUE);
	//symbolic link 파일일 경우 하늘색으로 출력
	else if(S_ISLNK(mode))
		printf(ANSI_COLOR_SKY);
	//GID bit이 설정되어 있을 경우 노란색 배경에 검은색 글씨로 출력
	if(mode &(S_ISGID)){
		printf(ANSI_COLOR_YEL_BACK);
		printf(ANSI_COLOR_BLACK);
	}
	//uid bit이 설정 되어 있을 경우 빨간색 배경으로 출력
	//uid, gid bit이 모두 설정되어 있을 경우 uid bit이 더 상위이다.
	if(mode & (S_ISUID)){
		printf(ANSI_COLOR_RESET);
		printf(ANSI_COLOR_RED_BACK);
	}
}
void sortStat(struct stat *info, char **name, int cnt, int opt_t){
	struct stat tmpStat;
	char *tmpName;
	//인자로 들어온 t옵션을 기준으로 어떠한 순서로 정렬할지 선택한다.
	for(int i=cnt-1; i>0; i--){
		for(int j=0; j<i; j++){
			//flag == 1일때 st_mtime비교, flag == 0일때 이름 비교 
			if((opt_t == 1 && !(info[j].st_mtime < info[j+1].st_mtime)) || (opt_t== 0 && !(strcmp(name[j],name[j+1]) > 0)))
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
