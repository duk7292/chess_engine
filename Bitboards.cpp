
#include "Bitboards.h"

Bitboards::Bitboards(/* args */)
{
}

Bitboards::~Bitboards()
{
}

void Bitboards::write_boards_from_FEN(std::string FEN)
{
    int current_postion = 0;
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
            current_postion += c;
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

            current_postion++;
        }
    }
    turn = FEN[FEN.length() - 1];
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

u_int16_t *Bitboards::get_legal_rook_moves()
{
    std::vector<uint16_t> legal_moves;
    legal_moves.reserve(16);

    int8_t rook_board_index = turn == 0 ? 3 : 9;

    uint64_t rooks_board = boards[rook_board_index];

    for (int8_t i = 0; i < 64; i++)
    {
        if ((boards[rook_board_index] & (1ULL << i)) != 0)
        {

            int8_t moves_up = -1;
            int8_t moves_down = -1;
            int8_t moves_left = -1;
            int8_t moves_right = -1;
            for (int8_t mo)
                for (int8_t board_idx = 0; board_idx < 12; board_idx++)
                {
                }
        }
    }

    std::cout << static_cast<int>(rook_board_index) << std::endl;

    return nullptr;
}

uint16_t *Bitboards::get_legal_moves()
{
    get_legal_rook_moves();
    return nullptr;
}