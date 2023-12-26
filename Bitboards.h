#ifndef BITBOARDS_H
#define BITBOARDS_H
#include <cstdint>
#include <string>
#include <iostream>
#include <cstring>
#include <vector>
#include <bitset>
#include <chrono>
class Bitboards
{
private:
    uint64_t boards[12];
    int turn = 0;
    uint8_t en_passant = 0;
    bool castling[4] = {true, true, true, true};
    std::vector<uint16_t> get_legal_rook_moves();
    std::vector<uint16_t> get_legal_bishop_moves();
    std::vector<uint16_t> get_legal_queen_moves();
    std::vector<uint16_t> get_legal_knight_moves();
    std::vector<uint16_t> get_legal_pawn_moves();

public:
    Bitboards();
    ~Bitboards();

    void write_boards_from_FEN(std::string FEN);
    void copy_state(Bitboards *bitboards);
    uint64_t *get_boards();
    int get_turn();
    std::vector<uint16_t> get_legal_moves();
};

#endif
