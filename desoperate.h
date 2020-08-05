#ifndef DESOPERATE_H
#define DESOPERATE_H

#include <QObject>

class DesOperate
{
public:
    DesOperate(QString key);
    ~DesOperate();

private:
    QString m_key[16];
    QString m_K;

    QString Ik(QString K);
    QString Lk(QString IkK, int lnum);
    QString Pk(QString X);
    QString IPmc(QString mc);
    QString UnIPmc(QString mc);
    QString dec2b(int n, int fl);
    QString F(QString R, QString K);
    QString desC(QString M);
    QString desM(QString C);
    QString b2ct(QString b);
    QString ct2b(QString ct);
    QString key2b(QString key);

    bool makeKey();
    QString makeData(QString M);
    QString reData(QString M);

public:
    QString msg_handle(QString msg);
    QString remsg(QString msg);
};

#endif // DESOPERATE_H
