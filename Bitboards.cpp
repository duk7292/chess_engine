
#include "Bitboards.h"

Bitboards::Bitboards(/* args */)
{
}

Bitboards::~Bitboards()
{
}

void Bitboards::write_boards_from_FEN(std::string FEN)
{
    int current_postion = 63;
    for (char c : FEN)
    {
        if (c == '|')
        {
            break;
        }
        if (c == '/')
        {
            continue;
        }
        else if (c >= '1' && c <= '8')
        {
            current_postion -= std::stoi(std::string(1, c));
        }
        else
        {
            switch (c)
            {
            case 'P':
                boards[0] |= 1ULL << current_postion;
                break;
            case 'N':
                boards[1] |= 1ULL << current_postion;
                break;
            case 'B':
                boards[2] |= 1ULL << current_postion;
                break;
            case 'R':
                boards[3] |= 1ULL << current_postion;
                break;
            case 'Q':
                boards[4] |= 1ULL << current_postion;
                break;
            case 'K':
                boards[5] |= 1ULL << current_postion;
                break;
            case 'p':
                boards[6] |= 1ULL << current_postion;
                break;
            case 'n':
                boards[7] |= 1ULL << current_postion;
                break;
            case 'b':
                boards[8] |= 1ULL << current_postion;
                break;
            case 'r':
                boards[9] |= 1ULL << current_postion;
                break;
            case 'q':
                boards[10] |= 1ULL << current_postion;
                break;
            case 'k':
                boards[11] |= 1ULL << current_postion;
                break;
            default:
                break;
            }

            current_postion--;
        }
    }
    turn = std::stoi(std::string(1, FEN[FEN.length() - 1]));
    if (FEN[FEN.length() - 6] == '-')
    {
        castling[3] = false;
    }
    if (FEN[FEN.length() - 7] == '-')
    {
        castling[2] = false;
    }
    if (FEN[FEN.length() - 9] == '-')
    {
        castling[1] = false;
    }
    if (FEN[FEN.length() - 10] == '-')
    {
        castling[0] = false;
    }
    if (FEN[FEN.length() - 3] != '-')
    {
        std::string numberStr = FEN.substr(FEN.length() - 4, 2);
        en_passant = std::stoi(numberStr);
    }
}

void Bitboards::copy_state(Bitboards *bitboards)
{
    memcpy(boards, bitboards->get_boards(), sizeof(uint64_t) * 12);
    turn = bitboards->get_turn();
}

uint64_t *Bitboards::get_boards()
{
    return boards;
}

int Bitboards::get_turn()
{
    return turn;
}

std::vector<uint16_t> Bitboards::get_legal_rook_moves()

