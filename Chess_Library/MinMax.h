#include <vector>
#include <string>
#include <thread>
#include <iostream>
#include <chrono>
#include <atomic>
#include <future>
#include <algorithm>
#include <numeric>
#include "Bitboards.h"

class MinMax
{
private:
    int evaluate(Bitboards *board, std::vector<uint16_t> moves, std::vector<uint16_t> moves_before);

    void worker(int depth, bool maximizingPlayer, std::vector<uint16_t> moves_before);

    int minMax(Bitboards *board, int depth, bool isMaximizingPlayer, int alpha, int beta, std::vector<uint16_t> moves_before);

public:
    uint16_t get_best_move(std::string fen, int time);
};
