
#include "Bitboards.h"

Bitboards::Bitboards(/* args */)
{
}

Bitboards::~Bitboards()
{
}

std::string indexToPosition(int index)
{
    if (index < 0 || index > 63)
    {
        return "Ungültiger Index";
    }

    char file = 'h' - (index % 8); // Spalte: h-a
    char rank = '1' + (index / 8); // Reihe: 1-8

    return std::string(1, file) + std::string(1, rank);
}
void Bitboards::write_boards_from_FEN(std::string FEN)
{

    for (int i = 0; i < 12; i++)
    {
        boards[i] = 0;
    }
    en_passant = 0;
    for (int i = 0; i < 4; i++)
    {
        castling[i] = true;
    }

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
    turn_G = std::stoi(std::string(1, FEN[FEN.length() - 1]));
    if (FEN[FEN.length() - 6] == '-')
    {
        castling[1] = false;
    }
    if (FEN[FEN.length() - 7] == '-')
    {
        castling[0] = false;
    }
    if (FEN[FEN.length() - 8] == '-')
    {
        castling[3] = false;
    }
    if (FEN[FEN.length() - 9] == '-')
    {
        castling[2] = false;
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
    turn_G = bitboards->get_turn();
    en_passant = bitboards->en_passant;
    for (int i = 0; i < 4; i++)
    {
        castling[i] = bitboards->castling[i];
    }
}

uint64_t *Bitboards::get_boards()
{
    return boards;
}

int Bitboards::get_turn()
{
    return turn_G;
}

std::vector<uint16_t> Bitboards::get_legal_rook_moves_absolute(int turn)

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
                            if ((turn == 0 && board_idx <= 5) || (turn == 1 && board_idx > 5) || turn != turn_G)
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
                            if ((turn == 0 && board_idx <= 5) || (turn == 1 && board_idx > 5) || turn != turn_G)
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
                            if ((turn == 0 && board_idx <= 5) || (turn == 1 && board_idx > 5) || turn != turn_G)
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
                            if ((turn == 0 && board_idx <= 5) || (turn == 1 && board_idx > 5) || turn != turn_G)
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

std::vector<uint16_t> Bitboards::get_legal_bishop_moves_absolute(int turn)
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
                            if ((turn == 0 && board_idx <= 5) || (turn == 1 && board_idx > 5) || turn != turn_G)
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
                            if ((turn == 0 && board_idx <= 5) || (turn == 1 && board_idx > 5) || turn != turn_G)
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
                            if ((turn == 0 && board_idx <= 5) || (turn == 1 && board_idx > 5) || turn != turn_G)
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
                            if ((turn == 0 && board_idx <= 5) || (turn == 1 && board_idx > 5) || turn != turn_G)
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

std::vector<uint16_t> Bitboards::get_legal_queen_moves_absolute(int turn)
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
                            if ((turn == 0 && board_idx <= 5) || (turn == 1 && board_idx > 5) || turn != turn_G)
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
                            if ((turn == 0 && board_idx <= 5) || (turn == 1 && board_idx > 5) || turn != turn_G)
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
                            if ((turn == 0 && board_idx <= 5) || (turn == 1 && board_idx > 5) || turn != turn_G)
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
                        else if ((queen_pos - moves_out) < 0)
                        {
                            moves_right = moves_out - 1;
                        }
                        else if ((boards[board_idx] & (1ULL << (queen_pos - moves_out))) != 0)
                        {
                            if ((turn == 0 && board_idx <= 5) || (turn == 1 && board_idx > 5) || turn != turn_G)
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
                            if ((turn == 0 && board_idx <= 5) || (turn == 1 && board_idx > 5) || turn != turn_G)
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
                            if ((turn == 0 && board_idx <= 5) || (turn == 1 && board_idx > 5) || turn != turn_G)
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
                            if ((turn == 0 && board_idx <= 5) || (turn == 1 && board_idx > 5) || turn != turn_G)
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
                            if ((turn == 0 && board_idx <= 5) || (turn == 1 && board_idx > 5) || turn != turn_G)
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

std::vector<uint16_t> Bitboards::get_legal_knight_moves_absolute(int turn)
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
                        if ((turn == 0 && board_idx <= 5) || (turn == 1 && board_idx > 5) || turn != turn_G)
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

std::vector<uint16_t> Bitboards::get_legal_pawn_moves_absolute(int turn)
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
                    step2 = false;
                }
                if (((boards[board_idx] & (1ULL << (pawn_pos + (2 * move_offset)))) != 0))
                {
                    step2 = false;
                }
                if ((turn == 1 && board_idx > 5) || (turn == 0 && board_idx <= 5) || turn != turn_G)
                {
                    if (pawn_pos % 8 != 0)
                    {
                        if (boards[board_idx] & (1ULL << (pawn_pos + move_offset - 1)) || turn != turn_G)
                        {
                            kill_left = true;
                        }
                    }
                    if (pawn_pos % 8 != 7)
                    {
                        if (boards[board_idx] & (1ULL << (pawn_pos + move_offset + 1)) || turn != turn_G)
                        {
                            kill_right = true;
                        }
                    }
                }
            }
            if (!(turn == 0 && pawn_pos >= 48 && pawn_pos <= 55) && !(turn == 1 && pawn_pos >= 8 && pawn_pos <= 15))
            {

                step2 = false;
            }
            if (en_passant == pawn_pos + move_offset - 1 && en_passant != 0 && pawn_pos % 8 != 0)
            {

                uint16_t move = 0;
                move |= (pawn_pos & 0x3F);
                move |= ((pawn_pos + move_offset - 1) & 0x3F) << 6;
                legal_moves.push_back(move);
            }
            if (en_passant == pawn_pos + move_offset + 1 && pawn_pos % 8 != 7)
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
                move |= 0x8000;
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
                    // Set the 15th bit to 1 for non-capturing moves
                    legal_moves.push_back(move);
                }
            }
            if (step2)
            {

                uint16_t move = 0;
                move |= (pawn_pos & 0x3F);
                move |= ((pawn_pos + (2 * move_offset)) & 0x3F) << 6;
                move |= 0x8000; // Set the 15th bit to 1 for non-capturing moves
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

std::vector<uint16_t> Bitboards::get_legal_king_moves_absolute(int turn)
{
    std::vector<uint16_t> legal_moves;

    int8_t king_board_index = turn == 0 ? 11 : 5;

    for (int8_t king_pos = 0; king_pos < 64; king_pos++)
    {
        if ((boards[king_board_index] & (1ULL << king_pos)) != 0)
        {
            int8_t move_offsets[8] = {-9, -8, -7, -1, 1, 7, 8, 9};
            int8_t rows_up[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
            for (int move_idx = 0; move_idx < 8; move_idx++)
            {
                if (king_pos + move_offsets[move_idx] < 0 || king_pos + move_offsets[move_idx] >= 64)
                {
                    continue;
                }

                if (((int)(((king_pos) + move_offsets[move_idx]) / 8)) - ((int)((king_pos) / 8)) != rows_up[move_idx])
                {
                    continue;
                }
                bool valid_move = true;
                for (int8_t board_idx = 0; board_idx < 12; board_idx++)
                {
                    if ((boards[board_idx] & (1ULL << (king_pos + move_offsets[move_idx]))) != 0)
                    {
                        if ((turn == 0 && board_idx <= 5) || (turn == 1 && board_idx > 5) || turn != turn_G)
                        {
                            continue;
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
                    move |= (king_pos & 0x3F);
                    move |= ((king_pos + move_offsets[move_idx]) & 0x3F) << 6;
                    legal_moves.push_back(move);
                }
            }
            break;
        }
    }
    return legal_moves;
}

std::vector<uint16_t> Bitboards::get_legal_king_moves(int turn, std::vector<uint16_t> enemy_moves, std::vector<std::vector<uint8_t>> prot_lines)
{
    std::vector<uint16_t> legal_moves;

    uint8_t king_board_index = turn == 0 ? 11 : 5;

    for (uint8_t king_pos = 0; king_pos < 64; king_pos++)
    {
        if ((boards[king_board_index] & (1ULL << king_pos)) != 0)
        {
            int8_t move_offsets[8] = {-9, -8, -7, -1, 1, 7, 8, 9};
            int8_t rows_up[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
            for (int move_idx = 0; move_idx < 8; move_idx++)
            {
                if (king_pos + move_offsets[move_idx] >= 64)
                {
                    continue;
                }
                if (king_pos + move_offsets[move_idx] < 0)
                {
                    continue;
                }

                if (((int)(((king_pos) + move_offsets[move_idx]) / 8)) - ((int)((king_pos) / 8)) != rows_up[move_idx])
                {
                    continue;
                }
                bool valid_move = true;

                for (uint8_t board_idx = 0; board_idx < 12; board_idx++)
                {
                    if ((boards[board_idx] & (1ULL << (king_pos + move_offsets[move_idx]))) != 0)
                    {
                        if ((turn == 0 && board_idx <= 5) || (turn == 1 && board_idx > 5))
                        {
                            continue;
                        }
                        else
                        {
                            valid_move = false;
                            break;
                        }
                    }
                }

                for (uint8_t prot_line_idx = 0; prot_line_idx < prot_lines.size(); prot_line_idx++)
                {
                    if (prot_lines[prot_line_idx][0] == 0)
                    {

                        if (move_offsets[move_idx] == king_pos - prot_lines[prot_line_idx][1])
                        {
                            valid_move = false;
                            break;
                        }
                    }
                }

                for (uint16_t enemy_moves_idx = 0; enemy_moves_idx < enemy_moves.size(); enemy_moves_idx++)
                {

                    uint16_t mask = 0x3F << 6; // Isoliert die Bits 6 bis 11
                    uint16_t isolated_16_bit = enemy_moves[enemy_moves_idx] & mask;
                    isolated_16_bit >>= 6;
                    uint8_t isolated = isolated_16_bit & 0xFF;

                    if (((int)isolated) == ((int)(king_pos + move_offsets[move_idx])) && enemy_moves[enemy_moves_idx] <= 32767)
                    {

                        valid_move = false;
                        break;
                    }
                }
                if (valid_move)
                {
                    uint16_t move = 0;
                    move |= (king_pos & 0x3F);
                    move |= ((king_pos + move_offsets[move_idx]) & 0x3F) << 6;
                    legal_moves.push_back(move);
                }
            }
        }
    }

    return legal_moves;
}

std::vector<uint8_t> Bitboards::get_enpassant_killer_pos(int turn)
{

    std::vector<uint8_t> enpassant_kill_pos;
    enpassant_kill_pos.reserve(2);

    if (turn == 1)
    {
        if ((boards[0] & (1ULL << (en_passant - 9))) != 0)
        {
            enpassant_kill_pos.push_back(en_passant - 9);
        }
        if ((boards[0] & (1ULL << (en_passant - 7))) != 0)
        {
            enpassant_kill_pos.push_back(en_passant - 7);
        }
    }

    else
    {
        if ((boards[6] & (1ULL << (en_passant + 9))) != 0)
        {
            enpassant_kill_pos.push_back(en_passant + 9);
        }
        if ((boards[6] & (1ULL << (en_passant + 7))) != 0)
        {
            enpassant_kill_pos.push_back(en_passant + 7);
        }
    }

    return enpassant_kill_pos;
}

bool Bitboards::is_check(std::vector<uint16_t> enemy_moves, int turn)
{
    int8_t king_board_index = turn == 0 ? 11 : 5;

    for (int8_t king_pos = 0; king_pos < 64; king_pos++)
    {
        if ((boards[king_board_index] & (1ULL << king_pos)) != 0)
        {
            for (uint16_t move_idx = 0; move_idx < enemy_moves.size(); move_idx++)
            {

                uint16_t mask = 0x3F << 6;
                uint16_t isolated = enemy_moves[move_idx] & mask;
                isolated >>= 6;

                if ((isolated) == king_pos && enemy_moves[move_idx] <= 32767)
                {

                    return true;
                }
            }
            break;
        }
    }
    return false;
}

std::vector<std::vector<uint8_t>> Bitboards::get_king_prot_lines(int turn)
{
    std::vector<std::vector<uint8_t>> prot_lines;

    int8_t move_offsets[8] = {8, 7, 1, -7, -8, -9, -1, 9};
    int8_t rows_up[8] = {1, 1, 0, -1, -1, -1, 0, 1};

    std::vector<int8_t> important_enemy_boards = turn == 0 ? std::vector<int8_t>{2, 3, 4} : std::vector<int8_t>{8, 9, 10};
    int8_t king_board_index = turn == 0 ? 11 : 5;

    for (int8_t king_pos = 0; king_pos < 64; king_pos++)
    {
        if ((boards[king_board_index] & (1ULL << king_pos)) != 0)
        {
            for (int8_t prot_line_idx = 0; prot_line_idx < 8; prot_line_idx++)
            {
                std::vector<uint8_t> prot_line;
                int8_t friendly_figures_on_line = 0;
                int8_t friendly_figures_on_line_pos = 0;
                bool important_enemy_figure_on_line = false;
                bool unimportant_enemy_figure_on_line = false;
                int8_t important_enemy_figure_on_line_pos;
                for (int move_idx = 1; move_idx <= 8; move_idx++)
                {
                    if (king_pos + move_offsets[prot_line_idx] * move_idx < 0 || king_pos + move_offsets[prot_line_idx] * move_idx >= 64)
                    {
                        break;
                    }

                    if (((int)(((king_pos) + move_offsets[prot_line_idx] * move_idx) / 8)) - ((int)(((king_pos) + move_offsets[prot_line_idx] * (move_idx - 1)) / 8)) != rows_up[prot_line_idx])
                    {
                        break;
                    }
                    for (int8_t board_idx = 0; board_idx < 12; board_idx++)
                    {
                        if ((boards[board_idx] & (1ULL << ((king_pos) + move_offsets[prot_line_idx] * move_idx))) != 0)
                        {
                            if ((turn == 1 && board_idx <= 5) || (turn == 0 && board_idx > 5))
                            {

                                friendly_figures_on_line++;
                                friendly_figures_on_line_pos = (king_pos) + move_offsets[prot_line_idx] * move_idx;
                            }
                            else if ((turn == 0 && board_idx <= 5) || (turn == 1 && board_idx > 5))
                            {
                                if (!(std::find(important_enemy_boards.begin(), important_enemy_boards.end(), board_idx) != important_enemy_boards.end()))
                                {
                                    unimportant_enemy_figure_on_line = true;
                                }
                                if (!((prot_line_idx % 2 == 0 && (boards[important_enemy_boards[1]] & (1ULL << ((king_pos) + move_offsets[prot_line_idx] * move_idx))) != 0) ||
                                      (prot_line_idx % 2 == 1 && (boards[important_enemy_boards[0]] & (1ULL << ((king_pos) + move_offsets[prot_line_idx] * move_idx))) != 0) ||
                                      ((boards[important_enemy_boards[2]] & (1ULL << ((king_pos) + move_offsets[prot_line_idx] * move_idx))) != 0)))
                                {
                                    unimportant_enemy_figure_on_line = true;
                                }
                            }
                            break;
                        }
                    }
                    if (friendly_figures_on_line > 1)
                    {

                        break;
                    }
                    if ((prot_line_idx % 2 == 0 && (boards[important_enemy_boards[1]] & (1ULL << ((king_pos) + move_offsets[prot_line_idx] * move_idx))) != 0) ||
                        (prot_line_idx % 2 == 1 && (boards[important_enemy_boards[0]] & (1ULL << ((king_pos) + move_offsets[prot_line_idx] * move_idx))) != 0) ||
                        ((boards[important_enemy_boards[2]] & (1ULL << ((king_pos) + move_offsets[prot_line_idx] * move_idx))) != 0))
                    {

                        important_enemy_figure_on_line = true;
                        important_enemy_figure_on_line_pos = (king_pos) + move_offsets[prot_line_idx] * move_idx;
                        break;
                    }
                }
                if (friendly_figures_on_line > 1 || unimportant_enemy_figure_on_line)
                {
                    continue;
                }
                else if (important_enemy_figure_on_line)
                {
                    prot_line.push_back(friendly_figures_on_line_pos);
                    for (int current_postion = king_pos; current_postion != important_enemy_figure_on_line_pos; current_postion += move_offsets[prot_line_idx])
                    {
                        prot_line.push_back(current_postion + move_offsets[prot_line_idx]);
                    }

                    prot_lines.push_back(prot_line);
                }
            }
        }
    }
    return prot_lines;
}
std::string decode_move(uint16_t move)
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
std::vector<uint16_t> Bitboards::erase_ilegal_moves(std::vector<uint16_t> moves, std::vector<std::vector<uint8_t>> king_prot_lines, int turn)
{

    for (uint16_t prot_line = 0; prot_line < king_prot_lines.size(); prot_line++)
    {

        for (uint16_t move_idx = 0; move_idx < moves.size(); move_idx++)
        {
            uint16_t mask = 0x3F;
            uint16_t start_isolated_16_bit = moves[move_idx] & mask;
            uint8_t start_isolated = start_isolated_16_bit & 0xFF;
            if (start_isolated == king_prot_lines[prot_line][0])
            {
                mask = 0x3F << 6;
                uint16_t end_isolated_16_bit = moves[move_idx] & mask;
                end_isolated_16_bit >>= 6;
                uint8_t end_isolated = end_isolated_16_bit & 0xFF;
                bool end_is_on_prot_line = false;

                for (uint8_t prot_pos = 1; prot_pos < king_prot_lines[prot_line].size(); prot_pos++)
                {
                    if (end_isolated == king_prot_lines[prot_line][prot_pos])
                    {
                        end_is_on_prot_line = true;
                        break;
                    }
                }
                if (!end_is_on_prot_line)
                {

                    moves.erase(moves.begin() + move_idx);
                    move_idx--;
                }
            }
        }
    }

    if (en_passant)
    {
        std::vector<uint16_t> en_passant_moves_to_erase;
        for (uint16_t move : moves)
        {
            int start = move & 0x3F;
            if ((boards[turn_G ? 0 : 6] & (1ULL << (start))) != 0)
            {
                Bitboards *temp = new Bitboards();
                temp->copy_state(this);

                temp->make_move(move);

                std::vector<uint16_t> enemy_moves = temp->get_legal_king_moves_absolute(!turn);

                std::vector<uint16_t> enemy_rook_moves = temp->get_legal_rook_moves_absolute(!turn);
                enemy_moves.insert(enemy_moves.end(), enemy_rook_moves.begin(), enemy_rook_moves.end());

                std::vector<uint16_t> enemy_bishop_moves = temp->get_legal_bishop_moves_absolute(!turn);
                enemy_moves.insert(enemy_moves.end(), enemy_bishop_moves.begin(), enemy_bishop_moves.end());

                std::vector<uint16_t> enemy_queen_moves = temp->get_legal_queen_moves_absolute(!turn);
                enemy_moves.insert(enemy_moves.end(), enemy_queen_moves.begin(), enemy_queen_moves.end());

                std::vector<uint16_t> enemy_knight_moves = temp->get_legal_knight_moves_absolute(!turn);
                enemy_moves.insert(enemy_moves.end(), enemy_knight_moves.begin(), enemy_knight_moves.end());

                std::vector<uint16_t> enemy_pawn_moves = temp->get_legal_pawn_moves_absolute(!turn);
                enemy_moves.insert(enemy_moves.end(), enemy_pawn_moves.begin(), enemy_pawn_moves.end());

                if (temp->is_check(enemy_moves, turn))
                {

                    en_passant_moves_to_erase.push_back(move);
                }
                delete temp;
            }
        }

        for (auto it = moves.begin(); it != moves.end();)
        {

            if (std::find(en_passant_moves_to_erase.begin(), en_passant_moves_to_erase.end(), *it) != en_passant_moves_to_erase.end())
            {

                it = moves.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }
    return moves;
}

std::vector<uint16_t> Bitboards::erase_ilegal_moves_when_check(std::vector<uint16_t> moves, int turn, std::vector<uint16_t> enemy_moves, std::vector<std::vector<uint8_t>> king_prot_lines)
{

    int8_t king_board_index = turn == 0 ? 11 : 5;

    int attack_count = 0;
    int line_attack_count = 0;
    int line_attack_idx;
    int normal_attack_pos;
    for (uint8_t king_prot_line_idx = 0; king_prot_line_idx < king_prot_lines.size(); king_prot_line_idx++)
    {
        if (king_prot_lines[king_prot_line_idx][0] == 0)
        {
            line_attack_count++;
            line_attack_idx = king_prot_line_idx;
        }
    }

    for (int8_t king_pos = 0; king_pos < 64; king_pos++)
    {
        if ((boards[king_board_index] & (1ULL << king_pos)) != 0)
        {
            for (uint16_t move_idx = 0; move_idx < enemy_moves.size(); move_idx++)
            {

                uint16_t mask = 0x3F << 6;
                uint16_t isolated = enemy_moves[move_idx] & mask;
                isolated >>= 6;

                if ((enemy_moves[move_idx] >> 12) & 0x1)
                {
                    move_idx += 3;
                }
                if ((isolated) == king_pos && enemy_moves[move_idx] <= 32767)
                {
                    mask = 0x3F;
                    attack_count++;
                    normal_attack_pos = enemy_moves[move_idx] & mask;
                }
            }
            break;
        }
    }
    if (attack_count > 1)
    {
        return std::vector<uint16_t>();
    }
    else if (line_attack_count == 1)
    {

        for (uint16_t move_idx = 0; move_idx < moves.size(); move_idx++)
        {

            uint16_t mask = 0x3F << 6;
            uint16_t isolated_16_bit = moves[move_idx] & mask;
            isolated_16_bit >>= 6;
            uint8_t isolated = isolated_16_bit & 0xFF;

            bool valid_move = false;
            for (uint8_t prot_pos = 1; prot_pos < king_prot_lines[line_attack_idx].size(); prot_pos++)
            {
                if (isolated == king_prot_lines[line_attack_idx][prot_pos])
                {

                    if (prot_pos == king_prot_lines[line_attack_idx].size() - 1)
                    {

                        if (moves[move_idx] <= 32767)
                        {
                            valid_move = true;
                            break;
                        }
                    }
                    else
                    {
                        valid_move = true;
                        break;
                    }
                }
            }
            if (!valid_move)
            {
                moves.erase(moves.begin() + move_idx);
                move_idx--;
            }
        }
    }
    else if (attack_count == 1)
    {

        for (uint16_t move_idx = 0; move_idx < moves.size(); move_idx++)
        {

            uint16_t mask = 0x3F << 6;
            uint16_t isolated = moves[move_idx] & mask;
            isolated >>= 6;

            if (en_passant != 0)
            {
                if (normal_attack_pos == (isolated + 8) - turn * 16)
                {
                    uint16_t isolated_start = moves[move_idx] & 0x3F;
                    uint8_t pawn_board_index = turn == 0 ? 6 : 0;
                    if ((boards[pawn_board_index] & (1ULL << isolated_start)) != 0)
                    {
                        continue;
                    }
                }
            }

            if (!((isolated) == normal_attack_pos && moves[move_idx] <= 32767))
            {

                moves.erase(moves.begin() + move_idx);
                move_idx--;
            }
        }
    }

    return moves;
}

std::vector<uint16_t> Bitboards::get_legal_casteling_moves(int turn, std::vector<uint16_t> enemy_moves)
{
    bool casteling_king_side = turn ? castling[2] : castling[0];
    bool casteling_queen_side = turn ? castling[3] : castling[1];

    for (int board_idx = 0; board_idx < 12; board_idx++)
    {
        if ((boards[board_idx] & (1ULL << (turn ? 4 : 60))) != 0 || (boards[board_idx] & (1ULL << (turn ? 5 : 61))) != 0 || (boards[board_idx] & (1ULL << (turn ? 6 : 62))) != 0)
        {
            casteling_queen_side = false;
        }
        if ((boards[board_idx] & (1ULL << (turn ? 2 : 58))) != 0 || (boards[board_idx] & (1ULL << (turn ? 1 : 57))) != 0)
        {
            casteling_king_side = false;
        }
    }
    for (uint16_t enemy_moves_idx = 0; enemy_moves_idx < enemy_moves.size(); enemy_moves_idx++)
    {

        uint16_t mask = 0x3F << 6;
        uint16_t isolated = enemy_moves[enemy_moves_idx] & mask;
        isolated >>= 6;

        if ((((int)isolated) == (turn ? 1 : 57) || ((int)isolated) == (turn ? 2 : 58)) && enemy_moves[enemy_moves_idx] <= 32767)
        {

            casteling_king_side = false;
        }
        if ((((int)isolated) == (turn ? 5 : 61) || ((int)isolated) == (turn ? 4 : 60)) && enemy_moves[enemy_moves_idx] <= 32767)
        {
            casteling_queen_side = false;
        }
    }
    std::vector<uint16_t> legal_moves;
    if (casteling_king_side)
    {
        uint16_t move = 0;
        move |= (turn ? 3 : 59) & 0x3F;
        move |= ((turn ? 1 : 57) & 0x3F) << 6;
        move |= 0x4000;
        legal_moves.push_back(move);
    }
    if (casteling_queen_side)
    {
        uint16_t move = 0;
        move |= (turn ? 3 : 59) & 0x3F;
        move |= ((turn ? 5 : 61) & 0x3F) << 6;
        move |= 0x4000;
        legal_moves.push_back(move);
    }

    return legal_moves;
}

std::vector<uint16_t> Bitboards::get_legal_moves()
{

    std::vector<uint16_t> legal_moves;

    // enemy moves
    std::vector<uint16_t> enemy_moves = get_legal_king_moves_absolute(!turn_G);

    std::vector<uint16_t> enemy_rook_moves = get_legal_rook_moves_absolute(!turn_G);
    enemy_moves.insert(enemy_moves.end(), enemy_rook_moves.begin(), enemy_rook_moves.end());

    std::vector<uint16_t> enemy_bishop_moves = get_legal_bishop_moves_absolute(!turn_G);
    enemy_moves.insert(enemy_moves.end(), enemy_bishop_moves.begin(), enemy_bishop_moves.end());

    std::vector<uint16_t> enemy_queen_moves = get_legal_queen_moves_absolute(!turn_G);
    enemy_moves.insert(enemy_moves.end(), enemy_queen_moves.begin(), enemy_queen_moves.end());

    std::vector<uint16_t> enemy_knight_moves = get_legal_knight_moves_absolute(!turn_G);
    enemy_moves.insert(enemy_moves.end(), enemy_knight_moves.begin(), enemy_knight_moves.end());

    std::vector<uint16_t> enemy_pawn_moves = get_legal_pawn_moves_absolute(!turn_G);
    enemy_moves.insert(enemy_moves.end(), enemy_pawn_moves.begin(), enemy_pawn_moves.end());

    // absolute moves
    std::vector<uint16_t> absolute_moves;

    std::vector<uint16_t> rook_moves = get_legal_rook_moves_absolute(turn_G);
    absolute_moves.insert(absolute_moves.end(), rook_moves.begin(), rook_moves.end());

    std::vector<uint16_t> bishop_moves = get_legal_bishop_moves_absolute(turn_G);
    absolute_moves.insert(absolute_moves.end(), bishop_moves.begin(), bishop_moves.end());

    std::vector<uint16_t> queen_moves = get_legal_queen_moves_absolute(turn_G);
    absolute_moves.insert(absolute_moves.end(), queen_moves.begin(), queen_moves.end());

    std::vector<uint16_t> knight_moves = get_legal_knight_moves_absolute(turn_G);
    absolute_moves.insert(absolute_moves.end(), knight_moves.begin(), knight_moves.end());

    std::vector<uint16_t> pawn_moves = get_legal_pawn_moves_absolute(turn_G);
    absolute_moves.insert(absolute_moves.end(), pawn_moves.begin(), pawn_moves.end());

    // is check
    bool check = is_check(enemy_moves, turn_G);
    std::vector<std::vector<uint8_t>> king_prot_lines = get_king_prot_lines(turn_G);

    legal_moves = erase_ilegal_moves(absolute_moves, king_prot_lines, turn_G);

    std::vector<uint16_t> legal_king_moves = get_legal_king_moves(turn_G, enemy_moves, king_prot_lines);
    if (check)
    {
        legal_moves = erase_ilegal_moves_when_check(legal_moves, turn_G, enemy_moves, king_prot_lines);
    }
    else
    {
        std::vector<uint16_t> casteling = get_legal_casteling_moves(turn_G, enemy_moves);
        legal_moves.insert(legal_moves.end(), casteling.begin(), casteling.end());
    }

    legal_moves.insert(legal_moves.end(), legal_king_moves.begin(), legal_king_moves.end());

    return legal_moves;
}
uint8_t Bitboards::get_en_passant()
{
    return en_passant;
}
void Bitboards::make_move(uint16_t move)
{

    // Extrahieren der Start- und Zielpositionen
    int start = move & 0x3F;                // Die ersten 6 Bits
    int end = (move >> 6) & 0x3F;           // Die nächsten 6 Bits
    bool isPromotion = (move >> 12) & 0x1;  // Das 13. Bit für Pawn Promotion
    int promotionType = (move >> 13) & 0x3; // Die Bits 14 und 15 für den Typ der Promotion
    int moveType = 0;
    for (int board_idx = 0; int(board_idx) < 12; board_idx++)
    {
        if ((boards[board_idx] & (1ULL << start)) != 0)
        {
            moveType = board_idx;
            break;
        }
    }

    // en passant
    // doing en passant
    if (moveType == (turn_G ? 0 : 6))
    {
        if (en_passant)
        {

            if (turn_G)
            {
                if (((start + 9 == en_passant) || (start + 7 == en_passant)) && end == en_passant && en_passant != 0 && (start >= 32 && start <= 39))
                {

                    boards[6] &= ~(1ULL << (en_passant - 8));
                    en_passant = 0;
                }
            }
            else
            {
                if (((start - 9 == en_passant) || (start - 7 == en_passant)) && end == en_passant && en_passant != 0 && (start >= 24 && start <= 31))
                {

                    boards[0] &= ~(1ULL << (en_passant + 8));

                    en_passant = 0;
                }
            }
        }
    }

    // making en passant possible
    if (moveType == (turn_G ? 0 : 6))
    {

        if (abs(start - end) == 16)
        {
            if (((boards[turn_G ? 6 : 0] & (1ULL << (end + 1))) != 0) || ((boards[turn_G ? 6 : 0] & (1ULL << (end - 1))) != 0))
            {
                en_passant = turn_G ? (start + 8) : (start - 8);
            }
            else
            {
                en_passant = 0;
            }
        }
        else
        {
            en_passant = 0;
        }
    }
    else
    {
        en_passant = 0;
    }
    // Casteling
    int rook_casteling_move_positions[8] = {0, 2, 7, 4, 56, 58, 63, 60};
    if (moveType == (turn_G ? 5 : 11) && abs(start - end) == 2)
    {
        int rookStart, rookEnd;
        if (turn_G)
        {
            if (end < start)
            {
                // Kingside castling
                rookStart = rook_casteling_move_positions[0];
                rookEnd = rook_casteling_move_positions[1];
            }
            else
            {
                // Queenside castling
                rookStart = rook_casteling_move_positions[2];
                rookEnd = rook_casteling_move_positions[3];
            }
        }
        else
        {
            if (end > start)
            {
                // Kingside castling
                rookStart = rook_casteling_move_positions[6];
                rookEnd = rook_casteling_move_positions[7];
            }
            else
            {
                // Queenside castling
                rookStart = rook_casteling_move_positions[4];
                rookEnd = rook_casteling_move_positions[5];
            }
        }

        // Move the rook
        for (int board_idx = 0; int(board_idx) < 12; board_idx++)
        {
            if ((boards[board_idx] & (1ULL << rookStart)) != 0)
            {
                boards[board_idx] &= ~(1ULL << rookStart);
                boards[board_idx] |= (1ULL << rookEnd);
                break;
            }
        }
    }
    bool is_one_casteling_true = std::any_of(std::begin(castling), std::end(castling), [](bool v)
                                             { return v; });
    std::vector<int> own_rooks, enemy_rooks;
    int rook_positions[4] = {56, 63, 0, 7};
    for (int i = 0; i < 4; ++i)
    {
        if (castling[i])
        {
            if ((turn_G && i >= 2) || (!turn_G && i < 2))
            {
                own_rooks.push_back(rook_positions[i]);
            }
            else
            {
                enemy_rooks.push_back(rook_positions[i]);
            }
        }
    }
    if (is_one_casteling_true)
    {

        if (own_rooks.size() > 0)
        {

            if ((start == 7 && turn_G == 1) || (start == 63 && turn_G == 0))
            {

                castling[turn_G ? 3 : 1] = false;
            }
            else if ((start == 0 && turn_G == 1) || (start == 56 && turn_G == 0))
            {
                castling[turn_G ? 2 : 0] = false;
            }
            if ((start == 3 && turn_G == 1) || (start == 59 && turn_G == 0))
            {
                castling[turn_G ? 3 : 1] = false;
                castling[turn_G ? 2 : 0] = false;
            }
        }
        if (enemy_rooks.size() > 0)
        {
            if ((end == 7 && turn_G == 0) || (end == 63 && turn_G == 1))
            {
                castling[turn_G ? 1 : 3] = false;
            }
            else if ((end == 0 && turn_G == 0) || (end == 56 && turn_G == 1))
            {
                castling[turn_G ? 0 : 2] = false;
            }
        }
    }
    // normel move
    for (int board_idx_enemy = 0; int(board_idx_enemy) < 12; board_idx_enemy++)
    {
        if ((boards[board_idx_enemy] & (1ULL << end)) != 0)
        {
            boards[board_idx_enemy] &= ~(1ULL << end);
            break;
        }
    }

    boards[moveType] &= ~(1ULL << start);
    if (isPromotion)
    {
        boards[promotionType + (turn_G ? 1 : 7)] |= (1ULL << end);
    }
    else
    {
        boards[moveType] |= (1ULL << end);
    }

    turn_G = !turn_G;
}