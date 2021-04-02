#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void sigcallback(int signo)
{
    printf("signo : %d\n", signo);
}

int main()
{
    //act --》 入参
    struct sigaction act;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    act.sa_handler = sigcallback;

    //oldact 出参
    struct sigaction oldact;
    sigaction(3, &act, &oldact);

    getchar();

    sigaction(3, &oldact, NULL);

    while(1)
    {
        printf("linux\n");
        sleep(1);
    }
    return 0;
}
