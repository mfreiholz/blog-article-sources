#include "packet.h"

Packet::Packet()
{}

Packet::~Packet()
{}

void
Packet::serialize(std::vector<uint8_t>& buf)
{
	buf.push_back(header);
	buf.push_back(flags);
	buf.push_back(htons(type) >> 8);
	buf.push_back(htons(type) >> 0);
	buf.push_back(htonl(size) >> 24);
	buf.push_back(htonl(size) >> 16);
	buf.push_back(htonl(size) >> 8);
	buf.push_back(htonl(size) >> 0);
	for (auto b : data)
		buf.push_back(b);
	buf.push_back(checksum);
}