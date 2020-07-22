#include "counter.h"
#include "threadPool.h"

int counter(std::ifstream& infile) {
	std::string line = "";
	ThreadPool pool(2);
	int total = 0;
	std::vector<std::future<int>> futures;
	while (!infile.eof()) {
		getline(infile, line);
		futures.push_back(pool.exec(handler, line));
	}
	for (int i = 0; i < futures.size(); i++) {
		total += futures[i].get();
	}
	return total;
}

int handler(std::string& line) {
	int acc = 0;
	int value = 0;
	std::istringstream ss(line);
	while (ss >> value) {
		acc += value;
	}
	return acc;
}
