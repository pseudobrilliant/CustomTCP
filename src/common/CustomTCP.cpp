#include <poll.h>
#include "CustomTCP.h"

using namespace std;

CustomTCP::CustomTCP(const char* source_ip, int source_port, int packet_size):
Protocol(source_ip, source_port, packet_size)
{
}

tcphdr* CustomTCP::CreateTCP(tcphdr *tcp, unsigned short source, unsigned short target,
                             unsigned int seq, unsigned short ack_num, unsigned short offset,
                             unsigned short syn, unsigned short ack, unsigned short urgent)
{
	tcp->source = htons(source);
	tcp->dest = htons(target);
	tcp->seq = htons(seq);
	tcp->ack_seq = ack_num;
	tcp->doff = offset;
    tcp->res1 = 0;
    tcp->fin = 0;
	tcp->syn = syn;
    tcp->rst = 0;
    tcp->psh;
    tcp->ack = ack;
    tcp->urg = urgent;
    tcp->ece = 0;
    tcp->cwr = 0;
	tcp->window = htons(32767);
	tcp->check = 0;
}

bool CustomTCP::Connect()
{
    //Attempts to open a raw socket to the target and set its options
    socket_id = socket(PF_INET, SOCK_RAW, IPPROTO_TCP);
    if(socket_id >= 0)
    {
        int one = 1;
        const int * val = &one;
        //Attempts to notify the socket that our header will be pre-included with the packet.
        int ret = setsockopt(socket_id, IPPROTO_IP, IP_HDRINCL,val, sizeof(one));
        ret &= bind(socket_id,(sockaddr*) source_socket_addr,sizeof(sockaddr_in));
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

void CustomTCP::Handshake(const char* _target_ip, int _target_port)
{
    handshake = true;
    SetTarget(_target_ip,_target_port);

    char *packet = new char[packet_size];
    int size = CreatePacket(packet, 0, 0, 5, 1, 0, 0, packet_size);
    SendPacket(packet, size);

    char *buffer = new char[packet_size];
    if(Receive(buffer,packet_size) >0)
    {
        ProcessMessage(buffer);
    }
}

void CustomTCP::ProcessMessage(char *buffer)
{
    char *packet = new char[packet_size];

    iphdr *ip = (iphdr*) buffer;
    tcphdr *msg = (tcphdr*)(buffer + sizeof(iphdr));

    if(msg->ack == 0 && msg->syn == 1 && target_socket_addr == nullptr)
    {
        in_addr temp;
        temp.s_addr = ip->daddr;
        SetTarget(inet_ntoa(temp),ntohs(msg->source));
        int size = CreatePacket(packet, 0, 0, 5, 1, 1, 0, packet_size);
        SendPacket(packet, size);
    }
    else
    if(msg->ack == 1 && msg->syn == 0 && handshake) {
        int size = CreatePacket(packet, 0, 0, 5, 0, 1, 0, packet_size);
        SendPacket(packet, size);
        handshake = false;
    }
}


int CustomTCP::CreatePacket(char *packet, int _seq, int _ack_num, int _offset, int _syn, int _ack, int _urgent, int _size)
{
    memset(packet, 0, _size);

    iphdr *ip = (struct iphdr *) packet;
    CreateIP(ip, source_ip, target_ip, sizeof(struct tcphdr));

    tcphdr *tcp = (struct tcphdr *) (packet + sizeof(struct iphdr));
    CreateTCP(tcp, source_port, target_port, _seq,_ack_num,_offset,_syn,_ack,_urgent);

    tcp->check = Checksum((unsigned short *) packet, ip->tot_len);

    return ip->tot_len;
}

int CustomTCP::Receive(char * buffer, int size)
{
    pollfd tcpfds[1];
    int res = 0;

    tcpfds[0].fd = socket_id;
    tcpfds[0].events = POLLIN;

    res = poll(tcpfds,1,35000);

    if(res < 0)
    {
        cout << "ERROR: Unable to poll sockets\n";
    }
    else
    if(res == 0)
    {
        cout << "Timeout occurred no messages received or sent\n";
    }
    else
    {
        cout << "Message Received\n";
        recv(socket_id,buffer,size,0);
    }

    return res;
}

void CustomTCP::Disconnect()
{
    Protocol::Disconnect();
}

CustomTCP::~CustomTCP()
{
    Disconnect();
}


