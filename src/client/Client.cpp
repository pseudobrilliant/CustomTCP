#include <stdio.h>
#include "../common/CustomTCP.h"

int main()
{

	CustomTCP *custom = new CustomTCP("127.0.0.1","127.0.0.1",1347,1348,1024);
	
	return 0;
}
