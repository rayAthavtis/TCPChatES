#include "rsaoperate.h"

#include <QDebug>

RsaOperate::RsaOperate() {
    // 选取两个大质数
    m_p = RandPrime(16);
    m_q = RandPrime(16);

    m_n = m_p * m_q;
    m_f = (m_p - 1) * (m_q - 1);
    // 大整数e与f互质
    do {
        // 随机生成e
        m_e = QRandomGenerator::global()->generate64() % 65536;
        // 奇数
        m_e |= 1;
    } while (gcd(m_e, m_f)!=1);  // 判断互质
    // 确定解密密钥d
    m_d = Euclid(m_e, m_f);
    m_s = 0;
    quint64 t = m_n >> 1;
    while (t) {
        m_s++;
        t >>= 1;
    }
    // e和n是公开的，d不公开
}

RsaOperate::~RsaOperate() {
}

// 模乘运算
quint64 RsaOperate::mul_mod(quint64 p, quint64 q, quint64 n) {
    // return (p % n) * (q % n) % n;
    return p * q % n;
}

// 模幂运算
quint64 RsaOperate::pow_mod(quint64 base, quint64 p, quint64 n) {
    quint64 ret = 1;
    for (; p!=0; p--) {  // 操作次数
        while (!(p & 1)) {  // 为奇数时跳出
            p >>= 1;
            base = mul_mod(base, base, n);
        }
        ret = mul_mod(base, ret, n);
    }
    return ret;
}

// 随机大质数判断
bool RsaOperate::RabinMillerKnl(quint64 &n) {
    quint64 q = n - 1;  // <n的正奇数
    int k = 0;
    for (; !(q & 1); k++) {
        q >>= 1;
    }
    // 随机数2～n-1
    quint64 a = QRandomGenerator::global()->generate64() % (n - 3) + 2;
    quint64 val = pow_mod(a, q, n);
    if (val==1) { return true; }
    // 循环检验，奇合数有四分之一的概率通过
    for (int i=0; i<k; i++) {
        quint64 z = 1;
        for (int j=0; j<i; j++) { z *= 2; }
        if (pow_mod(a, z*q, n)==n-1) { return true; }
    }
    return false;
}

// 重复调用RabinMiller质数判别函数
bool RsaOperate::RabinMiller(quint64 &n, int loop=100) {
    for (int i=0; i<loop; i++)
    { if (!RabinMillerKnl(n)) { return false; } }
    return true;
}

quint64 RsaOperate::RandPrime(char bits) {
    quint64 base;
    do {
        base = 1 << (bits - 1);  // 保证最高位是1以确保足够大
        // 加上随机数
        base +=  QRandomGenerator::global()->generate64() % base;
        base |= 1;  // 保证最低位是1以确保是奇数
    } while (!RabinMiller(base, 30));  // RabinMiller测试30次
    return base;  // 全部通过则认为得到质数
}

// 求最大公约数
quint64 RsaOperate::gcd(quint64 &p, quint64 &q) {
    quint64 a = p > q ? p : q;
    quint64 b = p < q ? p : q;
    if (p==q) { return p; }
    else {
        // 辗转相除法
        while (b) {
            a %= b;
            quint64 tmp = a;
            a = b;
            b = tmp;
        }
        return a;
    }
}

// 私钥生成
quint64 RsaOperate::Euclid(quint64 e, quint64 n) {
    quint64 max = 0xffffffffffffffff - n;  // 限制
    quint64 i = 1;
    while (1) {
        // 尝试可能的i值，求d
        if (((i*n)+1)%e==0) { return ((i*n)+1)/e; }
        i++;
        quint64 tmp = (i + 1) * n;
        // 超过阈值，密钥生成失败
        if (tmp>max) { return 0; }
    }
}

// 密钥分配
// 加密，公钥传参
quint64 RsaOperate::rsa_encode(quint16 m, quint64 e, quint64 n) {
    return pow_mod(m, e, n);
}

// 解密
quint16 RsaOperate::rsa_decode(quint64 c) {
    quint64 res = pow_mod(c, m_d, m_n);
    quint16 *tmp = (quint16 *) &res;
    // 高16位(tmp[0])有效
    if (tmp[1]!=0 || tmp[2]!=0 || tmp[3]!=0)
    { return 0; }
    else { quint16 ret = tmp[0]; return ret; }
}

// 获取公钥
PublicK RsaOperate::getPublicKey() {
    PublicK ret;
    ret.p_e = this->m_e;
    ret.p_n = this->m_n;
    return ret;
}

// 加密8位字符串形式的key
QString RsaOperate::rsaEncode(QString key, quint64 e, quint64 n) {
    QString ret = "";
    for (int i=0; i<8; i++) {
        quint16 partK = key.at(i).unicode();
        // qDebug()<<"partK: "<<partK;

        quint64 tt = rsa_encode(partK, e, n);
        // qDebug()<<"tt: "<<tt;
        // here
        ret.append(QObject::tr("%1").arg(tt) + " ");
    }
    return ret;
}

// 解密成8位字符串key
QString RsaOperate::rsaDecode(QString C) {
    QString ret = "";
    QStringList ls = C.split(' ');
    for (int i=0; i<8; i++) {
        quint64 partk = ls.at(i).toULongLong();
        // qDebug()<<"partK: "<<partk;
        quint16 tt = rsa_decode(partk);
        // qDebug()<<"tt: "<<tt;
        ret.append(QChar(tt));
    }
    return ret;
}