{
    std::vector<uint16_t> legal_moves;
    legal_moves.reserve(64);

    int8_t rook_board_index = turn == 0 ? 9 : 3;
    for (int8_t rook_pos = 0; rook_pos < 64; rook_pos++)
    {

        if ((boards[rook_board_index] & (1ULL << rook_pos)) != 0)
        {
            int8_t moves_up = -1;
            int8_t moves_down = -1;
            int8_t moves_left = -1;
            int8_t moves_right = -1;
            for (int8_t moves_out = 1; moves_out <= 8; moves_out++)
            {
                if (moves_up != -1 && moves_down != -1 && moves_left != -1 && moves_right != -1)
                {
                    break;
                }
                for (int8_t board_idx = 0; board_idx < 12; board_idx++)
                {
                    if (moves_up == -1)
                    {
                        if ((rook_pos + (8 * moves_out)) >= 64)
                        {
                            moves_up = moves_out - 1;
                        }
                        else

                            if ((boards[board_idx] & (1ULL << (rook_pos + (8 * moves_out)))) != 0)
                        {
                            if ((turn == 0 && board_idx <= 5) || (turn == 1 && board_idx > 5))
                            {
                                moves_up = moves_out;
                                uint16_t move = 0;
                                move |= (rook_pos & 0x3F);
                                move |= ((rook_pos + (moves_out * 8)) & 0x3F) << 6;
                                legal_moves.push_back(move);
                            }
                            else
                            {
                                moves_up = moves_out - 1;
                            }
                        }
                    }
                    if (moves_down == -1)
                    {
                        if ((rook_pos - (8 * moves_out)) < 0)
                        {
                            moves_down = moves_out - 1;
                        }
                        else if ((boards[board_idx] & (1ULL << (rook_pos - (8 * moves_out)))) != 0)
                        {
                            if ((turn == 0 && board_idx <= 5) || (turn == 1 && board_idx > 5))
                            {
                                moves_down = moves_out;
                                uint16_t move = 0;
                                move |= (rook_pos & 0x3F);
                                move |= ((rook_pos - (moves_out * 8)) & 0x3F) << 6;
                                legal_moves.push_back(move);
                            }
                            else
                            {
                                moves_down = moves_out - 1;
                            }
                        }
                    }
                    if (moves_left == -1)
                    {
                        if ((rook_pos + moves_out) % 8 == 0)
                        {
                            moves_left = moves_out - 1;
                        }
                        else

                            if ((boards[board_idx] & (1ULL << (rook_pos + moves_out))) != 0)
                        {
                            if ((turn == 0 && board_idx <= 5) || (turn == 1 && board_idx > 5))
                            {
                                moves_left = moves_out;
                                uint16_t move = 0;
                                move |= (rook_pos & 0x3F);
                                move |= ((rook_pos + moves_out) & 0x3F) << 6;
                                legal_moves.push_back(move);
                            }
                            else
                            {
                                moves_left = moves_out - 1;
                            }
                        }
                    }
                    if (moves_right == -1)
                    {
                        if ((rook_pos - moves_out + 1) % 8 == 0)
                        {
                            moves_right = moves_out - 1;
                        }
                        else if ((boards[board_idx] & (1ULL << (rook_pos - moves_out))) != 0)
                        {
                            if ((turn == 0 && board_idx <= 5) || (turn == 1 && board_idx > 5))
                            {
                                moves_right = moves_out;
                                uint16_t move = 0;
                                move |= (rook_pos & 0x3F);
                                move |= ((rook_pos - moves_out) & 0x3F) << 6;
                                legal_moves.push_back(move);
                            }
                            else
                            {
                                moves_right = moves_out - 1;
                            }
                        }
                    }
                }

                if (moves_up == -1)
                {
                    uint16_t move = 0;
                    move |= (rook_pos & 0x3F);
                    move |= ((rook_pos + (moves_out * 8)) & 0x3F) << 6;
                    legal_moves.push_back(move);
                }
                if (moves_down == -1)
                {
                    uint16_t move = 0;
                    move |= (rook_pos & 0x3F);
                    move |= ((rook_pos - (moves_out * 8)) & 0x3F) << 6;
                    legal_moves.push_back(move);
                }
                if (moves_left == -1)
                {
                    uint16_t move = 0;
                    move |= (rook_pos & 0x3F);
                    move |= ((rook_pos + moves_out) & 0x3F) << 6;
                    legal_moves.push_back(move);
                }
                if (moves_right == -1)
                {
                    uint16_t move = 0;
                    move |= (rook_pos & 0x3F);
                    move |= ((rook_pos - moves_out) & 0x3F) << 6;
                    legal_moves.push_back(move);
                }
            }
        }
    }

    return legal_moves;
}

