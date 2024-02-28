from socket import *
import time 

def sendeth(src, dst, eth_type, payload, interface = "enp3s0"):
  """Send raw Ethernet packet on interface."""

  assert(len(src) == len(dst) == 6) # 48-bit ethernet addresses
  assert(len(eth_type) == 2) # 16-bit ethernet type

  s = socket(AF_PACKET, SOCK_RAW)

  # From the docs: "For raw packet
  # sockets the address is a tuple (ifname, proto [,pkttype [,hatype]])"
  s.bind((interface, 0))

  packet = dst + src + eth_type + payload

  return s.send(packet)

if __name__ == "__main__":
  while (1):
    sendeth(b"\xD8\xBB\xC1\x57\xAA\x16",
            b"\xDE\xAD\xBE\xEF\x00\x00",
            # b"\xFF\xFF\xFF\xFF\xFF\xFF",
            b"\x00\x40",
            b"0101010101010101010101010101010101010101010101010101010101010101")
    
    time.sleep(5)