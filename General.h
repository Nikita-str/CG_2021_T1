#ifndef MAIN_GENERAL_H
#define MAIN_GENERAL_H
struct Point
{
    int x;
    int y;
};

struct GameTime
{
    //explicit GameTime(double time) : cur_sec(time) {};
    //static GameTime NowTime;

    static GameTime* Now() { static GameTime ret {}; return &ret; }

    inline void SetCur(double sec) { cur_sec = sec; }
    inline double GetSecAfter(double time_sec) { return cur_sec - time_sec; }
    inline double GetTime() { return cur_sec; }

private:
    double cur_sec = 0;
};

enum class E_Dir
{
    UP,
    DOWN,
    LEFT,
    RIGHT
};

#endif