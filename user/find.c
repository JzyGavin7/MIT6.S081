#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char* fmtname(char *path){

    static char buf[DIRSIZ+1];
    char *p;

    for(p=path+strlen(path); p >= path && *p != '/'; p--);
    p++;

    memmove(buf, p, strlen(p));
    buf[strlen(p)] = 0;
    // printf("%d %s\n", strlen(buf), buf);
    return buf;
}

void find(char *path, char *filename){
    int fd;
    struct stat st;
    struct dirent de;
    char buf[512], *p, *f;

    if((fd = open(path, 0)) < 0){
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    if(fstat(fd, &st) < 0){
        fprintf(2, "find: cannot stat %s\n", path);
        return;
    }

    f = fmtname(path);
    // printf("%d %s\n", strlen(f), f);
    if(strcmp(f, filename) == 0){
        printf("%s\n", path);
    }

    if(st.type == T_DIR){
      if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
            printf("find: path too long\n");
            return;
        }
        strcpy(buf, path);
        p = buf + strlen(buf);
        *p++ = '/';
        while(read(fd, &de, sizeof(de)) == sizeof(de)){
            if(de.inum == 0)
                continue;
            if(strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
                continue;
            memmove(p, de.name, strlen(de.name));
            p[strlen(de.name)] = 0;

            // printf("%s\n", buf);
            find(buf, filename);
        }
    }
    close(fd);
}

int main(int argc, char *argv[]){
    if(argc != 3){
        fprintf(2, "find: argument missing");
        exit(1);
    }

    find(argv[1], argv[2]);
    exit(0);
}