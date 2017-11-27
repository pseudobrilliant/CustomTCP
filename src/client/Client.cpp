
#include <stdio.h>
#include "../common/CustomTCP.h"
#include "../common/StandardTCP.h"
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
	string str = "";
	string input = "";
	cout<<"Please provide a file to transmit.\n";
	cin >> input;

	if(input != "")
	{
		str = ReadFile(input.c_str());
	}


	cout<<"Run using TCP or Custom.\n";
	cin >> input;

	if(input == "Custom") {
		CustomTCP *client = new CustomTCP("127.0.0.1", 1347, 512);
		client->Connect(350000);
		if (!client->Handshake("127.0.0.1", 1357, str.size())) {
			cout << "Could not finalize handshake.\n";
			return -1;
		}
		cout << "ACK Received --> Connection Socket #" << client->target_port << "\n";

		client->SendData(str);

		cout << "Any key to exit.";
		cin >> input;
		return 1;
	}
	else
	{
		StandardTCP *client = new StandardTCP("127.0.0.1", 1347, 512);
		client->Connect("127.0.0.1",1357,350000);
		client->Send_Data(str);

		cout << "Any key to exit.";
		cin >> input;
		return 1;
	}
}

