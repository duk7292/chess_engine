#ifndef BITBOARDS_H
#define BITBOARDS_H
#include <cstdint>
#include <string>
#include <iostream>
 #include <cstring>
class Bitboards
{
private:
    uint64_t boards[12];
    int turn;

    u_int16_t *get_legal_king_moves();
    

public:
    Bitboards(/* args */);
    ~Bitboards();

    void write_boards_from_FEN(std::string FEN);
    void copy_state(Bitboards *bitboards);
    uint64_t *get_boards();
    int get_turn();
    u_int16_t *get_legal_moves();
};

#endif
