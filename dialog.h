#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QtNetwork>
#include "rsaoperate.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Dialog; }
QT_END_NAMESPACE

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();

private:
    Ui::Dialog *ui;
    QTcpServer *server;
    QList<QTcpServer*> servers_list;
    QList<QTcpSocket*> socks_list;
    quint16 serverpt;
    quint16 c_pt;
    bool new_name;
    QString spassword;
    RsaOperate rsa;

    QStringList clients;
    QList<PublicK> cpk_list;

private slots:
    QString getTime();
    void data_change();
    void new_Connection();
    void new_client();
    void del_Connection();
    void on_m_start_clicked();
    void on_m_send_clicked();
};
#endif // DIALOG_H
