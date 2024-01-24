
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <thread>
#include <chrono>
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
//     bitboards->write_boards_from_FEN("2q5/3P4/8/8/8/7K/8/8|----|--|1");
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
    std::ifstream infile("FEN-tests/Custom/FEN-in/custom_fen_file_25M.txt");
    std::ofstream outfile("FEN-tests/Custom/Moves-out/custom_moves_file_25M.txt");
    std::string line;

    auto start = std::chrono::high_resolution_clock::now(); // Start measuring time

    while (std::getline(infile, line))
    {
        Bitboards *bitboards = new Bitboards();
        bitboards->write_boards_from_FEN(line);

        std::vector<uint16_t> legalMoves = bitboards->get_legal_moves();

        for (const auto &move : legalMoves)
        {
            outfile << decodeMove(move) << " ";
        }
        outfile << std::endl;
        delete bitboards;
    }

    auto end = std::chrono::high_resolution_clock::now();                               // Stop measuring time
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start); // Calculate duration in milliseconds

    std::cout << "Time taken: " << duration.count() << " milliseconds" << std::endl;

    return 0;
}

// void process_fen_strings(const std::vector<std::string> &fenStrings, size_t start, size_t end)
// {
//     Bitboards *bitboards = new Bitboards(); // Erstellen des Bitboards-Objekts einmal pro Thread

//     for (size_t i = start; i < end; ++i)
//     {
//         bitboards->write_boards_from_FEN(fenStrings[i]);
//         std::vector<uint16_t> legalMoves = bitboards->get_legal_moves();
//         // Hier könnten Sie die legalMoves verwenden, falls nötig
//     }

//     delete bitboards; // Löschen des Bitboards-Objekts am Ende der Thread-Ausführung
// }

// int main()
// {
//     std::ifstream infile("FEN-tests/Custom/FEN-in/custom_fen_file-500k.txt");
//     std::string line;
//     std::vector<std::string> fenStrings;

//     while (std::getline(infile, line))
//     {
//         fenStrings.push_back(line);
//     }
//     std::cout << "FEN-Strings loaded" << std::endl;

//     auto start = std::chrono::high_resolution_clock::now();

//     const size_t num_threads = std::thread::hardware_concurrency();
//     std::vector<std::thread> threads(num_threads);

//     size_t chunk_size = fenStrings.size() / num_threads;
//     for (size_t i = 0; i < num_threads; ++i)
//     {
//         size_t start_index = i * chunk_size;
//         size_t end_index = (i + 1 == num_threads) ? fenStrings.size() : start_index + chunk_size;

//         threads[i] = std::thread(process_fen_strings, std::ref(fenStrings), start_index, end_index);
//     }

//     for (auto &thread : threads)
//     {
//         thread.join();
//     }

//     auto end = std::chrono::high_resolution_clock::now();
//     auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

//     std::cout << "Time taken: " << duration.count() << " milliseconds" << std::endl;

//     return 0;
// }