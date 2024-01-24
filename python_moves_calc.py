import chess
import time

def decode_move(move, board):
    # Überprüfen, ob der Zug eine Rochade ist
    if board.is_kingside_castling(move):
        return "e1g1" if board.turn == chess.WHITE else "e8g8"
    elif board.is_queenside_castling(move):
        return "e1c1" if board.turn == chess.WHITE else "e8c8"

    # Extrahieren der Start- und Zielpositionen
    start = move.from_square
    end = move.to_square
    is_promotion = move.promotion is not None
    promotion_piece = ''

    if is_promotion:
        promotion_type = move.promotion
        if promotion_type == chess.KNIGHT:
            promotion_piece = 'N'
        elif promotion_type == chess.BISHOP:
            promotion_piece = 'B'
        elif promotion_type == chess.ROOK:
            promotion_piece = 'R'
        else:
            promotion_piece = 'Q'

    # Umwandeln der Positionen in Schachnotation
    start_square = chess.square_name(start)
    end_square = chess.square_name(end)

    # Zusammensetzen des Zuges
    decoded_move = start_square + end_square

    if is_promotion:
        decoded_move += "=" + promotion_piece

    return decoded_move

def main():
    start_time = time.time()

    with open('FEN-tests/Standard/FEN-in/short_fen_25M.txt', 'r') as infile, open('FEN-tests/Standard/Moves-out/py_moves_25M.txt', 'w') as outfile:
        for line in infile:
            board = chess.Board(line.strip())
            legal_moves = board.legal_moves

            for move in legal_moves:
                decoded_move = decode_move(move, board)
                outfile.write(decoded_move + " ")
           
            outfile.write('\n')

    end_time = time.time()
    execution_time = end_time - start_time
    print("Execution time:", execution_time, "seconds")



main()
