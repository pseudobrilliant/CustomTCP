#include <stdio.h>
#include "../common/StandardTCP.h"

int main()
{
        StandardTCP *custom = new StandardTCP("127.0.0.1",1348,1024);
        custom->Connect();

        return 0;
}

