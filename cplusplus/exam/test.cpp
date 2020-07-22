#include "counter.h"
#include <iostream>
#include <fstream>

int generate_file(std::ofstream &outfile) {
	int total_sum = 0;
	for(int i = 0; i < 100; ++i){
		int num = rand() % 1000;
		outfile << num << " ";
		total_sum += num;
		if((i + 1) % 10 == 0 && i != 99)
			outfile << "\n";
	}
	return total_sum;
}

int main() {
	std::ofstream outfile("file");
	int true_total = generate_file(outfile);
	outfile.close();

	std::ifstream infile("file");
	int total = counter(infile);
	infile.close();

	std::cout << "Total sum: " << total << '\n';
	std::cout << "True total sum: " << true_total << '\n';
	if (total == true_total) {
		std::cout << "OK.\n";
	} else {
		std::cout << "Incorrect.\n";
	}

	return 0;
}
