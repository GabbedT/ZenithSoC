sudo ethtool -K enp3s0 rx-fcs on
sudo ethtool -K enp3s0 rx-all on
sudo ifconfig enp3s0 promisc