
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
#include "TestLegalMovesCode.h"
#include "MinMax.h"
uint16_t encodeMove(const std::string &move)
{
    // Extrahieren der Start- und Zielpositionen aus der Notation
    int startFile = 'h' - move[0];
    int startRank = move[1] - '1';
    int endFile = 'h' - move[2];
    int endRank = move[3] - '1';

    int start = startRank * 8 + startFile;
    int end = endRank * 8 + endFile;

    bool isPromotion = move.length() > 4;
    int promotionType = 0; // Standardwert, falls keine Promotion

    if (isPromotion)
    {
        // Bestimmen des Promotion-Typs basierend auf dem Buchstaben
        char promotionPiece = move[6];
        switch (promotionPiece)
        {
        case 'N':
            promotionType = 0;
            break;
        case 'B':
            promotionType = 1;
            break;
        case 'R':
            promotionType = 2;
            break;
        case 'Q':
            promotionType = 3;
            break;
        }
    }

    // Zusammensetzen des Zuges in uint16_t
    uint16_t encodedMove = start;
    encodedMove |= (end << 6);
    if (isPromotion)
    {
        encodedMove |= (1 << 12);
        encodedMove |= (promotionType << 13);
    }

    return encodedMove;
}

std::string decodeMove(uint16_t move)
{
    // Extrahieren der Start- und Zielpositionen
    int start = move & 0x3F;                // Die ersten 6 Bits
    int end = (move >> 6) & 0x3F;           // Die nächsten 6 Bits
    bool isPromotion = (move >> 12) & 0x1;  // Das 13. Bit für Pawn Promotion
    int promotionType = (move >> 13) & 0x3; // Die Bits 14 und 15 für den Typ der Promotion

    // Umwandeln der Positionen in Schachnotation (h1, a8, etc.)
    std::string startFile = std::string(1, 'h' - (start % 8));
    std::string startRank = std::string(1, '1' + (start / 8));

    std::string endFile = std::string(1, 'h' - (end % 8));
    std::string endRank = std::string(1, '1' + (end / 8));
    std::string promotionPiece;
    if (isPromotion)
    {
        switch (promotionType)
        {
        case 0:
            promotionPiece = "N";
            break; // Springer
        case 1:
            promotionPiece = "B";
            break; // Läufer
        case 2:
            promotionPiece = "R";
            break; // Turm
        default:
            promotionPiece = "Q";
            break; // Dame
        }
    }

    // Zusammensetzen des Zuges
    std::string decodedMove = startFile + startRank + endFile + endRank;

    // Hinzufügen der Bauernumwandlung, falls vorhanden
    if (isPromotion)
    {
        decodedMove += "=" + promotionPiece;
    }

    return decodedMove;
}

// Eine einfache Hash-Funktion für ein Schachbrett
size_t board_hash(const uint64_t *boards)
{
    size_t h = 0;
    for (int i = 0; i < 12; ++i)
    {
        h ^= std::hash<uint64_t>()(boards[i]) + 0x9e3779b9 + (h << 6) + (h >> 2);
    }
    return h;
}

// Vergleichsfunktion für Schachbretter
bool boards_equal(const uint64_t *a, const uint64_t *b)
{
    for (int i = 0; i < 12; ++i)
    {
        if (a[i] != b[i])
            return false;
    }
    return true;
}

