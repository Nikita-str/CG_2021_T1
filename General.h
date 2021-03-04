#ifndef MAIN_GENERAL_H
#define MAIN_GENERAL_H

#include <iostream>
#include <string>

static constexpr int TILE_SZ = 32;

static constexpr int W_WIDTH = 1024;
static constexpr int W_HEIGHT = 720;
static constexpr int W2_WIDTH = W_WIDTH / 2;
static constexpr int W2_HEIGHT = W_HEIGHT / 2;
static constexpr int W_DIAG = W_WIDTH * W_WIDTH + W_HEIGHT * W_HEIGHT;
static constexpr int W2_DIAG = W2_WIDTH * W2_WIDTH + W2_HEIGHT * W2_HEIGHT;

struct Point
{
    int x;
    int y;
};

inline bool operator< (const Point a, const Point b)
{
    if (a.x != b.x)return a.x < b.x;
    return a.y < b.y;
}

inline bool operator== (const Point a, const Point b)
{
    return (a.x == b.x) && (a.y == b.y);
}

inline Point operator* (const Point a, const int mul) { return {a.x * mul, a.y * mul}; }
inline Point operator/ (const Point a, const int div) { return {a.x / div, a.y / div}; }

inline Point operator+ (const Point a, const Point b)
{
    return {a.x + b.x, a.y + b.y};
}
struct Size
{
    int w;
    int h;
    bool IsEmpty()const { return w == 0 && h == 0; }
};


static constexpr Point TILE_2 {TILE_SZ / 2 , TILE_SZ / 2};

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

static inline bool Is_X_Dir(E_Dir dir) { return (dir == E_Dir::LEFT) || (dir == E_Dir::RIGHT); }
static inline bool Is_Y_Dir(E_Dir dir) { return (dir == E_Dir::UP) || (dir == E_Dir::DOWN); }

static inline void error(std::string s) { std::cout << s << std::endl; std::exit(1); }

static inline void end_trim(std::string &s)
{
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char c) {return !std::isspace(c);}).base(), s.end());
}

#endif