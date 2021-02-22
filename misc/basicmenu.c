#include <stdio.h>
#include <stdlib.h>
#include <termios.h>    	//termios, TCSANOW, ECHO, ICANON
#include <unistd.h>     	//STDIN_FILENO
#define WBBF "\x1B[30;47m" 	//white background black font
#define NORM "\x1B[0m"		// default

short running = 1;

int main(int argc, char const *argv[])
{

	int state = 0;
	
	static struct termios oldt, newt;

    /*tcgetattr gets the parameters of the current terminal
    STDIN_FILENO will tell tcgetattr that it should write the settings
    of stdin to oldt*/
    tcgetattr( STDIN_FILENO, &oldt);
    /*now the settings will be copied*/
    newt = oldt;

    /*ICANON normally takes care that one line at a time will be processed
    that means it will return if it sees a "\n" or an EOF or an EOL*/
    newt.c_lflag &= ~(ICANON | ECHO);          

    /*Those new settings will be set to STDIN
    TCSANOW tells tcsetattr to change attributes immediately. */
    tcsetattr( STDIN_FILENO, TCSANOW, &newt);
	while (running)
	{
		system("clear");
		for (int i = 0 ; i < 10 ; i ++){
			if(i == state)
				printf("%s%d. menu item\n%s",WBBF,i,NORM);
			else
				printf("%d. menu item\n",i);
		}
		int chr1, chr2;
 		chr1 = getchar();
 		if (chr1 == '\x1B') //to check scroll key interrupt
 		{
  			chr2 = getchar();  //to read arrow key
  			if(chr2 == '['){
				chr2 = getchar();
				switch(chr2)
				{
				case 'A':
					if(state > 0) state--;
					else state = 9;
					break;
				case 'B':
					if(state < 9) state++;
					else state = 0;
					break;
				};
  			}
 		}
		else if(chr1 == '\xA')
		 	if(state == 9) running = 0;
	}
	
	/*restore the old settings*/
    tcsetattr( STDIN_FILENO, TCSANOW, &oldt);

	return 0;
}
