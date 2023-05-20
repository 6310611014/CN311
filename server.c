#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>

#define MAX_MSG 1024

int main(){
    int sd = 0;
    int client_sd = 0;
    int high_score = 0;
    char rcv_score[MAX_MSG];
    char send_score[MAX_MSG];
    struct sockaddr_in servAddr;
    
    sd = socket(AF_INET, SOCK_STREAM, 0);

    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(5000);

    bind(sd, (struct sockaddr*)&servAddr, sizeof(servAddr));

    listen(sd, 5);
    
    while(1) {
    	client_sd = accept(sd, (struct sockaddr *)NULL, NULL);
    	sprintf(send_score, "%d", high_score);
    	send(client_sd, send_score,  sizeof(send_score), 0);
    	recv(client_sd, rcv_score, MAX_MSG, 0);
    	high_score = atoi(rcv_score);
    	printf("%d", high_score);
    	    
    	close(client_sd);
    }
    close(sd);
    return 0;
    
}
