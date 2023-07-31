//UDP server program

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>

using namespace std;

const int MAXBUF = 1024; 

int main(){

  int udp_socket;
  int return_status = 0;
  int server_port = 12345;
  struct sockaddr_in udp_server, udp_client;
  char buff[MAXBUF + 1];

  udp_socket = socket(AF_INET,SOCK_DGRAM,0);
  if(udp_socket == -1){
    cerr << "Could not create the socket\n";
    exit(1);
  } else {
    cerr << "Socket created\n";
  }
  udp_server.sin_family = AF_INET;
  udp_server.sin_addr.s_addr = htonl(INADDR_ANY);
  udp_server.sin_port = htons(server_port);

  return_status = bind(udp_socket, (struct sockaddr *)&udp_server, sizeof(udp_server));
  if(return_status == 0){
    cerr << "Bind complete\n";
  } else {
    cerr << "Could not bind\n";
    close(udp_socket);
    exit(1);
  }
  int client_addr_len = sizeof(struct sockaddr_in);
  while(true){
    return_status = recvfrom(udp_socket,buff,MAXBUF,0,(struct sockaddr *)&udp_client, (socklen_t*)&client_addr_len);
    if(return_status == -1){
      cerr << "Could not receive message\n";
    } else {
      char filename[MAXBUF + 1];
      int bytes_read;
      buff[return_status] = '\0';
      FILE *file_ptr = fopen(buff,"r");
      if(file_ptr == NULL){
        cerr << "File not found!\n";
        close(udp_socket);
        exit(1);
      } else {
        while(true){
          bytes_read = fread(filename,1,MAXBUF,file_ptr);
          if(bytes_read == 0){
            filename[bytes_read] = '\0';
            sendto(udp_socket, filename,strlen(filename),0,(struct sockaddr *)&udp_client, (socklen_t)sizeof(udp_client));  
            sendto(udp_socket, filename,strlen(filename),0,(struct sockaddr *)&udp_client, (socklen_t)sizeof(udp_client));  
            sendto(udp_socket, filename,strlen(filename),0,(struct sockaddr *)&udp_client, (socklen_t)sizeof(udp_client));  
            break;
          }
          filename[bytes_read] = '\0';
          sendto(udp_socket, filename, strlen(filename),0,(struct sockaddr *)&udp_client, (socklen_t)sizeof(udp_client));
        }
        cerr << "File transfer complete\n";
      }
      fclose(file_ptr);
    }
  }
  close(udp_socket);

  return 0;
}