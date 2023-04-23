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

    buf[n] = 0; // 将'\n'改为'\0',不加这一条，最后一个参数string不会terminate
    if(n == 0) return 0;

    // 参数以空格分割开来
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
    // 参数列表不需要xargs
    for(int i = 1; i < argc; i++){
        fix_argv[i - 1] = argv[i];
    }
    // printf("%s\n", fix_argv[1]);

    int end;
    while((end = readline(fix_argv, argc - 1)) != 0){
        // marks the end of argument list
        fix_argv[end] = 0;
        if(fork() == 0){
            exec(argv[1], fix_argv);
        }
        // 加不加else是有区别的，如果加了else，只会在父进程里wait，如果没加，父子进程都会wait
        wait(0);
    }
    exit(0);
}