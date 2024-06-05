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
#include <fstream>
class Bitboards
{
private:
    uint64_t boards[12];
    int turn_G = 0;
    uint8_t en_passant = 0;
    bool castling[4] = {true, true, true, true};

    // absolute moves

    std::vector<std::vector<uint8_t>> get_king_prot_lines(int turn);

    std::vector<uint16_t> erase_ilegal_moves(std::vector<uint16_t> moves, std::vector<std::vector<uint8_t>> king_prot_lines, int turn);

    std::vector<uint16_t> erase_ilegal_moves_when_check(std::vector<uint16_t> moves, int turn, std::vector<uint16_t> enemy_moves, std::vector<std::vector<uint8_t>> king_prot_lines);

    std::vector<uint16_t> get_legal_king_moves(int turn, std::vector<uint16_t> enemy_moves, std::vector<std::vector<uint8_t>> prot_lines);

    std::vector<uint16_t> get_legal_casteling_moves(int turn, std::vector<uint16_t> enemy_moves);

    std::vector<uint8_t> get_enpassant_killer_pos(int turn);

public:
    Bitboards();
    ~Bitboards();

    std::vector<uint16_t> get_legal_rook_moves_absolute(int turn);
    std::vector<uint16_t> get_legal_bishop_moves_absolute(int turn);
    std::vector<uint16_t> get_legal_queen_moves_absolute(int turn);
    std::vector<uint16_t> get_legal_knight_moves_absolute(int turn);
    std::vector<uint16_t> get_legal_pawn_moves_absolute(int turn);
    std::vector<uint16_t> get_legal_king_moves_absolute(int turn);

    void write_boards_from_FEN(std::string FEN);
    void copy_state(Bitboards *bitboards);
    bool is_check(std::vector<uint16_t> enemy_moves, int turn);
    void make_move(uint16_t move);
    uint64_t *get_boards();
    int get_turn();
    std::vector<uint16_t> get_legal_moves();

    std::vector<uint16_t> get_absolute_moves();

    uint8_t get_en_passant();

    bool *get_castling();
    void flip_turn() { turn_G = !turn_G; }
    std::string generateFEN() const
    {
        std::string FEN;
        int emptyCount = 0;

        for (int rank = 7; rank >= 0; --rank)
        {
            for (int file = 7; file >= 0; --file)
            {
                int position = rank * 8 + file;
                char piece = '.';
                for (int i = 0; i < 12; ++i)
                {
                    if (boards[i] & (1ULL << position))
                    {
                        static const char *pieceNotation = "PNBRQKpnbrqk";
                        piece = pieceNotation[i];
                        break;
                    }
                }

                if (piece == '.')
                {
                    ++emptyCount;
                }
                else
                {
                    if (emptyCount > 0)
                    {
                        FEN += std::to_string(emptyCount);
                        emptyCount = 0;
                    }
                    FEN += piece;
                }
            }
            if (emptyCount > 0)
            {
                FEN += std::to_string(emptyCount);
                emptyCount = 0;
            }
            if (rank > 0)
                FEN += '/';
        }

        FEN += ' ';
        FEN += (turn_G == 1) ? 'w' : 'b';

        FEN += ' ';
        if (!castling[0] && !castling[1] && !castling[2] && !castling[3])
        {
            FEN += '-';
        }
        else
        {
            if (castling[0])
                FEN += 'K';
            if (castling[1])
                FEN += 'Q';
            if (castling[2])
                FEN += 'k';
            if (castling[3])
                FEN += 'q';
        }

        FEN += ' ';
        if (en_passant >= 0)
        {
            int file = en_passant % 8;
            int rank = 0 + (en_passant / 8); // Korrektur, um den Index in eine Schachbrett-Reihe umzurechnen
            FEN += char('h' - file);
            FEN += std::to_string(rank + 1); // +1, da Schachreihen bei 1 beginnen
        }
        else
        {
            FEN += '-';
        }

        // Hier könnten weitere Informationen wie Halbzug- und Zugnummer hinzugefügt werden
        FEN += " 0 1";

        return FEN;
    }
};

#endif
