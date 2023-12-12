#include <map>
#include <string>
#include <vector>

class StatisticResolver {
public:
	static std::map<std::string, double> countMathParams(std::map<std::string, int> data);
	static std::map<std::string, double> KSTestForInt(std::vector<char*> data);
private:
	static bool compareIntegers(const char* a, const char* b);
	static double alpha;
	static std::map<int, double> KSTable;
};