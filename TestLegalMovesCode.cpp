#include "TestLegalMovesCode.h"

size_t TestLegalMovesCode::board_hash(const uint64_t *boards)
{
    size_t h = 0;
    for (int i = 0; i < 12; ++i)
    {
        h ^= std::hash<uint64_t>()(boards[i]) + 0x9e3779b9 + (h << 6) + (h >> 2);
    }
    return h;
}

// Vergleichsfunktion für Schachbretter
bool TestLegalMovesCode::boards_equal(const uint64_t *a, const uint64_t *b)
{
    for (int i = 0; i < 12; ++i)
    {
        if (a[i] != b[i])
            return false;
    }
    return true;
}

uint64_t TestLegalMovesCode::perft(Bitboards &board, int depth, std::unordered_map<size_t, int> &history, std::vector<std::string> &positions, int targetDepth)
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

void TestLegalMovesCode::process_fen(const std::string &fen, int maxDepth, int index)
{
    Bitboards board;
    board.write_boards_from_FEN(fen);
    std::unordered_map<size_t, int> history;
    auto start = std::chrono::high_resolution_clock::now(); // Record start time
    // Iterate through each depth up to maxDepth
    for (int depth = 1; depth <= maxDepth; ++depth)
    {
        std::vector<std::string> positions; // Temporary container for positions at this depth
        perft(board, depth, history, positions, depth);

        // Print the count of unique positions at this depth
        if ((int)positions.size() != perftResults[index][depth - 1])
        {
            std::cout << "🔴 Error: " << fen << " depth " << depth << " expected " << perftResults[index][depth - 1] << " got " << positions.size() << std::endl;
        }
    }

    auto end = std::chrono::high_resolution_clock::now(); // Record end time
    std::chrono::duration<double> elapsed = end - start;  // Calculate elapsed time

    std::cout << "Thread " << index << " runtime: " << elapsed.count() << " seconds" << std::endl; // Print index and elapsed time
}

void TestLegalMovesCode::test_legal_moves_code()
{
    std::vector<std::thread> threads;

    for (size_t i = 0; i < fensString.size(); ++i)
    {
        std::string fen = fensString[i];
        int maxDepth = perftResults[i].size();

        threads.emplace_back(&TestLegalMovesCode::process_fen, this, fen, maxDepth, i);
    }

    // Wait for all threads to finish
    for (auto &thread : threads)
    {
        thread.join();
    }
}
