//UDP client program

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <time.h>

using namespace std;

const int MAXBUF = 2048;

int main(){

  int udp_socket;
  int return_status = 0;
  int server_port = 12345;
  struct sockaddr_in udp_server, udp_client;
  char buff[MAXBUF + 1];
  int total_bytes = 0;

  pid_t  client_pid = getpid();

  udp_socket = socket(AF_INET,SOCK_DGRAM,0);

  if(udp_socket == -1){
    cerr << "Could not create the UDP client socket\n";
    exit(1);
  } else {
    cerr << "Socket created\n";
  }
  udp_client.sin_family = AF_INET;
  udp_client.sin_addr.s_addr = INADDR_ANY;
  udp_client.sin_port = 0;

  return_status = bind(udp_socket,(struct sockaddr *)&udp_client, sizeof(udp_client));
  if(return_status == 0){
    cerr << "Bind success\n";
  } else {
    cerr << "Could not bind\n";
    close(udp_socket);
    exit(1);
  }
  char hostbuffer[256];
  int hostname = gethostname(hostbuffer,sizeof(hostbuffer));
  if(hostname != 0){
    cerr << "Could not find hostname\n";
    close(udp_socket);
    exit(1);
  }
  
  struct hostent* host_addr;
  host_addr = gethostbyname(hostbuffer);
  if(host_addr == NULL){
    cerr << "Could not find host addr!\n";
    close(udp_socket);
    exit(1);
  }
  udp_server.sin_family = AF_INET;
  udp_server.sin_addr.s_addr = inet_addr(inet_ntoa(*(struct in_addr*)host_addr->h_addr));
  udp_server.sin_port = htons(server_port);
  strcpy(buff,"Test.txt");
  clock_t start_time = clock();
  return_status = sendto(udp_socket,buff,strlen(buff) + 1,0,(struct sockaddr *)&udp_server,sizeof(udp_server));
  if(return_status == -1){
    cerr << "some error occured while sending message\n";
    close(udp_socket);
    exit(1);
  } else {
    cerr << "starting to receive file\n";
    string filename(buff);
    filename = filename.substr(0,filename.length() - 4);
    filename += "+Protocol=UDP+" + to_string(client_pid) + ".txt";
    char book_save[filename.length() +  1];
    strcpy(book_save, filename.c_str());
    FILE *file_writer = fopen(book_save,"w");
    int server_addr_len = sizeof(struct sockaddr_in);
    total_bytes = 0;
    while(true){
      return_status = recvfrom(udp_socket,buff,MAXBUF,0,(struct sockaddr *)&udp_server,(socklen_t *)&server_addr_len);
      if(return_status == -1 || return_status == 0){
        break;
      } else {
        total_bytes += return_status;
        buff[return_status] = '\0';
        fwrite(buff,1,return_status,file_writer);
      }
    }
    fclose(file_writer);
    close(udp_socket);
    float time_taken = (float) (clock() - start_time)/CLOCKS_PER_SEC;
    printf("Took %.6f sec\n",time_taken);
    printf("Throughput: %.3f Bps\n",(float)total_bytes/(float)time_taken);
  }  
  return 0;
}