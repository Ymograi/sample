#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>

#define DEST_IP "127.0.0.1"
#define DEST_PORT 3491

int main()
{
  int sockfd,size,check,i;
  char name[100],ip[100];
  struct sockaddr_in dest_addr;
  sockfd = socket(AF_INET, SOCK_DGRAM, 0);// creating  a scoket
  if(sockfd == -1)
  {
    printf("Socket Error!!Socket Not Created\n" );//error checking
    return 0;
  }
  dest_addr.sin_family = AF_INET;// Host byte order
  dest_addr.sin_port = htons(DEST_PORT);//short,network byte order
  dest_addr.sin_addr.s_addr = inet_addr(DEST_IP);//we enter the Destination where data will be sent.
  memset(&(dest_addr.sin_zero), '\0', 8);//set zero to rest of constructor
  size = sizeof(dest_addr); //specifying size to be size of structure
 printf("Enter the name of website :");//Enter Name of website
 scanf("%s",name);
 i=0;
 while(name[i]!='\0'){
  name[i]=tolower(name[i]);
  i++;
}
sendto(sockfd,name,sizeof(name),0,(struct sockaddr *)&dest_addr,size);//send to is used to send data where socketfd is the socket name is the name of the website entered.we set flags to zero,length is size of variable name and use destination address with the length of destination address to send the data
recvfrom(sockfd,ip,sizeof(ip),0,NULL,NULL);//use to recieve data through sockfd socket and ip is the buffer where it is stored,flags are set to 0 and source address and address length is set to NULL
printf("%s\n",ip);
  return 0;
}
