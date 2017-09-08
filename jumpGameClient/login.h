#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QDialog>

class login : public QDialog
{
    Q_OBJECT
public:
    explicit login(QWidget *parent = nullptr);

signals:

public slots:

public:
    QPushButton* m_button;
    QLineEdit *m_ip, *m_port;
    QLabel *m_label1, *m_lablel2;
};

#endif // LOGIN_H
