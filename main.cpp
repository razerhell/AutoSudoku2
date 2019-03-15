#include <iostream>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <cstdint>

#include "zsodukomatrix.hpp"


std::unordered_map<uint16_t, uint16_t> z_flag_to_num;
std::vector<uint16_t> z_num_to_flag;

int LoadFileToVector(std::string fileName, std::vector<uint16_t>& data);

int main(int argc, char* argv[])
{
	
	ZSodukoMatrix m;
	std::vector<uint16_t> data;
	LoadFileToVector("127.txt", data);

	m.SetData(data);

	m.SolveSoduko();

	m.PrintToConsole();

	return 0;
}

int LoadFileToVector(std::string fileName, std::vector<uint16_t>& data)
{
	data.resize(81);
	std::ifstream in(fileName);
	if (!in.is_open()) return 0;
	int i = 0;
	while (i < 81 && !in.eof())
		in >> data[i++];
	return 1;
}