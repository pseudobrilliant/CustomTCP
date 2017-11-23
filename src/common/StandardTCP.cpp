#include <poll.h>
#include "StandardTCP.h"

using namespace std;

StandardTCP::StandardTCP(const char* source_ip, const char* target_ip, int source_port, int target_port, int packet_size):
Protocol(source_ip, target_ip, source_port, target_port, packet_size)
{
	Connect();
}

void StandardTCP::Connect()
{
	//Attempts to open a raw socket to the target and set its options
	socket_id = socket(PF_INET, SOCK_RAW,IPPROTO_TCP);
	if(socket_id >= 0)
	{
		int one = 1;
		const int * val = &one;
		//Attempts to notify the socket that our header will be pre-included with the packet.
		if (setsockopt(socket_id, IPPROTO_IP, IP_HDRINCL,val, sizeof(val)) >= 0
                && bind(socket_id,(struct sockaddr*)source_socket_addr,(socklen_t )sizeof(source_socket_addr) >=0))
		{	
			cout << "\nSuccessfully Created and Configured Sending Socket\n";
		}
		else
		{	
			cout << "\nERROR: Unable to configure and bind socket.\n";
            return;
		}
	}
	else
	{
		cout << "\nERROR: Unable to open socket with code " << std::strerror(errno) << "\n";
        return;
	}

    Receive();
}

void StandardTCP::Send_Packet(const char *packet, int size)
{
    int ret = sendto(socket_id,packet,size,0,(struct sockaddr*)source_socket_addr,sizeof(sockaddr_in) >= 0);
	if (re > = 0)
	{
        cout << "\nSuccessfully sent message ''";
	}

    Receive();
}

void StandardTCP::Receive()
{
    pollfd tcpfds[1];
    int recv;

    tcpfds[0].fd = socket_id;
    tcpfds[0].events = POLLIN;

    recv = poll(tcpfds,1,35000);

    if(recv < 0)
    {
        cout << "\nERROR: Unable to poll sockets";
    }
    else
    if(recv == 0)
    {
        cout << "\nTimeout occurred no messages received or sent";
    }
    else
    {
        cout << "\nMessage Received";
    }

}


void StandardTCP::Disconnect()
{

}

StandardTCP::~StandardTCP()
{
    close(socket_id);
}


