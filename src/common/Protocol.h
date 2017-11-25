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
#include <boost/signals2/signal.hpp>

using namespace std;

class Protocol
{

	public:
		const char *source_ip;
		const char *target_ip;
		int source_port;
		int target_port;

		Protocol(const char *source_ip, int source_port, int packet_size);
		~Protocol();
		virtual bool Connect();
		virtual bool SendPacket(const char *packet, int size);
		virtual int Receive(char *buffer, int size) = 0;
		virtual void Disconnect();

        void SetTarget(const char *_target_ip, int _target_port);

        typedef boost::signals2::signal<void()> signal_t;
        void RegisterSent(boost::function<void()> callback);
        void RegisterReceived(boost::function<void()> callback);

	protected:

		int packet_size;
		int socket_id;
		bool handshake = false;
		struct sockaddr_in *source_socket_addr;
        struct sockaddr_in *target_socket_addr;

        signal_t message_sent;
        signal_t message_received;

		void CreateIP(iphdr *ip, const char *source, const char *target, int tcp_size);
		unsigned short Checksum(unsigned short *data, unsigned short length);
};