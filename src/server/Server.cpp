#include <stdio.h>
#include "../common/CustomTCP.h"

int main()
{
        CustomTCP *server = new CustomTCP("127.0.0.1",1357,512);
        server->Connect(350000);
        if(server->ProcessMessage() != SYN)
        {
            cout<<"Did not receive SYN message to start connection.\n";
            return -1;
        }
        cout<<"SYN Received --> SYN_ACK Sent\n";

        if(server->ProcessMessage() != ACK)
        {
            cout<<"Did not receive ACK message to finalize handshake.\n";
            return -1;
        }
        cout<<"ACK Received --> Connection Socket #"<<server->target_port<<"\n";

        string str = server->ReceiveData();

        if(str != "")
        {
            cout << "Received the following complete Message: \n" << str;
        }

        return 0;
}

