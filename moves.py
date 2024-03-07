def generate_chess_moves():
    columns = 'abcdefgh'
    rows = '12345678'
    moves = []
    promotion_pieces = ['q', 'r', 'b', 'n']  # For pawn promotions
    
    # Generate moves for non-pawn pieces and non-castling scenarios
    for start_col in columns:
        for start_row in rows:
            for end_col in columns:
                for end_row in rows:
                    if start_col != end_col or start_row != end_row:  # Excludes no-moves
                        moves.append(f"{start_col}{start_row}{end_col}{end_row}")
    
    # Add pawn promotions, considering every possible promotion move
    for col in columns:
        for piece in promotion_pieces:
            # Promotion by moving forward to the last rank
            moves.append(f"{col}7{col}8={piece}")
            moves.append(f"{col}2{col}1={piece}")
            
            # Promotion by capturing to the left or right on the last rank
            if col > 'a':  # Can capture to the left
                moves.append(f"{col}7{chr(ord(col)-1)}8={piece}")
                moves.append(f"{col}2{chr(ord(col)-1)}1={piece}")
            if col < 'h':  # Can capture to the right
                moves.append(f"{col}7{chr(ord(col)+1)}8={piece}")
                moves.append(f"{col}2{chr(ord(col)+1)}1={piece}")
    
    # Castling moves, simplified representation
    moves.extend(['e1g1', 'e1c1', 'e8g8', 'e8c8'])
    
    return moves

all_moves = generate_chess_moves()
print(f"Total number of unique chess moves generated: {len(all_moves)}")
