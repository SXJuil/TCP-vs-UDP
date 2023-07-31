//TCP client program

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

int main(int argc, char *argv[]){
  
  int client_socket = 0;
  int server_port = 12345;
  int return_status = 0;
  char buff[MAXBUF + 1];
  int bytes_received;
  struct sockaddr_in server;
  int total_bytes = 0;

  pid_t  client_pid = getpid();

  client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if(client_socket == -1){
    cerr << "Client not create socket\n";
    exit(1);
  } else {
    cerr << "Client socket created\n";
  }
  bzero(&server,sizeof(server));
  server.sin_family = AF_INET;
  server.sin_port = htons(server_port);

  return_status = connect(client_socket,(struct sockaddr*)&server,sizeof(server));
  clock_t start_time = clock();
  if(!return_status){
    cerr << "Connection successful\n";
  } else {
    cerr << "Could not connect to the address\n";
    close(client_socket);
    exit(1);
  }
  char book_name[] = "Test.txt";
  write(client_socket,book_name,sizeof(book_name));
  string filename(book_name);
  filename = filename.substr(0,filename.length()-4);
  filename += "+Protocol=TCP+" + to_string(client_pid) + ".txt";
  char book_save[filename.length() +  1];
  strcpy(book_save, filename.c_str());
  FILE *file_writer = fopen(book_save,"w");
  total_bytes = 0;
  while(true){
    bytes_received = read(client_socket, buff, sizeof(buff));
    if(bytes_received == 0)
      break;
    total_bytes += bytes_received;
    buff[bytes_received] = '\0';
    fwrite(buff,1,bytes_received,file_writer);
  }
  close(client_socket);
  float time_taken = (float)(clock() - start_time)/CLOCKS_PER_SEC;
  fclose(file_writer);
  printf("Took %.6f sec\n",time_taken);
  printf("Throughput: %.3f Bps \n",(float)total_bytes/(float)time_taken);
  
  return 0;
}