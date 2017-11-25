#include <thread>
#include "gtest/gtest.h"
#include "CustomTCP.h"

class CustomTCPTestFixture : public testing::Test
{
    public:
    CustomTCP *client;
    CustomTCP *server;
    const char* client_ip = "127.0.0.1";
    const char* server_ip = "127.0.0.1";
    int client_port = 1347;
    int server_port = 1348;

    CustomTCPTestFixture(){}

    void SetUp()
    {
        client = new CustomTCP(client_ip, client_port, 1024);
        server = new CustomTCP(server_ip, server_port, 1024);
    }

    void TearDown()
    {
        client->Disconnect();
        server->Disconnect();
        delete(client);
        delete(server);
    }

    ~CustomTCPTestFixture(){}

};

TEST_F(CustomTCPTestFixture,Connecting)
{
    EXPECT_TRUE(client->Connect());
    EXPECT_TRUE(server->Connect());
}

TEST_F(CustomTCPTestFixture,CreatePacket)
{

    char *packet =new char[1024];
    client->SetTarget("127.0.0.1", server_port);
    client->CreatePacket(packet, 0, 0, 5, 1, 0, 0, 1024);

    iphdr *ip = (iphdr*)packet;
    tcphdr *tcp = (tcphdr*)(packet + sizeof(iphdr));

    EXPECT_EQ(ip->version,4);
    EXPECT_EQ(ip->saddr,inet_addr(client_ip));
    EXPECT_EQ(ip->daddr,inet_addr(server_ip));

    EXPECT_EQ(tcp->source_port,htons(client_port));
    EXPECT_EQ(tcp->target_port,htons(server_port));
    EXPECT_EQ(tcp->sequence_number, 0);
    EXPECT_EQ(tcp->ack,0);
    EXPECT_EQ(tcp->syn,1);
}


TEST_F(CustomTCPTestFixture,Handshake)
{
    client->Connect();
    server->Connect();

    std::thread t([](CustomTCPTestFixture* fixture){
        std::this_thread::sleep_for(chrono::milliseconds(500));
        fixture->client->Handshake("127.0.0.1", fixture->server_port);
    },this);


    char *buffer =new char[1024];
    int ret = server->Receive(buffer,1024);
    ASSERT_GT(ret,0);

    tcphdr* tcp = (tcphdr *)(buffer + sizeof(iphdr));
    ASSERT_EQ(tcp->syn,1);
    ASSERT_EQ(tcp->ack,0);

    server->ProcessMessage(buffer);
    t.join();

    EXPECT_EQ(server->source_port, server_port);
    EXPECT_EQ(server->target_port, client_port);
    EXPECT_STREQ(server->source_ip, server_ip);
    EXPECT_STREQ(server->target_ip, client_ip);
}

TEST_F(CustomTCPTestFixture,Sending)
{
    client->Connect();
    client->SetTarget("127.0.0.1", server_port);
    char *packet =new char[1024];
    int size = client->CreatePacket(packet,0, 0, 5, 1, 0, 0, 1024);
    EXPECT_GE(size,0);
    EXPECT_GT(client->SendPacket(packet,size),0);
}

TEST_F(CustomTCPTestFixture,Receiving)
{
    client->Connect();
    server->Connect();
    client->SetTarget("127.0.0.1", server_port);
    char *packet =new char[1024];
    int size = client->CreatePacket(packet, 0, 0, 5, 1, 0, 0, 1024);

    EXPECT_GE(size,0);
    EXPECT_TRUE(client->SendPacket(packet,size));

    char *buffer =new char[1024];
    int ret = server->Receive(buffer,1024);
    EXPECT_GT(ret,0);

    if(ret > 0)
    {
        tcphdr *tcp = (tcphdr*) (buffer + sizeof(iphdr));
        EXPECT_EQ(tcp->source_port,htons(client_port));
        EXPECT_EQ(tcp->sequence_number,0);
        EXPECT_EQ(tcp->ack_number,0);
        EXPECT_EQ(tcp->data_offset,5);
        EXPECT_EQ(tcp->syn,1);
    }

    EXPECT_LE(client->Receive(buffer,1024),0);
}
