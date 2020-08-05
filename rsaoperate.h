#ifndef RSAOPERATE_H
#define RSAOPERATE_H

#include <QRandomGenerator>
#include <QObject>

// 公钥结构体
struct PublicK {
    quint64 p_e;
    quint64 p_n;
};

class RsaOperate
{
public:
    RsaOperate();
    ~RsaOperate();

private:
    quint64 m_p;  // p
    quint64 m_q;  // q
    quint64 m_f;  // (p-1)*(q-1)
    quint64 m_d;  // d
    quint64 m_s;

    static quint64 mul_mod(quint64 p, quint64 q, quint64 n);  // 模乘
    static quint64 pow_mod(quint64 base, quint64 p, quint64 n);  // 模幂
    bool RabinMillerKnl(quint64 &n);
    bool RabinMiller(quint64 &n, int loop);  // RabinMiller质数判别
    quint64 RandPrime(char bits);  // 随机大质数生成
    quint64 gcd(quint64 &p, quint64 &q);  // gcd求最大公约数
    quint64 Euclid(quint64 e, quint64 d);  // 欧几里得算法

public:
    // 公钥
    quint64 m_n;  // n
    quint64 m_e;  // e
    PublicK getPublicKey();  // 公钥获取

    static quint64 rsa_encode(quint16 m, quint64 e, quint64 n);  // 加密
    static QString rsaEncode(QString key, quint64 e, quint64 n);  // 加密

    quint16 rsa_decode(quint64 c);  // 解密
    QString rsaDecode(QString C);  // 解密
};


#endif // RSAOPERATE_H
