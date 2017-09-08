#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "model.h"
#include <QMouseEvent>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostAddress>
#include <QLabel>
#include <QPixmap>
#include <QLabel>
#include "chat.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

    void  _initServer();
    void _choose_color();

    void _send_route();
    void _get_ip();

public slots:
    void _accept_connection();
    void _recv_message();
    void _on_sendButton_clicked();

    void _start();
    void _withdraw();
    void _surrender();
    void _peace();

protected:
    void paintEvent(QPaintEvent *ev);
    void mousePressEvent(QMouseEvent *ev);

private:
    void _paint_chessboard();
    void _paint_chess();
    void _paint_chosen();

    void _send_message(QString s);

    void _info_message(QString s);
    void _action_message(QString s);
    void _mes_message(QString s);

    void _allow_withdraw();
    void _allow_peace();
    void _fail();
    void _win();

private:
    Ui::Widget *ui;

    bool m_ready, y_ready, m_turn;
    bool m_start_game;
    QPixmap m_picture, m_player1, m_player2;

    QTcpServer  *m_listen;
    QPushButton *m_start, *m_withdraw, *m_surrender, *m_peace;
    QTcpSocket  *m_readWrite;
    Chat* m_chatter;
    Model m_model;
};

#endif // WIDGET_H
