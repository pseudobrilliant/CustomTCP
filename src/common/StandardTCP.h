#include "Protocol.h"

class StandardTCP : public  Protocol
{


        public:
                bool Connect(const char* ip, int port, int _timeout);
                StandardTCP(const char *source_ip, int source_port, int packet_size);
                bool Initialize(int _timeout);
                int Receive(char *buffer, int size);
                void Disconnect();
                ~StandardTCP();
                bool Receive_Data();
                bool Send_Data(string data);

        protected:
            void Handshake();

            void ProcessMessage(char *buffer);

            int socket_id;


};

