#include <poll.h>
#include "CustomTCP.h"

using namespace std;

CustomTCP::CustomTCP(const char* source_ip, int source_port, int packet_size):
Protocol(source_ip, source_port, packet_size)
{
}

tcphdr * CustomTCP::CreateTCP(tcphdr *tcp, unsigned short source, unsigned short target, unsigned int seq,
                              unsigned short ack_num, unsigned short offset, unsigned short syn, unsigned short ack,
                              unsigned short urgent, unsigned short window_size)
{
    tcp->source = htons(source);
    tcp->dest = htons(target);
    tcp->seq = htonl(seq);
    tcp->ack_seq = htonl(ack_num);
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
    tcp->window = htons(window_size);
    tcp->check = 0;
}

bool CustomTCP::Connect(int _timeout)
{
    //Attempts to open a raw socket to the target and set its options
    socket_id = socket(PF_INET, SOCK_RAW, IPPROTO_TCP);
    if(socket_id >= 0)
    {
        sequence_number = 0;
        timeout = _timeout;
        int one = 1;
        const int * val = &one;
        //Attempts to notify the socket that our header will be pre-included with the packet.
        int ret = setsockopt(socket_id, IPPROTO_IP, IP_HDRINCL,val, sizeof(one));
        ret &= bind(socket_id,(sockaddr*) source_socket_addr,sizeof(sockaddr_in));
        if (ret >= 0)
        {
            cout << "Successfully created and configured socket on port #"<<source_port<<"\n";
            return true;
        }
        else
        {
            cout << "ERROR: Unable to configure and bind socket on port #"<<source_port<<"\n";
            return false;
        }
    }
    else
    {
        cout << "ERROR: Unable to open socket with code " << std::strerror(errno)<<"\n";
        return false;
    }
}

bool CustomTCP::Handshake(const char *_target_ip, int _target_port, int _window_size)
{
    handshake = true;
    SetTarget(_target_ip,_target_port);
    window_size = _window_size;
    char *packet = new char[packet_size];
    int size = CreatePacket(packet, sequence_number, 0, 5, 1, 0, 0, window_size,0, nullptr);
    SendPacket(packet, size);

    if(ProcessMessage() == SYN_ACK)
    {
       return true;
    }

    return false;
}

Packet_Type CustomTCP::ProcessMessage()
{
    char *buffer = new char[packet_size];

    if(Receive(buffer,packet_size))
    {
        iphdr *ip = (iphdr *) buffer;
        tcphdr *msg = (tcphdr *) (buffer + sizeof(iphdr));
        char *packet = new char[packet_size];

        if (msg->ack == 0 && msg->syn == 1 && target_socket_addr == nullptr)
        {
            handshake = true;
            window_size = ntohs(msg->window);
            in_addr temp;
            temp.s_addr = ip->daddr;
            SetTarget(inet_ntoa(temp), ntohs(msg->source));
            int size = CreatePacket(packet, sequence_number, ntohl(msg->seq) + 1, 5, 1, 1, 0, window_size, 0, nullptr);
            SendPacket(packet, size);
            return SYN;
        }
        else
        if (msg->ack == 1 && msg->syn == 1 && handshake)
        {
            sequence_number = ntohl(msg->ack_seq);
            int size = CreatePacket(packet, sequence_number, ntohl(msg->seq) + 1, 5, 0, 1, 0, window_size, 0, nullptr);
            SendPacket(packet, size);
            return SYN_ACK;
        }
        else
        if (msg->ack == 1 && msg->syn == 0 && handshake)
        {
            sequence_number = ntohl(msg->ack_seq);
            handshake = false;
            return ACK;
        }
    }

    return NONE;
}


int CustomTCP::CreatePacket(char *packet, int _seq, int _ack_num, int _offset, int _syn, int _ack, int _urgent, int window_size,
                            int _size, char *_data) {
    memset(packet, 0, packet_size);

    iphdr *ip = (struct iphdr *) packet;
    CreateIP(ip, source_ip, target_ip, sizeof(struct tcphdr) + _size);

    tcphdr *tcp = (struct tcphdr *) (packet + sizeof(struct iphdr));
    CreateTCP(tcp, source_port, target_port, _seq, _ack_num, _offset, _syn, _ack, _urgent, window_size);

    if (_size > 0)
    {
        memcpy(packet + sizeof(iphdr) + sizeof(tcphdr), _data, _size);
    }

    tcp->check = Checksum((unsigned short *) packet, ip->tot_len);

    return ip->tot_len;
}

int CustomTCP::Receive(char * buffer, int size)
{
    pollfd tcpfds[1];
    int res = 0;

    tcpfds[0].fd = socket_id;
    tcpfds[0].events = POLLIN;

    res = poll(tcpfds,1,timeout);

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
            res = recvfrom(socket_id, buffer, size, 0,NULL, NULL);

        if(res > 0)
        {
            iphdr* ip = (iphdr*) buffer;
            tcphdr* tcp = (tcphdr*)(buffer + sizeof(iphdr));

            if(ip->daddr == inet_addr(source_ip) && tcp->dest == htons(source_port))
            {
                return size;
            }
            else
            {
                return Receive(buffer,size);
            }
        }
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

void CustomTCP::SendData(string data)
{
    char* packet = new char[packet_size];
    int segment_size = packet_size - sizeof(iphdr) - sizeof(tcphdr);
    char* data_p = const_cast<char*>(data.c_str());
    window_size = data.size();
    int window_left = data.size();
    int window_sent = 0;


    while(window_left > 0)
    {
        segment_size = window_left < segment_size ? window_left: segment_size;

        char* data_segment = new char[segment_size];
        memcpy(data_segment,data_p,segment_size);

        int size = CreatePacket(packet,sequence_number, 0, 5, 0, 0, 0, segment_size, segment_size, data_segment);
        SendPacket(packet, size);

        char *buffer = new char[packet_size];
        if(Receive(buffer,packet_size))
        {
            tcphdr* tcp = (tcphdr*)(buffer + sizeof(iphdr));
            int ack_seq = ntohl(tcp->ack_seq);
            if(tcp->ack == 1 && ack_seq == window_sent + segment_size)
            {
                sequence_number += segment_size;
                window_left = ntohs(tcp->window);
                window_sent += segment_size;
                data_p += segment_size;
            }
        }
        else
        {
            cout<<"ERROR: Packet sent but no ACK received.\n";
            return;
        }
    }

    if(window_sent < window_size)
    {
        cout<<"ERROR: Unable to transmit entire message, server reported finished.";
    }
}


string CustomTCP::ReceiveData()
{
    char* data = new char[window_size];
    char* buffer = new char[packet_size];
    char* data_p = data;
    int window_received = 0;

    while(window_received < window_size)
    {
        if (Receive(buffer, packet_size))
        {
            tcphdr *tcp = (tcphdr *) (buffer + sizeof(iphdr));
            char* recv_data = (buffer + sizeof(iphdr) + sizeof(tcphdr));
            int recv_window = ntohs(tcp->window);

            memcpy(data_p,recv_data,recv_window);
            data_p +=recv_window;

            window_received += recv_window;
            int window_left = window_size - window_received;

            char *packet = new char[1024];
            int size = CreatePacket(packet,sequence_number,window_received ,5,0,1,0,window_left,0, nullptr);

            SendPacket(packet,size);
        }
        else
        {
            cout << "ERROR: Timeout unable to receive full message";
            return "";
        }
    }

    return string(data,window_size);
}