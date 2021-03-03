#include "GameMap.h"
#include "Movement.h"

void Movement:: Move(int dx, int dy, int speed)
{
    double now_t = GameTime::Now().GetTime();

    if (dx || dy) {
        double dt = now_t - last_time;
        if (obj_size.IsEmpty()) {
            x += dt * dx * speed;
            y += dt * dy * speed;
            last_moved_time = now_t;
            moved = true;
        } else {

            bool move_x = false;
            if (dx) {
                double x_back = x;
                double x_temp = x + dt * dx * speed;

                for (; (dx > 0) ? (x_temp > x_back) : (x_temp < x_back); x_temp += (dx > 0) ? -1 : 1) {
                    bool can_stay = GameMap::GetCur()->CanStay(
                        (dx > 0) ? GameMap::E_CanStayType::Right : GameMap::E_CanStayType::Left,
                        Point {.x = (int)x_temp, .y = (int)y}, obj_size, can_stay_on_empty, Id
                    );
                    if (can_stay) {
                        move_x = true;
                        break;
                    }
                }

                if (move_x)x = x_temp;
            }

            bool move_y = false;
            if (dy) {
                double y_back = y;
                double y_temp = y + dt * dy * speed;

                for (; (dy > 0) ? (y_temp > y_back) : (y_temp < y_back); y_temp += (dy > 0) ? -1 : 1) {
                    bool can_stay = GameMap::GetCur()->CanStay(
                        (dy > 0) ? GameMap::E_CanStayType::Up : GameMap::E_CanStayType::Down,
                        Point {.x = (int)x, .y = (int)y_temp}, obj_size, can_stay_on_empty, Id
                    );
                    if (can_stay) {
                        move_y = true;
                        break;
                    }
                }

                if (move_y)y = y_temp;
            }

            if (move_x || move_y) {
                last_moved_time = now_t;
                moved = true;
            }

        }

    }

    last_time = now_t;
}
