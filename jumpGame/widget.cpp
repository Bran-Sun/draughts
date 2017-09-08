#include "widget.h"
#include "ui_widget.h"
#include <QPainter>
#include <QPen>
#include "model.h"
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <QMessageBox>
#include <QHostInfo>
#include <QPalette>
#include <QSound>
#include <QNetworkInterface>
#include <QInputDialog>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    setWindowTitle("Server");
    QPalette pal;
    pal.setColor(QPalette::Background, QColor("#FFF8DC"));
    setAutoFillBackground(true);
    setPalette(pal);

    m_picture.load("../../../hat.png");
    m_picture = m_picture.scaled(QSize(40, 40), Qt::KeepAspectRatio);
    m_player1.load("../../../player1.png");
    m_player1 = m_player1.scaled(QSize(200, 200), Qt::KeepAspectRatio);
    ui->label->setPixmap(m_player1);

    m_player2.load("../../../player2.png");
    m_player2 = m_player2.scaled(QSize(200, 200), Qt::KeepAspectRatio);

    m_chatter = new Chat(this);
    m_chatter->setGeometry(0, 600, 250, 150);
    _get_ip();

    m_start = ui->pushButton;
    m_start->setStyleSheet("QPushButton{color: #C1FFC1; background:#CDB79E; border-radius:5px; font-size: 30px; font-family:微软雅黑}"
                           "QPushButton:hover{background-color:#C1FFC1; color: #CDB79E;}");

    m_withdraw = ui->pushButton_2;
    m_withdraw->setStyleSheet("QPushButton{color: #C1FFC1; background:#CDB79E; border-radius:5px; font-size: 30px; font-family:微软雅黑}"
                           "QPushButton:hover{background-color:#C1FFC1; color: #CDB79E;}");

    m_surrender = ui->pushButton_3;
    m_surrender->setStyleSheet("QPushButton{color: #C1FFC1; background:#CDB79E; border-radius:5px; font-size: 30px; font-family:微软雅黑}"
                               "QPushButton:hover{background-color:#C1FFC1; color: #CDB79E;}");
    m_peace = ui->pushButton_4;
    m_peace->setStyleSheet("QPushButton{color: #C1FFC1; background:#CDB79E; border-radius:5px; font-size: 30px; font-family:微软雅黑}"
                           "QPushButton:hover{background-color:#C1FFC1; color: #CDB79E;}");

    connect(m_start, SIGNAL(clicked(bool)), this, SLOT(_start()));
    connect(m_withdraw, SIGNAL(clicked(bool)), this, SLOT(_withdraw()));
    connect(m_surrender, SIGNAL(clicked(bool)), this, SLOT(_surrender()));
    connect(m_peace, SIGNAL(clicked(bool)), this, SLOT(_peace()));

    m_ready = false;
    y_ready = false;
    m_turn = false;
    m_start_game = false;
}

Widget::~Widget()
{
    delete ui;
}

void Widget::paintEvent(QPaintEvent *ev)
{
    _paint_chessboard();
    _paint_chess();
    if (m_turn)
        _paint_chosen();
}

void Widget::_paint_chessboard()
{
    QPainter painter(this);

    painter.setBrush(QBrush(QColor("#FFD39B")));
    int top = 50, left = 350, interval = 55;

    for (int i = 0; i< 10; i++)
        for (int j = 0; j < 5; j++)
        {
            if (i % 2 == 0)
                painter.drawRect(left + interval * i, top + interval * (j * 2), interval, interval);
            else
                painter.drawRect(left + interval * i, top + interval * (j * 2 + 1), interval, interval);
        }
    painter.setBrush(QBrush(QColor("#B22222")));
    for (int i = 0; i< 10; i++)
        for (int j = 0; j < 5; j++)
        {
            if (i % 2 != 0)
                painter.drawRect(left + interval * i, top + interval * (j * 2), interval, interval);
            else
                painter.drawRect(left + interval * i, top + interval * (j * 2 + 1), interval, interval);
        }

}

void Widget::_paint_chess()
{
    QPainter painter(this);

    int top = 50, left = 350, interval = 55, margin = 5, radius = 45;

    for (int i = 0; i < 10; i++)
        for (int j = 0; j < 10; j++)
        {
            switch(m_model._get_data(i, j))
            {
            case Model::WHITE_CHESS:
                painter.setBrush(QBrush(Qt::white));
                painter.drawEllipse(left + j * interval + margin, top + i * interval + margin, radius, radius);
                break;
            case Model::BLACK_CHESS:
                painter.setBrush(QBrush(Qt::black));
                painter.drawEllipse(left + j * interval + margin, top + i * interval + margin, radius, radius);
                break;
            case Model::WHITE_QUEEN:
                painter.setBrush(QBrush(Qt::white));
                painter.drawEllipse(left + j * interval + margin, top + i * interval + margin, radius, radius);
                painter.setBrush(QBrush(Qt::yellow));
                painter.drawPixmap(left + j * interval + margin + 2, top + i * interval + margin, m_picture);
                break;
            case Model::BLACK_QUEEN:
                painter.setBrush(QBrush(Qt::black));
                painter.drawEllipse(left + j * interval + margin, top + i * interval + margin, radius, radius);
                painter.setBrush(QBrush(Qt::yellow));
                painter.drawPixmap(left + j * interval + margin + 2, top + i * interval + margin, m_picture);
                break;
            default:
                break;
            }
        }
}

