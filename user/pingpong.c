#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define RD 0
#define WR 1

//int main(int argc, char *argv[]) {
//
//	int p[2];
//	int pid;
//	pipe(p);
//
//	if(fork() == 0) {
//		pid = getpid();
//		char buffer[1];
//
//		if(read(p[0], buffer, 1) != 1){
//			fprintf(2, "failed to read in child\n");
//			exit(1);
//		}
//		close(p[0]);
//		printf("%d: received ping\n", pid);
//
//		if(write(p[1], "a", 1) != 1){
//			fprintf(2, "failed to write in child\n");
//			exit(1);
//		}
//		close(p[1]);
//
//		exit(0);
//	} else {
//		pid = getpid();
//		char buffer[1];
//
//		if(write(p[1], "x", 1) != 1){
//			fprintf(2, "failed to write in parent\n");
//			exit(1);
//		}
//		close(p[1]);
//
//		wait(0); 
//      // 如果没有wait，父进程的read不是阻塞的，可以直接从p[0]读取父进程写入p[1]的数据，从而直接结束父进程，而子进程的read会一直block
//		if(read(p[0], buffer, 1) != 1){
//			fprintf(2, "failed to read in parent\n");
//			exit(1);
//		}
//		// printf("%s", buffer);
//		close(p[0]);
//		printf("%d: received pong\n", pid);
//
//		exit(0);
//	}
//}


int main(int argc, char *argv[]){
    int p1[2], p2[2];
    int pid;

    pipe(p1);
    pipe(p2);

    if(fork() == 0){
        pid = getpid();
        char buffer[1];

        close(p1[WR]);
        close(p2[RD]);

        if(read(p1[RD], buffer, 1) != 1){
            fprintf(2, "failed to read in child\n");
            exit(1);
        }

        close(p1[RD]);
		printf("%d: received ping\n", pid);

        if(write(p2[WR], buffer, 1) != 1){
            fprintf(2, "failed to write in child\n");
            exit(1);
        }

        close(p2[WR]);
        exit(0);
    } else{

        pid = getpid();
        char buffer[1];

        close(p1[RD]);
        close(p2[WR]);

        if(write(p1[WR], "x", 1) != 1){
            fprintf(2, "failed to write in parent");
            exit(1);
        }

        close(p1[WR]);

        if(read(p2[RD], buffer, 1) != 1){
            fprintf(2, "failed to read in parent");
            exit(1);
        }

        close(p2[RD]);
        printf("%d: received pong\n", pid);

        exit(0);
    }
}