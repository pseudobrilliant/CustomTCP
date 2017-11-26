#include <stdio.h>
#include "../common/CustomTCP.h"
#include <iostream>
#include <fstream>
#include <sstream>

string ReadFile(const char* filename)
{
	ifstream file(filename);
	stringstream buffer;

	buffer << file.rdbuf();
	string str = buffer.str();

	return str;
}

int main()
{
	string str = ReadFile("/home/cblythe/Code/TCP-IP2/src/client/sample.txt");
	CustomTCP *client= new CustomTCP("127.0.0.1",1347,512);
	client->Connect(3500);
	if(!client->Handshake("127.0.0.1", 1357, str.size()))
	{
		cout<<"Could not finalize handshake.\n";
		return -1;
	}
	cout<<"ACK Received --> Connection Socket #"<<client->target_port<<"\n";

	client->SendData(str);

	return 0;
}

