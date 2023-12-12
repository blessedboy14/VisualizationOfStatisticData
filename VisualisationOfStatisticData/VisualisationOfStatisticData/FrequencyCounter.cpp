#include "FrequencyCounter.h"
#include <algorithm>

std::map<std::string, int> FrequencyCounter::countFrequencyEach(std::vector<char*> data) {
	std::map<std::string, int> frequencyMap;
	for (const auto& var : data) {
		frequencyMap[var]++;
		if (frequencyMap.size() > FrequencyCounter::maxItems) {
			throw new std::exception("This data don't have something in common");
		}
	}
	return frequencyMap;
}

std::map<std::string, int> FrequencyCounter::countAlphabetFrequency(std::vector<char*> data) {
	std::map<std::string, int> freq;
	for (auto line : data) {
		char* temp = line;
		while (*temp) {
			char c = toupper(*temp);
			if (c >= 'A' && c <= 'Z') {
				std::string stringVal(1, c);
				freq[stringVal]++;
				if (freq.size() > FrequencyCounter::maxItems) {
					throw new std::exception("This data don't have something in common");
				}
			}
			temp++;
		}
	}
	return freq;
}

std::map<std::string, int> FrequencyCounter::countWordsFrequency(std::vector<char*> data) {
	std::map<std::string, int> freq;
	for (auto line : data) {
		std::string str(line);
		std::transform(str.begin(), str.end(), str.begin(),
			[](unsigned char c) { return std::tolower(c); });
		freq[str]++;
	}
	std::multimap<int, std::string> sortedFreq;
	for (auto& pair : freq) {
		sortedFreq.insert({ pair.second, pair.first });
	}
	int count = 0;
	std::map<std::string, int> resultMap;
	for (auto itr = sortedFreq.rbegin(); itr != sortedFreq.rend() && count < FrequencyCounter::wordsDisplay; ++itr) {

		resultMap[itr->second] = itr->first;
		count++;

	}
	return resultMap;
}
