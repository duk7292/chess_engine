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
    bitboards->write_boards_from_FEN("8/8/8/2P1p3/3K4/8/8/8|kq|KQ|--|1");
    bitboards->get_legal_moves();
    // printArray(bitboards->get_boards(), 12);

    return 0;
}
