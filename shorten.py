def copy_every_nth_line(input_file_path, output_file_path, n):
    with open(input_file_path, 'r') as file:
        lines = file.readlines()

    with open(output_file_path, 'w') as file:
        for i, line in enumerate(lines):
            if (i + 1) % n == 0:  # (i + 1) da enumerate bei 0 beginnt
                file.write(line)

# Pfad der Eingabe- und Ausgabedatei
input_file_path = 'FEN-tests/Standard/FEN-in/unique00.fen'
output_file_path = 'short_fen_25M.txt'
n = 2  # Beispiel: Kopiere jede fünfte Zeile

copy_every_nth_line(input_file_path, output_file_path, n)
