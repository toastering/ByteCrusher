#include <Windows.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

char previousb = 0;
std::string readline() {
	std::string returnme;
	std::getline(std::cin, returnme);
	return returnme;
}

std::string readf(const std::string& path) { //Nothing else read files correctly for some reason.
	auto close_file = [](FILE* f) {fclose(f); };
	auto holder = std::unique_ptr<FILE, decltype(close_file)>(fopen(path.c_str(), "rb"), close_file);
	if (!holder)
		return "";
	FILE* f = holder.get();
	if (fseek(f, 0, SEEK_END) < 0)
		return "";
	const long size = ftell(f);
	if (size < 0)
		return "";
	if (fseek(f, 0, SEEK_SET) < 0)
		return "";
	std::string res;
	res.resize(size);
	fread(const_cast<char*>(res.data()), 1, size, f);
	return res;
}

std::vector<std::string> split(const std::string& str, const std::string& delim)
{
	std::vector<std::string> tokens;
	size_t prev = 0, pos = 0;
	do
	{
		pos = str.find(delim, prev);
		if (pos == std::string::npos) pos = str.length();
		std::string token = str.substr(prev, pos - prev);
		if (!token.empty()) tokens.push_back(token);
		prev = pos + delim.length();
	} while (pos < str.length() && prev < str.length());
	return tokens;
}

char handlebyte(char byte, int method) {
	switch (method)
	{
	case 1:
		byte++;
		return byte;
	case 2:
		byte--;
		return byte;
	case 3:
		byte = byte ^ 'W';
		return byte;
	case 4:
		byte += 'W';
		return byte;
	case 5:
		byte = rand() % 256;
		return byte;
	case 6:
		byte = 0;
		return byte;
	case 7:
		byte = previousb;
		return byte;
	case 8:
		byte = byte * byte;
		return byte;
	case 9:
		byte = byte / byte;
		return byte;
	case 10:
		byte = (char)INT_MAX;
		return byte;
	case 11:
		byte = byte + rand() % 256;
		return byte;
	case 12:
		byte = byte - rand() % 256;
		return byte;
	case 13:
		byte = byte * rand() % 256;
		return byte;
	case 14:
		byte = byte / rand() % 256;
		return byte;
	default:
		return byte;
	}
}

int main() {
	srand(time(NULL));
	std::cout << "Please input file path: ";
	std::string path = readline();
	std::string fbytes = readf(path);

	if (fbytes == "") {
		std::cout << "Invalid file." << std::endl;
		main();
	}

	std::cout << fbytes.length() << std::endl;
	std::vector<std::string> lengthstuff = split(readline(), "-");
	int start = std::stoi(lengthstuff[0]); 
	int end = std::stoi(lengthstuff[1]);
	std::cout << "Please enter method: ";
	int method = std::stoi(readline());

	for (int i = start; i < end; i++) {
		char b = fbytes[i];
		fbytes[i] = handlebyte(fbytes[i], method);
		previousb = b;
	}

	std::ofstream stream;
	stream.open(std::string(split(path, ".")[0] + "_c." + split(path, ".")[1]), std::ios::binary | std::ios::ate);
	stream << fbytes;
	stream.close();

	std::cout << "Done!" << std::endl;

	main();
	return EXIT_SUCCESS;
}