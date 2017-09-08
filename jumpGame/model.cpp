#include "model.h"
#include <QPoint>
#include <iostream>
#include <algorithm>
#include <iterator>


Model::Model()
{
    std::vector<Status> vec(10, NO_CHESS);
    m_map.assign(10, vec);

    std::vector<bool> vec2(10, false);
    m_can.assign(10, vec2);
    m_searched = m_can;

    m_chosen = false;
    m_chosen_point = QPoint(0, 0);

}

void Model::_initialize()
{
    Status me, enemy, my_queen, enemy_queen;
    if (m_color == BLACK)
    {
        me = BLACK_CHESS;
        my_queen = BLACK_QUEEN;
        enemy = WHITE_CHESS;
        enemy_queen = WHITE_QUEEN;
    }
    else
    {
        me = WHITE_CHESS;
        my_queen = WHITE_QUEEN;
        enemy = BLACK_CHESS;
        enemy_queen = BLACK_QUEEN;
    }
    for (int i = 6; i < 10; i++)
        for (int j = 0; j < 5; j++)
        {
            if (i % 2 == 0)
            {
                m_chess.push_back(QPoint(i, j * 2 + 1));
                m_map[i][j * 2 + 1] = me;
                m_map[i - 6][j * 2 + 1] = enemy;
            }
            else
            {
                m_chess.push_back(QPoint(i, j * 2));
                m_map[i][j * 2] = me;
                m_map[i - 6][j * 2] = enemy;
            }
        }

    test();
   /* m_map[6][5] = my_queen;
    m_map[6][1] = my_queen;
    m_map[6][3] = my_queen;
    m_map[3][4] = enemy_queen;
    m_map[3][8] = enemy_queen;
    m_map[3][6] = enemy_queen;*/

    _calculate_route();
   // print();
   // print_route();
}

Model::Status Model::_get_data(int x, int y) const
{
    return m_map[x][y];
}

void Model::print()
{
    for (int i = 0; i < 10; i ++)
    {
        for (int j = 0; j < 10; j++)
            std::cout << m_map[i][j] << " ";
        std::cout <<std::endl;
    }
}

bool Model::_in(int x, int y) const
{
    if ( x >= 0 && x < 10 && y >= 0 && y < 10) return true;
    else return false;
}

void Model::_set_chosen(int x, int y)
{
    if (_in(x, y) && m_can[x][y])
    {
        m_chosen = true;
        m_chosen_point = QPoint(x, y);
    }
    else
        m_chosen = false;
}

bool Model::_get_if_chosen() const
{
    return m_chosen;
}

QPoint Model::_get_chosen() const
{
    return m_chosen_point;
}

void Model::_process(int x, int y)
{
    if (!m_chosen) _set_chosen(x, y);
    else
      _move(x, y);

}

void Model::_clean()
{
    for (int i = 0; i < 10; i++)
        for (int j = 0; j < 10; j++)
        {
            m_can[i][j] = false;
            m_searched[i][j] = false;
        }
    m_route.clear();
    m_step.clear();
    m_chosen = false;
}

void Model::_calculate_route()
{
    _clean();
    Status chess1, chess2;
    if (m_color == BLACK)
    {
        chess1 = BLACK_CHESS;
        chess2 = BLACK_QUEEN;
    }
    else
    {
        chess1 = WHITE_CHESS;
        chess2 = WHITE_QUEEN;
    }

    for (int i = 0 ; i < 10; i++)
        for (int j = 0; j < 10; j++)
        {
            if (m_map[i][j] ==  chess1)
            {
                _calculate_chess(i, j);
            }
            else if (m_map[i][j] == chess2)
            {
                _calculate_queen(i, j);
            }
        }
    unique(m_route.begin(), m_route.end());
    for (auto i : m_route)
    {
        m_can[i[0].x()][i[0].y()] = true;
    }
}

void Model::_calculate_chess(int x, int y)
{
    Status enemy1, enemy2;
    if (m_color == BLACK)
    {
        enemy1 = WHITE_CHESS;
        enemy2 = WHITE_QUEEN;
    }
    else
    {
        enemy1 = BLACK_CHESS;
        enemy2 = BLACK_QUEEN;
    }

    const int dx[4] = {-1, -1, +1, +1};
    const int dy[4] = {+1, -1, +1, -1};

    for (int i = 0; i < 10;i++)
        for (int j = 0; j < 10; j++)
            m_searched[i][j] = false;

    std::vector<QPoint> route;
    route.push_back(QPoint(x, y));
    for (int i = 0; i < 2; i++)
    {
        int newx = x + dx[i];
        int newy = y + dy[i];
        if (!_in(newx, newy))
            continue;
        if (m_map[newx][newy] == NO_CHESS)
        {
            route.push_back(QPoint(newx, newy));
            _add_route(route, 0);
            route.pop_back();
        }
    }
    for (int i = 0; i < 4; i++)
    {
        int newx = x + dx[i];
        int newy = y + dy[i];
        if (!_in(newx, newy))
            continue;
        if (m_map[newx][newy] == enemy1 || m_map[newx][newy] == enemy2)
        {
            int nextx = newx + dx[i];
            int nexty = newy + dy[i];
            if (!_in(nextx, nexty))
                continue;
            if (m_map[nextx][nexty] == NO_CHESS)
            {
                route.push_back(QPoint(newx, newy));
                route.push_back(QPoint(nextx, nexty));
                m_searched[newx][newy] = true;
                _next_step(nextx, nexty, route, 1);
                route.pop_back();
                route.pop_back();
                m_searched[newx][newy] = false;
            }
        }

    }
}