std::vector<uint16_t> Bitboards::get_legal_bishop_moves()
{
    std::vector<uint16_t> legal_moves;
    legal_moves.reserve(32);

    int8_t bishop_board_index = turn == 0 ? 8 : 2;
    for (int8_t bishop_pos = 0; bishop_pos < 64; bishop_pos++)
    {
        if ((boards[bishop_board_index] & (1ULL << bishop_pos)) != 0)
        {
            int8_t moves_up_left = -1;
            int8_t moves_up_right = -1;
            int8_t moves_down_left = -1;
            int8_t moves_down_right = -1;
            for (int8_t moves_out = 1; moves_out <= 8; moves_out++)
            {
                if (moves_up_left != -1 && moves_up_right != -1 && moves_down_left != -1 && moves_down_right != -1)
                {
                    break;
                }
                for (int8_t board_idx = 0; board_idx < 12; board_idx++)
                {
                    if (moves_up_left == -1)
                    {
                        if ((bishop_pos + (9 * moves_out)) >= 64 || (bishop_pos + (9 * moves_out)) % 8 == 0)
                        {
                            moves_up_left = moves_out - 1;
                        }
                        else

                            if ((boards[board_idx] & (1ULL << (bishop_pos + (9 * moves_out)))) != 0)
                        {
                            if ((turn == 0 && board_idx <= 5) || (turn == 1 && board_idx > 5))
                            {
                                moves_up_left = moves_out;
                                uint16_t move = 0;
                                move |= (bishop_pos & 0x3F);
                                move |= ((bishop_pos + (moves_out * 9)) & 0x3F) << 6;
                                legal_moves.push_back(move);
                            }
                            else
                            {
                                moves_up_left = moves_out - 1;
                            }
                        }
                    }
                    if (moves_up_right == -1)
                    {
                        if ((bishop_pos + (7 * moves_out)) >= 64 || (bishop_pos + (7 * moves_out)) % 8 == 7)
                        {
                            moves_up_right = moves_out - 1;
                        }
                        else if ((boards[board_idx] & (1ULL << (bishop_pos + (7 * moves_out)))) != 0)
                        {
                            if ((turn == 0 && board_idx <= 5) || (turn == 1 && board_idx > 5))
                            {
                                moves_up_right = moves_out;
                                uint16_t move = 0;
                                move |= (bishop_pos & 0x3F);
                                move |= ((bishop_pos + (moves_out * 7)) & 0x3F) << 6;
                                legal_moves.push_back(move);
                            }
                            else
                            {
                                moves_up_right = moves_out - 1;
                            }
                        }
                    }
                    if (moves_down_left == -1)
                    {
                        if ((bishop_pos - (7 * moves_out)) < 0 || (bishop_pos - (7 * moves_out)) % 8 == 0)
                        {
                            moves_down_left = moves_out - 1;
                        }
                        else

                            if ((boards[board_idx] & (1ULL << (bishop_pos - (7 * moves_out)))) != 0)
                        {
                            if ((turn == 0 && board_idx <= 5) || (turn == 1 && board_idx > 5))
                            {
                                moves_down_left = moves_out;
                                uint16_t move = 0;
                                move |= (bishop_pos & 0x3F);
                                move |= ((bishop_pos - (moves_out * 7)) & 0x3F) << 6;
                                legal_moves.push_back(move);
                            }
                            else
                            {
                                moves_down_left = moves_out - 1;
                            }
                        }
                    }
                    if (moves_down_right == -1)
                    {
                        if ((bishop_pos - (9 * moves_out)) < 0 || (bishop_pos - (9 * moves_out)) % 8 == 7)
                        {
                            moves_down_right = moves_out - 1;
                        }
                        else

                            if ((boards[board_idx] & (1ULL << (bishop_pos - (9 * moves_out)))) != 0)
                        {
                            if ((turn == 0 && board_idx <= 5) || (turn == 1 && board_idx > 5))
                            {
                                moves_down_right = moves_out;
                                uint16_t move = 0;
                                move |= (bishop_pos & 0x3F);
                                move |= ((bishop_pos - (moves_out * 9)) & 0x3F) << 6;
                                legal_moves.push_back(move);
                            }
                            else
                            {
                                moves_down_right = moves_out - 1;
                            }
                        }
                    }
                }

                if (moves_up_left == -1)
                {
                    uint16_t move = 0;
                    move |= (bishop_pos & 0x3F);
                    move |= ((bishop_pos + (moves_out * 9)) & 0x3F) << 6;
                    legal_moves.push_back(move);
                }
                if (moves_up_right == -1)
                {
                    uint16_t move = 0;
                    move |= (bishop_pos & 0x3F);
                    move |= ((bishop_pos + (moves_out * 7)) & 0x3F) << 6;
                    legal_moves.push_back(move);
                }
                if (moves_down_left == -1)
                {
                    uint16_t move = 0;
                    move |= (bishop_pos & 0x3F);
                    move |= ((bishop_pos - (moves_out * 7)) & 0x3F) << 6;
                    legal_moves.push_back(move);
                }
                if (moves_down_right == -1)
                {
                    uint16_t move = 0;
                    move |= (bishop_pos & 0x3F);
                    move |= ((bishop_pos - (moves_out * 9)) & 0x3F) << 6;
                    legal_moves.push_back(move);
                }
            }
        }
    }

    return legal_moves;
}

