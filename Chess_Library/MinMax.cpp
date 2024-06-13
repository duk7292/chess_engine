#include "MinMax.h"

std::vector<Bitboards> depth_1_boards;
std::vector<int> scores_depth_1;
std::vector<std::vector<int>> scores_depth_2;
std::vector<std::vector<uint16_t>> moves_depth_2;
int depth_1_done = 0;
int depth_1_start = 0;
std::atomic<bool> should_continue = true;
std::mutex score_mutex;
std::mutex boards_mutex;
std::string decode_Move(uint16_t move)
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
void timer_thread(int time_limit)
{
    std::this_thread::sleep_for(std::chrono::seconds(time_limit));
    should_continue = false;
    std::cout << "Time is up!" << std::endl;
}
void MinMax::order_Moves(){

    for (size_t depth = 0; depth < scores_depth_2.size(); ++depth) {
    // Create a vector of pairs (score, move) for the current depth level
    std::vector<std::pair<int, uint16_t>> paired;

    for (size_t i = 0; i < scores_depth_2[depth].size(); ++i) {
        paired.emplace_back(scores_depth_2[depth][i], moves_depth_2[depth][i]);
    }

    // Sort the pairs based on scores for the current depth level
    std::sort(paired.begin(), paired.end(), [](const std::pair<int, uint16_t>& a, const std::pair<int, uint16_t>& b) {
        return a.first > b.first; // Sort in descending order of scores
    });

    // Unpack the sorted pairs back into scores and moves for the current depth level
    for (size_t i = 0; i < paired.size(); ++i) {
        scores_depth_2[depth][i] = paired[i].first;
        moves_depth_2[depth][i] = paired[i].second;
    }
    }

}
int MinMax::evaluate(Bitboards *board, std::vector<uint16_t> moves)
{
    int turn = board->get_turn();

    uint64_t *boards = board->get_boards();

    // Piece values
    const int pawnValue = 1;
    const int knightValue = 3;
    const int bishopValue = 3;
    const int rookValue = 5;
    const int queenValue = 15;
    const int kingValue = 200;

    const int killMoveValue = 5;

    const int pieceValueMultiplier = 10;
    // Piece values array
    const int pieceValues[6] = {pawnValue, knightValue, bishopValue, rookValue, queenValue, kingValue};

    std::vector<std::vector<int>> positionMatrix = {
        { 0, 1, 2, 3, 3, 2, 1, 0 },
        { 1, 2, 3, 4, 4, 3, 2, 1 },
        { 2, 3, 4, 5, 5, 4, 3, 2 },
        { 3, 4, 5, 6, 6, 5, 4, 3 },
        { 3, 4, 5, 6, 6, 5, 4, 3 },
        { 2, 3, 4, 5, 5, 4, 3, 2 },
        { 1, 2, 3, 4, 4, 3, 2, 1 },
        { 0, 1, 2, 3, 3, 2, 1, 0 }
    };

    int score = 0;

    if (moves.empty())
    {

        board->flip_turn();

        std::vector<uint16_t> e_moves = board->get_absolute_moves();

        if (board->is_check(e_moves, turn))
        {

            if (turn == 0)
            {
                return 100000;
            }
            else
            {
                return -100000;
            }
        }
        else
        {
            return 0;
        }
    }
    for (int i = 0; i < 12; i++) {
        for (int j = 0; j < 64; j++) {
            if (boards[i] & (1ULL << j)) {
                // Determine the piece type (0-5 for white, 6-11 for black)
                int pieceType = i % 6;

                // Calculate the row and column from the 0-63 index
                int row = j / 8;
                int col = j % 8;

                // Add or subtract the value of the piece and its positional value
                if (i < 6) {
                    score += pieceValues[pieceType] * pieceValueMultiplier;
                    score += positionMatrix[row][col] * 2.5; // Add positional value for white pieces
                } else {
                    score -= pieceValues[pieceType] * pieceValueMultiplier;
                    score -= positionMatrix[row][col] * 2.5; // Subtract positional value for black pieces
                }
            }
        }
    }


    for (uint16_t move : moves)
    {
        if ((move & (1 << 15)) == 0)
        {
            if (turn == 0)
            {

                score -= killMoveValue;
            }
            else
            {
                score += killMoveValue;
            }
        }
    }
    if (turn == 0)
    {

        score -= moves.size();
    }
    else
    {
        score += moves.size();
    }

    bool isCheck = board->is_check(moves, !turn);
    if (isCheck)
    {
        if (turn == 0)
        {
            score -= 200;
        }
        else
        {
            score += 200;
        }
    }

    return score;
}

