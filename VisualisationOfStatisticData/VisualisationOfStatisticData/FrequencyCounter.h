#pragma once
#include <map>
#include <vector>
#include <string>

class FrequencyCounter {
public:
	std::map<std::string, int> countFrequencyEach(std::vector<char*>);
	std::map<std::string, int> countAlphabetFrequency(std::vector<char*>);
	std::map<std::string, int> countWordsFrequency(std::vector<char*>);
private:
	const char* charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	const int maxItems = 30;
	const int wordsDisplay = 20;
};
