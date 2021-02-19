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

    static GameTime& Now() { static GameTime ret {}; return ret; }

    inline void SetCur(double sec) { cur_sec = sec; }
    inline double GetSecAfter(double time_sec) { return cur_sec - time_sec; }
    inline double GetTime() { return cur_sec; }
    inline bool TimeCome(double event_time) { return cur_sec >= event_time; }

private:
    double cur_sec = 0;
};

enum class E_Dir
{
    UP,
    DOWN,
    LEFT,
    RIGHT,
};

enum class E_X_Dir{Not, Left, Right};
enum class E_Y_Dir{Not, Down, Up};

inline bool Is_X_Dir(E_Dir dir) { return (dir == E_Dir::LEFT) || (dir == E_Dir::RIGHT); }
inline bool Is_Y_Dir(E_Dir dir) { return (dir == E_Dir::UP) || (dir == E_Dir::DOWN); }

#endif