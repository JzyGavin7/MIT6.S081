#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/param.h"


int readline(char *argv[MAXARG], int index){
    char buf[512];
    int n = 0;

    while(read(0, buf + n, 1) == 1){
        if(n == 511){
            fprintf(2, "xrags: argment is to long\n");
            exit(1);
        }
        if(buf[n] == '\n') break;
        n++;
    }

    buf[n] = 0; // this step is neccesary
    if(n == 0) return 0;


    int i = 0;
    while(buf[i] == ' ' && i < n) i++;
    while(i < n){
        argv[index++] = buf + i;
        while(buf[i] != ' ' && i < n) i++;
        while(buf[i] == ' ' && i < n) buf[i++] = 0;
    }
    // printf("%s\n", argv[index-1]);

    return index;
}

int main(int argc, char *argv[]){
    if(argc <= 1){
        fprintf(2, "Usage: xargs command (args)");
        exit(1);
    }

    char *fix_argv[MAXARG];
    for(int i = 1; i < argc; i++){
        fix_argv[i - 1] = argv[i];
    }
    // printf("%s\n", fix_argv[1]);

    int end;
    while((end = readline(fix_argv, argc - 1)) != 0){
        fix_argv[end] = 0;
        if(fork() == 0){
            exec(argv[1], fix_argv);
        }
        wait(0);
    }
    exit(0);
}