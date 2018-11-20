package main

import (
	"fmt"
	"io"

	"github.com/evnix/pcap-reader"
)

func main() {

	fname := "/vagrant/packet.pcap"
	pcaprdr := pcapreader.PCapReader{}
	pcaprdr.Open(fname)
	defer pcaprdr.Close()
	fmt.Println(pcaprdr.PcapHdr.MagicNumber)

	for {
		packetHeader, data, err := pcaprdr.ReadNextPacket()
		if err == io.EOF {
			fmt.Println("Reached end of file.")
			break
		}
		fmt.Println("Packet Header:", packetHeader)
		fmt.Println("Data:", data)
	}

}
