#include <iostream>
#include <fstream>
#include <vector>
#include "flatbuffers/flatbuffers.h"
#include "schema_generated.h"
#include "person.fbs"
using namespace MySpace;

void printPerson(const Person* person) {
    std::cout << "Person: Name = " << person->Name()->c_str()
              << ", Age = " << person->Age()
              << ", Weight = " << person->Weight()
              << ", Gender = " << person->Gender()->c_str()
              << std::endl;
}

void printGroup(const Group* group) {
    std::cout << "Group: GroupName = " << group->GroupName()->c_str()
              << ", AvgAge = " << group->AvgAge()
              << ", AvgWeight = " << group->AvgWeight()
              << ", Names = [";

    const auto& names = *group->Names();
    for (flatbuffers::uoffset_t i = 0; i < names.size(); ++i) {
        std::cout << names.Get(i)->c_str();
        if (i < names.size() - 1) {
            std::cout << ", ";
        }
    }

    std::cout << "]" << std::endl;
}

int main() {
    // Read the  file
    const char* filename = "fb_bytes.bin";
    std::ifstream file(filename, std::ios::binary );
    file.seekg(0, std::ios::end);
    size_t length = file.tellg();
    file.seekg(0, std::ios::beg);
    std::vector<char> buffer(size);
    file.read(buffer.data(), length);
    file.close();
    // Verify the buffer
    if (!VerifyClientBuffer(Verifier(buffer,length)) {
        std::cout << "Error: Invalid buffer format" << std::endl;
        return 1;
    }

    // Access the root of the buffer
    auto client = GetClient(buffer.data());

    // Print the content based on the type
    if (client->type() == Any_Person) {
        printPerson(client->Any_as_Person());
    } else if (client->type() == Any_Group) {
        printGroup(client->Any_as_Group());
    } else {
        std::cout << "Error: Unknown client type" << std::endl;
        return 1;
    }

    return 0;
}
