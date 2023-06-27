#ifndef HISTORY_H
#define HISTORY_H

#include "boardstate.h"

#include <vector>
#include <cstddef>

class History
{
public:
    History();
    History(const History& other);
    History& operator=(const History& other);

    bool empty() const;
    size_t size() const;
    void push(const BoardState& data);
    void clear();
    void resize(size_t size);
    void pop();
    BoardState top() const;
    BoardState operator[](size_t i) const;

private:
    std::vector<BoardState> _data;
};

#endif // HISTORY_H
