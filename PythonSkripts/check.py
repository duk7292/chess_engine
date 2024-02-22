

def compare_moves(file1, file2):
    dif_count = 0
    with open(file1, 'r') as f1, open(file2, 'r') as f2:
        for i, (line1, line2) in enumerate(zip(f1, f2), start=1):
            set1 = set(line1.strip().split())
            set2 = set(line2.strip().split())
            if set1 != set2:
                missing_in_file1 = set2 - set1
                missing_in_file2 = set1 - set2
                if missing_in_file1:
                    print(f"{i} Moves in File 2 not in File 1: {missing_in_file1}")
                if missing_in_file2:
                    print(f"{i} Moves in File 1 not in File 2: {missing_in_file2}")

                dif_count += 1
            if(i % 500000 == 0):
                print(i)
            
    print(f"{dif_count} lines differ.")
        

def main():
    file1 = 'FEN-tests/Custom/Moves-out/custom_moves_file_25M.txt'  # Datei 1
    file2 = 'FEN-tests/Standard/Moves-out/py_moves_25M.txt'   # Datei 2 (ersetzen Sie dies durch den Namen Ihrer zweiten Datei)
    compare_moves(file1, file2)

if __name__ == "__main__":
    main()
