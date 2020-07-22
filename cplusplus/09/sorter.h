#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <thread>
#include <algorithm>
#include <mutex>

void sort(std::ofstream& outfile);
void handler(std::ofstream& outfile);
void merge(std::vector<std::string>& file_names);
void file_copy(const std::string& file_name1, const std::string& file_name2);
void random_sample_file(std::ofstream &out);
void clean(std::vector<std::string>& file_names);
int check();
