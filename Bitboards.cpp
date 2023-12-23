
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
    std::cout << "turn: " << turn << std::endl;
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

uint16_t *Bitboards::get_legal_rook_moves()
{
    std::vector<uint16_t> legal_moves;

    int8_t rook_board_index = turn == 0 ? 9 : 3;


    for (int8_t rook_pos = 0; rook_pos < 64; rook_pos++)
    {
        if ((boards[rook_board_index] & (1ULL << rook_pos)) != 0)
        {

            int8_t moves_up = -1;
            int8_t moves_down = -1;
            int8_t moves_left = -1;
            int8_t moves_right = -1;
            for (int8_t moves_out = 1; moves_out <=8; moves_out++) 
            {
                for (int8_t board_idx = 0; board_idx < 12; board_idx++) 
                {
                    if (moves_up  == -1)
                    {
                        if((rook_pos + (8 * moves_out)) >= 64)
                        {
                            moves_up = moves_out-1;
                            
                        }
                        
                        if ((boards[board_idx] & (1ULL << (rook_pos + (8 * moves_out)))) != 0) 
                        {
                            if((turn == 0 && board_idx <=5 )||( turn == 1 && board_idx > 5))
                            {
                                moves_up = moves_out;
                                uint16_t move = 0; 
                                move |= (rook_pos & 0x3F);
                                move |= ((rook_pos + (8 * moves_out)) & 0x3F) << 6; 
                                //legal_moves.push_back(move); 
                            }else 
                            {
                                moves_up = moves_out - 1;
                            }
                        }else 
                        {
                           /* uint16_t move = 0; 
                            move |= (rook_pos & 0x3F);
                            move |= ((rook_pos + (8 * moves_out)) & 0x3F) << 6; 
                            legal_moves.push_back(move); */
                        }
                    }
                    if (moves_down  == -1)
                    {
                        if((rook_pos - (8 * moves_out)) < 0)
                        {
                            moves_down = moves_out-1;
                        }
                        if ((boards[board_idx] & (1ULL << (rook_pos - (8 * moves_out)))) != 0) 
                        {
                            if((turn == 0 && board_idx <=5) || (turn == 1 && board_idx > 5)) 
                            {
                                moves_down = moves_out;
                                uint16_t move = 0; 
                                move |= (rook_pos & 0x3F);
                                move |= ((rook_pos - (8 * moves_out)) & 0x3F) << 6; 
                                legal_moves.push_back(move); 
                            }else 
                            {
                                moves_down = moves_out - 1;
                            }
                        }else{

                                std::cout << "debug" << std::endl;
                            uint16_t move = 0; 
                            move |= (rook_pos & 0x3F);
                            move |= ((rook_pos - (8 * moves_out)) & 0x3F) << 6; 
                            legal_moves.push_back(move);
                        }

                    }
                    if(moves_left == -1)
                    {
                        if((rook_pos + moves_out ) % 8 == 0)
                        {
                            moves_left = moves_out-1;
                        }

                        if ((boards[board_idx] & (1ULL << (rook_pos + moves_out))) != 0) 
                        {
                            if((turn == 0 && board_idx <=5 )|| (turn == 1 && board_idx > 5) )
                            {
                                moves_left = moves_out;
                               /* uint16_t move = 0;
                                move |= (rook_pos & 0x3F);
                                move |= ((rook_pos + moves_out) & 0x3F) << 6;
                                legal_moves.push_back(move);*/
                            }else 
                            {
                                moves_left = moves_out - 1;
                            }
                        }else 
                        {
                           /* uint16_t move = 0;
                            move |= (rook_pos & 0x3F);
                            move |= ((rook_pos + moves_out) & 0x3F) << 6;
                            legal_moves.push_back(move);*/
                        }
                    }
                    if(moves_right == -1)
                    {
                        if((rook_pos - moves_out+1) % 8 == 0)
                        {
                            moves_right = moves_out-1;
                        }
                        if ((boards[board_idx] & (1ULL << (rook_pos - moves_out)) )!= 0) 
                        {
                            if((turn == 0 && board_idx <=5) ||( turn == 1 && board_idx > 5)) 
                            {
                                moves_right = moves_out;
                              /*  uint16_t move = 0;
                                move |= (rook_pos & 0x3F);
                                move |= ((rook_pos - moves_out) & 0x3F) << 6;
                                legal_moves.push_back(move);*/
                            }else 
                            {
                                moves_right = moves_out - 1;
                            }
                        }else
                        {
                            /*uint16_t move = 0;
                            move |= (rook_pos & 0x3F);
                            move |= ((rook_pos - moves_out) & 0x3F) << 6;
                            legal_moves.push_back(move);*/
                        }
                    }


                }
            }
            
        std::cout << "rook_pos: " << static_cast<int>(rook_pos) << std::endl;
        std::cout << "moves_up: " << static_cast<int>(moves_up) << std::endl;
        std::cout << "moves_down: " << static_cast<int>(moves_down) << std::endl;
        std::cout << "moves_left: " << static_cast<int>(moves_left) << std::endl;
        std::cout << "moves_right: " << static_cast<int>(moves_right) << std::endl;
        }


    
    }
    for (int i = 0; i < legal_moves.size(); i++) {
        std::cout << std::bitset<16>(legal_moves[i]) << " ";
    }
    return legal_moves.data();
}


uint16_t *Bitboards::get_legal_moves()
{
    uint16_t *arr= get_legal_rook_moves();

    
    return nullptr;
}