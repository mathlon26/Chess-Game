#ifndef CANVAS__H
#define CANVAS__H

#include "../dependencies/Style.hpp"

class Canvas : public Style
{
public:
    Canvas() : Style{} {};
    ~Canvas() = default;

    virtual void Draw() const = 0;
private:
};

#endif // CANVAS__H