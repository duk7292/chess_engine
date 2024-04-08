
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <thread>
#include <chrono>
#include <unordered_map>
#include <future>
#include "Chess_Library/Bitboards.h"
#include "Chess_Library/MinMax.h"
#include "utils.h"

int main()
{

    for (int i = 0; i < 4272; i++)
    {

        uint16_t a = CNNumberToMove(i);

        int b = MoveToCNNumber(a);

        if (b == i)
        {
            std::cout << "correct at " << i << " " << decodeMove(a) << std::endl;
        }
        else
        {
            std::cout << "false at " << i - 4096 << " " << decodeMove(a) << std::endl;
        }
    }
    return 0;
}
