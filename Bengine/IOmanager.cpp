#include "IOManager.h"
#include <fstream>

namespace Bengine {

	bool IOManager::readFileToBuffer(std::string filePath, std::vector<unsigned char>& buffer)
	{
		std::ifstream file(filePath, std::ios::binary);
		if (file.fail()) {
			perror(filePath.c_str());
			return false;
		}

		// seek to the end
		file.seekg(0, std::ios::end);

		// get the file size
		int fileSize = file.tellg();
		file.seekg(0, std::ios::beg);

		// reduce useless fileheaders
		fileSize -= file.tellg();

		buffer.resize(fileSize);
		// trick to get char pointer from vector
		file.read((char*) & (buffer[0]), fileSize);

		file.close();

		return true;
	}


}