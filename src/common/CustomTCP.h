#include "Protocol.h"

enum Packet_Type
{
    SYN,
    SYN_ACK,
    ACK,
    MSG,
    NONE
};

class CustomTCP : public  Protocol
{
    public:
		int timeout = 0;
		int sequence_number = 0;
		CustomTCP(const char *source_ip, int source_port, int packet_size);
		bool Connect(int _timeout);
		int Receive(char *buffer, int size);
		void Disconnect();
		int CreatePacket(char *packet, int seq, int ack_num, int offset, int syn,
                         int ack, int urgent, int window_size, int size, char *data);
		bool Handshake(const char *_target_ip, int _target_port, int window_size = 0);
		Packet_Type ProcessMessage();
        string ReceiveData();
		~CustomTCP();

    void SendData(string basic_string);

protected:

		tcphdr *CreateTCP(tcphdr *tcp, unsigned short source, unsigned short target, unsigned int seq,
                                  unsigned short ack_num, unsigned short offset, unsigned short syn, unsigned short ack,
                                  unsigned short urgent, unsigned short window_size);

   unsigned short window_size;


};

