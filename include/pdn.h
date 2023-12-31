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
        Move() = default;
        Move(const std::vector<int>& cells, bool is_eat)
            : _cells(cells)
            , _is_eat(is_eat)
        {}

        std::vector<int> _cells;
        bool _is_eat;

        bool operator==(const PDN::Move& other) const;
        bool valid() const;
    };

    static PDN from_history(const History& hist);
    History to_history() const;

    std::vector<Move> _moves;
};

#endif // PDN_H
