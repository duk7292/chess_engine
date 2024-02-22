import chess

def perft(board, depth):
    """
    Generiert rekursiv alle Züge bis zu einer bestimmten Tiefe und gibt die Knoten an der Ziel-Tiefe zurück.

    :param board: Die Schachstellung als Objekt, das Züge generieren kann.
    :param depth: Die Tiefe, bis zu der Züge generiert werden sollen.
    :return: Eine Liste von FEN-Strings der Stellungen an der Ziel-Tiefe.
    """
    if depth == 0:
        return [board.fen()]
    
    nodes = []
    for move in board.legal_moves:
        board.push(move)
        if depth == 1:
            nodes.extend(perft(board, depth - 1))
        else:
            nodes.extend(perft(board, depth - 1))
        board.pop()
    return nodes

def write_nodes_to_file(nodes, file_name):
    """
    Schreibt die Knoten (FEN-Strings) in eine Datei.

    :param nodes: Die Liste der FEN-Strings.
    :param file_name: Der Name der Datei.
    """
    with open(file_name, 'w') as file:
        for node in nodes:
            file.write(node + '\n')

if __name__ == "__main__":
    start_fen = "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1"  # Standard Schach Startposition
    depth = 3
    board = chess.Board(start_fen)  # Initialisiert das Brett mit der Startposition aus dem FEN-String
    nodes = perft(board, depth)
    write_nodes_to_file(nodes, f'nodes_depth_{depth}.txt')
