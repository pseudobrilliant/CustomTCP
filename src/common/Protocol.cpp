#include "Protocol.h"

Protocol::Protocol(const char* _source_ip, const char* _target_ip, int _source_port, int _target_port, int _packet_size)
{
	source_ip = _source_ip;
	target_ip = _target_ip;
	source_port = _source_port;
	target_port = _target_port;
	packet_size = _packet_size;

	source_socket_addr = new sockaddr_in();
	source_socket_addr->sin_family = AF_INET;
	source_socket_addr->sin_port = htons(source_port);
	source_socket_addr->sin_addr.s_addr = inet_addr(source_ip);

	target_socket_addr = new sockaddr_in();
    target_socket_addr->sin_family = AF_INET;
	target_socket_addr->sin_port = htons(target_port);
	target_socket_addr->sin_addr.s_addr = inet_addr(target_ip);
}

void Protocol::CreateIP(iphdr *ip, const char *source, const char *target, int tcp_size)
{
	ip->ihl = 5;//header length
	ip->version = 4;//version
	ip->tos = 16; //type of service
	ip->tot_len = sizeof(struct iphdr) + tcp_size;//total length
	ip->id = htons(54321);
	ip->frag_off = 0;//fragment offset
	ip->ttl = 64;//time to live
	ip->protocol = IPPROTO_TCP;//protocol => TCP
	ip->check = 0;
	ip->saddr = inet_addr(source);
	ip->saddr = inet_addr(target);
}

void Protocol::Connect()
{

}

void Protocol::Send_Packet(const char *packet, int size)
{

    int ret = sendto(socket_id,packet,size,0,(struct sockaddr*)target_socket_addr,sizeof(sockaddr_in));
    if (ret > 0)
    {
        cout << "\nSuccessfully sent message ''";
    }
    else
    {
        cout << "ERROR:Unable to send message with code \"" <<std::strerror(errno) << "\"\n";
        return;
    }
}


void Protocol::Receive()
{

}

void Protocol::Disconnect()
{

}

unsigned short Protocol::Checksum(unsigned short *data, unsigned short  length)
{
	unsigned int sum = 0;
	
	while(length-- > 0)
	{
		sum += *(data++);
	}

	sum = (sum >> 16);
	sum += (sum &0xFFFF);
	sum += sum >> 16;


	return  (unsigned short) ~sum;
}

Protocol::~Protocol()
{

}


