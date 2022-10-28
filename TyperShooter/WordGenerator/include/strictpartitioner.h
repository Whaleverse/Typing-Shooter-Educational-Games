#ifndef STRICTPARTITIONER_H
#define STRICTPARTITIONER_H
#include "partitioner.h"
#include <string>
#include <vector>

// A stricter partitioner that adds a item to the partition based on all the rules,
// as opposed to any of the rules, like Partitioner does
typedef bool (* isInAll)(std::string items);
class StrictPartitioner : public Partitioner {
    std::vector<isInAll> rules;

public:
    // Create a partitioner with a rule
    StrictPartitioner(isInAll rule);

    // Returns whether the item is in the partition (matches **all** of the rules)
    bool contains(std::string item);
};

#endif // STRICTPARTITIONER_H
