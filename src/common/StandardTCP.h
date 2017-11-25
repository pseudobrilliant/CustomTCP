#include "Protocol.h"

class StandardTCP : public  Protocol
{


        public:
                StandardTCP(const char *source_ip, int source_port, int packet_size);
                bool Connect();
                int Receive(char *buffer, int size);
                void Disconnect();
                ~StandardTCP();

        protected:
		void Handshake();

	void ProcessMessage(char *buffer);
};

