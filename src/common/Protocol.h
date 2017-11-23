#include <iostream>
#include <cstring>
#include <cerrno>
#include <clocale>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>

using namespace std;

class Protocol
{

	public:
		Protocol(const char *source_ip, const char *target_ip, int source_port, int target_port, int packet_size);
		~Protocol();
		virtual void Connect() = 0;
		virtual void Send_Packet(const char *packet, int size);
		virtual void Receive() = 0;
		virtual void Disconnect() = 0;

	protected:
		const char *source_ip;
		const char *target_ip;
		int source_port;
		int target_port;
		int packet_size;
		int socket_id;
		struct sockaddr_in *source_socket_addr;
        struct sockaddr_in *target_socket_addr;

		void CreateIP(iphdr *ip, const char *source, const char *target, int tcp_size);
		unsigned short Checksum(unsigned short *data, unsigned short length);
};
