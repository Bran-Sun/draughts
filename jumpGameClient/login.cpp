#include "login.h"

login::login(QWidget *parent) : QDialog(parent)
{
    setWindowTitle("请输入对方的ip地址");
    setFixedSize(300, 200);
    int top = 30, height = 30;
    m_label1 = new QLabel(tr("IP:"), this);
    m_label1->setGeometry(50, top, 30, height);

    m_ip = new QLineEdit(this);
    m_ip->setGeometry(100, top, 80, height);

    m_label1 = new QLabel(tr("Port"), this);
    m_label1->setGeometry(50, top + 60, 30, height);

    m_port = new QLineEdit(this);
    m_port->setGeometry(100, top + 60, 80, height);

    m_button = new QPushButton(tr("确定"), this);
    m_button->setGeometry(100, top + 100, 80, 50);

}
