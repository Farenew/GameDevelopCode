#pragma once
#include <vector>
#include <string>

namespace Bengine {

	// IOManager would be a static class, means it is only created once
	// so we don't need constructor and destroctur
	class IOManager
	{
	public:
		static bool readFileToBuffer(std::string filePath, std::vector<unsigned char>& buffer);
	};

}