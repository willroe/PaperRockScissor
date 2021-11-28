#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h> 

int numCheck(const char* str, int argc); //Function to check input

int main(int argc, char*argv[])
{
	    //variables 
		int n, status,i;
		char id[4];

		if(numCheck(argv[1], argc) == 0)
		{
			exit(1);
		}
		
		if(fork()==0)
		{
			execl("referee", "referee", argv[1], (char*)NULL);		/* Relative path name */
		}
		else
		{
			for(i=1;i<=2;i++)
			{
	 		  if(fork()==0)
			  {
				  execl("player", "player" , 0, (char*)NULL);	/* Relative path name*/			
			  }
			}
		}
		for (n = 1; n <= 3; n++){	/* Wait for the children to exit */
			wait(&status);
		}
			return 0;
}

int numCheck(const char* str, int argc)
{

	if (argc != 2) //Makes sure there is two arguments which are ./play and a number
	{
		printf("Program requires one int argument. Usage: play <turns> \n");
		return 0;
	}

	if (strlen(str) > 5) // Keeps the number of games from being over 100,000
	{
		printf("Please use a number less then 100,000.\n");
		return 0;
	}

    while (*str != '\0') // Makes sure that it's a number 
	{
		if (*str < '0' || *str > '9')
		{
			printf("Program requires one int argument. Usage: play <turns> \n");
			return 0;
		}
			
		str++;
	}

	return 1; //exits program no errors.
}