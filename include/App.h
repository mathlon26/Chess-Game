#ifndef APP__H
#define APP__H

class App
{
public:
    App() {};
    ~App() {};

    void Exit() {m_exitCall = true;}
    bool ShouldRun() {return !m_exitCall;}
    virtual void Init() = 0;
    virtual void Update() = 0;
private:
    bool m_exitCall {false};
};

#endif // APP__H