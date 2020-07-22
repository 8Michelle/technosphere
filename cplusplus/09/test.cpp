#include "sorter.h"

int main() {
	std::ofstream file("input", std::ios::binary);
	random_sample_file(file);
	file.close();
	sort(file);
	check();
	return 0;
}
