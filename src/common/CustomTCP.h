#include "Protocol.h"

class CustomTCP : public  Protocol
{
        public:
			struct tcphdr
			{
				unsigned short int source_port;
				unsigned short int target_port;
				unsigned int sequence_number;
				unsigned int ack_number;
				unsigned char reserved:4;
				unsigned char data_offset:4;
				//Control Bits
				unsigned int res1:4;
				unsigned int hlen:4;
				unsigned int fin:1;
				unsigned int syn:1;
				unsigned int rst:1;
				unsigned int psh:1;
				unsigned int ack:1;
				unsigned int urg:1;
				unsigned int res2:2;

				unsigned short int window;
				unsigned short int checksum;
				unsigned short int urgent;
			};

			CustomTCP(const char *source_ip, const char *target_ip, int source_port, int target_port, int packet_size);
			void Connect();
			void Receive();
			void Disconnect();
			~CustomTCP();

        protected:
			CustomTCP::tcphdr *
			CreateTCP(tcphdr *tcp, int source, int target, int seq, int ack_num, int offset, int syn, int ack,
                                  int urgent);
			void Handshake();
};

