#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <zconf.h>
#include <stdio.h>

#define MAX 80
#define PORT 8080
#define SA struct sockaddr
#define maxprocesses 10


struct account{
    int accountNumber;
    int accountBalance;
};
//spliting function
int split(char cmd[],char*parsed[],char splitter[]){
    char *ptr = strtok(cmd, splitter);
    int i = 0;
    while (ptr != NULL) {
        parsed[i] = ptr;
        ptr = strtok(NULL, splitter);
        i = i + 1;
    }
    return i;
}
//convert from string to integer
int toString(char a[]) {
    int c, sign, offset, n;

    if (a[0] == '-') {  // Handle negative integers
        sign = -1;
    }

    if (sign == -1) {  // Set starting position to convert
        offset = 1;
    }
    else {
        offset = 0;
    }

    n = 0;

    for (c = offset; a[c] != '\0'; c++) {
        if (a[c]!='\n'){
            n = n * 10 + a[c] - '0';
        }

    }

    if (sign == -1) {
        n = -n;
    }

    return n;
}

// Function designed for chat between client and server.
void func(int sockfd)
{
	char buff[MAX];
	int n;
	// infinite loop for chat
	for (;;) {
		bzero(buff, MAX);

		// read the message from client and copy it in buffer
		read(sockfd, buff, sizeof(buff));
		// print buffer which contains the client contents
		printf("From client: %s\t To client : ", buff);
		bzero(buff, MAX);
		n = 0;
		// copy server message in the buffer
		while ((buff[n++] = getchar()) != '\n')
			;

		// and send that buffer to client
		write(sockfd, buff, sizeof(buff));

		// if msg contains "Exit" then server exit and chat ended.
		if (strncmp("exit", buff, 4) == 0) {
			printf("Server Exit...\n");
			break;
		}
	}
}


// Driver function
int main()
{
	int sockfd, connfd, len;
	struct sockaddr_in servaddr, cli;
	char fileName[50];
    struct account * accounts = (struct process*)malloc(maxprocesses *  sizeof(struct account));
    FILE *fptr;
    char line[12];
    int j=0 ;
    char delim[] = " ";
    char *stri[3] = {'\0'};





    // socket create and verification
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		printf("socket creation failed...\n");
		exit(0);
	}
	else
		printf("Socket successfully created..\n");
	bzero(&servaddr, sizeof(servaddr));

	// assign IP, PORT
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(PORT);

	// Binding newly created socket to given IP and verification
	if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
		printf("socket bind failed...\n");
		exit(0);
	}
	else
		printf("Socket successfully binded..\n");

	// Now server is ready to listen and verification
	if ((listen(sockfd, 5)) != 0) {
		printf("Listen failed...\n");
		exit(0);
	}
	else
		printf("Server listening..\n");
    //taking file from user
    printf("enter server file name : \n");
    scanf("%[^\n]%*c", fileName);
    if ((fptr = fopen(fileName, "r")) == NULL)
    {
        printf("Error! opening file");
        // Program exits if file pointer returns NULL.
        exit(1);
    }
    while (fgets(line, 12, (FILE*)fptr)!=NULL){
        int account_para=split(line,stri,delim);
      //  accounts[j].
        accounts[j].accountNumber= toString(stri[0]);

        accounts[j].accountBalance=toString(stri[1]);
        j++;

    }

    fclose(fptr);

    for (int i =0 ;i<j ; i++){
        printf("%d %d\n",accounts[i].accountNumber, accounts[i].accountBalance);
    }

    len = sizeof(cli);

	// Accept the data packet from client and verification
	connfd = accept(sockfd, (SA*)&cli, &len);
	if (connfd < 0) {
		printf("server acccept failed...\n");
		exit(0);
	}
	else
		printf("server acccept the client...\n");



    // Function for chatting between client and server
	func(connfd);

	// After chatting close the socket
	close(sockfd);
}
