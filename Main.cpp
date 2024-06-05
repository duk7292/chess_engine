
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <thread>
#include <chrono>
#include <unordered_map>
#include <future>
#include "../Chess_Library/Bitboards.h"
#include "../Chess_Library/MinMax.h"
#include "../utils.h"

int main()
{
    MinMax *minMax = new MinMax();

    uint16_t bestMove;

    bestMove = minMax->get_best_move("rnbqkb1r/ppp2ppp/5n2/3p4/8/2N2N2/PPPP1PPP/R1BQKB1R|----|--|1", 10);

    std::cout << "Best Move: " << decodeMove(bestMove) << std::endl;

    // Bitboards *board = new Bitboards();
    // board->write_boards_from_FEN("1Q3k2/1R6/8/4r1p1/p3p3/2B5/P4PPP/4K1R1|----|--|0");
    // std::vector<uint16_t> moves = board->get_legal_moves();

    // for (auto move : moves)
    // {
    //     std::cout << "Move: " << decodeMove(move) << " " << std::endl;
    //     printBits(move);
    // }
    //  testTime();

    return 0;
}
