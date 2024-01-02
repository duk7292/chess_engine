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
    bitboards->write_boards_from_FEN("8/8/rP1K1P1q/2P1B3/3p4/b5p1/7q/8|kq|KQ|--|1");
    bitboards->get_legal_moves();
    // printArray(bitboards->get_boards(), 12);

    return 0;
}
