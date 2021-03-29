#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

int main()
{
	int fd[2];
	int ret = pipe(fd);
	if (ret < 0)
	{
		perror("pipe");
		return -1;
	}
	printf("fd[0]:%d, fd[1]:%d\n", fd[0], fd[1]);

	int flag = fcntl(fd[0], F_GETFL);
	fcntl(fd[0], F_SETFL, flag | O_NONBLOCK);

#if 0
	//写端文件描述符被设置成为非阻塞属性
	int flag = fcntl(fd[1], F_GETFL);
	printf("flag-fd[1] : %d\n", flag);

	fcntl(fd[1], F_SETFL, flag | O_NONBLOCK);

	flag = fcntl(fd[1], F_GETFL);
	printf("flag-fd[1] : %d\n", flag);
#endif

	int pid = fork();
	if (pid < 0)
	{
		perror("fork");
		return -1;
	}
	else if (pid == 0)
	{
		//child
		//close(fd[1]);

		//while(1)
		{
			char buf[1024] = { 0 };
			ssize_t ret = read(fd[0], buf, sizeof(buf)-1);
			printf("ret = %d\n", ret);
			if (ret < 0)
			{
				perror("read");
				// break;
			}
			sleep(1);
		}


		//child
#if 0
		close(fd[0]);
		int count = 0;
		while (1)
		{
			ssize_t w_size = write(fd[1], "l", 1);
			printf("w_size : %d\n", w_size);
			if (w_size <= 0)
			{
				perror("write");
				break;
			}
			count++;
			printf("write : %d\n", count);
		}
#endif
	}
	else
	{
		//close(fd[1]);
		while (1)
		{
			printf("i am father\n");
			sleep(1);
		}
#if 0
		//father
		//close(fd[0]);
		//char buf[1024] = {0};
		//read(fd[0], buf, sizeof(buf) - 1);
		//printf("i am father: buf is %s\n", buf);
		while (1)
		{
			printf("i am father\n");
			sleep(1);
		}
#endif
	}
#if 0
	write(fd[1], "linux-57", 8);

	char buf[1024] = { 0 };
	read(fd[0], buf, sizeof(buf)-1);

	printf("buf is %s\n", buf);

	memset(buf, '\0', sizeof(buf));
	read(fd[0], buf, sizeof(buf)-1);

	printf("buf22222 is %s\n", buf);
#endif
	while (1)
	{
		sleep(1);
	}
	return 0;
}