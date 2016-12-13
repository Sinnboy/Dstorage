#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "make_log.h"


#define FILE_ID_LEN (256)

#define LOG_TEST_MUDULE "test"
#define LOG_TEST_PROC "fdfs"

int main(int argc, char *argv[])
{

    if(argc < 2)
    {
        printf("argc err\n");
        exit(1);
    }

    char buf[1024] = {0};

    int pipefd[2];
    int ret;
    ret = pipe(pipefd);
    if(ret == -1)
    {
        perror("pipe");
        exit(1);
    }
    pid_t pid;

    pid = fork();


    if(pid == -1)
    {
        printf("fork function err\n");
        exit(1);
    }

    if(pid == 0)
    {
        close(pipefd[0]);

        dup2(pipefd[1],STDOUT_FILENO);

        execlp("fdfs_upload_file", "fdfs_upload_file", "./conf/client.conf", argv[1],NULL);

        perror("exec error");
    }
    else
    {
        close(pipefd[1]);
        wait(NULL);
        read(pipefd[0],buf,sizeof(buf));
        LOG(LOG_TEST_MUDULE, LOG_TEST_PROC, "file_id=[%s]", file_id);
    }


    return 0;
}
