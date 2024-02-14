def read_board_positions_from_file(file_path):
    positions = set()
    with open(file_path, 'r') as file:
        for line in file:
            board_position = line.strip().split(' ')[0]  # Extrahiert den Teil vor dem ersten Leerzeichen
            positions.add(board_position)
    return positions

def find_differences(set1, set2):
    
    # Finde Elemente, die nur in einem der beiden Sets vorkommen
    return set1.symmetric_difference(set2), set1.difference(set2), set2.difference(set1)

def main(file1_path, file2_path):
    positions1 = read_board_positions_from_file(file1_path)
    positions2 = read_board_positions_from_file(file2_path)
    
    symmetric_diff, diff1, diff2 = find_differences(positions1, positions2)
    
    if symmetric_diff:
        print("Es gibt Unterschiede zwischen den Dateien.") 
        
        if diff1:
            print("\nPositionen nur in Datei 1:")
            for position in diff1:
                print(position)
        if diff2:
            print("\nPositionen nur in Datei 2:")
            for position in diff2:
                print(position)
    else:
        print("Die Positionen in beiden Dateien sind identisch.")

# Dateipfade anpassen
file1_path = 'nodes_depth_3.txt'
file2_path = 'perft_positions_depth_3.txt'
main(file1_path, file2_path)
