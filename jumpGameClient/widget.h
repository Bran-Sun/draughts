#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "model.h"
#include <QMouseEvent>
#include <QtNetwork>
#include <QLabel>
#include <QPixmap>
#include "chat.h"
#include "login.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

    void connectHost();
    void _choose_color();

    void _send_route();

public slots:
    void _recv_message();
    void _on_sendButton_clicked();

    void _start();
    void _withdraw();
    void _surrender();
    void _peace();

    void _connect();

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

    QTcpSocket* m_readWrite;
    QPushButton *m_start, *m_withdraw, *m_surrender, *m_peace;
    Chat* m_chatter;
    Model m_model;
    login *m_login;
    int m_port;
    QString m_ip;
};

#endif // WIDGET_H
