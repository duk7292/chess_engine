#include "MinMax.h"

void timer_thread(int time_limit, std::atomic<bool> &should_continue)
{
    std::this_thread::sleep_for(std::chrono::seconds(time_limit));
    should_continue = false;
}
int MinMax::evaluate(Bitboards *board)
{
    uint64_t *boards = board->get_boards();

    int score = 0;

    for (int i = 0; i < 12; i++)
    {
        for (int j = 0; j < 64; j++)
        {
            if (boards[i] & (1ULL << j))
            {
                if (i < 6)
                {
                    score += 1;
                }
                else
                {
                    score -= 1;
                }
            }
        }
    }
    return score;
}
uint16_t MinMax::get_best_move(std::string fen, int time)
{
    std::atomic<bool> should_continue = true;
    std::thread t(timer_thread, time, std::ref(should_continue));
    uint16_t bestMove = 0;

    Bitboards board;

    board.write_boards_from_FEN(fen);

    std::vector<uint16_t> moves = board.get_legal_moves();

    std::vector<Bitboards> depth_1_boards;
    std::vector<int> scores_depth_1;
    std::vector<bool> is_depth_1_done;
    for (uint16_t move : moves)
    {
        Bitboards newBoard;
        newBoard.copy_state(&board);
        newBoard.make_move(move);
        depth_1_boards.push_back(newBoard);
        scores_depth_1.push_back(evaluate(&newBoard));
        is_depth_1_done.push_back(false);
    }
    // order moves by score :: Later

    int hardware_threads = std::thread::hardware_concurrency();
    std::vector<std::atomic<bool>> thread_availability(hardware_threads);

    for (auto &thread : thread_availability)
    {
        thread = true;
    }
    t.join();
    return bestMove;
}