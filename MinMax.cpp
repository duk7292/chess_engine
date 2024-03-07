#include "MinMax.h"

std::vector<Bitboards> depth_1_boards;
std::vector<int> scores_depth_1;
int depth_1_done = 0;
int depth_1_start = 0;
std::atomic<bool> should_continue = true;
std::mutex score_mutex;
std::mutex boards_mutex;

void timer_thread(int time_limit)
{
    std::this_thread::sleep_for(std::chrono::seconds(time_limit));
    should_continue = false;
    std::cout << "Time is up!" << std::endl;
}
int MinMax::evaluate(Bitboards *board)
{
    uint64_t *boards = board->get_boards();

    // Piece values
    const int pawnValue = 1;
    const int knightValue = 3;
    const int bishopValue = 3;
    const int rookValue = 5;
    const int queenValue = 9;
    const int kingValue = 0; // King's value is not included because its presence is mandatory

    // Piece values array
    const int pieceValues[6] = {pawnValue, knightValue, bishopValue, rookValue, queenValue, kingValue};

    int score = 0;

    for (int i = 0; i < 12; i++)
    {
        for (int j = 0; j < 64; j++)
        {
            if (boards[i] & (1ULL << j))
            {
                // Determine the piece type (0-5 for white, 6-11 for black)
                int pieceType = i % 6;

                // Add or subtract the value of the piece
                if (i < 6)
                {
                    score += pieceValues[pieceType];
                }
                else
                {
                    score -= pieceValues[pieceType];
                }
            }
        }
    }
    return score;
}

uint16_t MinMax::get_best_move(std::string fen, int time)
{

    std::thread t(timer_thread, time - 1);

    uint16_t bestMove = 0;
    Bitboards board;

    board.write_boards_from_FEN(fen);

    std::vector<uint16_t> moves = board.get_legal_moves();

    for (uint16_t move : moves)
    {
        Bitboards newBoard;
        newBoard.copy_state(&board);
        newBoard.make_move(move);
        depth_1_boards.push_back(newBoard);
        scores_depth_1.push_back(evaluate(&newBoard));
    }

    // order moves by score :: Later

    int hardware_threads = std::thread::hardware_concurrency();
    std::vector<std::thread> threads;
    int depth = 0;
    while (should_continue)
    {

        if (depth_1_start == 0)
        {
            depth++;
            for (int i = 0; i < hardware_threads; ++i)
            {
                threads.emplace_back(&MinMax::worker, this, depth, board.get_turn() == 0);
            }
        }

        if (depth_1_done >= (int)depth_1_boards.size())
        {
            depth_1_done = 0;
            depth_1_start = 0;
            for (auto &thread : threads)
            {
                if (thread.joinable())
                {
                    thread.join();
                }
            }
            // order moves by last score
            // bool sortAscending = board.get_turn() == 0;

            // std::vector<size_t> indices(scores_depth_1.size());
            // std::iota(indices.begin(), indices.end(), 0);

            // std::sort(indices.begin(), indices.end(), [&](size_t i, size_t j)
            //           { return sortAscending ? scores_depth_1[i] < scores_depth_1[j] : scores_depth_1[i] > scores_depth_1[j]; });

            // std::vector<Bitboards> sortedBoards(depth_1_boards.size());
            // std::vector<int> sortedScores(scores_depth_1.size());

            // for (size_t i = 0; i < indices.size(); ++i)
            // {
            //     sortedBoards[i] = depth_1_boards[indices[i]];
            //     sortedScores[i] = scores_depth_1[indices[i]];
            // }

            // std::swap(depth_1_boards, sortedBoards);
            // std::swap(scores_depth_1, sortedScores);
        }
    }
    std::cout << "Depth: " << depth << std::endl;

    for (auto &thread : threads)
    {
        if (thread.joinable())
        {
            thread.join();
        }
    }
    t.join();

    return bestMove;
}

void MinMax::worker(int depth, bool maximizingPlayer)
{
    try
    {

        while (true)
        {
            int index;
            {
                std::lock_guard<std::mutex> lock(boards_mutex);
                if (depth_1_start >= (int)depth_1_boards.size())
                    break;

                index = depth_1_start;
                depth_1_start++;
            }

            if (index < (int)depth_1_boards.size())
            {

                int score = minMax(&depth_1_boards[index], depth, maximizingPlayer, -100000, 100000);

                {
                    std::lock_guard<std::mutex> score_lock(score_mutex);
                    scores_depth_1[index] = score;
                    depth_1_done++;
                }
            }
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}

int MinMax::minMax(Bitboards *board, int depth, bool isMaximizingPlayer, int alpha, int beta)
{
    std::vector<uint16_t> moves = board->get_legal_moves();

    if (depth == 0 || moves.empty() || !should_continue)
    {
        return evaluate(board);
    }

    if (isMaximizingPlayer)
    {
        int bestValue = -1000;
        for (uint16_t move : moves)
        {
            Bitboards newBoard;
            newBoard.copy_state(board);
            newBoard.make_move(move);
            int value = minMax(&newBoard, depth - 1, false, alpha, beta);
            bestValue = std::max(bestValue, value);
            alpha = std::max(alpha, bestValue);
            if (beta <= alpha)
                break; // Beta cut-off
        }
        return bestValue;
    }
    else
    {
        int bestValue = 1000;
        for (uint16_t move : moves)
        {
            Bitboards newBoard;
            newBoard.copy_state(board);
            newBoard.make_move(move);
            int value = minMax(&newBoard, depth - 1, true, alpha, beta);
            bestValue = std::min(bestValue, value);
            beta = std::min(beta, bestValue);
            if (beta <= alpha)
                break; // Alpha cut-off
        }
        return bestValue;
    }
}
