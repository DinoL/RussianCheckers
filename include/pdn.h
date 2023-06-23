#ifndef PDN_H
#define PDN_H

#include "history.h"

#include <vector>
#include <utility>

class PDN
{
public:
    PDN();

    static PDN from_history(const History& hist);

    std::vector<std::pair<int, int>> _moves;
};

#endif // PDN_H
