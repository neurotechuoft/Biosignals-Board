#include "stdio.h"
#include "stdlib.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <inttypes.h>
#include <unistd.h>

#include <wiringPi.h>

#define BUFSIZE 1024

using namespace std;

int main(int argc, char *argv[])
{

        if(argc != 3){
                fprintf(stderr,"Usage: %s <server_ip> <server_port>\n", argv[0]);
                exit(0);
        }
        struct sockaddr_in servaddr;
        int sockfd;
        char buf[BUFSIZE];
        char mesg[BUFSIZE];
        uint16_t port;

        if( (sockfd = socket(AF_INET, SOCK_DGRAM, 0 )) < 0 ){
                perror("invalid socket");
                exit(0);
        }
        sscanf(argv[2],"%d",&port);

        bzero( &servaddr, sizeof(servaddr ));
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(port);

	// convert address, e.g., 127.0.0.1, to the right format 
        if( inet_pton( AF_INET, argv[1], &servaddr.sin_addr ) <= 0 ){
                perror( "inet_pton for address" );
                exit(0);
        }

        if( connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0 ){
                perror( "connect to associative memory at server" );
                exit(0);
        }

        ifstream infile;

        infile.open("eeg.txt");
	int j = 0;
        while (!infile.eof()) {
                string line;
                getline(infile,line);
		
		if(line.length()==0)
			break;

                char *_line = strndup(line.c_str(),line.length());

                uint32_t channels[8];
                channels[0] = atoi(strtok(_line,","));
                for(int i = 1; i < 8; ++i){
                        channels[i] = atoi(strtok(NULL,","));
                }
		//if(j < 3)
                //printf("%lu:%lu:%lu:%lu:%lu:%lu:%lu:%lu\n",channels[0],channels[1],channels[2],channels[3],channels[4],channels[5],channels[6],channels[7]);
                //sprintf(mesg,"%d:%d:%d:%d:%d:%d:%d:%d",i+1,i+2,i+3,i+4,i+5,i+6,i+7,i+8);
                send(sockfd,channels,sizeof(channels),0);
        	//j++;
		//printf("%d\n",j);
		free(_line);
		
		//delay 8ms
		delay(8);
	}
	printf("File read completed\n");
        infile.close();
        close(sockfd);
        exit(0);
}


