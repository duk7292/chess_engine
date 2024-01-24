def standard_to_custom_fen(standard_fen):
    parts = standard_fen.split(' ')
    
    # Standard-FEN-Teile
    board, turn, castling, en_passant, _, _ = parts

    # Umwandeln der Rochade-Optionen
    
    # Umwandeln der Rochade-Optionen
    castling_custom = ''.join([opt if opt in castling else '-' for opt in 'KQkq'])

    # Umwandeln der En-passant-Option
    en_passant_custom = '--' if en_passant == '-' else str(notation_to_index(en_passant))

    # Umwandeln der Zugfarbe
    turn_custom = '0' if turn == 'b' else '1'

    # Zusammenstellen des benutzerdefinierten FEN-Strings
    custom_fen = f"{board}|{castling_custom}|{en_passant_custom}|{turn_custom}"
    return custom_fen

def notation_to_index(notation):
    if notation == '-':
        return '-'
    file = notation[0]
    rank = int(notation[1])
    file_index = ord('h') - ord(file)
    return (rank - 1) * 8 + file_index


def process_fen_file(input_file_path, output_file_path):
    with open(input_file_path, 'r') as file:
        lines = file.readlines()

    with open(output_file_path, 'w') as file:
        for line in lines:
            custom_fen = standard_to_custom_fen(line.strip())
            file.write(custom_fen + '\n')

# Pfad der Eingabe- und Ausgabedatei
input_file_path = 'FEN-tests/Standard/FEN-in/short_fen_25M.txt'
output_file_path = 'FEN-tests/Custom/FEN-in/custom_fen_file_25M.txt'

#process_fen_file(input_file_path, output_file_path)
process_fen_file(input_file_path, output_file_path)

