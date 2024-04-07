from PIL import Image

# Funzione per convertire un colore da 12-bit a 24-bit
def convert_12bit_to_24bit(color_12bit):
    # Assicurati che il colore sia nel formato corretto
    if len(color_12bit) != 4 or not all(c in '0123456789ABCDEFabcdef' for c in color_12bit[1:]):
        raise ValueError(f"Formato del colore non valido: {color_12bit}")
    # Estrai i componenti del colore
    r, g, b = color_12bit[1], color_12bit[2], color_12bit[3]
    # Duplica ogni cifra e convertila in un intero
    r, g, b = int(r*2, 16), int(g*2, 16), int(b*2, 16)
    return (r, g, b)

# Apri il file .txt e leggi le righe
file_path = '/home/gabbed/Projects/ZenithSoC/Testbench/VGA/vga_frames.txt'
with open(file_path, 'r') as file:
    lines = file.readlines()

# Crea una nuova immagine 640x480
img = Image.new('RGB', (640, 480))

# Processa ogni riga e ogni colore
for y, line in enumerate(lines):
    # Separa i colori nella riga
    colors = line.strip().split(' ')
    
    for x, color in enumerate(colors):
        # Converte il colore da 12-bit a 24-bit
        converted_color = convert_12bit_to_24bit(color)
        # Imposta il colore del pixel
        img.putpixel((x, y), converted_color)

# Salva l'immagine
img.save('output_image.png')