#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <thread>
#include <chrono>
#include <unordered_map>
#include <future>
#include "Main.h"
#include "Bitboards.h"
class TestLegalMovesCode
{
public:
    std::vector<std::string> fensString = {
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR|KQkq|--|1",
        "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R|KQkq|--|1",
        "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8|----|--|1",
        "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1|--kq|--|1",
        "r2q1rk1/pP1p2pp/Q4n2/bbp1p3/Np6/1B3NBn/pPPP1PPP/R3K2R|KQ--|--|0",
        "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R|KQ--|--|1",
        "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1|----|--|1",
    };
    std::vector<std::vector<int>> perftResults = {
        {20, 400, 8902, 197281, 4865609},
        {48, 2039, 97862, 4085603},
        {14, 191, 2812, 43238, 674624, 11030083},
        {6, 264, 9467, 422333, 15833292},
        {6, 264, 9467, 422333, 15833292},
        {44, 1486, 62379, 2103487},
        {46, 2079, 89890, 3894594}};

    void test_legal_moves_code();

private:
    size_t board_hash(const uint64_t *boards);
    bool boards_equal(const uint64_t *a, const uint64_t *b);
    uint64_t perft(Bitboards &board, int depth, std::unordered_map<size_t, int> &history, std::vector<std::string> &positions, int targetDepth);
    void process_fen(const std::string &fen, int maxDepth, int index);
};