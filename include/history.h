#ifndef HISTORY_H
#define HISTORY_H

#include "boardstate.h"

#include <vector>

class History
{
public:
    History();

    bool empty() const
    {
        return _data.empty();
    }

    void push(const BoardState& data)
    {
        _data.push_back(data);
    }

    BoardState operator[](size_t i) const
    {
        return _data[i];
    }

    size_t size() const
    {
        return _data.size();
    }

    void clear()
    {
        _data.clear();
    }

    void pop()
    {
        if (!empty())
        {
            _data.pop_back();
        }
    }

    BoardState top() const
    {
        return _data.back();
    }

private:
    std::vector<BoardState> _data;
};

#endif // HISTORY_H
