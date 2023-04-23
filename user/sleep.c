#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]){

	if(argc != 2) {
		char *message = "usage: sleep <time>\n";
		// printf("No time argument");
		// fprintf(2, "Usage: sleep seconds\n");
		// fprintf(1, "usage: sleep <time>\n");
		write(2, message, strlen(message));
		exit(1);
	}

	int t = atoi(argv[1]);
	sleep(t);

	exit(0);	
}