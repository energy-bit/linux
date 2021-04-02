#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void sigcallback(int signo)
{
    printf("signo : %d\n", signo);
}

int main()
{
    signal(2, sigcallback);
    signal(40, sigcallback);

    sigset_t set;
    sigfillset(&set);

    sigset_t oldset;
    sigprocmask(SIG_SETMASK, &set, &oldset);

    getchar();

    sigprocmask(SIG_SETMASK, &oldset, NULL);

    while(1)
    {
        printf("linux so easy\n");
        sleep(1);
    }
    return 0;
}
