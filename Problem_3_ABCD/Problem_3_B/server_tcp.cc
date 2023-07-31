//TCP server program

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>

using namespace std;

const int MAXBUF = 1024;

int main(int argc, char *argv[]){
  int server_socket = 0;
  int server_port = 12345;
  int return_status = 0;
  struct sockaddr_in server;

  char server_message[MAXBUF + 1];

  server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if(server_socket == -1){
    cerr << "Could not create socket\n";
    exit(1);
  } else {
    cerr << "Socket was created\n";
  }

  bzero(&server, sizeof(server));
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = htonl(INADDR_ANY);
  server.sin_port = htons(server_port);

  return_status = bind(server_socket, (struct sockaddr *)&server, sizeof(server));

  if(return_status == 0){
    cerr << "Binding done\n";
  } else {
    cerr << "Binding not done\n";
    close(server_socket);
    exit(1);
  }
  return_status = listen(server_socket, 4);
  if(return_status == -1){
    cerr << "Cannot listen on socket\n";
    close(server_socket);
    exit(1);
  }
  while(true){
    struct sockaddr_in client_name = {0};
    int client_socket = 0;
    int client_len = sizeof(client_name);

    client_socket = accept(server_socket, (struct sockaddr*)&client_name, (socklen_t *)&client_len);
    if(client_socket == -1){
      cerr << "cannot accept connections\n";
      close(server_socket);
      exit(1);
    } else {
      cerr << "Client socket connected\n";
      char filename[500];
      int bytes_read;
      bytes_read = read(client_socket, filename,sizeof(filename));
      filename[bytes_read] = '\0';
      FILE *file_ptr = fopen(filename,"r");
      while(true){
        bytes_read = fread(server_message,1,MAXBUF,file_ptr);
        if(bytes_read == 0){
          break;
        }
        server_message[bytes_read] = '\0';
        write(client_socket, server_message, strlen(server_message));
      }
      fclose(file_ptr);
      close(client_socket);
      cerr <<"File transfer complete\n";
    }
  }
  close(server_socket);
  return 0;
}