std::vector<uint16_t> Bitboards::get_legal_queen_moves()
{
    std::vector<uint16_t> legal_moves;
    legal_moves.reserve(64);

    int8_t queen_board_index = turn == 0 ? 10 : 4;
    for (int8_t queen_pos = 0; queen_pos < 64; queen_pos++)
    {
        if ((boards[queen_board_index] & (1ULL << queen_pos)) != 0)
        {
            int8_t moves_up = -1;
            int8_t moves_down = -1;
            int8_t moves_left = -1;
            int8_t moves_right = -1;
            int8_t moves_up_left = -1;
            int8_t moves_up_right = -1;
            int8_t moves_down_left = -1;
            int8_t moves_down_right = -1;
            for (int8_t moves_out = 1; moves_out <= 8; moves_out++)
            {
                if (moves_up != -1 && moves_down != -1 && moves_left != -1 && moves_right != -1 && moves_up_left != -1 && moves_up_right != -1 && moves_down_left != -1 && moves_down_right != -1)
                {
                    break;
                }
                for (int8_t board_idx = 0; board_idx < 12; board_idx++)
                {
                    if (moves_up == -1)
                    {
                        if ((queen_pos + (8 * moves_out)) >= 64)
                        {
                            moves_up = moves_out - 1;
                        }
                        else if ((boards[board_idx] & (1ULL << (queen_pos + (8 * moves_out)))) != 0)
                        {
                            if ((turn == 0 && board_idx <= 5) || (turn == 1 && board_idx > 5))
                            {
                                moves_up = moves_out;
                                uint16_t move = 0;
                                move |= (queen_pos & 0x3F);
                                move |= ((queen_pos + (moves_out * 8)) & 0x3F) << 6;
                                legal_moves.push_back(move);
                            }
                            else
                            {
                                moves_up = moves_out - 1;
                            }
                        }
                    }
                    if (moves_down == -1)
                    {
                        if ((queen_pos - (8 * moves_out)) < 0)
                        {
                            moves_down = moves_out - 1;
                        }
                        else if ((boards[board_idx] & (1ULL << (queen_pos - (8 * moves_out)))) != 0)
                        {
                            if ((turn == 0 && board_idx <= 5) || (turn == 1 && board_idx > 5))
                            {
                                moves_down = moves_out;
                                uint16_t move = 0;
                                move |= (queen_pos & 0x3F);
                                move |= ((queen_pos - (moves_out * 8)) & 0x3F) << 6;
                                legal_moves.push_back(move);
                            }
                            else
                            {
                                moves_down = moves_out - 1;
                            }
                        }
                    }
                    if (moves_left == -1)
                    {
                        if ((queen_pos + moves_out) % 8 == 0)
                        {
                            moves_left = moves_out - 1;
                        }
                        else if ((boards[board_idx] & (1ULL << (queen_pos + moves_out))) != 0)
                        {
                            if ((turn == 0 && board_idx <= 5) || (turn == 1 && board_idx > 5))
                            {
                                moves_left = moves_out;
                                uint16_t move = 0;
                                move |= (queen_pos & 0x3F);
                                move |= ((queen_pos + moves_out) & 0x3F) << 6;
                                legal_moves.push_back(move);
                            }
                            else
                            {
                                moves_left = moves_out - 1;
                            }
                        }
                    }
                    if (moves_right == -1)
                    {
                        if ((queen_pos - moves_out) % 8 == 7)
                        {
                            moves_right = moves_out - 1;
                        }
                        else if ((boards[board_idx] & (1ULL << (queen_pos - moves_out))) != 0)
                        {
                            if ((turn == 0 && board_idx <= 5) || (turn == 1 && board_idx > 5))
                            {
                                moves_right = moves_out;
                                uint16_t move = 0;
                                move |= (queen_pos & 0x3F);
                                move |= ((queen_pos - moves_out) & 0x3F) << 6;
                                legal_moves.push_back(move);
                            }
                            else
                            {
                                moves_right = moves_out - 1;
                            }
                        }
                    }
                    if (moves_up_left == -1)
                    {
                        if ((queen_pos + (9 * moves_out)) >= 64 || (queen_pos + (9 * moves_out)) % 8 == 0)
                        {
                            moves_up_left = moves_out - 1;
                        }
                        else if ((boards[board_idx] & (1ULL << (queen_pos + (9 * moves_out)))) != 0)
                        {
                            if ((turn == 0 && board_idx <= 5) || (turn == 1 && board_idx > 5))
                            {
                                moves_up_left = moves_out;
                                uint16_t move = 0;
                                move |= (queen_pos & 0x3F);
                                move |= ((queen_pos + (moves_out * 9)) & 0x3F) << 6;
                                legal_moves.push_back(move);
                            }
                            else
                            {
                                moves_up_left = moves_out - 1;
                            }
                        }
                    }
                    if (moves_up_right == -1)
                    {
                        if ((queen_pos + (7 * moves_out)) >= 64 || (queen_pos + (7 * moves_out)) % 8 == 7)
                        {
                            moves_up_right = moves_out - 1;
                        }
                        else if ((boards[board_idx] & (1ULL << (queen_pos + (7 * moves_out)))) != 0)
                        {
                            if ((turn == 0 && board_idx <= 5) || (turn == 1 && board_idx > 5))
                            {
                                moves_up_right = moves_out;
                                uint16_t move = 0;
                                move |= (queen_pos & 0x3F);
                                move |= ((queen_pos + (moves_out * 7)) & 0x3F) << 6;
                                legal_moves.push_back(move);
                            }
                            else
                            {
                                moves_up_right = moves_out - 1;
                            }
                        }
                    }
                    if (moves_down_left == -1)
                    {
                        if ((queen_pos - (7 * moves_out)) < 0 || (queen_pos - (7 * moves_out)) % 8 == 0)
                        {
                            moves_down_left = moves_out - 1;
                        }
                        else if ((boards[board_idx] & (1ULL << (queen_pos - (7 * moves_out)))) != 0)
                        {
                            if ((turn == 0 && board_idx <= 5) || (turn == 1 && board_idx > 5))
                            {
                                moves_down_left = moves_out;
                                uint16_t move = 0;
                                move |= (queen_pos & 0x3F);
                                move |= ((queen_pos - (moves_out * 7)) & 0x3F) << 6;
                                legal_moves.push_back(move);
                            }
                            else
                            {
                                moves_down_left = moves_out - 1;
                            }
                        }
                    }
                    if (moves_down_right == -1)
                    {
                        if ((queen_pos - (9 * moves_out)) < 0 || (queen_pos - (9 * moves_out)) % 8 == 7)
                        {
                            moves_down_right = moves_out - 1;
                        }
                        else if ((boards[board_idx] & (1ULL << (queen_pos - (9 * moves_out)))) != 0)
                        {
                            if ((turn == 0 && board_idx <= 5) || (turn == 1 && board_idx > 5))
                            {
                                moves_down_right = moves_out;
                                uint16_t move = 0;
                                move |= (queen_pos & 0x3F);
                                move |= ((queen_pos - (moves_out * 9)) & 0x3F) << 6;
                                legal_moves.push_back(move);
                            }
                            else
                            {
                                moves_down_right = moves_out - 1;
                            }
                        }
                    }
                }

                if (moves_up == -1)
                {
                    uint16_t move = 0;
                    move |= (queen_pos & 0x3F);
                    move |= ((queen_pos + (moves_out * 8)) & 0x3F) << 6;
                    legal_moves.push_back(move);
                }
                if (moves_down == -1)
                {
                    uint16_t move = 0;
                    move |= (queen_pos & 0x3F);
                    move |= ((queen_pos - (moves_out * 8)) & 0x3F) << 6;
                    legal_moves.push_back(move);
                }
                if (moves_left == -1)
                {
                    uint16_t move = 0;
                    move |= (queen_pos & 0x3F);
                    move |= ((queen_pos + moves_out) & 0x3F) << 6;
                    legal_moves.push_back(move);
                }
                if (moves_right == -1)
                {
                    uint16_t move = 0;
                    move |= (queen_pos & 0x3F);
                    move |= ((queen_pos - moves_out) & 0x3F) << 6;
                    legal_moves.push_back(move);
                }
                if (moves_up_left == -1)
                {
                    uint16_t move = 0;
                    move |= (queen_pos & 0x3F);
                    move |= ((queen_pos + (moves_out * 9)) & 0x3F) << 6;
                    legal_moves.push_back(move);
                }
                if (moves_up_right == -1)
                {
                    uint16_t move = 0;
                    move |= (queen_pos & 0x3F);
                    move |= ((queen_pos + (moves_out * 7)) & 0x3F) << 6;
                    legal_moves.push_back(move);
                }
                if (moves_down_left == -1)
                {
                    uint16_t move = 0;
                    move |= (queen_pos & 0x3F);
                    move |= ((queen_pos - (moves_out * 7)) & 0x3F) << 6;
                    legal_moves.push_back(move);
                }
                if (moves_down_right == -1)
                {
                    uint16_t move = 0;
                    move |= (queen_pos & 0x3F);
                    move |= ((queen_pos - (moves_out * 9)) & 0x3F) << 6;
                    legal_moves.push_back(move);
                }
            }
        }
    }

    return legal_moves;
}

