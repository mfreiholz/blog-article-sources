#include "packet.h"
#include "parser.h"

int main(int argc, char** argv)
{
	// Network buffer.
	std::vector<uint8_t> buf;

	// CLIENT SIDE
	// Serialize and send packets.
	// Instead of sending we simply write them to a buffer `buf`.

	// 1st packet.
	Packet pkt1;
	pkt1.header = 0xAF;
	pkt1.flags = 0x00;
	pkt1.sequence = 0x0001;
	pkt1.size = 3;
	pkt1.data = { 0x11, 0x22, 0x33 };
	pkt1.checksum = 0xFF;
	pkt1.serialize(buf);

	// Add some random invalid bytes to network buffer.
	buf.push_back(0x87);
	buf.push_back(0x09);
	buf.push_back(0x43);

	// 2nd packet.
	Packet pkt2;
	pkt2.header = 0xAF;
	pkt2.flags = 0x80;
	pkt2.sequence = 0x0002;
	pkt2.size = 1;
	pkt2.data = { 0x88 };
	pkt2.checksum = 0xEE;
	pkt2.serialize(buf);

	// SERVER SIDE
	// Parse packets from network/buffer `buf`.

	// Parsing.
	Parser parser;
	Packet packet;
	auto p = buf.data();
	auto plen = buf.size();
	for (;;)
	{
		size_t bytesRead = 0;
		if (parser.parse(p, plen, bytesRead, packet))
		{
			// At this point the `packet` is complete.
			// todo: handlePacket(packet);
			printf("INFO new packet! sequence=%d; size=%d\n",
				   packet.sequence, packet.size);
		}
		if (bytesRead < plen)
		{
			p += bytesRead;
			plen -= bytesRead;
			continue;
		}
		break;
	}

	return 0;
}