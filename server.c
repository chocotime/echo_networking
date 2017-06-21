#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define BUF_SIZE 1024
void error_handling(char *msg);

int main(int argc, char *argv[]) 
{
	int serv_sock;
	int clnt_sock;
	int str_len, i;
	int answer;
	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;
	socklen_t clnt_addr_size;
	
	char message[BUF_SIZE];
	if(argc!=2) // 이름까지 합해서 2개
	{
		// 이름 포트 이런식으로 쓰라는 말
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}
	
	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if(serv_sock == -1)
		error_handling("socket() error");
	
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(argv[1]));
	
	if(bind(serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1)
		error_handling("bind() error");

	if(listen(serv_sock, 5) == -1)
		error_handling("listen() error");
		clnt_addr_size = sizeof(clnt_addr);
	for(i = 0 ; i < 5 ; i++)
	{
		clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);	
		if(clnt_sock == -1)
			error_handling("accept() error");
		else
			printf("Connected Client %d \n", i+1);	
		while(str_len = read(clnt_sock, message, BUF_SIZE) != 0)
			write(clnt_sock, message, sizeof(message));
		close(clnt_sock);
	}
	close(serv_sock);
	
	return 0;
}

void error_handling(char *msg)
{
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}
