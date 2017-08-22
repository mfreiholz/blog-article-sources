#pragma once
#include <stdint.h>
#include <vector>

#ifdef _WIN32
#include <WinSock2.h>
#else
#include <arpa/inet.h>
#endif

class Packet
{
public:
	Packet();
	~Packet();

	/*
		Serializes the packet for network transport.
		Fields are written to `buf` in network byte order (big-endian).
	*/
	void serialize(std::vector<uint8_t>& buf);

public:
	uint8_t header = 0xAF; // Marks the begin of `Packet`
	uint8_t flags = 0x00; // Custom bitmask flags.
	uint16_t type = 0x0000; // Identifies the packet's type - what is it used for.
	uint32_t size = 0x00000000; // Size of `data`.
	std::vector<uint8_t> data; // Custom data.
	uint8_t checksum = 0x00; // XOR checksum.
};