#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>

#define MAX_MSG 1024

/* ถ้าเรา run โปรแกรมฝั่ง server, client จะส่งคะแนนสูงสุดที่ผู้เล่นทำได้ไว้ในฝั่ง server 
   แล้วฝั่ง server ก็จะเก็บคะแนนที่ถูกส่งมาจากฝั่ง client ไว้ 
   สมมติว่ามีผู้เล่นหลายคนก็จะทำให้ผู้เล่นคนอื่นๆ สามารถรู้คะแนนสูงสุดที่ได้ในปัจจุบัน 
   ถึงแม้ว่าผู้เล่นจะเริ่มเกมใหม่ หรือ ออกจากเกม */

/* ถ้าเราไม่ได้ run โปรแกรมฝั่ง server เมื่อผู้เล่นเริ่มเกมใหม่ หรือ ออกจากเกม 
   คะแนนสูงสุดที่ผู้เล่นทำได้จะถูก reset ให้กลายเป็น 0 ทุกครั้งที่เริ่มเล่นเกมใหม่
   เนื่องจากคะแนนไม่ได้ถูกเก็บไว้ ทำให้ผู้เล่นไม่สามารถรู้ได้เลยว่าคะแนนสูงสุดที่ทำได้คือกี่คะแนน */

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
    	    
    	close(client_sd);
    }
    close(sd);
    return 0;
    
}
