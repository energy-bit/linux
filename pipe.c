#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main()
{
	//1.创建匿名管道
	int fd[2];
	int ret = pipe(fd);
	if (ret < 0)
	{
		perror("pipe");
		return 0;
	}
	printf("fd[0]:%d\n", fd[0]);
	printf("fd[1]:%d\n", fd[1]);

	//2.创建子进程
	ret = fork();
	if (ret < 0)
	{
		perror("fork");
		return 0;
	}
	//程序跑起来，验证子进程当中是否有fd[0], fd[1]
	else if (ret == 0)
	{
		close(fd[0]);
		//child
		write(fd[1], "linux so easy!!", 15);

		sleep(10);

		write(fd[1], "linux so easy!!!!!!!", 20);
	}
	else
	{
		close(fd[1]);
		//father
		sleep(5); //保证子进程一定将数据写到匿名管道当中了，其实5s都是太大的一个数字了，CPU在运算的时候，是比微妙还低的一个数量级

		char buf[1024] = { 0 };

		read(fd[0], buf, sizeof(buf)-1);
		printf("i am father: buf is: %s\n", buf);

		memset(buf, '\0', sizeof(buf));
		read(fd[0], buf, sizeof(buf)-1);
		printf("i am father2222: buf is: %s\n", buf);
	}

	//当父子进程执行完毕上面的逻辑后，从if语句当中跳出来之后，都会执行下面的代码
	while (1)
	{
		sleep(1);
	}
	return 0;
}