#include <stdio.h>
#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h> 
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#define MAX_MSG 1024

int jump, speed = 100000;
int score = 0;
bool game_end = false;
bool paused = false;
int x;
char press;
char highscore_str[1024];

pthread_mutex_t score_mutex;

void title()
{
    mvprintw(0, 30, "Detail: Press P to pause, Press spacebar to jump.");
    mvprintw(1, 30, "        Press X to exit, Press R to restart.");
    mvprintw(2, 30, "COIN: %d, High Score: %s", score, highscore_str);
}

void dino(int walk)
{
    static int a = 1;

    /*walk = 0 (walk), walk = 1 (jump-up), walk = 2 (jump-down)*/
    if (walk == 0) {
        jump = 0;
    } else if (walk == 1) {
        jump += 2;
    } else {
        jump -= 2;
    }
    
    /*dino body*/
    mvprintw(16 - jump, 2, "         ��������");
    mvprintw(17 - jump, 2, "         � ^ ����");
    mvprintw(18 - jump, 2, "         ��������");
    mvprintw(19 - jump, 2, " �     �������� ");
    mvprintw(20 - jump, 2, " ���  ���������� ");
    mvprintw(21 - jump, 2, " ������������  � ");
    mvprintw(22 - jump, 2, "   ���������     ");
    
    /*dino legs*/
    if (walk == 1 || walk == 2){
    	mvprintw(23 - jump, 2, "    ��� ��       \n");
    	mvprintw(24 - jump, 2, "    ��   ��      \n");
    } else if(a == 1) {
    	mvprintw(23, 2, "    ����  ���    \n");
    	mvprintw(24, 2, "      ��         \n");
    	a = 2;
    } else if(a == 2) {
    	mvprintw(23, 2, "     ��� ��      \n");
    	mvprintw(24, 2, "          ��     \n");
    	a = 1;
    }
    
    /*ground*/
    for(int g = 0; g < 81; g++) {
    	mvprintw(25, g, "�");
    }
    printf("\n");
   
}

int coin()
{
    /*a coin*/
    mvprintw(3, 74 - x, " ����\n");
    mvprintw(4, 74 - x, "������\n" );
    mvprintw(5, 74 - x, " ����\n");
    x++;

    if (x > 74) {
    	x = 0;
    }

    if (x > 69 && jump != 10) {
        game_end = true;
    }

    return speed;
}

/*get score*/
void* score_speed(void* arg) 
{
    while (press != 'x' || press != 'X') {
        if (x > 55 && x < 69 && jump == 10) {
    	    x = 0; 
            pthread_mutex_lock(&score_mutex);
    	    score++;
            pthread_mutex_unlock(&score_mutex);
	
    	    if (speed > 10000) {
            	speed -= 1000;
            }
        }
    }
    return NULL;
}

/*input*/
void* user_input(void* arg) {
    while (press != 'x' || press != 'X') {
        if (!game_end) {
	    if (press == ' ') {
		if (paused) {
		    paused = false;
		}  
	    } /*p to paused game */
		
	    if (press == 'p' || press == 'P') {
			paused = !paused;
		    }
	}
		    
	/*p to restart game */
	if (press == 'r' || press == 'R') {
	    paused = false;
	    game_end = false;
	    speed = 100000;
	    score = 0;
	    x = 0;
	}
    }
}

/*high_score*/
void* high_score(void* arg) {
    int sd;  
    struct sockaddr_in servAddr;
    
    sd = socket(AF_INET, SOCK_STREAM, 0);
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(5000);
    inet_aton("127.0.0.1", &servAddr.sin_addr);
    connect(sd, (struct sockaddr *) &servAddr, sizeof(servAddr));
    
    recv(sd, highscore_str, MAX_MSG, 0);
    while (press != 'x' || press != 'X') {
    	if (score > atoi(highscore_str)) {
      	    sprintf(highscore_str, "%d",score);
    	}
    }
    send(sd, highscore_str, sizeof(highscore_str), 0);

}



/*void draw()*/
 
int main() {
    pthread_t score_thread, input_thread, highscore_thread, coin_thread;

    initscr();
    noecho();
    nodelay(stdscr, TRUE);

    pthread_mutex_init(&score_mutex, NULL);

    pthread_create(&score_thread, NULL, score_speed, NULL);
    pthread_create(&input_thread, NULL, user_input, NULL);
    pthread_create(&highscore_thread, NULL, high_score, NULL);

    while (true) {
        press = getch();
    
        if (!game_end && !paused) {
	/*space bar to unpaused game or jump */
	     if (press == ' ') {
		     for (int i = 0; i < 5; i++) {
		   	clear();
		    	title();
		    	dino(1);
		    	coin();
		    	refresh();
		    	usleep(speed);
		     }
		        
		     for (int j = 0; j < 5; j++) {
		    	clear();
		    	title();
		    	dino(2);
		    	coin();
		    	refresh();
		    	usleep(speed);
		     }
		}

	    /*nomal walk */
	    else {
		clear();
		title();
		dino(0);
		coin();
		refresh();
		usleep(speed);
	    }
        } else if (game_end) {
	     clear();
	     mvprintw(10, 30, "The End! Press R to restart");
	     mvprintw(11, 37, "Press X to exit");
	     refresh();
	     usleep(speed);
	}
        
	if (press == 'x' || press == 'X') {
		return 0;
	}	
        
    }
    pthread_mutex_destroy(&score_mutex);
    pthread_join(score_thread, NULL);
    pthread_join(input_thread, NULL);
    pthread_join(highscore_thread, NULL);
    /*pthread_join(draw_thread, NULL);*/
}
