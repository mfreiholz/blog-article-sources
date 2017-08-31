#include "parser.h"

#ifdef _WIN32
#include <WinSock2.h>
#else
#include <arpa/inet.h>
#endif

bool
Parser::parse(uint8_t* data, size_t len, size_t& bytesRead, Packet& packet)
{
	bytesRead = 0;
	for (size_t i = 0; i < len; ++i)
	{
		const auto b = data[i];
		bytesRead++;

		switch (_step)
		{
			// Header.
			case 0:
				if (b != 0xAF)
				{
					_step = 0;
					continue;
				}
				packet.header = b;
				_step++;
				break;

			// Flags.
			case 1:
				packet.flags = b;
				_step++;
				break;

			// Type (2 bytes!).
			case 2:
				packet.type = uint16_t(b) << 8;
				_step++;
				break;

			case 3:
				packet.type |= uint16_t(b) << 0;
				packet.type = ntohs(packet.type);
				_step++;
				break;

			// Size (4 bytes!).
			case 4:
				packet.size = uint32_t(b) << 24;
				_step++;
				break;

			case 5:
				packet.size |= uint32_t(b) << 16;
				_step++;
				break;

			case 6:
				packet.size |= uint32_t(b) << 8;
				_step++;
				break;

			case 7:
				packet.size |= uint32_t(b) << 0;
				packet.size = ntohl(packet.size);
				_step++;

				packet.data.clear();
				if (packet.size > 0)
				{
					packet.data.reserve(packet.size);
				}
				else
				{
					_step++; // Skip data step.
				}
				break;

			// Data.
			case 8:
				packet.data.push_back(b);

				if (packet.data.size() == packet.size)
					_step++;
				break;

			// Checksum.
			case 9:
				packet.checksum = b;
				_step = 0;
				return true;

		}
	}
	return false;
}