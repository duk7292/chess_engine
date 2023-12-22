#ifndef BITBOARDS_H
#define BITBOARDS_H
#include <cstdint>
#include <string>

class Bitboards
{
private:
    uint64_t boards[12];

public:
    Bitboards(/* args */);
    ~Bitboards();

    void write_boards_from_FEN(std::string FEN);
    void copy_state(Bitboards *bitboards);
    uint64_t *get_boards();
};

#endif
