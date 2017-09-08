#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <QPoint>

class Model
{
public:
    enum Status
    {
        NO_CHESS,
        BLACK_CHESS,
        WHITE_CHESS,
        BLACK_QUEEN,
        WHITE_QUEEN
    };

    enum Chess
    {
        BLACK = 0,
        WHITE
    };

public:
    Model();
    void test();
    Status _get_data(int x, int y) const;
    bool _get_if_chosen() const;
    QPoint _get_chosen() const;

    void _set_chosen(int x, int y);
    void _move(int x, int y);
    void _process(int x, int y);
    bool _get_if_can(int x, int y) const;

    void _initialize();
    void _set_color(Chess color);
    void _perform_enemy(std::vector<QPoint> y_route);

    void _allow_withdraw();
    void _withdraw();

    std::vector<QPoint> _get_move_point() const;
    std::vector<QPoint> _get_end_point() const;

    bool _if_fail() const;

private:
    bool _in(int x, int y) const;
    bool _enemy_reach_border(int x, int y) const;
    bool _my_reach_border(int x, int y) const;

    void _clean();
    void _calculate_route();
    void _calculate_chess(int x, int y);
    void _calculate_queen(int x, int y);
    void _next_step(int x, int y, std::vector<QPoint> route, int num);
    void _queen_next_step(int x, int y, std::vector<QPoint> route, int num, int pre_dir);

    void _add_route(std::vector<QPoint> route, int num);

    void print();
    void print_route();

public:
    std::vector<QPoint> m_pre_route;

private:
    std::vector<std::vector<Status>> m_map;
    std::vector<std::vector<Status>> m_pre;
    std::vector<std::vector<Status>> m_enemy_pre;
    std::vector<QPoint> m_chess;
    std::vector<std::vector<QPoint>> m_route;
    std::vector<int> m_step;
    std::vector<std::vector<bool>> m_can;
    std::vector<std::vector<bool>> m_searched;

    QPoint m_chosen_point;
    Chess m_color;

    bool m_chosen;
};

#endif // MODEL_H
