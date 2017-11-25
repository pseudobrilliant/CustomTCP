#include <stdio.h>
#include "../common/CustomTCP.h"

int main()
{

	CustomTCP *custom = new CustomTCP("127.0.0.1",1347,1024);
	custom->Connect();
	custom->Handshake("127.0.0.1",1348);

	return 0;
}
