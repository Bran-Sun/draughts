#include "chat.h"

Chat::Chat(QWidget *parent) : QWidget(parent)
{
    m_dialog = new QTextEdit(this);
    m_dialog->setGeometry(0, 0, 250, 100);

    m_input = new QTextEdit(this);
    m_input->setGeometry(0, 100, 200, 40);

    m_send = new QPushButton(tr("发送"), this);
    m_send->setGeometry(200, 100, 40, 40);
}
