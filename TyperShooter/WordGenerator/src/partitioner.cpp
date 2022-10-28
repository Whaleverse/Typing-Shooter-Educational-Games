#include "partitioner.h"

typedef bool (* isInAny)(std::string items);
// Create a partitioner with a rule
Partitioner::Partitioner(isInAny rule) {
    addRule(rule);
}

// Adds a new rule to the partition
void Partitioner::addRule(isInAny newRule) {
    rules.push_back(newRule);
}

// Returns whether the item is in the partition (matches any of the rules)
bool Partitioner::contains(std::string item) {
    return std::any_of(rules.begin(), rules.end(), [item](isInAny rule) {
        return rule(item);
    });
}

// Returns a new partitioned vector (matching any of the rules)
std::vector<std::string> Partitioner::partition(std::vector<std::string> items) {
    std::vector<std::string> newVector;
    for (auto item : items)
        if (contains(item)) newVector.push_back(item);
    return newVector;
}