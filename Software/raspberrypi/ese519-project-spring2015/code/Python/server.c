#include "stdio.h"
#include "stdlib.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include "string.h"
#include <inttypes.h>

#define BUFSIZE 1024

//#define DEBUG
/*
void get_ip(char ip[]){


    char ac[80];
    gethostname(ac, sizeof(ac));
    struct hostent *hp;
    hp = gethostbyname(ac);
    char* temp = (char*) malloc( 15 * sizeof(char));
    sprintf(temp,"%d.%d.%d.%d\n", hp->h_addr_list[0][0], hp->h_addr_list[0][1], hp->h_addr_list[0][2], hp->h_addr_list[0][3]);
    strcpy(ip,temp);
    free(temp);

//
 FILE* pipe = popen("hostname --ip-address -i", "r");
    if (!pipe) return;
    char buffer[128];
    char result = "";
    while(!feof(pipe)) {
        if(fgets(buffer, 128, pipe) != NULL)
                result += buffer;

    }
    std::string temp = result.substr(0, result.size()-1);
    pclose(pipe);
    strcpy(ip,temp.c_str());

}

*/

int main() {

        char buf[1024];
        struct sockaddr_in cliaddr, servaddr;
        socklen_t len = sizeof(cliaddr);

        int sockfd = socket(AF_INET, SOCK_DGRAM, 0 );
        if( sockfd < 0 )
        {
                perror( "creating socket for listenfd" );
                exit(0);
        }

        bzero( &servaddr, sizeof(servaddr) );
        servaddr.sin_family = AF_INET;
        servaddr.sin_addr.s_addr = htonl( INADDR_ANY );
        servaddr.sin_port = htons(9898);

        if( bind(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0 )
        {
                perror( "bind on socket failed");
                exit(0);
        }
 	
	//if(listen(sockfd,5)<0) {
        //        perror("Listen on socket failed");
        //        exit(1);
        //}

	/*Parameters for readings : 128 samples per second, 
	This server reads each sample (8 channels) at once and appends 128 samples to a file with 4 second windows*/
	printf("waiting for client\n");

    	//int acceptfd = accept(sockfd, (struct sockaddr *)&cliaddr,&len);
      	//if(acceptfd < 0) {
      	//	perror("Accept on socket failed");
        //      	exit(0);
    	//}

	 #ifdef DEBUG
       		printf("Client: %s, Length:%d\n",buf,strlen(buf));
              	char cliip[20];
         	struct sockaddr_in *test = &cliaddr;
          	inet_ntop(AF_INET, &test->sin_addr, cliip, 20);
          	printf("Client IP: %s\n", cliip);
            	printf("Socket: %d\n",sockfd);
     	#endif
	size_t seconds = 0;
	
	FILE *file = fopen("input_eeg.txt","a");
	printf("File created\n");
	while (1) {
		
		if(seconds >= 4) {
			//system("printf '%s' \"$(tail -n +2 input_eeg.txt)\" > input_eeg.txt");
			//FILE *file = fopen("input_eeg.txt","a");
			//printf("\ndeleted one line\n");
			if (seconds == 4)
				fclose(file);
		}
		
		int num_samples = 0;
		char *one_sec = (char *)calloc(4500,sizeof(int));
//		char c = ';';
               	char new_line = '\n';

		//per second 128 samples
		while (num_samples < 128){	

			char sample[1024];
			uint32_t channels[8];
                	int recvlen = recvfrom(sockfd,channels,sizeof(channels),0,(struct sockaddr *)&cliaddr,&len);
                	if(recvlen < 0){
                        	perror("invalid message received");
                        	exit(1);
               		}
			sprintf(sample,"%"SCNd32":%"SCNd32":%"SCNd32":%"SCNd32":%"SCNd32":%"SCNd32":%"SCNd32":%"SCNd32":\0",
				channels[0],channels[1],channels[2],channels[3],channels[4],channels[5],channels[6],channels[7]);
			
			printf("%s\n",sample);
			//get first sample of the second
			if(num_samples == 0)
				strcpy(one_sec,sample);
			else {
				strcat(one_sec,sample);
			}
			/*
			sscanf(strtok(buf,":"),"%"SCNu32,&channels[0]);
			int i;
			for(i = 0; i< 7 ; ++i)
				sscanf(strtok(buf)

			*/
			num_samples++;
			//printf("\nSample: %d\n",num_samples);
		}
		strcat(one_sec,&new_line);
		if(seconds >=4 ) {
			system("printf '%s' \"$(tail -n +2 input_eeg.txt)\" > input_eeg.txt");
			FILE *file1 = fopen("input_eeg.txt","a");
			fputs(one_sec,file1);
			fclose(file1);
		}
		else
			fputs(one_sec,file);
		printf("\nGot 128 samples more!\n");
		seconds ++;
		free(one_sec);
		//if (seconds >= 4){
		//	fclose(file);
		//}
	}

	close(sockfd);
	//close(acceptfd);
	return 0;
}