void Widget::_paint_chosen()
{
    QPainter painter(this);
    QPen pen(Qt::green, 3);
    painter.setPen(pen);

    int top = 50, left = 350, interval = 55;

    if (m_model._get_if_chosen())
    {
        QPoint p = m_model._get_chosen();
        painter.drawRect(left + p.y() * interval, top + p.x() * interval, interval, interval);
        std::vector<QPoint> choice = m_model._get_end_point();
        for (auto i : choice)
        {
            painter.drawRect(left + i.y() * interval, top + i.x() *  interval, interval, interval);
        }
    }
    else
    {
        std::vector<QPoint> choice = m_model._get_move_point();
        for (auto i : choice)
        {
            painter.drawRect(left + i.y() * interval, top + i.x() * interval, interval, interval);
        }
    }
}


void Widget::mousePressEvent(QMouseEvent *ev)
{
    if (!m_turn) return;

    int top = 50, left = 350, interval = 55;

    int x = ev->pos().rx();
    int y = ev->pos().ry();

    int xi = (x - left) / interval;
    int yi = (y - top) / interval;

    if (x < left) xi = -1;
    if (y < top) yi = -1;

    m_model.m_pre_route.clear();
    m_model._process(yi, xi);
    if (m_model.m_pre_route.size() > 0)
    {
        _send_route();
        m_model.m_pre_route.clear();
        m_turn = false;
    }
    update();
}

void Widget::_initServer()
{
    QString text = QInputDialog::getText(this, tr("设置端口号"), tr("端口号："), QLineEdit::Normal, tr(""));
    int port = text.toInt();
    m_listen = new QTcpServer(this);
    m_listen->listen(QHostAddress::Any, port);
    connect(m_listen, SIGNAL(newConnection()), this, SLOT(_accept_connection()));

    QString s;

    s = "Port: " + text;
    m_chatter->m_dialog->append(s);
}

void Widget::_accept_connection()
{
    m_readWrite = m_listen->nextPendingConnection();
    connect(m_readWrite, SIGNAL(readyRead()), this, SLOT(_recv_message()));
    connect(m_chatter->m_send, SIGNAL(clicked(bool)), this, SLOT(_on_sendButton_clicked()));
    ui->label_3->setPixmap(m_player2);
    ui->label_4->setText(tr("玩家2"));
}

void Widget::_recv_message()
{
    QString info;
    info += m_readWrite->readAll();
    QString s = info.left(1);
    if (s == "0")
        _mes_message(info);
    else if (s == "1")
        _action_message(info);
    else
        _info_message(info);
}

void Widget::_on_sendButton_clicked()
{
    QByteArray *array = new QByteArray;
    array->clear();
    QString mess = "0" + m_chatter->m_input->toPlainText();
    array->append(mess);
    m_readWrite->write(array->data());
    m_chatter->m_dialog->append("Player 1:" + m_chatter->m_input->toPlainText());
    m_chatter->m_input->clear();
}

void Widget::_send_message(QString s)
{
    QByteArray *array = new QByteArray;
    array->clear();
    array->append(s);
    m_readWrite->write(array->data());
}


/*When send message
 * 0:mes_message: talking in dialog
 * 1:action_message: the route of a chess
 * 2:info_message: start, peace, surrender, quit
 */

void Widget::_start()
{
    m_ready = true;
    m_start_game = true;

    QByteArray *array = new QByteArray;
    array->clear();
    QString s;
    s = "2ready";
    array->append(s);
    m_readWrite->write(array->data());
    m_chatter->m_dialog->append("Player 1: I'm ready!");
}

void Widget::_withdraw()
{
    if (!m_start_game) return;
    QString s;
    s = "2withdraw";
    _send_message(s);

}

void Widget::_surrender()
{
    if (!m_start_game) return;
    _send_message("2surrender");
    _fail();
}

void Widget::_peace()
{
    if (!m_start_game) return;
    _send_message("2peace");
}

