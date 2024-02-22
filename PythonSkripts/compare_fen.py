def read_board_positions_from_file(file_path):
    position_counts = {}
    with open(file_path, 'r') as file:
        for line in file:
            board_position = line.strip().split(' ')[0]  # Extracts the part before the first space
            if board_position in position_counts:
                position_counts[board_position] += 1
            else:
                position_counts[board_position] = 1
    return position_counts

def find_differences(counts1, counts2):
    all_positions = set(counts1.keys()) | set(counts2.keys())
    diff1 = {position: counts1[position] for position in all_positions if position in counts1 and position not in counts2}
    diff2 = {position: counts2[position] for position in all_positions if position in counts2 and position not in counts1}
    shared_positions = {position for position in all_positions if position in counts1 and position in counts2 and counts1[position] != counts2[position]}
    
    symmetric_diff = {position: (counts1.get(position, 0), counts2.get(position, 0)) for position in shared_positions}
    
    return symmetric_diff, diff1, diff2

def main(file1_path, file2_path):
    positions1_counts = read_board_positions_from_file(file1_path)
    positions2_counts = read_board_positions_from_file(file2_path)
    
    symmetric_diff, diff1, diff2 = find_differences(positions1_counts, positions2_counts)
    
    if symmetric_diff or diff1 or diff2:
        print("There are differences between the files.")
        
        if diff1:
            print("\nPositions only in file 1 or with different counts:")
            for position, count in sorted(diff1.items(), key=lambda item: item[1], reverse=True):
                print(f"{position}: {count}")
                
        if diff2:
            print("\nPositions only in file 2 or with different counts:")
            for position, count in sorted(diff2.items(), key=lambda item: item[1], reverse=True):
                print(f"{position}: {count}")
                
        if symmetric_diff:
            print("\nPositions with different counts in both files:")
            # Sort by the difference in counts, showing larger differences first, prioritizing file 1
            sorted_sym_diff = sorted(symmetric_diff.items(), key=lambda item: item[1][0] - item[1][1], reverse=True)
            for position, counts in sorted_sym_diff:
                print(f"{position}: File 1 Count - {counts[0]}, File 2 Count - {counts[1]}")
    else:
        print("The positions in both files are identical or have identical counts.")

# Adjust file paths
file1_path = 'nodes_depth_3.txt'
file2_path = 'perft_positions_depth_3.txt'
main(file1_path, file2_path)
