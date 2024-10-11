#ifndef NETWORK_INTERFACE_HPP
#define NETWORK_INTERFACE_HPP

#include "global_values.hpp"
#include <sys/socket.h>

class Network_interface{
private:
	int socket_address;
public:
	void initiliaze_as_server(int port_address, Piece_color Selected_color); // Server initilization
	void initiliaze_as_client(char ip_address[], int port_address, Piece_color &Given_color); // Client initilization
	bool receive_Move(MOVE_CLASS &receivedMove);
	void send_Move(MOVE_CLASS sendMove);
	~Network_interface();
};

#endif
