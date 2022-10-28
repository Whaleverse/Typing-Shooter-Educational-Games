#ifndef PARTITIONER_H
#define PARTITIONER_H
#include <vector>
#include <string>
#include <algorithm>


// A class for partitioning string data structures based on whether the item passes
// any of the rule functions set
typedef bool (* isInAny)(std::string items);
class Partitioner {
    std::vector<isInAny> rules;

public:
    // Create a partitioner with a rule
    Partitioner(isInAny rule);

    // Adds a new rule to the partition
    void addRule(isInAny newRule);

    // Returns whether the item is in the partition (matches _any_ of the rules)
    bool contains(std::string item);

    // Returns a new partitioned vector based on whether the partition contains() it
    std::vector<std::string> partition(std::vector<std::string> items);
};

#endif // PARTITIONER_H
