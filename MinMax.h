#include <vector>
#include <string>
#include <thread>
#include <iostream>
#include <chrono>
#include <atomic>
#include <future>
#include "Bitboards.h"
class MinMax
{
private:
    int evaluate(Bitboards *board);

    int worker(Bitboards *board, int depth, int id);

public:
    uint16_t get_best_move(std::string fen, int time);
};
