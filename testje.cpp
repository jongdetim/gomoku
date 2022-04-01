#include <iostream>

int main()
{
    for (int n = 0; n < 3; ++n)
    {
        if ((0xC0 << n & 0x80) == 0x80)
            std::cout << "true" << std::endl;
        else
            std::cout << "false" << std::endl;
    }
return 0;
}

bool naam[3] = {true, true, false};