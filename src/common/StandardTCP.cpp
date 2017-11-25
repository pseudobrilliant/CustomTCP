#include <poll.h>
#include "StandardTCP.h"

using namespace std;

StandardTCP::StandardTCP(const char* source_ip, int source_port, int packet_size):
Protocol(source_ip, source_port, packet_size)
{
}

bool StandardTCP::Connect()
{
    //Attempts to open a raw socket to the target and set its options
    socket_id = socket(PF_INET,SOCK_DGRAM,IPPROTO_TCP);
    if(socket_id >= 0)
    {
        int one = 1;
        const int * val = &one;
        //Attempts to notify the socket that our header will be pre-included with the packet.
        int ret = setsockopt(socket_id, IPPROTO_IP, SO_DEBUG,val, sizeof(one));
        if (ret >= 0)
        {
            cout << "Successfully Created and Configured Socket\n";
            return true;
        }
        else
        {
            cout << "ERROR: Unable to configure and bind socket\n";
            return false;
        }
    }
    else
    {
        cout << "ERROR: Unable to open socket with code " << std::strerror(errno)<<"\n";
        return false;
    }
}

int StandardTCP::Receive(char* buffer, int size)
{
    pollfd tcpfds[1];
    int recv;

    tcpfds[0].fd = socket_id;
    tcpfds[0].events = POLLIN;

    recv = poll(tcpfds,1,35000);

    if(recv < 0)
    {
        cout << "ERROR: Unable to poll sockets\n";
    }
    else
    if(recv == 0)
    {
        cout << "Timeout occurred no messages received or sent\n";
    }
    else
    {
        cout << "Message Received\n";
    }

    return recv;
}
/*
int StandardTCP::CreatePacket( char* packet, int seq, int ack_num, int offset, int syn, int ack, int urgent, int size)
{
    memset(packet, 0, packet_size);

    iphdr *ip = (struct iphdr *) packet;
    CreateIP(ip, source_ip, target_ip, sizeof(struct tcphdr));

    tcphdr *tcp = (struct tcphdr *) (packet + sizeof(struct iphdr));
    CreateTCP(tcp, source_port, target_port, seq,ack_num,offset,syn,ack,urgent);

    tcp->checksum = Checksum((unsigned short *) packet, ip->tot_len);

    return ip->tot_len;
}


void StandardTCP::ProcessMessage(char *buffer)
{
    char *packet = new char[packet_size];

    tcphdr *msg = (tcphdr*)(buffer + sizeof(iphdr));
    if(msg->ack == 1 and msg->syn == 1)
    {
        int size = CreatePacket(packet, 0, 0, 5, 1, 1, 0, packet_size);
        SendPacket(packet, size);
    }
}*/

void StandardTCP::Disconnect()
{
    Protocol::Disconnect();
}

StandardTCP::~StandardTCP()
{
    close(socket_id);
}


