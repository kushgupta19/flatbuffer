#include <iostream>
#include <fstream>
#include <vector>
#include "flatbuffers/flatbuffers.h"
#include "schema_generated.h"
#include "person.fbs"

using namespace MySpace;

int main() {
    flatbuffers::FlatBufferBuilder builder;

    // Create a person
    auto person = CreatePerson(builder, builder.CreateString("Ram"), 21, 76.5, builder.CreateString("Male"));

    // Create a group
    std::vector<flatbuffers::Offset<flatbuffers::String>> groupNames;
    groupNames.push_back(builder.CreateString("Ram"));
    groupNames.push_back(builder.CreateString("Shyam"));
    groupNames.push_back(builder.CreateString("Raghuveer"));

    auto group = CreateGroup(builder, builder.CreateString("FightClub"), 24.5, 66, builder.CreateVector(groupNames));

    // Create a client (either person or group)
    ClientBuilder clientBuilder(builder);
    clientBuilder.add_Person(person);
    
    // Uncomment the line to encode a group instead
    // clientBuilder.add_Group(group);

    auto client = clientBuilder.Finish();

    // Finish the buffer
    builder.Finish(client);

    // Save to a binary file
    const char* filename = "fb_bytes.bin";
    std::ofstream file(filename, std::ios::binary);
    file.write(reinterpret_cast<const char*>(builder.GetBufferPointer()), builder.GetSize());

    std::cout << "Encoding completed successfully." << std::endl;

    return 0;
}
