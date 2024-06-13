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
    int evaluate(Bitboards *board, std::vector<uint16_t> moves);

    void worker(int depth, bool maximizingPlayer);

    int minMax(Bitboards *board,int index, int depth, int max_depth, bool isMaximizingPlayer, int alpha, int beta);

    void order_Moves();

public:
    uint16_t get_best_move(std::string fen, int time);
};