std::vector<uint16_t> Bitboards::get_legal_knight_moves()
{
    std::vector<uint16_t> legal_moves;
    legal_moves.reserve(8);
    int8_t move_offsets[8] = {-17, -15, -10, -6, 6, 10, 15, 17};
    int8_t knight_board_index = turn == 0 ? 7 : 1;
    for (int8_t knight_pos = 0; knight_pos < 64; knight_pos++)
    {
        if ((boards[knight_board_index] & (1ULL << knight_pos)) != 0)
        {

            for (int move_idx = 0; move_idx < 8; move_idx++)
            {

                if (knight_pos + move_offsets[move_idx] < 0 || knight_pos + move_offsets[move_idx] >= 64)
                {

                    continue;
                }
                // works dont touch !!!
                int8_t rows_up = (int)((move_offsets[move_idx] * 1.69) / 10);

                if (((int)(((knight_pos) + move_offsets[move_idx]) / 8)) - ((int)((knight_pos) / 8)) != rows_up)
                {

                    continue;
                }
                bool valid_move = true;
                for (int8_t board_idx = 0; board_idx < 12; board_idx++)
                {
                    if ((boards[board_idx] & (1ULL << (knight_pos + move_offsets[move_idx]))) != 0)
                    {
                        if ((turn == 0 && board_idx <= 5) || (turn == 1 && board_idx > 5))
                        {
                            break;
                        }
                        else
                        {
                            valid_move = false;
                            break;
                        }
                    }
                }
                if (valid_move)
                {
                    uint16_t move = 0;
                    move |= (knight_pos & 0x3F);
                    move |= ((knight_pos + move_offsets[move_idx]) & 0x3F) << 6;
                    legal_moves.push_back(move);
                }
            }
        }
    }
    return legal_moves;
}

