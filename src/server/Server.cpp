#include <stdio.h>
#include "../Common/StandardTCP.h"

int main()
{
        StandardTCP *custom = new StandardTCP("127.0.0.1","127.0.0.1",1348,1347,1024);

        return 0;
}

