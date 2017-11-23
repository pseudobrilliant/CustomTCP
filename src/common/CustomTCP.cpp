#include <poll.h>
#include "CustomTCP.h"

using namespace std;

CustomTCP::CustomTCP(const char* source_ip, const char* target_ip, int source_port, int target_port, int packet_size):
Protocol(source_ip, target_ip, source_port, target_port, packet_size)
{
    Connect();
    Handshake();
}

CustomTCP::tcphdr * CustomTCP::CreateTCP(tcphdr *tcp, int source, int target, int seq, int ack_num, int offset, int syn, int ack, int urgent)
{
	tcp->source_port = htons(source);
	tcp->target_port = htons(target);
	tcp->sequence_number = htons(seq);
	tcp->ack_number = ack_num;
	tcp->data_offset = offset;
	tcp->syn = syn;
    tcp->ack = ack;
	tcp->window = htons(32767);
	tcp->checksum = 0;
	tcp->urgent = urgent;
}

void CustomTCP::Connect()
{
	//Attempts to open a raw socket to the target and set its options
	socket_id = socket(PF_INET, SOCK_RAW,IPPROTO_TCP);
	if(socket_id >= 0)
	{
		int one = 1;
		const int * val = &one;
		//Attempts to notify the socket that our header will be pre-included with the packet.
        int ret = setsockopt(socket_id, IPPROTO_IP, IP_HDRINCL,val, sizeof(one));
        if (ret >= 0)
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
}

void CustomTCP::Handshake()
{
    char packet_arr[packet_size];
    memset(packet_arr, 0, packet_size);
    char *packet = packet_arr;

    iphdr *ip = (struct iphdr *) packet;
    CreateIP(ip, source_ip, target_ip, sizeof(struct tcphdr));

    tcphdr *tcp = (struct tcphdr *) (packet + sizeof(struct iphdr));
    CreateTCP(tcp, source_port, target_port, 0, 0, 5, 1, 0, 0);

    tcp->checksum = Checksum((unsigned short *) packet, ip->tot_len);

    Send_Packet(packet,ip->tot_len);
}

void CustomTCP::Receive()
{
    pollfd tcpfds[1];
    int recv;

    tcpfds[0].fd = socket_id;
    tcpfds[0].events = POLLIN;

    recv = poll(tcpfds,1,3500);

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

void CustomTCP::Disconnect()
{

}

CustomTCP::~CustomTCP()
{
    close(socket_id);
}


