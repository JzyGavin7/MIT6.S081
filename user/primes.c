#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void primes(int rd){
    int pr, num;
    // 第一个数必是primes
    if(read(rd, &pr, sizeof(pr)) != sizeof(pr)){
        fprintf(2, "primes: read fail %d\n", pr);
        exit(1);
    }
    printf("prime %d\n", pr);

    int p[2];
    pipe(p);

    // 拿出第一个数后要判断一下，不然在没有其他数据的情况继续递归，子进程读不出数据，会报read fail，但我们想要的是结束递归
    if(read(rd, &num, sizeof(num)) == sizeof(num)){
        if(fork() == 0){
            close(p[1]); // 如果这里不close，read就会block，会一直等待数据输入
            close(rd);
            primes(p[0]);
        } else{
            close(p[0]);
            do{
                if(num % pr != 0){
                    // printf("%d\n", num);
                    if(write(p[1], &num, sizeof(num)) != sizeof(num)){
                        fprintf(2, "primes: write fail\n");
                        exit(1);
                    }
                }
            }while(read(rd, &num, sizeof(num)) == sizeof(num));
            close(rd);
            close(p[1]); // 同理也会造成read block，所以要及时关闭写端
            wait(0);
        }
    }
    exit(0);
}

int main(char argc, char *argv[]){
    int p[2];
    pipe(p);

    if(fork() == 0){
        close(p[1]);
        primes(p[0]);
    }

    close(p[0]);
    for(int i = 2; i <= 35; i++){
        if(write(p[1], &i , sizeof(i)) != sizeof(i)){
            fprintf(2, "primes: write fail\n");
            exit(1);
        }
    }
    close(p[1]);
    wait(0);
    exit(0);
}