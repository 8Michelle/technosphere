#include <vector>
#include <ostream>

using Test = int (*)();

int testInit();
int testDiffTypes();
int testExceptions();

class TestType {
public:
	TestType(std::vector<int> testFields);
	friend std::ostream& operator<<(std::ostream& out,
			const TestType& testObject);
private:
	std::vector<int> _testFields;
};