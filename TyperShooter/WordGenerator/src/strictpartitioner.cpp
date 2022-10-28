#include "strictpartitioner.h"

typedef bool (* isInAll)(std::string items);
// Create a partitioner with a rule
StrictPartitioner::StrictPartitioner(isInAll rule) : Partitioner(rule) {}

// Returns whether the item is in the partition (matches **all** of the rules)
bool StrictPartitioner::contains(std::string item) {
    return std::all_of(rules.begin(), rules.end(), [item](isInAll rule) {
        return rule(item);
    });
}
