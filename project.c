#include <stdio.h>
#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h> 
#include <pthread.h>
/*#include <windows.h>*/

int jump, speed = 100000;
int score = 0;
bool game_end = false;

pthread_mutex_t score_mutex;

void title()
{
    mvprintw(1, 30, "Press P to pause, Press spacebar to jump");
    mvprintw(2, 30, "COIN:");
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
    mvprintw(15 - jump, 2, "                 ");
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

void* score_counter(void* arg) 
{
    static int x;

    while (!game_logic) {
        pthread_mutex_lock(&score_mutex);

        if (x > 55 && x < 69 && jump == 10) {
            score++;
        }

        pthread_mutex_unlock(&score_mutex);
        
        usleep(10000);
    }

    return NULL;
}

int game_logic()
{
    static int x;

    /*a coin*/
    mvprintw(3, 74 - x, " ����\n");
    mvprintw(4, 74 - x, "������\n" );
    mvprintw(5, 74 - x, " ����\n");
    x++;

    if (x > 74) {
    	x = 0;
    }
    
    /*get score*/
    if (x > 55 && x < 69 && jump == 10) {
    	x = 0; 
        pthread_mutex_lock(&score_mutex);
    	score++;
        pthread_mutex_unlock(&score_mutex);

    	if (speed > 10000) {
            speed -= 1000;
        }
    }

    mvprintw(2, 53, "%d", score);

    if (x > 69 && jump != 10) {
        game_end = true;
    }

    return speed;
}
 
int main() {
    char press;
    bool paused = false;

    pthread_t score_thread;

    initscr();
    noecho();
    nodelay(stdscr, TRUE);

    pthread_mutex_init(&score_mutex, NULL);

    pthread_create(&score_thread, NULL, score_counter, NULL);

    while (!game_end) {
    	press = getch();

    	if (press == ' ') {
            if (paused) {
                paused = false;
            } else {
                for (int i = 0; i < 5; i++) {
    	     	clear();
    	     	title();
    	     	dino(1);
    	     	game_logic();
    	     	refresh();
    	     	usleep(speed);
                }
                
                for (int j = 0; j < 5; j++) {
    	        clear();
    	        title();
    	     	dino(2);
    	     	game_logic();
    	     	refresh();
    	     	usleep(speed);
                }
            }	     
    	} else if (press == 'p' || press == 'P') {
            paused = !paused;
        } else if (press == 'r' || press == 'R') {
            if (game_end) {
                game_end = false;
                speed = 200000;
                score = 0;
                pthread_create(&score_thread, NULL, score_counter, NULL);
            }
        }

        if (!paused && !game_end) {
            clear();
            title();
            dino(0);
            game_logic();
            pthread_mutex_lock(&score_mutex);
            mvprintw(2, 53, "%d", score);
            pthread_mutex_unlock(&score_mutex);
            refresh();
            usleep(speed);
        } else if (game_end) {
            mvprintw(10, 30, "You collected all the coins! Press R to restart");
            refresh();
        }
    }

    pthread_mutex_destroy(&score_mutex);
    pthread_mutex_join(score_thread, NULL);
}