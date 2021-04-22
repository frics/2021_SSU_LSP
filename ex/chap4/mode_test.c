#include<stdio.h>
#include<sys/stat.h> //mode_t


void strmode(mode_t mode, char *buf){
	//전체 권한
	const char chars[] = "rwxrwxrwx";
	for(size_t i =0; i<9; i++){
		printf("%o\n", (1 << (8-i)));
		buf[i] = (mode & (1 << (8-i))) ? chars[i] : '-';
	}
	buf[9] = '\0';
}

int main(void){
	char buf[10];
	mode_t examples[] = { 0, 0666, 0777, 0700, 0100, 01, 02, 03, 04, 05, 06, 07};

	size_t num_examples = sizeof(examples) / sizeof(examples[0]);
	for(size_t i = 0; i < num_examples; i++){
		strmode(examples[i], buf);
		printf("%04o is %s\n", examples[i], buf);
	}
	return 0;
}
