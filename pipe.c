#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main()
{
	//1.���������ܵ�
	int fd[2];
	int ret = pipe(fd);
	if (ret < 0)
	{
		perror("pipe");
		return 0;
	}
	printf("fd[0]:%d\n", fd[0]);
	printf("fd[1]:%d\n", fd[1]);

	//2.�����ӽ���
	ret = fork();
	if (ret < 0)
	{
		perror("fork");
		return 0;
	}
	//��������������֤�ӽ��̵����Ƿ���fd[0], fd[1]
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
		sleep(5); //��֤�ӽ���һ��������д�������ܵ������ˣ���ʵ5s����̫���һ�������ˣ�CPU�������ʱ���Ǳ�΢��͵�һ��������

		char buf[1024] = { 0 };

		read(fd[0], buf, sizeof(buf)-1);
		printf("i am father: buf is: %s\n", buf);

		memset(buf, '\0', sizeof(buf));
		read(fd[0], buf, sizeof(buf)-1);
		printf("i am father2222: buf is: %s\n", buf);
	}

	//�����ӽ���ִ�����������߼��󣬴�if��䵱��������֮�󣬶���ִ������Ĵ���
	while (1)
	{
		sleep(1);
	}
	return 0;
}