import chess

def perft(board, depth, path=""):
    """
    Generiert rekursiv alle Züge bis zu einer bestimmten Tiefe und gibt die Knoten an der Ziel-Tiefe zurück,
    inklusive der Züge, die gemacht wurden, um jede Position zu erreichen, im spezifizierten Format.

    :param board: Die Schachstellung als Objekt, das Züge generieren kann.
    :param depth: Die Tiefe, bis zu der Züge generiert werden sollen.
    :param path: Der Pfad der gemachten Züge, um die aktuelle Position zu erreichen, als String.
    :return: Eine Liste von Strings, jeder enthält einen FEN-String der Positionen an der Ziel-Tiefe gefolgt vom Pfad der Züge.
    """
    if depth == 0:
        return [board.fen() + " " + path.strip()]
    
    nodes = []
    for move in board.legal_moves:
        from_square = chess.square_name(move.from_square)
        to_square = chess.square_name(move.to_square)
        promotion = ""
        if move.promotion:
            promotion = "=" + chess.piece_name(move.promotion).upper()
        move_string = f"{from_square}{to_square}{promotion}"
        board.push(move)
        new_path = f"{path} {move_string}".strip()
        nodes.extend(perft(board, depth - 1, new_path))
        board.pop()
    return nodes

def write_nodes_to_file(nodes, file_name):
    """
    Schreibt die Knoten (FEN-Strings mit Zugpfaden) in eine Datei.

    :param nodes: Die Liste der FEN-Strings mit Zugpfaden.
    :param file_name: Der Name der Datei.
    """
    with open(file_name, 'w') as file:
        for node in nodes:
            file.write(node + '\n')

if __name__ == "__main__":
    start_fen = "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1"  # Example chess starting position
    depth = 5  # Adjust depth as needed for testing
    board = chess.Board(start_fen)  # Initialize the board with the starting position from the FEN string
    nodes = perft(board, depth)
    write_nodes_to_file(nodes, f'nodes_path_depth_{depth}.txt')
