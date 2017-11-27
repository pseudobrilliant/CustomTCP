#include <thread>
#include "gtest/gtest.h"
#include "StandardTCP.h"

class StandardTCPTestFixture : public testing::Test
{
    public:
    StandardTCP *client;
    StandardTCP *server;
    const char* client_ip = "127.0.0.1";
    const char* server_ip = "127.0.0.1";
    int client_port = 1387;
    int server_port = 1397;

    StandardTCPTestFixture(){}

    void SetUp()
    {
        client = new StandardTCP(client_ip, client_port, 1024);
        server = new StandardTCP(server_ip, server_port, 1024);
    }

    void TearDown()
    {
        client->Disconnect();
        server->Disconnect();
        delete(client);
        delete(server);
    }

    ~StandardTCPTestFixture(){}

};

TEST_F(StandardTCPTestFixture,Connecting)
{
    std::thread t([](StandardTCPTestFixture* fixture){
        std::this_thread::sleep_for(chrono::milliseconds(500));
        EXPECT_TRUE(fixture->client->Connect("127.0.0.1",fixture->server_port,3500));
    },this);

    EXPECT_TRUE(server->Initialize(3500));

    t.join();
}

TEST_F(StandardTCPTestFixture,Sending)
{
    std::thread t([](StandardTCPTestFixture* fixture){
        std::this_thread::sleep_for(chrono::milliseconds(500));
        EXPECT_TRUE(fixture->client->Connect("127.0.0.1",fixture->server_port,3500));
        std::this_thread::sleep_for(chrono::milliseconds(500));
        EXPECT_TRUE(fixture->client->Send_Data("Hello World"));
    },this);

    EXPECT_TRUE(server->Initialize(3500));
    EXPECT_TRUE(server->Receive_Data());

    t.join();
}
