#include "Protocol.h"
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

class CustomTCP : public  Protocol
{
    public:


        CustomTCP(const char *source_ip, int source_port, int packet_size);
		bool Connect();
		int Receive(char *buffer, int size);
		void Disconnect();
		int CreatePacket(char* packet, int seq, int ack_num, int offset, int syn, int ack, int urgent, int size);
		void Handshake(const char* _target_ip, int _target_port);
		~CustomTCP();

	void ProcessMessage(char *buffer);

protected:
		tcphdr *
		CreateTCP(tcphdr *tcp, int source, int target, int seq, int ack_num, int offset, int syn, int ack,
                                  int urgent);

};

