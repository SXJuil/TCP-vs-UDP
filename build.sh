#!/bin/sh
g++ server_tcp.cc -o server_tcp && g++ client_tcp.cc -o client_tcp
g++ server_udp.cc -o server_udp && g++ client_udp.cc -o client_udp