uint16_t MinMax::get_best_move(std::string fen, int time)
{
    if (time < 3)
    {
        time = 3;
    }

    std::thread t(timer_thread, time - 1);

    uint16_t bestMove = 0;
    Bitboards board;

    board.write_boards_from_FEN(fen);

    std::vector<uint16_t> moves = board.get_legal_moves();

    moves_depth_2.resize(moves.size());
    scores_depth_2.resize(moves.size());

    for (int m_i_1 = 0; m_i_1 < moves.size(); m_i_1++)
    {
        Bitboards board1;
        board1.copy_state(&board);
        board1.make_move(moves[m_i_1]);
        std::vector<uint16_t> moves_1 = board1.get_legal_moves();
        depth_1_boards.push_back(board1);
        scores_depth_1.push_back(evaluate(&board1, moves_1));
        moves_depth_2[m_i_1] = moves_1;
    }


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
            //move ordering:
            order_Moves();
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
        }
    }

    for (auto &thread : threads)
    {
        if (thread.joinable())
        {
            thread.join();
        }
    }
    t.join();
    int hi = 0;
    int hv = 0;
    for (int i = 0; i < scores_depth_1.size(); i++)
    {
        std::cout << "Score: " << scores_depth_1[i] << " " << decode_Move(moves[i]) << std::endl;
        if (scores_depth_1[i] > hv)
        {
            hv = scores_depth_1[i];
            hi = i;
        }
    }
    std::cout << "Depth: " << depth << std::endl;
    return moves[hi];
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

                int score = minMax(&depth_1_boards[index],index, depth, depth, maximizingPlayer, -100000, 100000);

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

int MinMax::minMax(Bitboards *board,int index, int depth, int max_depth, bool isMaximizingPlayer, int alpha, int beta)
{
    std::vector<uint16_t> moves;
    if (depth == max_depth)
    {
        moves = moves_depth_2[index];
        scores_depth_2[index].resize(moves.size());
    }
    else
    {
         moves = board->get_legal_moves();
    }

    if (depth == 0 || !should_continue || moves.empty())
    {
        return evaluate(board, moves);
    }

    if (isMaximizingPlayer)
    {
        int bestValue = -100000000;
        for (int m_i = 0; m_i <moves.size(); m_i++ )
        {
            Bitboards newBoard;
            newBoard.copy_state(board);
            newBoard.make_move(moves[m_i]);
            int value = minMax(&newBoard,index, depth - 1, max_depth, false, alpha, beta);
            bestValue = std::max(bestValue, value);
            alpha = std::max(alpha, bestValue);
            if(depth == max_depth)
            {
                scores_depth_2[index][m_i] = bestValue;
            }
            if (beta <= alpha)
                break; // Beta cut-off
        }
        return bestValue;
    }
    else
    {
        int bestValue = 100000000;
        for (int m_i = 0; m_i <moves.size(); m_i++ )
        {
            Bitboards newBoard;
            newBoard.copy_state(board);
            newBoard.make_move(moves[m_i]);
            int value = minMax(&newBoard,index, depth - 1, max_depth, true, alpha, beta);
            bestValue = std::min(bestValue, value);
            beta = std::min(beta, bestValue);
            if(depth == max_depth)
            {
                scores_depth_2[index][m_i] = bestValue;
            }
            if (beta <= alpha)
                break; // Alpha cut-off
        }
        return bestValue;
    }
}