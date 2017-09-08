#ifndef CHAT_H
#define CHAT_H

#include <QWidget>
#include <QLabel>
#include <QTextEdit>
#include <QPushButton>

class Chat : public QWidget
{
    Q_OBJECT
public:
    explicit Chat(QWidget *parent = nullptr);

signals:

public slots:

public:
    QTextEdit* m_dialog;
    QTextEdit* m_input;
    QPushButton* m_send;
};

#endif // CHAT_H
