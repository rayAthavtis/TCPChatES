#include "dialog.h"
#include "ui_dialog.h"

#include <QMessageBox>
#include "desoperate.h"

/*
1 基于RSA算法的自动分配密钥加密聊天程序
2 实现密钥自动生成，并基于RSA算法进行密钥共享。DES密钥自动生成、RSA密钥分配以及DES加密通讯三个部分
3 加密过程对用户完全透明
*/

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);
    serverpt = 6666;  // 监听端口
    c_pt = 6667;
    new_name = true;
    spassword = "";
    QPixmap *pixmap = new QPixmap(":/new/prefix1/TcpChat.png");
    pixmap->scaled(ui->icon->size(), Qt::KeepAspectRatio);  // 设置pixmap缩放的尺寸
    ui->icon->setScaledContents(true);  // 设置属性允许/禁止label缩放它的内容充满整个可用空间
    ui->icon->setPixmap(*pixmap);
    server = new QTcpServer(this);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::new_Connection() {
    // 新端口配置服务, 等待
    QTcpServer *sv = new QTcpServer(this);
    servers_list.append(sv);
    sv->listen(QHostAddress::Any, c_pt);
    connect(sv, SIGNAL(newConnection()), this, SLOT(new_client()));

    // 发送分配的端口号给新客户
    QTcpSocket *sock = server->nextPendingConnection();
    QString tmp = QString::number(c_pt);
    QByteArray pt = tmp.toLocal8Bit();
    sock->write(pt);
    c_pt += 1;  // 端口号变更
}

void Dialog::new_client() {
    QTcpSocket *sock = servers_list.last()->nextPendingConnection();
    new_name = true;
    socks_list.append(sock);

    connect(sock, &QTcpSocket::readyRead, this, &Dialog::data_change);
    connect(sock, &QTcpSocket::disconnected, this, &Dialog::del_Connection);
}

QString Dialog::getTime() {
    time_t t = time(nullptr);
    char date_time[64] = {0};
    strftime(date_time, sizeof(date_time)-1, "%Y-%m-%d %H:%M:%S", localtime(&t));
    return date_time;
}

// 信息交换
void Dialog::data_change() {
    QByteArray data;
    int idx = 0;
    foreach (QTcpSocket *sock, socks_list) {
        if (sock->bytesAvailable() > 0) {  // 收到信息
            data = sock->readAll();
            QString tp = data.data();
            QStringList pkt = tp.split('\n');
            int max = pkt.size();
            switch (pkt.at(0).toInt()) {
            case 0: {  // 0 name e n
                if (max<4) { return ; }
                // 该客户公钥
                clients.append(pkt.at(1));  // 添加名字
                PublicK pk;
                pk.p_e = pkt.at(2).toULongLong();
                pk.p_n = pkt.at(3).toULongLong();
                cpk_list.append(pk);

                if (cpk_list.size()==1) {
                    // 发送服务器公钥
                    QString cc = "1\n" + tr("%1").arg(rsa.m_e) + '\n' + tr("%1").arg(rsa.m_n);
                    sock->write(cc.toLocal8Bit());
                    return ;
                }
                else {
                    // 向非创建聊天室的用户发送聊天室des密钥及提示成功进入聊天
                    // qDebug()<<"send spsd: "<<spassword;
                    QString tt = RsaOperate::rsaEncode(spassword, pk.p_e, pk.p_n);
                    QString psd = "4\n" + tt + '\n';
                    sock->write(psd.toLocal8Bit());

                    DesOperate desop(spassword);
                    QString show = "Sys: " + clients.at(idx) + " joined the chat.";
                    ui->m_browser->append("【" + getTime() + "】" + show);
                    QString tmp = show + '\n' + QString::number(clients.size()) + '\n' + clients.join(' ');
                    QString cc = "3\n" + desop.msg_handle(tmp);
                    data = cc.toLocal8Bit();
                }
                break;
            }
            case 2: {  // 聊天室成功创建 2 rE(psd)
                if (max<2) { return ; }
                spassword = rsa.rsaDecode(pkt.at(1));
                // qDebug()<<"spsd: "<<spassword;

                DesOperate desop(spassword);
                QString show = "Sys: " + clients.at(0) + " started a chat.";
                ui->m_browser->append("【" + getTime() + "】" + show);
                QString tmp = show + '\n' + QString::number(clients.size()) + '\n' + clients.at(0);
                QString cc = "3\n" + desop.msg_handle(tmp);
                sock->write(cc.toLocal8Bit());
                return ;
            }
            case 6: {  // 退出消息 6 dE(quit)
                if (max<2) { return ; }
                DesOperate desop(spassword);
                QString mm = desop.remsg(pkt.at(1));
                QString name = clients.at(idx);
                clients.removeAt(idx);
                QString show = "Sys: " + name + " has left.";
                ui->m_browser->append("【" + getTime() + "】" + show);
                QString tmp = show + '\n' + QString::number(clients.size()) + '\n' + clients.join('\n');  // 更新
                QString cc = "3\n" + desop.msg_handle(tmp);
                data = cc.toLocal8Bit();
                break;
            }
            case 7: {  // 用户聊天消息 7 dE(msg)
                if (max<2) { return ; }
                DesOperate desop(spassword);
                QString mm = desop.remsg(pkt.at(1));
                ui->m_browser->append("【" + getTime() + "】" + mm);
                break;
            }
            }
            foreach (QTcpSocket *sc, socks_list) { sc->write(data); }
            // if (index!=idx)
        }
        idx++;  // 索引+
    }
}

void Dialog::del_Connection() {
    int i = 0;
    foreach (QTcpSocket *sock, socks_list) {
        if (sock->state() == QTcpSocket::UnconnectedState) {
            sock->deleteLater();
            servers_list.at(i)->deleteLater();
            servers_list.removeAt(i);
            socks_list.removeAt(i);
        }
        i++;
    }
}

void Dialog::on_m_start_clicked() {
    if (!server->listen(QHostAddress::Any, serverpt)) {
        QMessageBox::warning(this, tr("Warning"), tr("The server is not started!"));
        return ;
    }
    connect(server, SIGNAL(newConnection()), this, SLOT(new_Connection()));
    ui->m_start->setEnabled(false);
    ui->m_send->setDefault(true);
}

void Dialog::on_m_send_clicked()
{
    QString msg = ui->m_msg->text();
    if (msg.isEmpty()) { return ; }
    msg = "Sys: " + msg;
    DesOperate desop(spassword);
    QString cc = "7\n" + desop.msg_handle(msg);

    foreach (QTcpSocket *sc, socks_list) { sc->write(cc.toLocal8Bit()); }

    ui->m_browser->append("【" + getTime() + "】" + msg);
    ui->m_msg->setText("");
}
