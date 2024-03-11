from socket import *
import time
import threading
import os
import sys

def sendeth(src, dst, eth_type, payload, interface="enp3s0"):
    assert(len(src) == len(dst) == 6)
    assert(len(eth_type) == 2)

    s = socket(AF_PACKET, SOCK_RAW)
    s.bind((interface, 0))

    packet = dst + src + eth_type + payload
    return s.send(packet)

def change_interface_speed(interface, speed):
    speed_str = {"1": "100", "2": "10"}.get(speed, None)
    if speed_str:
        cmd = f"sudo ethtool -s {interface} speed {speed_str.split('m')[0]} duplex full autoneg off"
        print(cmd)
        os.system(cmd)
    else:
        print("Speed option is not valid.")

def listen_for_input(interface, packet_data):
    while True:
        i = input()
        if i == "0":
            print("Sending packet...")
            sendeth(*packet_data, interface=interface)
        elif i in ["1", "2"]:
            change_interface_speed(interface, i)

def main():
    interface = "enp3s0"
    src = b"\xD8\xBB\xC1\x57\xAA\x16"
    dst = b"\xDE\xAD\xBE\xEF\x00\x00"
    eth_type = b"\x00\x40"
    payload = b"0101010101010101010101010101010101010101010101010101010101010101"

    packet_data = (src, dst, eth_type, payload)

    threading.Thread(target=listen_for_input, args=(interface, packet_data), daemon=True).start()

    while True:
        sendeth(src, dst, eth_type, payload, interface)
        time.sleep(5)

if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        print("Exiting.")
        sys.exit(0)