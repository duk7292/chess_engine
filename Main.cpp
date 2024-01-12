#include <iostream>
#include "Main.h"
#include <fstream>
#include <string>
#include "Bitboards.h"

void printArray(const uint64_t *arr, size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;
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
// int main()
// {
//     Bitboards *bitboards = new Bitboards();
//     bitboards->write_boards_from_FEN("1b2r1k1/1p1bRpp1/p4n2/3p3p/2qN1N1P/P1P2PP1/1P1Q1B2/R5K1|----|--|0");
//     std::vector<uint16_t> legalMoves = bitboards->get_legal_moves();
//     // printArray(bitboards->get_boards(), 12);
//     for (const auto &move : legalMoves)
//     {
//         std::cout << decodeMove(move) << " ";
//     }
//     return 0;
// }

int main()
{
    std::ifstream infile("custom_fen_file.txt");
    std::ofstream outfile("custom_moves_file.txt");
    std::string line;

    int a = 1;

    while (std::getline(infile, line))
    {

        Bitboards *bitboards = new Bitboards();
        bitboards->write_boards_from_FEN(line);

        // Hier rufen Sie die Funktion auf, die die legalen Züge zurückgibt.
        // Angenommen, get_legal_moves() gibt einen std::vector<uint16_t> zurück

        std::vector<uint16_t> legalMoves = bitboards->get_legal_moves();
        outfile << line << " ";
        for (const auto &move : legalMoves)
        {
            outfile << decodeMove(move) << " ";
        }
        outfile << std::endl;
        delete bitboards;
    }

    return 0;
}