void Model::_next_step(int x, int y, std::vector<QPoint> route, int num)
{
    Status enemy1, enemy2;
    if (m_color == BLACK)
    {
        enemy1 = WHITE_CHESS;
        enemy2 = WHITE_QUEEN;
    }
    else
    {
        enemy1 = BLACK_CHESS;
        enemy2 = BLACK_QUEEN;
    }

    const int dx[4] = {-1, -1, +1, +1};
    const int dy[4] = {+1, -1, +1, -1};

    for (int i = 0; i < 4; i++)
    {
        int newx = x + dx[i];
        int newy = y + dy[i];
        if (!_in(newx, newy))
            continue;
        if (!m_searched[newx][newy] && (m_map[newx][newy] == enemy1 || m_map[newx][newy] == enemy2))
        {
            int nextx = newx + dx[i];
            int nexty = newy + dy[i];
            if (!_in(nextx, nexty))
                continue;
            if (m_map[nextx][nexty] == NO_CHESS)
            {
                route.push_back(QPoint(newx, newy));
                route.push_back(QPoint(nextx, nexty));
                m_searched[newx][newy] = true;
                _next_step(nextx, nexty, route, num + 1);
                route.pop_back();
                route.pop_back();
                m_searched[newx][newy] = false;
            }
        }
    }
    _add_route(route, num);
}

void Model::_add_route(std::vector<QPoint> route, int num)
{
    if (m_route.size() == 0)
    {
        m_route.push_back(route);
        m_step.push_back(num);
    }
    else
    {
        if (m_step[0] < num)
        {
            m_route.clear();
            m_step.clear();
            m_route.push_back(route);
            m_step.push_back(num);
        }
        else if (m_step[0] == num)
        {
            m_route.push_back(route);
            m_step.push_back(num);
        }
    }
}

void Model::print_route()
{
    for (auto route : m_route)
    {
        for (auto point : route)
        {
            std::cout << point.x() << " " << point.y() << std::endl;
        }
        std::cout << std::endl;
    }
}

bool Model::_get_if_can(int x, int y) const
{
    return m_can[x][y];
}

void Model::_move(int x, int y)
{
    Status me1, me2, enemy1, enemy2;
    if (m_color == BLACK)
    {
        me1 = BLACK_CHESS;
        me2 = BLACK_QUEEN;
        enemy1 = WHITE_CHESS;
        enemy2 = WHITE_QUEEN;
    }
    else
    {
        me1 = WHITE_CHESS;
        me2 = WHITE_QUEEN;
        enemy1 = BLACK_CHESS;
        enemy2 = BLACK_QUEEN;
    }
    if (!_in(x, y)) return;
    QPoint p(x, y);

    if (m_can[x][y])
       m_chosen_point = p;
    Status init = m_map[m_chosen_point.x()][m_chosen_point.y()];

    for (auto i : m_route)
    {
        if (m_chosen_point == i[0] && p == i[i.size() - 1])
        {
            m_pre = m_map;
            m_pre_route = i;
            for (auto j : i)
            {
                m_map[j.x()][j.y()] = NO_CHESS;
            }
            if (_my_reach_border(x, y) && init != me2)
                m_map[x][y] = me2;
            else
                m_map[x][y] = init;
            m_chosen = false;
            break;
        }
    }
}

void Model::_set_color(Chess color)
{
    m_color = color;
    _initialize();
}

void Model::_calculate_queen(int x, int y)
{
    for (int i = 0; i < 10; i++)
        for (int j = 0; j < 10; j++)
            m_searched[i][j] = false;

    std::vector<QPoint> route;

    route.push_back(QPoint(x, y));
    _queen_next_step(x, y, route, 0, 4);
}

