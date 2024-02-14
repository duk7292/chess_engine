
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
void printArray(const uint64_t *arr, size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        std::cout << arr[i] << " ";
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
bool make_moves_unit_test()
{
    std::vector<uint16_t> Moves = {1025, 1383, 773, 708, 706, 1804, 1153, 67, 7793, 327, 2618};
    std::vector<std::string> FENs_before = {
        "rnbqk2r/pp1pppbp/2p2np1/8/2P5/4P1P1/PP1P1PBP/RNBQK1NR|KQkq|--|1",
        "rnb1kb1r/pp2pppp/3p1n2/q1p5/4P3/2P2B2/PP1P1PPP/RNBQK1NR|KQkq|--|0",
        "r1bqkbnr/pppn2pp/3p4/4pp2/3P4/2N1P2P/PPP2PP1/R1BQKBNR|KQkq|--|1",
        "r1bqkbnr/pppn2pp/3p4/4pp2/3P4/2N1P2P/PPPB1PP1/R2QKBNR|KQkq|--|1",
        "rnbqk1nr/pp2bppp/3p4/2p1p3/4P3/2N2N2/PPPP1PPP/R1BQKB1R|KQkq|--|1",
        "r1bqkbnr/1pp2ppp/p1np4/4p3/4P3/2N3P1/PPPP1P1P/R1BQKBNR|KQkq|--|1",
        "r2qkbnr/pbpp1ppp/1pn1p3/8/3PP3/3Q4/PPP2PPP/RNB1KBNR|KQkq|--|1",
        "rn1qkb1r/p2pppp1/bp3n1p/2p5/2P5/4PNP1/PP1PBP1P/RNBQK2R|KQkq|--|1",
        "rnbqk2r/pp4Pp/1bp5/8/3PP3/PPPB1N1P/1N2Q1B1/R3K2R|KQkq|--|1",
        "r2qkbnr/pp1npppp/2p5/3p1bB1/3P4/5N2/PPPNPPPP/R2QKB1R|KQkq|--|1",
        "rnb1kb1r/pp2pp1p/3p1np1/q1p5/4P3/2P2BP1/PP1P2PP/RNBQK1NR|KQkq|--|0"};
    std::vector<std::string> FENs_after = {
        "rnbqk2r/pp1pppbp/2p2np1/8/2P5/4P1PN/PP1P1PBP/RNBQK2R|KQkq|--|0",
        "rnb1kb1r/pp2pppp/3p1n2/2p5/4P3/2q2B2/PP1P1PPP/RNBQK1NR|KQkq|--|1",
        "r1bqkbnr/pppn2pp/3p4/4pp2/3P4/2N1P2P/PPPB1PP1/R2QKBNR|KQkq|--|0",
        "r1bqkbnr/pppn2pp/3p4/4pp2/3P4/2N1P2P/PPPBQPP1/R3KBNR|KQkq|--|0",
        "rnbqk1nr/pp2bppp/3p4/2p1p3/4P3/2N2N2/PPPPBPPP/R1BQK2R|KQkq|--|0",
        "r1bqkbnr/1pp2ppp/p1np4/4p3/3PP3/2N3P1/PPP2P1P/R1BQKBNR|KQkq|20|0",
        "r2qkbnr/pbpp1ppp/1pn1p3/8/3PP3/3Q1N2/PPP2PPP/RNB1KB1R|KQkq|--|0",
        "rn1qkb1r/p2pppp1/bp3n1p/2p5/2P5/4PNP1/PP1PBP1P/RNBQ1RK1|--kq|--|0",
        "rnbqk1Nr/pp5p/1bp5/8/3PP3/PPPB1N1P/1N2Q1B1/R3K2R|KQkq|--|0",
        "r2qkbnr/pp1npppp/2p5/3p1bB1/3P4/5N2/PPPNPPPP/2RQKB1R|K-kq|--|0",
        "rnb1k2r/pp2pp1p/3p1npb/q1p5/4P3/2P2BP1/PP1P2PP/RNBQK1NR|KQkq|--|1"};
    bool result = true;
    for (size_t i = 0; i < FENs_before.size(); i++)
    {
        const auto &FEN_b = FENs_before[i];
        const auto &FEN_a = FENs_after[i];
        Bitboards *bitboards_org = new Bitboards();
        bitboards_org->write_boards_from_FEN(FEN_b);
        Bitboards *bitboards = new Bitboards();
        bitboards->write_boards_from_FEN(FEN_a);
        bitboards_org->make_move(Moves[i]);
        std::vector<uint16_t> legalMoves_org = bitboards_org->get_legal_moves();
        std::vector<uint16_t> legalMoves = bitboards->get_legal_moves();
        bool test_res = true;
        if (legalMoves.size() != legalMoves_org.size())
        {
            result = false;
            test_res = false;
            std::cout << " 🔴 "
                      << "test " << (int)i << " failed" << std::endl;
        }
        else
        {
            for (size_t j = 0; j < legalMoves.size(); j++)
            {
                if (legalMoves[j] != legalMoves_org[j])
                {
                    result = false;
                    test_res = false;
                    std::cout << " 🔴 "
                              << "test " << (int)i << " failed" << std::endl;
                    break;
                }
            }
        }

        if (test_res)
        {
            std::cout << " 🟢 "
                      << "test " << (int)i << " passed" << std::endl;
        }
    }
    return result;
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

// int main()
// {
//     Bitboards *bitboards = new Bitboards();
//     bitboards->write_boards_from_FEN("8/2p5/3p4/KP5r/1R2Pp1k/8/6P1/8|----|19|0");
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
    std::cout << "Perft Position Writer" << std::endl;
    std::string fen = "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R|KQ--|--|1";
    int depth = 5; // oder jede gewünschte Tiefe
    run_perft_test_and_print_node_counts(fen, depth);
    return 0;
}
// E.P DisC DoubC
// void printBitboard(uint64_t bitboard)
// {
//     for (int row = 0; row < 8; ++row)
//     {
//         for (int col = 0; col < 8; ++col)
//         {
//             // Berechnen Sie die Position im Bitboard
//             int position = row * 8 + col;

//             // Prüfen Sie, ob das Bit an dieser Position gesetzt ist
//             if (bitboard & (1ULL << position))
//             {
//                 std::cout << "1 ";
//             }
//             else
//             {
//                 std::cout << ". ";
//             }
//         }
//         std::cout << std::endl;
//     }
// }

// int main()
// {
//     Bitboards board;
//     board.write_boards_from_FEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR|KQkq|--|1");
//     for (int i = 0; i < 4; i++)
//     {
//         std::string input;
//         std::getline(std::cin, input);
//         board.make_move(encodeMove(input));
//         printBitboard(board.get_boards()[0]);
//         std::cout << std::endl;
//         printBitboard(board.get_boards()[6]);
//         std::cout << (int)board.get_en_passant() << std::endl;
//     }
// }