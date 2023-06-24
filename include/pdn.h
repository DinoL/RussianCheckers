#ifndef PDN_H
#define PDN_H

#include "history.h"

#include <vector>
#include <utility>

class PDN
{
public:
    PDN();

    struct Move
    {
        std::vector<int> _cells;
        bool _is_eat;
    };

    static PDN from_history(const History& hist);

    std::vector<Move> _moves;
};

#endif // PDN_H
