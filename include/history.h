#ifndef HISTORY_H
#define HISTORY_H

#include "boardstate.h"

#include <vector>

class History
{
public:
    History();

    bool empty() const;
    size_t size() const;
    void push(const BoardState& data);
    void clear();
    void pop();
    BoardState top() const;
    BoardState operator[](size_t i) const;

private:
    std::vector<BoardState> _data;
};

#endif // HISTORY_H
