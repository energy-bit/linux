#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>

void func()
{
    int* lp = NULL;
    *lp = 10;
}

void func1()
{
    int arr[10] = {1,2,3,4,5,6,7,8,9,10};
    printf("arr[9] = %d\n", arr[9]);
    printf("arr[10] = %d\n", arr[10]);
}

void func3()
{
    char* lp = (char*)malloc(10);
    strcpy(lp, "so easy");
    printf("%s\n", lp);
    free(lp);
    lp = NULL;
    free(NULL);
}

int main()
{
    //func3();
    //kill(getpid(), 2);
    abort();
    while(1)
    {
        printf("linux\n");
        sleep(1);
    }
    return 0;
}
