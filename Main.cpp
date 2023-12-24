#include <iostream>
#include "Main.h"
#include "Bitboards.h"

void printArray(const uint64_t *arr, size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;
}

int main()
{
    Bitboards *bitboards = new Bitboards();
    bitboards->write_boards_from_FEN("r1bq1rk1/ppp2ppp/2nbpn2/3p4/3P4/1N1B1PPP/PPPQ4/R2BK2R|1");
    bitboards->get_legal_moves();
    // printArray(bitboards->get_boards(), 12);

    return 0;
}
