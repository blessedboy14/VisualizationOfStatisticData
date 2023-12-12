#include "StatisticDataResolver.hpp"
#include <stdexcept>
#include <cmath>
#include <algorithm>

double StatisticResolver::alpha = 0.01;
std::map<int, double> StatisticResolver::KSTable = { {5, 0.7279}, {10, 0.5147}, {15, 0.4202}, {20, 0.3639}, {25, 0.3255},
	{30, 0.2972}, {40, 0.2574}, {50, 0.2302}, {60, 0.2101}, {70, 0.1945}, {80, 0.1820} };

bool StatisticResolver::compareIntegers(const char* a, const char* b) {
	return std::stoi(a) < std::stoi(b);
}

std::map<std::string, double> StatisticResolver::countMathParams(std::map<std::string, int> data) {
	int sum = 0;
	int all_count = 0;
	double sum_squared = 0.0;
	std::map<std::string, double> vals = { {"expect", -1.0}, {"dispersion", -1.0}, {"deviation", -1.0}};
	for (auto const& [key, val] : data) {
		try {
			sum += std::stoi(key) * val;
			all_count += val;
			sum_squared += std::stoi(key) * std::stoi(key) * val;
		}
		catch (std::invalid_argument e) {
			return vals;
		}
	}
	double expectation = static_cast<double>(sum) / all_count;
	double dispersion = (sum_squared / all_count) - (expectation * expectation);
	double deviation = std::sqrt(dispersion);
	vals["expect"] = expectation; vals["dispersion"] = dispersion; vals["deviation"] = deviation;
	return vals;
}

std::map<std::string, double> StatisticResolver::KSTestForInt(std::vector<char*> data) {
	std::sort(data.begin(), data.end(), StatisticResolver::compareIntegers);
	std::vector<double> plus_max;
	std::vector<double> minus_max;
	int N = data.size();
	for (int i = 1; i < N + 1; i++) {
		double x = i / N - std::stoi(data[i - 1]);
		double y = (i - 1) / N;
		y = std::stoi(data[i - 1]) - y;
		if (0 - x <= 0) {
			plus_max.push_back(x);
		}
		if (0 - y <= 0) {
			minus_max.push_back(y);
		}
	}
	double k_plus_max = std::sqrt(N) * *std::max_element(plus_max.begin(), plus_max.end());
	double k_minus_max = std::sqrt(N) * *std::max_element(minus_max.begin(), minus_max.end());
	double k_max = std::max(k_plus_max, k_minus_max);
	double table_var = 1.63 / std::sqrt(N);
	std::map<std::string, double> res = { {"test_val", k_max}, {"table_val", table_var} };
	return res;
}