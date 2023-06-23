#include "history.h"

History::History()
{}

bool History::empty() const
{
    return _data.empty();
}

void History::push(const BoardState& data)
{
    _data.push_back(data);
}

BoardState History::operator[](size_t i) const
{
    return _data[i];
}

size_t History::size() const
{
    return _data.size();
}

void History::clear()
{
    _data.clear();
}

void History::pop()
{
    if (!empty())
    {
        _data.pop_back();
    }
}

BoardState History::top() const
{
    return _data.back();
}
