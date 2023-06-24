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

        bool operator==(const PDN::Move& other) const
        {
            return _cells == other._cells
                && _is_eat == other._is_eat;
        }
    };

    static PDN from_history(const History& hist);

    std::vector<Move> _moves;
};

#endif // PDN_H
