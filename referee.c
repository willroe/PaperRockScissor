#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h> 
#include <sys/socket.h>
#include <sys/un.h>
#include <fcntl.h>

#define SERVER_PATH     "./MySocket"

int main(int argc, char* argv[])
{
    //variables 
    char client_response[25];
    char p1PSR[25];
    char p2PSR[25];
    char ready1[25];
    char ready2[25];

    int server_fd, 	// Original server file descriptor 
        client_fd1,	// Client's file descriptor 
        client_fd2,
        server_len;
    int numP1;
    int numP2;
    int p1Score = 0;
    int p2Score = 0;
    int option;
    int gamecount = atoi(argv[1]);

    
    struct sockaddr	server;		// defined in socket.h
    server_fd = socket(AF_UNIX, SOCK_STREAM, 0);	// Create unnamed socket 
    server.sa_family = AF_UNIX; 	//Prepare socket Id: type and name
    strcpy(server.sa_data, SERVER_PATH); // Call it MySocket
    server_len = sizeof(server);

    // Bind the socket to the name 
    if ((bind(server_fd, &server, server_len)) < 0)
    {
        perror("bind error referee");
        exit(-1);
    }

    // Listen to the socket. Allow a maximum of four pending connections 
    if ((listen(server_fd, 4)) < 0)	
    {
        perror("listen error referee");
        exit(-1);
    }

    // Accept a client connection  wait until one is received 
    if ((client_fd1 = accept(server_fd, &server, &server_len)) < 0)	
    {
        perror("accept1 error referee");
        exit(-1);
    }

    // Accept a client connection  wait until one is received 
    if ((client_fd2 = accept(server_fd, &server, &server_len)) < 0)	
    {
        perror("accept2 error referee");
        exit(-1);
    }


    //Waits for "READY" from both players. 
    int flag1 = 1;
    int flag2 = 1;
    while (flag1 != 0 && flag2 != 0)
    {
        read(client_fd1, ready1, 1024);
        if (strcmp(ready1, "READY") == 0)
            flag1 = 0;
        read(client_fd2, ready2, 1024);
        if (strcmp(ready2, "READY") == 0)
            flag2 = 0;
    }

    printf("Written by: Will Roe\n\n");
    printf("Paper, Scissors, Rock: %s iterations\n\n", argv[1]);

    //For loop that go through the number of games the user requested. 
    for (int i = 1; i <= gamecount; i++)
    {
        printf("Go Players [%d] \n", (i));
        if ((send(client_fd1, "GO", sizeof("GO"), 0)) < 0)
        {
            perror("send error player");
            exit(-1);
        }

        if ((send(client_fd2, "GO", sizeof("GO"), 0)) < 0)
        {
            perror("send error player");
            exit(-1);
        }
        

        read(client_fd1, p1PSR, 1024); //reads from one of the players
        read(client_fd2, p2PSR, 1024); //reads from one of the players


        printf("\t Player 1: %s \n", p1PSR);
        printf("\t Player 2: %s \n", p2PSR);

       // converts PSR into an int to process later
        if (strcmp(p1PSR, "PAPER") == 0)
        {
            numP1 = 0;
        }
        else if (strcmp(p1PSR, "SCISSORS") == 0)
        {
            numP1 = -1;
        }
        else if (strcmp(p1PSR, "ROCK") == 0)
        {
            numP1 = -2;
        }

        if (strcmp(p2PSR, "PAPER") == 0)
        {
            numP2 = 0;
        }
        else if (strcmp(p2PSR, "SCISSORS") == 0)
        {
            numP2 = 1;
        }
        else if (strcmp(p2PSR, "ROCK") == 0)
        {
            numP2 = 2;
        }
      
        //Preparing for switch statement
        option = (numP1 + numP2);
        if (option == -1 || option == 2)
        {
            option = 0;
        }
        else if (option == 1 || option == -2)
        {
            option = 1;
        }
        else
        {
            option = 2;
        }

        //Switch to determin if player 1 wins, player 2 wins or it's a draw
        switch (option)
        {
        case 0:
            printf("\t Player 1 Wins\n");
            p1Score++;
            break;
        case 1:
            printf("\t Player 2 Wins\n");
            p2Score++;
            break;
        case 2:
            printf("\t Players Draw\n");
            break;
        default:
            printf("\t Error\n");
        }


    }

    //Final score displayed
    printf("Final Score: \n");
    printf("\t Player 1: %d\n", p1Score);
    printf("\t Player 2: %d\n", p2Score);

    if(p1Score > p2Score)
        printf("Player 1 Wins \n");
    else if (p1Score < p2Score)
        printf("Player 2 Wins \n");
    else
        printf("Player Draw \n");


    //Sends STOP to both players
    if ((send(client_fd1, "STOP", sizeof("STOP"), 0)) < 0)
    {
        perror("send error player");
        exit(-1);
    }

    if ((send(client_fd2, "STOP", sizeof("STOP"), 0)) < 0)
    {
        perror("send error player");
        exit(-1);
    }

			
    close(server_fd);	// Close server end of client's socket 
    close(client_fd1);  // Close player 1 socket 
    close(client_fd2);  // Close Player 2 socket 
    unlink(SERVER_PATH);// removes MySocket from files

	exit(1); 
}
