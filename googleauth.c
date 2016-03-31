#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>

#define MYPORT 3497
#define MY_IP "127.0.0.5"
#define BACKLOG 10

int main(){

  int sockfd, new_fd;
  char msg[100],name[100],buf[100];
  int i,j,temp,len;
  FILE *fp;
  struct sockaddr_in my_addr;
  struct sockaddr_in client;
  int sin_size;
  sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if(sockfd == -1)
  {
    printf("Socket Error!!Socket not Created\n" );
    return 0;
  }
  else
   printf("Success :%d\n",sockfd );
   my_addr.sin_family = AF_INET;
   my_addr.sin_port = htons(MYPORT);
   my_addr.sin_addr.s_addr = inet_addr(MY_IP);
   memset(&(my_addr.sin_zero), '\0', 8);
   bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr));
sin_size = sizeof(client);
 while(1)
  {
      strcpy(msg,"");
      recvfrom(sockfd,name,sizeof name, 0,(struct sockaddr *)&client,&sin_size);
      printf("Recieved %s\n",name);
        fp=fopen("googleauthor.txt","r");
      while(!feof(fp))
     {
         fscanf(fp,"%s",buf);
         if(strcmp(buf,name)==0)
        {
              fscanf(fp,"%s",msg);
              break;
        }
    }

   fclose(fp);
   sendto(sockfd,msg,sizeof msg,0,(struct sockaddr *)&client,sin_size);
 }

  return 0;
}