uint64_t perft(Bitboards &board, int depth, std::unordered_map<size_t, int> &history, std::vector<std::string> &positions, int targetDepth)
{
    if (depth == 0)
    {
        if (targetDepth == 0)
        {
            positions.push_back(board.generateFEN());
        }
        return 1;
    }

    uint64_t nodes = 0;
    std::vector<uint16_t> moves = board.get_legal_moves();

    for (uint16_t move : moves)
    {
        Bitboards newBoard;
        newBoard.copy_state(&board);
        newBoard.make_move(move);

        size_t hash = board_hash(newBoard.get_boards());
        if (history[hash] == 2)
        {
            continue;
        }

        history[hash]++;
        nodes += perft(newBoard, depth - 1, history, positions, targetDepth - 1);
        history[hash]--;
    }

    return nodes;
}
void run_perft_test_and_write_positions(const std::string &fen, int depth)
{
    Bitboards board;
    board.write_boards_from_FEN(fen);
    std::unordered_map<size_t, int> history;
    std::vector<std::string> positions;

    perft(board, depth, history, positions, depth);

    // Datei schreiben
    std::ofstream file("perft_positions_depth_" + std::to_string(depth) + ".txt");
    for (const std::string &position : positions)
    {
        file << position << std::endl;
    }
    file.close();

    std::cout << "Tiefe " << depth << ": " << positions.size() << " einzigartige Positionen geschrieben." << std::endl;
}
// Eine Funktion, um die Perft-Tests auszuführen und Positionen in eine Datei zu schreiben
void run_perft_test_and_print_node_counts(const std::string &fen, int maxDepth)
{
    Bitboards board;
    board.write_boards_from_FEN(fen);
    std::unordered_map<size_t, int> history;

    // Iterate through each depth up to maxDepth
    for (int depth = 1; depth <= maxDepth; ++depth)
    {
        std::vector<std::string> positions; // Temporary container for positions at this depth
        perft(board, depth, history, positions, depth);

        // Print the count of unique positions at this depth
        std::cout << "Depth " << depth << ": " << positions.size() << " unique positions." << std::endl;
    }
}
void printBits(uint16_t value)
{
    for (int i = 15; i >= 0; --i)
    { // Start from the most significant bit
        if (i % 3 == 2)
        {
            std::cout << ".";
        }
        if (value & (1 << i))
        { // If the ith bit is set
            std::cout << "1";
        }
        else
        {
            std::cout << "0";
        }
    }
    std::cout << std::endl; // New line after printing all bits
}
int CNNumberToMove(int number)
{
    uint16_t move = 0;
    number += 4096;
    if (number < 4096)
    {
        move = number;
        return move;
    }

    number -= 4096;
    int color = number / 88;

    number %= 88;
    int promotion = number % 4;
    int start_y = color ? 15 : 55;
    int end_y = color ? 7 : 63;
    // normal Promotion
    if (number < 32)
    {

        int x_offset = number / 4;

        move |= ((end_y - x_offset) << 6);
        move |= (start_y - x_offset);
        move |= (1 << 12);
        move |= (promotion << 13);
        return move;
    }
    number -= 32;
    if (number < 48)
    {
        int x_offset_start = 1 + number / 8;
        int x_offset_end = x_offset_start + ((number % 2 == 0) ? 1 : -1);
        move |= ((end_y - x_offset_end) << 6);
        move |= (start_y - x_offset_start);
        move |= (1 << 12);
        move |= (promotion << 13);
        return move;
    }
    number -= 48;
    if (number < 4)
    {

        move |= ((end_y - 1) << 6);
        move |= (start_y);
        move |= (1 << 12);
        move |= (promotion << 13);
        return move;
    }
    move |= ((end_y - 6) << 6);
    move |= (start_y - 7);
    move |= (1 << 12);
    move |= (promotion << 13);
    return move;
}

int main()
{

    // MinMax *minMax = new MinMax();
    // uint16_t bestMove;

    // bestMove = minMax->get_best_move("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR|KQkq|--|1", 10);

    // std::cout << "Best Move: " << decodeMove(bestMove) << std::endl;

    std::cout << decodeMove(CNNumberToMove(32)) << std::endl;

    return 0;
}

// {
//     Bitboards *bitboards = new Bitboards();
//     bitboards->write_boards_from_FEN("8/8/8/KPpp3r/1R2Pp1k/8/6P1/8|----|45|1");
//     std::vector<uint16_t> legalMoves = bitboards->get_legal_moves();
//     // printArray(bitboards->get_boards(), 12);
//     for (const auto &move : legalMoves)
//     {
//         std::cout << decodeMove(move) << " ";
//     }
//     return 0;
// }
// int main()
// {
//     std::cout << "Perft Position Writer" << std::endl;
//     std::string fen = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R|KQkq|--|1";
//     int depth = 7; // oder jede gewünschte Tiefe
//     run_perft_test_and_print_node_counts(fen, depth);
//     return 0;
// }
