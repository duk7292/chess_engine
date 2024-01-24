#ifndef BITBOARDS_H
#define BITBOARDS_H
#include <cstdint>
#include <string>
#include <iostream>
#include <cstring>
#include <vector>
#include <bitset>
#include <chrono>
#include <algorithm>
class Bitboards
{
private:
    uint64_t boards[12];
    int turn_G = 0;
    uint8_t en_passant = 0;
    bool castling[4] = {true, true, true, true};

    // absolute moves

    std::vector<uint16_t> get_legal_rook_moves_absolute(int turn);
    std::vector<uint16_t> get_legal_bishop_moves_absolute(int turn);
    std::vector<uint16_t> get_legal_queen_moves_absolute(int turn);
    std::vector<uint16_t> get_legal_knight_moves_absolute(int turn);
    std::vector<uint16_t> get_legal_pawn_moves_absolute(int turn);
    std::vector<uint16_t> get_legal_king_moves_absolute(int turn);

    bool is_check(std::vector<uint16_t> enemy_moves, int turn);

    std::vector<std::vector<uint8_t>> get_king_prot_lines(int turn);

    std::vector<uint16_t> erase_ilegal_moves(std::vector<uint16_t> moves, std::vector<std::vector<uint8_t>> king_prot_lines, int turn);

    std::vector<uint16_t> erase_ilegal_moves_when_check(std::vector<uint16_t> moves, int turn, std::vector<uint16_t> enemy_moves, std::vector<std::vector<uint8_t>> king_prot_lines);

    std::vector<uint16_t> get_legal_king_moves(int turn, std::vector<uint16_t> enemy_moves, std::vector<std::vector<uint8_t>> prot_lines);

    std::vector<uint16_t> get_legal_casteling_moves(int turn, std::vector<uint16_t> enemy_moves);

    std::vector<uint8_t> get_enpassant_killer_pos(int turn);

public:
    Bitboards();
    ~Bitboards();

    void write_boards_from_FEN(std::string FEN);
    void copy_state(Bitboards *bitboards);

    void make_move(uint16_t move);
    uint64_t *get_boards();
    int get_turn();
    std::vector<uint16_t> get_legal_moves();
};

#endif