void Widget::_info_message(QString s)
{
    QString mess = s.right(s.length() - 1);
    if (mess == "ready")  {
        m_chatter->m_dialog->append("Player 2:I'm ready.");
        if (m_ready) _choose_color();
        update();
    }
    else if (mess == "black")
    {
        m_model._set_color(Model::BLACK);
        m_turn = true;
        m_start_game = true;
        update();
    }
    else if (mess == "white")
    {
        m_model._set_color(Model::WHITE);
        m_start_game = true;
        update();
    }
    else if (mess == "withdraw")
    {
       int ret = QMessageBox::question(this, tr("请求"), tr("是否同意对方悔棋"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
       if (ret == QMessageBox::Yes)
           _allow_withdraw();
       else
           _send_message("2cannotwithdraw");
    }
    else if (mess == "cannotwithdraw")
    {
        int ret = QMessageBox::information(this, tr("回复"), tr("对手拒绝了您的请求"), QMessageBox::Yes, QMessageBox::Yes);
    }
    else if (mess == "allowwithdraw")
    {
        m_model._withdraw();
        m_turn = true;
        update();
    } else if (mess == "surrender")
    {
        _win();
    }
    else if (mess == "peace")
    {
        int ret = QMessageBox::question(this, tr("请求"), tr("是否同意和棋"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        if (ret == QMessageBox::Yes)
        {
            _send_message("2allowpeace");
            _allow_peace();
        }
        else
            _send_message("2cannotpeace");
    }
    else if (mess == "cannotpeace")
    {
        int ret = QMessageBox::information(this, tr("回复"), tr("对手拒绝和棋"), QMessageBox::Yes, QMessageBox::Yes);
    }
    else if (mess == "allowpeace")
    {
        _allow_peace();
    }
    else if (mess == "fail")
    {
        _win();
    }
}

void Widget::_mes_message(QString s)
{
    QString mess = s.right(s.length() - 1);
    m_chatter->m_dialog->append("Player 2:" + mess);
}

void Widget::_action_message(QString s)
{
    QString mess = s.right(s.length() - 1);
    int cnt = 0;
    std::vector<QPoint> r;
    std::cout << mess.toStdString() << std::endl;
    while (cnt < mess.size())
    {
        QString num = mess.at(cnt);
        int x = 9 - num.toInt();
        cnt++;
        num = mess.at(cnt);
        int y = 9 - num.toInt();
        r.push_back(QPoint(x, y));
        cnt++;
    }
    m_model._perform_enemy(r);
    if (m_model._if_fail())
    {
        _send_message("2fail");
        _fail();
    }
    update();
    m_turn = true;
}

void Widget::_choose_color()
{
    QString s;
    srand((unsigned)time(NULL));
    int r = rand() % 2;
    if (r == 1)
    {
        m_model._set_color(Model::BLACK);
        m_turn = true;
        s = "2white";
        update();
    }
    else
    {
        m_model._set_color(Model::WHITE);
        s = "2black";
        update();
    }
    QByteArray *array = new QByteArray;
    array->clear();
    array->append(s);
    m_readWrite->write(array->data());
    m_chatter->m_dialog->append("Player 1:I'm " + s.right(5));
    m_start_game = true;
}

void Widget::_send_route()
{
    QString s;
    s = "1";
    for (auto i : m_model.m_pre_route)
    {
        s = s + QString::number(i.x()) + QString::number(i.y());
    }
    QByteArray *array = new QByteArray;
    array->clear();
    array->append(s);
    m_readWrite->write(array->data());
}

void Widget::_allow_withdraw()
{
    m_model._allow_withdraw();
    _send_message("2allowwithdraw");
    m_turn = false;
    update();
}

void Widget::_win()
{
    m_turn = false;
    m_ready = false;
    y_ready = false;
    m_start_game = false;
    update();
    QSound::play("../../../success.wav");
    int ret = QMessageBox::information(this, tr("结果"), tr("恭喜你，获胜了!"), QMessageBox::Yes, QMessageBox::Yes);
}

void Widget::_fail()
{
    m_turn = false;
    m_ready = false;
    y_ready = false;
    m_start_game = false;
    update();
    int ret = QMessageBox::information(this, tr("结果"), tr("对不起，您输了。"), QMessageBox::Yes, QMessageBox::Yes);
}

void Widget::_get_ip()
{
    QList<QHostAddress> list = QNetworkInterface::allAddresses();
    QString ip;
    for (QList<QHostAddress>::iterator it = list.begin(); it != list.end(); ++it)
        if(it->protocol() == QAbstractSocket::IPv4Protocol)
        {
            ip = it->toString();
            m_chatter->m_dialog->append("ip: " + ip);
        }
}

void Widget::_allow_peace()
{
    m_turn = false;
    m_ready = false;
    y_ready = false;
    m_start_game = false;
    update();
    int ret = QMessageBox::information(this, tr("结果"), tr("和棋!"), QMessageBox::Yes, QMessageBox::Yes);
}
