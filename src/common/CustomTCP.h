#include "Protocol.h"

class CustomTCP : public  Protocol
{
    public:
		CustomTCP(const char *source_ip, int source_port, int packet_size);
		bool Connect();
		int Receive(char *buffer, int size);
		void Disconnect();
		int CreatePacket(char* packet, int seq, int ack_num, int offset, int syn, int ack, int urgent, int size);
		void Handshake(const char* _target_ip, int _target_port);
		void ProcessMessage(char *buffer);
		~CustomTCP();

	protected:
		tcphdr *CreateTCP(tcphdr *tcp, unsigned short source, unsigned short target, unsigned int seq, unsigned short ack_num,
                      unsigned short offset, unsigned short syn, unsigned short ack, unsigned short urgent);
};

