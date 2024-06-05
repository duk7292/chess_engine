#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <thread>
#include <chrono>
#include <unordered_map>
#include <future>

#ifndef UTILS_H
#define UTILS_H

void testTime()
{

    Bitboards *board = new Bitboards();
    board->write_boards_from_FEN("r1bqk2r/p1pp1ppp/2nb1n2/1pP1p3/P3P1P1/2NBQNP1/P1BP1P2/1R2K2R|K-kq|46|1");

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < 10000; i++)
    {
        board->get_legal_moves();
    }

    auto end = std::chrono::high_resolution_clock::now();

    // Calculate elapsed time in milliseconds
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    std::cout << "Time taken: " << duration << " milliseconds" << std::endl;
}

void printBits(uint16_t num)
{
    for (int bit = (sizeof(uint16_t) * 8) - 1; bit >= 0; bit--)
    {
        if (bit == 5 || bit == 11)
        {
            std::cout << "_";
        }
        std::cout << ((num >> bit) & 1);
    }
    std::cout << std::endl;
}

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

#endif