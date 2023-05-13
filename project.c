#include <stdio.h>

#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
/*#include <windows.h>*/

int jump, speed = 100000;

void title()
{
    mvprintw(1, 40,"Press X to Exit, Press Space to Jump");
    mvprintw(2, 40,"COIN SCORE : ");
}

void dino(int walk)
{
    static int a=1;
    /*walk = 0 (walk), walk = 1 (jump-up), walk = 2 (jump-down)*/
    if(walk==0) {
        jump=0;
    } else if(walk==1) {
        jump+=2;
    } else jump-=2;
    
    /*dino body*/
    mvprintw(15-jump,2,"                 ");
    mvprintw(16-jump,2,"         ��������");
    mvprintw(17-jump,2,"         � ^ ����");
    mvprintw(18-jump,2,"         ��������");
    mvprintw(19-jump,2," �     �������� ");
    mvprintw(20-jump,2," ���  ���������� ");
    mvprintw(21-jump,2," ������������  � ");
    mvprintw(22-jump,2,"   ���������     ");
    
    /*dino legs*/
    if(walk==1 || walk==2){
    	mvprintw(23-jump,2,"    ��� ��       \n");
    	mvprintw(24-jump,2,"    ��   ��      \n");
    } else if(a==1) {
    	mvprintw(23,2,"    ����  ���    \n");
    	mvprintw(24,2,"      ��         \n");
    	a=2;
    } else if(a==2) {
    	mvprintw(23,2,"     ��� ��      \n");
    	mvprintw(24,2,"          ��     \n");
    	a=1;
    }
    
    /*ground*/
    for(int g=0;g<81;g++) {
    	mvprintw(25,g,"�");
    }
    printf("\n");
   
}
int game_logic(int speed)
{
    static int x, score=0;
    /*a coin*/
    mvprintw(3,74-x," ����\n");
    mvprintw(4,74-x,"������\n" );
    mvprintw(5,74-x," ����\n");
    x++;

    if(x>74) {
    	x=0;
    }
    
    /*get score*/
    if(x > 55 && x < 69 && jump == 10) {
    	x=0; 
    	score++;
    	if(speed>10000)
        	speed-=1000;
    }
    mvprintw(2, 53,"%d",score);
}
#include <unistd.h> 
 
int main(){
    char press;
    initscr();
    noecho();
    nodelay(stdscr, TRUE);

    while(true) {
    	press = getch();
    	if (press==' '){
    	     for(int i=0;i<5;i++){
    	     	clear();
    	     	title();
    	     	dino(1);
    	     	game_logic(speed);
    	     	refresh();
    	     	usleep(speed);
    	     }
    	     for(int j=0;j<5;j++){
    	        clear();
    	        title();
    	     	dino(2);
    	     	game_logic(speed);
    	     	refresh();
    	     	usleep(speed);
    	     }	     
    	} else if (press=='x') {
    	     return 0;
    	} else {
    	        clear();
    	        title();
    	     	dino(0);
    	     	game_logic(speed);
    	     	refresh();
    	     	usleep(speed);
    	     
    	}

    }
   

}


