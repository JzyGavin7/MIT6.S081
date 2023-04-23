#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {

	int p[2];
	int pid;
	pipe(p);

	if(fork() == 0) {
		pid = getpid();
		char buffer[1];

		if(read(p[0], buffer, 1) != 1){
			fprintf(2, "failed to read in child\n");
			exit(1);
		}
		close(p[0]);
		printf("%d: received ping\n", pid);

		if(write(p[1], "a", 1) != 1){
			fprintf(2, "failed to write in child\n");
			exit(1);
		}
		close(p[1]);

		exit(0);
	} else {
		pid = getpid();
		char buffer[1];

		if(write(p[1], "x", 1) != 1){
			fprintf(2, "failed to write in parent\n");
			exit(1);
		}
		close(p[1]);

		wait(0);
		if(read(p[0], buffer, 1) != 1){
			fprintf(2, "failed to read in parent\n");
			exit(1);
		}
		// printf("%s", buffer);
		close(p[0]);
		printf("%d: received pong\n", pid);

		exit(0);
	}
}