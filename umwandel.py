def standard_to_custom_fen(standard_fen):
    parts = standard_fen.split(' ')
    
    # Standard-FEN-Teile
    board, turn, castling, en_passant, _, _ = parts

    # Umwandeln der Rochade-Optionen
    castling_custom = ''.join(['-' if opt not in castling else opt for opt in 'KQkq'])

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
input_file_path = 'shot_fen.fen'
output_file_path = 'custom_fen_file.txt'

process_fen_file(input_file_path, output_file_path)

