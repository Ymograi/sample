#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
//Hello This was Added
#define MYPORT 3491
#define DEST_IP "127.0.0.2"
#define DEST_PORT 3498
#define TLD_PORT 3497

int main(){

  int sockfd, new_fd ,port;
  char msg[100],name[100],buf[100],buf2[100];
  int i,j,temp,len;
  FILE *fp,*fp2;
  struct sockaddr_in my_addr;
  struct sockaddr_in client,dest_addr;
  int sin_size,size;
  sockfd = socket(AF_INET, SOCK_DGRAM, 0);//using udp sock_datagram and ip
  new_fd = socket(AF_INET, SOCK_DGRAM, 0);//using udp i.e sock_datagram and ip
  if(sockfd == -1)
  {
    printf("Socket Error!!\n" );
    return 0;
  }
  else
   printf("Sucess :%d\n",sockfd );
   my_addr.sin_family = AF_INET;//ip
   my_addr.sin_port = htons(MYPORT);//port no is converted into host to network short
   my_addr.sin_addr.s_addr = INADDR_ANY;//any address
   memset(&(my_addr.sin_zero), '\0', 8);
   bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr));//binding i,e assigning a transport address to the socket(a port no in ip networking)
sin_size = sizeof(client);
fp=fopen("cache.txt","w");//the first cache that we look into .//opening a file
fclose(fp);
fp2=fopen("mcache.txt","w");//opening the second cache
fclose(fp2);


while (1) {

  dest_addr.sin_family = AF_INET;
  dest_addr.sin_port = htons(DEST_PORT);
  dest_addr.sin_addr.s_addr = inet_addr(DEST_IP);
  memset(&(dest_addr.sin_zero), '\0', 8);
  //bind(new_fd, (struct sockaddr *)&dest_addr, sizeof(struct sockaddr));
  size = sizeof(dest_addr);
recvfrom(sockfd,name,sizeof name, 0,(struct sockaddr *)&client,&sin_size);//recieve data from client
//printf("recieved %s",name);
/*   while(1)*/

      strcpy(msg,"");//copying bank space to message
      fp=fopen("cache.txt","r");//opening cache for reading
    //  recvfrom(sockfd,name,sizeof name, 0,(struct sockaddr *)&client,&sin_size);
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
    if(strcmp(msg,"")==0)//if message is equal to blank string then it was a cache miss the address was not found
   {   printf("Cache Miss..\n");
             i=strlen(name);
             j=0;
             i--;                                 //removing www.xxx from www.xxx.xxx
             while(name[i] != '.')
             {
               j++;
               i--;
             }
             j=0;
             i++;
             while(name[i] != '\0')
            {

              buf[j]=name[i];
              j++;
              i++;
            }
             buf[j]='\0';
             printf("finding.. %s\n",buf);
     fp2=fopen("mcache.txt","r");
      while(!feof(fp2))
     {
         fscanf(fp2,"%s",buf2);
         if(strcmp(buf2,buf)==0)
        {
              fscanf(fp2,"%s",msg);
              break;
        }
    }
    fclose(fp2);
	if(strcmp(msg,"")!=0) // if message and blankspace dont match -> its a cache hit
	{
    	 printf("TLD cache hit..\n"); 
	
	}
	if(strcmp(msg,"")==0) //if msg and blank space match then ->its a cache miss as it was not found in 
          
	{  printf("TLD cache miss..\n");  
	     sendto(new_fd,buf,sizeof(buf),0,(struct sockaddr *)&dest_addr,size);//send data to root dns where tldns address is acquired
             recvfrom(new_fd,msg,sizeof(msg),0,NULL,NULL);//tldns address is acquired
		fp=fopen("mcache.txt","a");
                        fprintf(fp, "%s ",buf );//add data to mcache
                        fprintf(fp, "%s\n",msg );//add data to mcache
                        fclose(fp);
	}
             //printf("%s",msg);
             if(strcmp(msg,"")==0)//if not found
            {
                  strcpy(msg,"Not found...");
             sendto(new_fd,msg,sizeof msg,0,(struct sockaddr *)&client,sin_size);//send to client tldns server not found -> website address not found 
             //return 0;
          }
	
          else //if tldns server address was found then send message to 
              {

              printf("recieved tld %s\n",msg);
              dest_addr.sin_family = AF_INET;//Host Byte Order
              dest_addr.sin_port = htons(TLD_PORT);
              dest_addr.sin_addr.s_addr = inet_addr(msg);//setting address to the message recieved from root dns i.e address of tldns
              memset(&(dest_addr.sin_zero), '\0', 8);//setting rest of the agrument to zero
              size = sizeof(dest_addr);
              sendto(new_fd,name,sizeof(name),0,(struct sockaddr *)&dest_addr,size);//send website name to tldns
              recvfrom(new_fd,msg,sizeof(msg),0,NULL,NULL);//reciever from tldns
              //sendto(new_fd,msg,sizeof msg,0,(struct sockaddr *)&client,sin_size);
                if(strcmp(msg,"")==0)
                  {
                        strcpy(msg,"Not found...");//if authoritative server address is not found reply client with not found message
                        sendto(new_fd,msg,sizeof msg,0,(struct sockaddr *)&client,sin_size);//send not found message to client

                  }
                else{
                        printf("recieved NS %s\n",msg);//if some message(authoritative dns server i.e google or yahoo) is recieved that is not blank space then we use the address and send name to that address.
                        dest_addr.sin_family = AF_INET;
                        dest_addr.sin_port = htons(TLD_PORT);
                        dest_addr.sin_addr.s_addr = inet_addr(msg);//setting destination address to authoritative server's address
                        memset(&(dest_addr.sin_zero), '\0', 8);//setting alll to zero
                        size = sizeof(dest_addr);
                        sendto(new_fd,name,sizeof(name),0,(struct sockaddr *)&dest_addr,size);//sendin data to authoritative server
                        recvfrom(new_fd,msg,sizeof(msg),0,NULL,NULL);//recieving from authoritative server
                        sendto(new_fd,msg,sizeof(msg),0,(struct sockaddr *)&client,sin_size);//sending the reply to client 

                        fp=fopen("cache.txt","a");//adding the address to client 
                        fprintf(fp, "%s ",name );
                        fprintf(fp, "%s\n",msg );
                        fclose(fp);

                    }
              }
	    
          }
else
 { printf("Cache Hit..\n");//if there is a Cache hit
  sendto(new_fd,msg,sizeof msg,0,(struct sockaddr *)&client,sin_size);//send the found address to client
 }
}
  return 0;
}