void Model::_queen_next_step(int x, int y, std::vector<QPoint> route, int num, int pre_dir)
{
    Status enemy1, enemy2;
    if (m_color == BLACK)
    {
        enemy1 = WHITE_CHESS;
        enemy2 = WHITE_QUEEN;
    }
    else
    {
        enemy1 = BLACK_CHESS;
        enemy2 = BLACK_QUEEN;
    }

    const int dx[4] = {-1, -1, +1, +1};
    const int dy[4] = {-1, +1, +1, -1};

    std::vector<QPoint> new_router = route;

    for (int i = 0; i < 4; i++)
    {
        if (i == pre_dir) continue;

        int newx = x + dx[i];
        int newy = y + dy[i];
        if (!_in(newx, newy)) continue;
        new_router = route;

        while (_in(newx, newy) && m_map[newx][newy] == NO_CHESS)
        {
            new_router.push_back(QPoint(newx, newy));
            if (num == 0)
                _add_route(new_router, num);
            newx += dx[i];
            newy += dy[i];
        }

        if (!_in(newx, newy)) continue;

        if ((m_map[newx][newy] == enemy1 || m_map[newx][newy] == enemy2) && !m_searched[newx][newy])
        {
            int nextx, nexty;
            nextx = newx + dx[i];
            nexty = newy + dy[i];
            if (_in(nextx, nexty) && m_map[nextx][nexty] == NO_CHESS)
            {
               new_router.push_back(QPoint(newx, newy));
               new_router.push_back(QPoint(nextx, nexty));
               _add_route(new_router, num + 1);
               m_searched[newx][newy] = true;
               _queen_next_step(nextx, nexty, new_router, num + 1, (i + 2) % 4);
               nextx += dx[i];
               nexty += dy[i];

               while (_in(nextx, nexty) && m_map[nextx][nexty] == NO_CHESS)
               {
                    new_router.push_back(QPoint(nextx, nexty));
                    _add_route(new_router, num + 1);
                    _queen_next_step(nextx, nexty, new_router, num + 1, (i + 2) % 4);
                    nextx += dx[i];
                    nexty += dy[i];
               }
               m_searched[newx][newy] = false;
            }
        }
    }

}

void Model::_perform_enemy(std::vector<QPoint> y_route)
{
    m_enemy_pre = m_map;
    Status init = m_map[y_route[0].x()][y_route[0].y()];
    Status queen;
    if (init == BLACK_CHESS) queen = BLACK_QUEEN;
    else queen = WHITE_QUEEN;

    int len = y_route.size();
    for (auto i : y_route)
    {
        m_map[i.x()][i.y()] = NO_CHESS;
    }
    int endx = y_route[len - 1].x(), endy = y_route[len -1].y();

    if (_enemy_reach_border(endx, endy) && (init != queen))
        m_map[endx][endy] = queen;
    else
        m_map[endx][endy] = init;

    _calculate_route();
 //   print_route();
}

bool Model::_enemy_reach_border(int x, int y) const
{
    if (x == 9) return true;
    else return false;
}

bool Model::_my_reach_border(int x, int y) const
{
    if (x == 0) return true;
    return false;
}

std::vector<QPoint> Model::_get_move_point() const
{
    std::vector<QPoint> tem;
    for (auto i : m_route)
    {
        tem.push_back(i[0]);
    }
    unique(tem.begin(), tem.end());
    return tem;
}

std::vector<QPoint> Model::_get_end_point() const
{
    std::vector<QPoint> tem;
    for (auto i : m_route)
    {
        if (i[0] == m_chosen_point)
            tem.push_back(i[i.size() - 1]);
    }
    return tem;
}

void Model::_allow_withdraw()
{
    m_map = m_enemy_pre;
    _calculate_route();
}

void Model::_withdraw()
{
    m_map = m_pre;
    _calculate_route();
}

bool Model::_if_fail() const
{
    if (m_route.empty()) return true;
    else return false;
}

void Model::test()
{
    for (int i = 0; i < 10; i++)
        for (int j = 0; j < 10; j++)
            m_map[i][j] = NO_CHESS;
    Status me, enemy, my_queen, enemy_queen;
    if (m_color == BLACK)
    {
        me = BLACK_CHESS;
        my_queen = BLACK_QUEEN;
        enemy = WHITE_CHESS;
        m_map[1][2] = enemy;
        m_map[3][2] = enemy;
        m_map[5][2] = enemy;
        m_map[7][2] = enemy;
        m_map[2][9] = enemy;
        m_map[6][5] = enemy;

        m_map[1][6] = me;
        m_map[1][8] = me;
        m_map[4][3] = me;
        m_map[8][5] = me;

    }
    else
    {
        me = WHITE_CHESS;
        my_queen = WHITE_QUEEN;
        enemy = BLACK_CHESS;
        enemy_queen = BLACK_QUEEN;
        m_map[8][7] = me;
        m_map[6][7] = me;
        m_map[4][7] = me;
        m_map[2][7] = me;
        m_map[7][0] = me;
        m_map[3][4] = me;

        m_map[8][3] = enemy;
        m_map[8][1] = enemy;
        m_map[5][6] = enemy;
        m_map[1][4] = enemy;
    }


}
