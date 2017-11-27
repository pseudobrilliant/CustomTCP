#include <poll.h>
#include "StandardTCP.h"

using namespace std;

StandardTCP::StandardTCP(const char* source_ip, int source_port, int packet_size):
Protocol(source_ip, source_port, packet_size)
{
}

bool StandardTCP::Initialize(int _timeout)
{
    //Attempts to open a raw socket to the target and set its options
    socket_id = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (socket_id >= 0) {
        timeout = _timeout;
        int one = 1;
        const int *val = &one;
        //Attempts to notify the socket that our header will be pre-included with the packet.
        int ret = bind(socket_id, (sockaddr *) source_socket_addr, sizeof(sockaddr_in));
        ret &= listen(socket_id, 1);
        socklen_t size = sizeof(sockaddr_in);
        target_socket_addr = new sockaddr_in;
        socket_id = accept(socket_id,(sockaddr*) target_socket_addr,&size);

        if (ret >= 0 && socket_id >0)
        {
            target_port = ntohs(target_socket_addr->sin_port);
            target_ip = inet_ntoa(target_socket_addr->sin_addr);

            cout << "Successfully Created and Configured Socket\n";
            return true;
        } else {
            cout << "ERROR: Unable to configure and bind socket\n";
            return false;
        }
    } else {
        cout << "ERROR: Unable to open socket with code " << std::strerror(errno) << "\n";
        return false;
    }

}

bool StandardTCP::Connect(const char* ip, int port, int _timeout)
{
    SetTarget(ip,port);
    //Attempts to open a raw socket to the target and set its options
    socket_id = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (socket_id >= 0) {
        timeout = _timeout;
        int one = 1;
        const int *val = &one;
        //Attempts to notify the socket that our header will be pre-included with the packet.
        socklen_t size = sizeof(sockaddr_in);
        connect(socket_id,(sockaddr*) target_socket_addr,size);

        if (socket_id >=0)
        {
            cout << "Successfully Created and Configured Socket\n";
            return true;
        } else {
            cout << "ERROR: Unable to configure and bind with code " << std::strerror(errno) << "\n";
            return false;
        }
    } else {
        cout << "ERROR: Unable to open socket with code " << std::strerror(errno) << "\n";
        return false;
    }

}

bool StandardTCP::Receive_Data()
{
    bool ret = false;
    char* buffer = new char[packet_size];
    while(Receive(buffer,packet_size))
    {
        cout<<"Received Data as " << buffer;
        ret = true;
    }

    return ret;
}

bool StandardTCP::Send_Data(string data)
{
    const char* buff = data.c_str();
    if(send(socket_id,buff,data.size(),0))
    {
        return true;
    }

    return false;
}


int StandardTCP::Receive(char* buffer, int size)
{
    pollfd tcpfds[1];
    int ret;

    tcpfds[0].fd = socket_id;
    tcpfds[0].events = POLLIN;

    ret = poll(tcpfds,1,timeout);

    if(ret < 0)
    {
        cout << "ERROR: Unable to poll sockets\n";
    }
    else
    if(ret == 0)
    {
        cout << "Timeout occurred no messages received or sent\n";
    }
    else
    {
        cout << "Message Received\n";
        ret = recv(socket_id,buffer,size,0);
    }

    return ret;
}

void StandardTCP::Disconnect()
{
    Protocol::Disconnect();
    close(socket_id);
}

StandardTCP::~StandardTCP()
{
    Disconnect();
}


