#include <stdio.h>
#include "../common/CustomTCP.h"
#include "../common/StandardTCP.h"

int main()
{
    string input = "";
    cout<<"Run using TCP or Custom.\n";
    cin >> input;

    if(input == "Custom") {
        CustomTCP *server = new CustomTCP("127.0.0.1", 1357, 512);
        server->Connect(35000);
        if (server->ProcessMessage() != SYN) {
            cout << "Did not receive SYN message to start connection.\n";
            return -1;
        }
        cout << "SYN Received --> SYN_ACK Sent\n";

        if (server->ProcessMessage() != ACK) {
            cout << "Did not receive ACK message to finalize handshake.\n";
            return -1;
        }
        cout << "ACK Received --> Connection Socket #" << server->target_port << "\n";

        string str = server->ReceiveData();

        if (str != "") {
            cout << "Received the following complete Message: \n" << str;
        }

        return 0;
    }
    else
    {
        StandardTCP* server = new StandardTCP("127.0.0.1",1357,512);
        server->Initialize(35000);
        server->Receive_Data();
    }
}

