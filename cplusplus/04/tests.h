#include <cstdint>
#include <string>

struct Data {

	uint64_t a;
	bool b;
	uint64_t c;

	bool operator==(Data data);

	template <class Serializer>
	Error serialize(Serializer& serializer) {
		return serializer(a, b, c);
	}
};

int testSerializeDeserialize(Data data);
int testDeserializeString(std::string string, Data data);
int testDeserializeInvalidString(std::string string, Data data);