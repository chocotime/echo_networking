#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define BUF_SIZE 1024
void error_handling(char *msg);

int main(int argc, char* argv[]) 
{
	int sock;
	struct sockaddr_in serv_addr;
	char message[BUF_SIZE];
	int str_len, cur_len;
	int tmp;
	
	if(argc!=3) 
	{
		printf("Usage %s <IP> <port>\n", argv[0]);
		exit(1);
	}
	
	sock = socket(PF_INET, SOCK_STREAM, 0);
	if(sock == -1) 
		error_handling("socket() error");
	
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port = htons(atoi(argv[2]));
	
	if(connect(sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1)
		error_handling("connect() error");
	else 
		printf(" 연결이 되었다!~ \n");
	while(1)
	{	
		fputs("메시지를 입력해주세요 ( 종료키 : q ) : ", stdout);
		fgets(message, BUF_SIZE, stdin);
		
		if(!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
			break;
		str_len = write(sock, message, sizeof(message));
		cur_len = 0;
		while(cur_len < str_len) 
		{
			// Start at cur point
			tmp = read(sock, &message[cur_len], BUF_SIZE-1);
			if(tmp == -1)
				error_handling("read() error");
			cur_len += tmp;
		}
		message[cur_len]=0;
		printf("Message from server : %s", message);
	}
	close(sock);	
	return 0;
}

void error_handling(char *msg) 
{
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}
