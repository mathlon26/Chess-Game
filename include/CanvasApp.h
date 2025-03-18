#ifndef CANVAS_APP__H
#define CANVAS_APP__H

#include "Canvas.h"
#include "App.h"

class CanvasApp : public Canvas, public App
{
public:
    CanvasApp() : Canvas{}, App{} {};
    ~CanvasApp() = default;;

    virtual void Init() override = 0;
    virtual void Update() override = 0;
    virtual void Draw() const override = 0;
    virtual void Run() override = 0;

private:
};

#endif // CANVAS_APP__H