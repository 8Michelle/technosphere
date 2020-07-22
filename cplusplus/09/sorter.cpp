#include "sorter.h"

const size_t BUFSIZE = 1024 * 1024;
const size_t FILESIZE = 1024 * 1023;

std::recursive_mutex lock_1_, lock_2_, lock_;

void sort(std::ofstream& out) {
	std::thread thread1(handler, std::ref(out));
	std::thread thread2(handler, std::ref(out));
	thread1.join();
	thread2.join();
}

void handler(std::ofstream& out) {
	std::ifstream in("input", std::ios::binary);
	std::vector<std::string> file_names;
	for (int i = 0; !in.eof(); ++i) {
		lock_.lock();
		std::string file_name = "output_" + std::to_string(i);
		file_names.push_back(file_name);
		std::unique_ptr<uint64_t> buffer(new uint64_t [BUFSIZE]);
		in.read((char*) buffer.get(), BUFSIZE);
		size_t count = in.gcount();
		if(count) {
			size_t read_ = count / (2 * sizeof(uint64_t));
			std::sort(buffer.get(), buffer.get() + 2 * read_);
			out.open(file_name, std::ios::binary);
			out.write((char*) buffer.get(), read_ * sizeof(uint64_t));
			out.close();
			++i;
			file_name = "output_" + std::to_string(i);
			file_names.push_back(file_name);
			out.open(file_name, std::ios::binary);
			out.write((char*) (buffer.get() + read_), count - read_ * sizeof(uint64_t));
			out.close();
		}
		lock_.unlock();
	}
	if (lock_1_.try_lock())
		return;
	merge(file_names);
	clean(file_names);
	in.close();
}

void merge(std::vector<std::string>& file_names) {
	file_copy(file_names[0], "output");
	std::ifstream infile("file_copy", std::ios::binary);
	std::ofstream out("output", std::ios::binary);
	uint64_t count_1, count_2;
	for (size_t i = 1; i < file_names.size(); ++i) {
		file_copy("output", "file_copy");
		std::ifstream tmp_file(file_names[i], std::ios::binary);
		infile.read((char*)&count_1, sizeof(count_1));
		tmp_file.read((char*)&count_2, sizeof(count_2));
		while (!tmp_file.eof()) {
			if(!infile.gcount()) {
				while (!tmp_file.eof()) {
					out.write((char*)&count_2, sizeof(count_2));
					tmp_file.read((char*)&count_2, sizeof(count_2));
				}
			}
			else if (count_2 <=  count_1) {
				out.write((char*)&count_2, sizeof(count_2));
				tmp_file.read((char*)&count_2, sizeof(count_2));
			}
			else {
				out.write((char*)&count_1, sizeof(count_1));
				infile.read((char*)&count_1, sizeof(count_1));
			}
		}
		if (infile.gcount()) {
			out.write((char*)&count_1, sizeof(count_1));
			std::unique_ptr<uint64_t> buffer(new uint64_t[BUFSIZE]);
			while (!infile.eof()) {
				infile.read((char*)buffer.get(), BUFSIZE);
				out.write((char*)buffer.get(), infile.gcount());
			}
		}
	}
}

void file_copy(const std::string& file_name1, const std::string& file_name2) {
    lock_2_.lock();
    std::unique_ptr<uint64_t> buffer(new uint64_t[BUFSIZE]);
    std::ifstream infile(file_name1, std::ios::binary);
    std::ofstream outfile(file_name2, std::ios::binary);
    while (!infile.eof()) {
		infile.read((char*)buffer.get(), BUFSIZE);
        if (infile.gcount()) {
			outfile.write((char*)buffer.get(), infile.gcount());
        }
    }
	outfile.close();
	infile.close();
    lock_2_.unlock();
}

void clean(std::vector<std::string>& file_names) {
    for (auto &file_name: file_names) {
		remove(file_name.c_str());
    }
	remove("file_copy");
}

void random_sample_file(std::ofstream &out) {
	for (int i = 0; i < FILESIZE; ++i) {
		uint64_t rand_num = rand() % 100;
		out.write((char *)&rand_num, sizeof(rand_num));
	}
}

int check() {
	std :: ifstream in("output", std::ios::binary);
	std::unique_ptr<uint64_t> buffer(new uint64_t[BUFSIZE]);
	bool sorted = true;
	while (!in.eof()) {
		in.read((char*) buffer.get(), BUFSIZE);
		auto elem = buffer.get()[0];
		for (size_t i = 1; buffer.get()[i]; i++) {
			if (buffer.get()[i] < elem) {
				sorted = false;
				break;
			} else {
				elem = buffer.get()[i];
			}
		}
		if (!sorted) {
			break;
		}
	}
	if (sorted)
		std::cout << "OK. Sorting completed\n";
	else
		std::cout << "ERROR.\n";
	return 0;
}