std::vector<uint16_t> Bitboards::get_legal_pawn_moves()
{
    std::vector<uint16_t> legal_moves;
    legal_moves.reserve(16);
    int8_t pawn_board_index = turn == 0 ? 6 : 0;
    for (int8_t pawn_pos = 0; pawn_pos < 64; pawn_pos++)
    {
        if ((boards[pawn_board_index] & (1ULL << pawn_pos)) != 0)
        {
            int8_t move_offset = turn == 0 ? -8 : 8;
            bool step1 = true;
            bool step2 = true;
            bool kill_left = false;
            bool kill_right = false;
            for (int board_idx = 0; board_idx < 12; board_idx++)
            {
                if ((boards[board_idx] & (1ULL << (pawn_pos + move_offset))) != 0)
                {

                    step1 = false;
                    break;
                }
                if (((boards[board_idx] & (1ULL << (pawn_pos + (2 * move_offset)))) != 0))
                {
                    step2 = false;
                    break;
                }
                if ((turn == 1 && board_idx > 5) || (turn == 0 && board_idx <= 5))
                {
                    if (boards[board_idx] & (1ULL << (pawn_pos + move_offset - 1)))
                    {
                        kill_left = true;
                    }
                    if (boards[board_idx] & (1ULL << (pawn_pos + move_offset + 1)))
                    {
                        kill_right = true;
                    }
                }
            }
            if (!(turn == 0 && pawn_pos >= 48 && pawn_pos <= 55) && !(turn == 1 && pawn_pos >= 8 && pawn_pos <= 15))
            {
                step2 = false;
            }
            if (en_passant == pawn_pos + move_offset - 1)
            {
                uint16_t move = 0;
                move |= (pawn_pos & 0x3F);
                move |= ((pawn_pos + move_offset - 1) & 0x3F) << 6;
                legal_moves.push_back(move);
            }
            if (en_passant == pawn_pos + move_offset + 1)
            {
                uint16_t move = 0;
                move |= (pawn_pos & 0x3F);
                move |= ((pawn_pos + move_offset + 1) & 0x3F) << 6;
                legal_moves.push_back(move);
            }

            if (step1)
            {
                uint16_t move = 0;
                move |= (pawn_pos & 0x3F);
                move |= ((pawn_pos + move_offset) & 0x3F) << 6;
                if ((turn == 0 && pawn_pos >= 8 && pawn_pos <= 15) || (turn == 1 && pawn_pos >= 48 && pawn_pos <= 55))
                {
                    // Pawn promotion
                    for (int promotion_piece = 0; promotion_piece < 4; promotion_piece++)
                    {
                        uint16_t promotion_move = move;
                        promotion_move |= 0x1000;
                        promotion_move |= (promotion_piece << 13);
                        legal_moves.push_back(promotion_move);
                    }
                }
                else
                {

                    legal_moves.push_back(move);
                }
            }
            if (step2)
            {
                uint16_t move = 0;
                move |= (pawn_pos & 0x3F);
                move |= ((pawn_pos + (2 * move_offset)) & 0x3F) << 6;
                legal_moves.push_back(move);
            }
            if (kill_left)
            {
                uint16_t move = 0;
                move |= (pawn_pos & 0x3F);
                move |= ((pawn_pos + move_offset - 1) & 0x3F) << 6;

                if ((turn == 0 && pawn_pos >= 8 && pawn_pos <= 15) || (turn == 1 && pawn_pos >= 48 && pawn_pos <= 55))
                {
                    // Pawn promotion
                    for (int promotion_piece = 0; promotion_piece < 4; promotion_piece++)
                    {
                        uint16_t promotion_move = move;
                        promotion_move |= 0x1000;
                        promotion_move |= (promotion_piece << 13);
                        legal_moves.push_back(promotion_move);
                    }
                }
                else
                {
                    legal_moves.push_back(move);
                }
            }
            if (kill_right)
            {
                uint16_t move = 0;
                move |= (pawn_pos & 0x3F);
                move |= ((pawn_pos + move_offset + 1) & 0x3F) << 6;

                if ((turn == 0 && pawn_pos >= 8 && pawn_pos <= 15) || (turn == 1 && pawn_pos >= 48 && pawn_pos <= 55))
                {
                    // Pawn promotion
                    for (int promotion_piece = 0; promotion_piece < 4; promotion_piece++)
                    {
                        uint16_t promotion_move = move;
                        promotion_move |= 0x1000;
                        promotion_move |= (promotion_piece << 13);
                        legal_moves.push_back(promotion_move);
                    }
                }
                else
                {
                    legal_moves.push_back(move);
                }
            }
        }
    }
    return legal_moves;
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

std::vector<uint16_t> Bitboards::get_legal_moves()
{

    turn = !turn;
    std::vector<uint16_t> enemy_moves_no_king = get_legal_rook_moves() + get_legal_bishop_moves() + get_legal_queen_moves() + get_legal_knight_moves() + get_legal_pawn_moves();
    std::vector<uint16_t> enemy_moves_no_king;

    auto rook_moves = get_legal_rook_moves();
    enemy_moves_no_king.insert(enemy_moves_no_king.end(), rook_moves.begin(), rook_moves.end());

    auto bishop_moves = get_legal_bishop_moves();
    enemy_moves_no_king.insert(enemy_moves_no_king.end(), bishop_moves.begin(), bishop_moves.end());

    auto queen_moves = get_legal_queen_moves();
    enemy_moves_no_king.insert(enemy_moves_no_king.end(), queen_moves.begin(), queen_moves.end());

    auto knight_moves = get_legal_knight_moves();
    enemy_moves_no_king.insert(enemy_moves_no_king.end(), knight_moves.begin(), knight_moves.end());

    auto pawn_moves = get_legal_pawn_moves();
    enemy_moves_no_king.insert(enemy_moves_no_king.end(), pawn_moves.begin(), pawn_moves.end());
    std::vector<uint16_t> vec = get_legal_pawn_moves();
    std::cout << vec.size() << std::endl;
    for (int i = 0; i < vec.size(); i++)
    {
        std::bitset<16> bits(vec[i]);
        std::cout << decodeMove(vec[i]) << std::endl;
    }
    return std::vector<uint16_t>();
}