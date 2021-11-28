#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h> 
#include <sys/socket.h>
#include <sys/un.h>
#include <netdb.h>
#include <time.h>
#include <netinet/in.h>

#define SERVER_PATH     "./MySocket"

int main(int argc, char* argv[])
{
    char server_response[25];
    int  socket_fd, result, option, count=0;
    char p[25] = "PAPER";
    char s[25] = "SCISSORS";
    char r[25] = "ROCK";
    char ready[25] = "READY";
    char goStop[25];

    struct sockaddr client;
    socket_fd = socket(AF_UNIX, SOCK_STREAM, 0);	// Create an unnamed socket 
    strcpy(client.sa_data, SERVER_PATH);	        // Assign structure describing server socket
    client.sa_family = AF_UNIX;
   

    do	// Busy wait: loop until successful connection made 
    {
        result = connect(socket_fd, &client, sizeof(client));	// Attempt to connect the unnamed socket to the server 
        
    } while (result == -1);

    (send(socket_fd, ready, sizeof(ready), 0));

    //*********************************************************************************
    
    //While loop to play game until the referee sends "STOP" signal 
    int flag = 1;
    while (flag != 0)
    {
        read(socket_fd, goStop, 1024);
        if (strcmp(goStop, "GO") == 0)
        {
            srand(time(0) + getpid() + count); //Randomizing see with time + pid + a counter
            option = rand() % 3; // Here is where the PSR is choosen randomly 
            count++;

            //switch statement to send over actual string. 
            switch (option)
            {
            case 0:
                (send(socket_fd, p, sizeof(p), 0));
                break;
            case 1:
                (send(socket_fd, s, sizeof(s), 0));
                break;
            case 2:
                (send(socket_fd, r, sizeof(r), 0));
                break;
            default:
                printf("Player selected invalid move. "); //Player should never see this. 
            }
        }
        else // received STOP from referee and ending while loop. 
        {
            if (strcmp(goStop, "STOP") == 0)
            flag = 0;
        }
    }


    close(socket_fd); // Close server end of client's socket 


        exit(1);
    
}

