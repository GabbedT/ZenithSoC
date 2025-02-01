import socket
import struct
import numpy as np
import matplotlib.pyplot as plt
from scipy.io import wavfile

def capture_ethernet_packets(interface_name, output_file, num_packets=600):
    # Crea un socket raw ethernet
    s = socket.socket(socket.AF_PACKET, socket.SOCK_RAW, socket.htons(3))
    s.bind((interface_name, 0))
    
    # Apre il file per scrivere il payload
    with open(output_file, 'w') as f:
        collected_data = bytearray()
        
        for i in range(num_packets):
            # Riceve il pacchetto
            packet = s.recv(65535)
            
            # Estrae il payload (salta i primi 14 bytes dell'header ethernet)
            payload = packet[14:-4]
            collected_data.extend(payload)
            
            # Scrive il payload nel file in gruppi di due bytes
            for j in range(0, len(payload), 2):
                if j + 1 < len(payload):
                    hex_bytes = payload[j:j+2].hex()
                    f.write(f"{hex_bytes} ")
                else:
                    hex_bytes = payload[j:].hex()
                    f.write(hex_bytes)
            f.write("\n")
    
    return collected_data

def create_wav_file(data, output_wav, sample_rate=48000):
    # Converte i bytes in int16 (PCM 16-bit signed)
    audio_data = np.frombuffer(data, dtype=np.int16)
    audio_data = audio_data.byteswap()
    
    # Salva come file WAV
    wavfile.write(output_wav, sample_rate, audio_data)
    
    return audio_data

def plot_audio_waveform(audio_data, sample_rate=48000):
    # Crea l'asse del tempo
    time = np.arange(len(audio_data)) / sample_rate
    
    # Crea il grafico
    plt.figure(figsize=(15, 5))
    plt.plot(time[100:], audio_data[100:])
    plt.title('Audio Waveform')
    plt.xlabel('Time (seconds)')
    plt.ylabel('Amplitude')
    plt.grid(True)
    plt.savefig('waveform.png')
    plt.close()

def main():
    interface_name = "enp3s0"
    payload_file = "payload.txt"
    wav_file = "output.wav"
    
    try:
        print("Cattura pacchetti in corso...")
        collected_data = capture_ethernet_packets(interface_name, payload_file)
        
        print("Creazione file WAV...")
        audio_data = create_wav_file(collected_data, wav_file)
        
        print("Creazione grafico della forma d'onda...")
        plot_audio_waveform(audio_data)
        
        print("Elaborazione completata!")
        print(f"File creati: {payload_file}, {wav_file}, waveform.png")
        
    except PermissionError:
        print("Errore: È necessario eseguire lo script con privilegi di root per catturare pacchetti raw")
    except Exception as e:
        print(f"Si è verificato un errore: {str(e)}")

if __name__ == "__main__":
    main()