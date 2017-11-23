#include "Protocol.h"


class StandardTCP : public  Protocol
{


        public:
                StandardTCP(const char *source_ip, const char *target_ip, int source_port, int target_port, int packet_size);
                void Connect();
                void Send();
                void Receive();
                void Disconnect();
                ~StandardTCP();

        protected:
		void Handshake();
};

