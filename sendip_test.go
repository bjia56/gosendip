package gosendip

import (
	"fmt"
	"net"
	"testing"
	"time"
)

func TestSendIP_IPv4_UDP(t *testing.T) {
	// Start a UDP listener on a random port
	addr, err := net.ResolveUDPAddr("udp4", "127.0.0.1:0")
	if err != nil {
		t.Fatalf("Failed to resolve UDP address: %v", err)
	}
	conn, err := net.ListenUDP("udp4", addr)
	if err != nil {
		t.Fatalf("Failed to listen on UDP: %v", err)
	}
	defer conn.Close()

	port := conn.LocalAddr().(*net.UDPAddr).Port

	// Prepare sendip args to send a UDP packet to the listener
	args := []string{
		"-d", "HelloWorld",
		"-p", "ipv4",
		"-p", "udp",
		"-ud", fmt.Sprintf("%d", port),
		"127.0.0.1",
	}

	// Run SendIP in a goroutine
	done := make(chan struct{})
	go func() {
		_, err := SendIP(args)
		if err != nil {
			t.Errorf("SendIP failed: %v", err)
		}
		close(done)
	}()

	// Wait for a packet
	conn.SetReadDeadline(time.Now().Add(2 * time.Second))
	buf := make([]byte, 1024)
	n, _, err := conn.ReadFromUDP(buf)
	if err != nil {
		t.Fatalf("Did not receive UDP packet: %v", err)
	}
	if n == 0 {
		t.Fatalf("Received empty UDP packet")
	}

	<-done
}
