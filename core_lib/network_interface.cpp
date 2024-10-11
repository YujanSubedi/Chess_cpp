#include "global_values.hpp"
#include "network_interface.hpp"
#include <cstdlib>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>

void Network_interface::initiliaze_as_server(int port_address, Piece_color Selected_color){
    int old_socket, new_socket;
    old_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(old_socket<=0){
        perror("creation failed");
    }

    struct sockaddr_in serv_address;
    serv_address.sin_family = AF_INET;
    serv_address.sin_addr.s_addr = INADDR_ANY;
    serv_address.sin_port = htons(port_address);
    if(bind(old_socket, (struct sockaddr *)&serv_address, sizeof(serv_address)) < 0){
        perror("Binding failed");
    }
    listen(old_socket, 1);
    printf("Server is listening at port %d\n", port_address);
    struct sockaddr_in client_address;
    socklen_t client_len;
    new_socket = accept(old_socket, (struct sockaddr*)&client_address, &client_len);
    if(new_socket < 0){
        perror("Failed to connect to client");
    }
    close(old_socket);
    socket_address = new_socket;
    Piece_color Oppenent_color = Selected_color == white_colored ? black_colored : white_colored;
    send(socket_address, &Oppenent_color, sizeof(Oppenent_color), 0);
}

void Network_interface::initiliaze_as_client(char ip_address[], int port_address, Piece_color &Given_color){
    socket_address = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_address<0){
        perror("Error: socket creatioin failed");
    }
    struct sockaddr_in serv_address;
    serv_address.sin_family = AF_INET;
    serv_address.sin_port = htons(port_address);
    if(inet_pton(AF_INET, ip_address, &serv_address.sin_addr)<=0){
        perror("error");
    }
    if(connect(socket_address, (struct sockaddr *)&serv_address, sizeof(serv_address)) < 0){
        perror("error");
    }
    if(recv(socket_address, &Given_color, sizeof(Given_color), 0) < 0){
        printf("Error assiging color");
    }
    printf("Assigned color is %d\n", Given_color);
}

bool Network_interface::receive_Move(MOVE_CLASS &receivedMove){
    int valread =  recv(socket_address, &receivedMove, sizeof(MOVE_CLASS), MSG_DONTWAIT);
    printf("%lu\n", sizeof(MOVE_CLASS));
    if(valread>0){
        return true;
    }
    else if(valread==-1 && errno == EAGAIN){
        printf("No message received\n");
    }
    else if(valread==0){
        printf("Connection closed \n");
        Network_interface::~Network_interface();
        exit(1);
    }
    else{
        printf("Error occured %d\n", valread);
        Network_interface::~Network_interface();
        exit(1);
    }
    return false;
}

void Network_interface::send_Move(MOVE_CLASS sendMove){
    send(socket_address, &sendMove, sizeof(MOVE_CLASS), 0);
    printf("%lu\n", sizeof(MOVE_CLASS));
    printf("%lu\n", sizeof(sendMove));
    sleep(1);
}

Network_interface::~Network_interface(){
    close(socket_address);
    printf("Network Interface closed\n");
}
