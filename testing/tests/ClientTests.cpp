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
    int server_port = 1357;

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
    EXPECT_TRUE(client->Connect(3500));
    EXPECT_TRUE(server->Connect(3500));
}

TEST_F(CustomTCPTestFixture,CreatePacket)
{

    char *packet =new char[1024];
    client->SetTarget("127.0.0.1", server_port);
    client->CreatePacket(packet, 0, 0, 5, 1, 0, 0, 0, 0, nullptr);

    iphdr *ip = (iphdr*)packet;
    tcphdr *tcp = (tcphdr*)(packet + sizeof(iphdr));

    EXPECT_EQ(ip->version,4);
    EXPECT_EQ(ip->saddr,inet_addr(client_ip));
    EXPECT_EQ(ip->daddr,inet_addr(server_ip));

    EXPECT_EQ(tcp->source,htons(client_port));
    EXPECT_EQ(tcp->dest,htons(server_port));
    EXPECT_EQ(tcp->seq, 0);
    EXPECT_EQ(tcp->ack,0);
    EXPECT_EQ(tcp->syn,1);
}

TEST_F(CustomTCPTestFixture,Handshake)
{
    client->Connect(350000);
    server->Connect(350000);

    std::thread t([](CustomTCPTestFixture* fixture){
        std::this_thread::sleep_for(chrono::milliseconds(500));
        int seq_num = fixture->client->sequence_number;
        fixture->client->Handshake("127.0.0.1", fixture->server_port, 0);
        EXPECT_EQ(fixture->client->sequence_number,seq_num +1);
    },this);

    int seq_num = server->sequence_number;
    ASSERT_EQ(server->ProcessMessage(),SYN);
    ASSERT_EQ(server->ProcessMessage(),ACK);
    EXPECT_EQ(server->sequence_number,seq_num +1);
    t.join();

    EXPECT_EQ(server->source_port, server_port);
    EXPECT_EQ(server->target_port, client_port);
    EXPECT_STREQ(server->source_ip, server_ip);
    EXPECT_STREQ(server->target_ip, client_ip);
}

TEST_F(CustomTCPTestFixture,Sending)
{
    client->Connect(3500);
    client->SetTarget("127.0.0.1", server_port);
    char *packet =new char[1024];
    int size = client->CreatePacket(packet, 0, 0, 5, 1, 0, 0, 0, 0, nullptr);
    EXPECT_GE(size,0);
    EXPECT_GT(client->SendPacket(packet,size),0);
}

TEST_F(CustomTCPTestFixture,Receiving)
{
    client->Connect(3500);
    server->Connect(3500);
    client->SetTarget("127.0.0.1", server_port);
    char *packet =new char[1024];
    int size = client->CreatePacket(packet, 0, 0, 5, 1, 0, 0, 0, 0, nullptr);

    EXPECT_GE(size,0);
    ASSERT_TRUE(client->SendPacket(packet,size));

    char *buffer =new char[1024];
    ASSERT_GT(server->Receive(buffer,1024),0);

    tcphdr *tcp = (tcphdr*) (buffer + sizeof(iphdr));
    EXPECT_EQ(tcp->source,htons(client_port));
    EXPECT_EQ(tcp->seq,0);
    EXPECT_EQ(tcp->ack_seq,0);
    EXPECT_EQ(tcp->doff,5);
    EXPECT_EQ(tcp->syn,1);

    ASSERT_LE(client->Receive(buffer,1024),0);
}


TEST_F(CustomTCPTestFixture,SendingData)
{
    client->Connect(3500);
    server->Connect(3500);
    string data = "Hello World!!!";
    std::thread t([](CustomTCPTestFixture* fixture, string data){
        std::this_thread::sleep_for(chrono::milliseconds(500));
        int seq_num = fixture->client->sequence_number;
        fixture->client->Handshake("127.0.0.1", fixture->server_port,data.size());
        EXPECT_EQ(fixture->client->sequence_number,seq_num +1);
        std::this_thread::sleep_for(chrono::milliseconds(800));
        fixture->client->SendData(data);
    },this,data);

    int seq_num = server->sequence_number;
    ASSERT_EQ(server->ProcessMessage(),SYN);
    ASSERT_EQ(server->ProcessMessage(),ACK);
    EXPECT_EQ(server->sequence_number,seq_num +1);

    char *buffer =new char[1024];
    ASSERT_GT(server->Receive(buffer,1024),0);
    tcphdr *tcp = (tcphdr*) (buffer + sizeof(iphdr));
    char* rec_p = (buffer + sizeof(iphdr) + sizeof(tcphdr));
    string rec_data = string(rec_p,ntohs(tcp->window));
    EXPECT_STRCASEEQ(rec_data.c_str(),data.c_str());

    char *packet = new char[1024];
    int size = server->CreatePacket(packet,server->sequence_number,tcp->window,5,0,1,0,0,0, nullptr);
    server->SendPacket(packet,size);

    t.join();
}

TEST_F(CustomTCPTestFixture,ReceivingData)
{
    client->Connect(3500);
    server->Connect(3500);
    string data = "Hello World!!!";
    std::thread t([](CustomTCPTestFixture* fixture, string data){
        std::this_thread::sleep_for(chrono::milliseconds(500));
        int seq_num = fixture->client->sequence_number;
        fixture->client->Handshake("127.0.0.1", fixture->server_port,data.size());
        EXPECT_EQ(fixture->client->sequence_number,seq_num +1);
        std::this_thread::sleep_for(chrono::milliseconds(800));
        fixture->client->SendData(data);
    },this,data);

    int seq_num = server->sequence_number;
    ASSERT_EQ(server->ProcessMessage(),SYN);
    ASSERT_EQ(server->ProcessMessage(),ACK);
    EXPECT_EQ(server->sequence_number,seq_num +1);

    ASSERT_STREQ(server->ReceiveData().c_str(),data.c_str());

    t.join();
}