#include "desoperate.h"
#include <QDebug>

using namespace std;

static int I[56] = {57, 49, 41, 33, 25, 17,  9,  1, 58, 50, 42, 34, 26, 28,
                    10,  2, 59, 51, 43, 35, 27, 19, 11,  3, 60, 52, 44, 36,
                    63, 55, 47, 39, 31, 23, 15,  7, 62, 54, 46, 38, 30, 22,
                    14,  6, 61, 53, 45, 37, 29, 21, 13,  5, 28, 20, 12,  4};

static int P[48] = {14, 17, 11, 24,  1,  5,  3, 28, 15,  6, 21, 10,
                    23, 19, 12,  4, 26,  8, 16,  7, 27, 20, 13,  2,
                    41, 52, 31, 37, 47, 55, 30, 40, 51, 45, 33, 48,
                    44, 49, 39, 56, 34, 53, 46, 42, 50, 36, 29, 32};

static int IP[64] = {58, 50, 42, 34, 26, 18, 10,  2, 60, 52, 44, 36, 28, 20, 12,  4,
                     62, 54, 46, 38, 30, 22, 14,  6, 64, 56, 48, 40, 32, 24, 16,  8,
                     57, 49, 41, 33, 25, 17,  9,  1, 59, 51, 43, 35, 27, 19, 11,  3,
                     61, 53, 45, 37, 29, 21, 13,  5, 63, 55, 47, 39, 31, 23, 15,  7};

static int UnIP[64] = {40,  8, 48, 16, 56, 24, 64, 32, 39,  7, 47, 15, 55, 23, 63, 31,
                       38,  6, 46, 14, 54, 22, 62, 30, 37,  5, 45, 13, 53, 21, 61, 29,
                       36,  4, 44, 12, 52, 20, 60, 28, 35,  3, 43, 11, 51, 19, 59, 27,
                       34,  2, 42, 10, 50, 18, 58, 26, 33,  1, 41,  9, 49, 17, 57, 25};

static int EP[48] = {32,  1,  2,  3,  4,  5,
                      4,  5,  6,  7,  8,  9,
                      8,  9, 10, 11, 12, 13,
                     12, 13, 14, 15, 16, 17,
                     16, 17, 18, 19, 20, 21,
                     20, 21, 22, 23, 24, 25,
                     24, 25, 26, 27, 28, 29,
                     28, 29, 30, 31, 32,  1};

static int S_box[8][64] = {{14,  4, 13,  1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7,
                             0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8,
                             4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0,
                            15, 12,  8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  6, 13},
                           {15,  1,  8, 14,  6, 11,  3,  4,  9,  7,  2, 13, 12,  0,  5, 10,
                             3, 13,  4,  7, 15,  2,  8, 14, 12,  0,  1, 10,  6,  9, 11,  5,
                             0, 14,  7, 11, 10,  4, 13,  1,  5,  8, 12,  6,  9,  3,  2, 15,
                            13,  8, 10,  1,  3, 15,  4,  2, 11,  6,  7, 12,  0,  5, 14,  9},
                           {10,  0,  9, 14,  6,  3, 15,  5,  1, 13, 12,  7, 11,  4,  2,  8,
                            13,  7,  0,  9,  3,  4,  6, 10,  2,  8,  5, 14, 12, 11, 15,  1,
                            13,  6,  4,  9,  8, 15,  3,  0, 11,  1,  2, 12,  5, 10, 14,  7,
                             1, 10, 13,  0,  6,  9,  8,  7,  4, 15, 14,  3, 11,  5,  2, 12},
                           { 7, 13, 14,  3,  0,  6,  9, 10,  1,  2,  8,  5, 11, 12,  4, 15,
                            13,  8, 11,  5,  6, 15,  0,  3,  4,  7,  2, 12,  1, 10, 14,  9,
                            10,  6,  9,  0, 12, 11,  7, 13, 15,  1,  3, 14,  5,  2,  8,  4,
                             3, 15,  0,  6, 10,  1, 13,  8,  9,  4,  5, 11, 12,  7,  2, 14},
                           { 2, 12,  4,  1,  7, 10, 11,  6,  8,  5,  3, 15, 13,  0, 14,  9,
                            14, 11,  2, 12,  4,  7, 13,  1,  5,  0, 15, 10,  3,  9,  8,  6,
                             4,  2,  1, 11, 10, 13,  7,  8, 15,  9, 12,  5,  6,  3,  0, 14,
                            11,  8, 12,  7,  1, 14,  2, 13,  6, 15,  0,  9, 10,  4,  5,  3},
                           {12,  1, 10, 15,  9,  2,  6,  8,  0, 13,  3,  4, 14,  7,  5, 11,
                            10, 15,  4,  2,  7, 12,  9,  5,  6,  1, 13, 14,  0, 11,  3,  8,
                             9, 14, 15,  5,  2,  8, 12,  3,  7,  0,  4, 10,  1, 13, 11,  6,
                             4,  3,  2, 12,  9,  5, 15, 10, 11, 14,  1,  7,  6,  0,  8, 13},
                           { 4, 11,  2, 14, 15,  0,  8, 13,  3, 12,  9,  7,  5, 10,  6,  1,
                            13,  0, 11,  7,  4,  9,  1, 10, 14,  3,  5, 12,  2, 15,  8,  6,
                             1,  4, 11, 13, 12,  3,  7, 14, 10, 15,  6,  8,  0,  5,  9,  2,
                             6, 11, 13,  8,  1,  4, 10,  7,  9,  5,  0, 15, 14,  2,  3, 12},
                           {13,  2,  8,  4,  6, 15, 11,  1, 10,  9,  3, 14,  5,  0, 12,  7,
                             1, 15, 13,  8, 10,  3,  7,  4, 12,  5,  6, 11,  0, 14,  9,  2,
                             7, 11,  4,  1,  9, 12, 14,  2,  0,  6, 10, 13, 15,  3,  5,  8,
                             2,  1, 14,  7,  4, 10,  8, 13, 15, 12,  9,  0,  3,  5,  6, 11}};

static int Pv[32] = {16,  7, 20, 21, 29, 12, 28, 17,  1, 15, 23, 26,  5, 18, 31, 10,
                     2,  8, 24, 14, 32, 27,  3,  9, 19, 13, 30,  6, 22, 11,  4, 25};


DesOperate::DesOperate(QString key) {
    if (key.length()!=8) {
        qDebug()<<"key error";
        m_K = "PASSWORD";
    }
    else
        m_K = key;
    makeKey();
}

DesOperate::~DesOperate() {
}

QString DesOperate::Ik(QString K) {
    QString res;
    for (int i=0; i<56; i++)
        res.append(K.at(I[i]-1));
    return res;
}

QString DesOperate::Lk(QString IkK, int lnum) {
    int n = 0;
    if (lnum<2 || lnum==8 || lnum==15)
        n = 1;
    else
        n = 2;
    for (int i=0; i<n; i++) {
        QString tp0 = IkK.mid(0, 1);
        QString tp1 = IkK.mid(1, 27);
        QString tp2 = IkK.mid(28, 1);
        QString tp3 = IkK.mid(29, 27);
        IkK = tp1 + tp0 + tp3 + tp2;
    }
    return IkK;
}

QString DesOperate::Pk(QString X) {
    QString res;
    for (int i=0; i<48; i++)
        res.append(X.at(P[i]-1));
    return res;
}

QString DesOperate::IPmc(QString mc) {
    QString res;
    for (int i=0; i<64; i++)
        res.append(mc.at(IP[i]-1));
    return res;
}

QString DesOperate::UnIPmc(QString mc) {
    QString res;
    for (int i=0; i<64; i++)
        res.append(mc.at(UnIP[i]-1));
    return res;
}

QString DesOperate::dec2b(int n, int fl) {
    QString res;
    if (fl==0) {
        int a[4], i;
        for (i=0; i<4; i++) {
            a[i] = n%2;
            n = n/2;
        }
        for (i=i-1; i>=0; i--)
            res.append(QString::number(a[i]));
    }
    else {
        int a[8], i;
        for (i=0; i<8; i++) {
            a[i] = n%2;
            n = n/2;
        }
        for (i=i-1; i>=0; i--)
            res.append(QString::number(a[i]));
    }
    return res;
}

QString DesOperate::F(QString R, QString K) {
    QString res;
    QString rtmp;
    for (int i=0; i<48; i++)
        res.append(R.at(EP[i]-1));
    // qDebug()<<"EP(R): "<<res;
    for (int i=0; i<6; i++) {
        QString tp0(res.mid(i*8, 8));  // 8位一组进行异或
        QString tp1(K.mid(i*8, 8));
        int val0 = tp0.toInt(nullptr, 2);
        int val1 = tp1.toInt(nullptr, 2);
        rtmp.append(dec2b(val0 xor val1, 1));
    }
    res = rtmp;
    // qDebug()<<"EP(R) xor K: "<<res;
    int y[8];
    for (int i=0; i<8; i++) {
        QString tp0;
        tp0.append(res.at(i*6));
        tp0.append(res.at(i*6+5));
        int row = tp0.toInt(nullptr, 2);
        QString tp1 = res.mid(i*6+1, 4);
        int col = tp1.toInt(nullptr, 2);
        y[i] = S_box[i][16*row+col];
        // qDebug()<<"y["<<i<<"]: "<<y[i];
    }
    QString tmp;
    for (int i=0; i<8; i++)
        tmp.append(dec2b(y[i], 0));
    res = "";
    for (int i=0; i<32; i++)
        res.append(tmp[Pv[i]-1]);
    // qDebug()<<"Pv: "<<res;
    return res;
}

QString DesOperate::desC(QString M) {
    QString L;
    QString R;
    QString tmpL(M.mid(0, 32));
    QString tmpR(M.mid(32, 32));
    QString res;
    for (int i=0; i<16; i++) {
        L = tmpR;
        QString tp = F(tmpR, m_key[i]);
        for (int i=0; i<4; i++) {
            QString tp0(tmpL.mid(i*8, 8));
            QString tp1(tp.mid(i*8, 8));
            int val0 = tp0.toInt(nullptr, 2);
            int val1 = tp1.toInt(nullptr, 2);
            R.append(dec2b(val0 xor val1, 1));
        }
        tmpL = L;
        tmpR = R;
        R = "";
        // qDebug()<<"LR_"<<(i+1)<<": "<<tmpL<<" "<<tmpR;
    }
    res = UnIPmc(tmpR + tmpL);
    return res;
}

QString DesOperate::desM(QString C) {
    QString L;
    QString R;
    QString tmpL(C.mid(0, 32));
    QString tmpR(C.mid(32, 32));
    QString res;
    for (int i=15; i>=0; i--) {
        L = tmpR;
        QString tp = F(tmpR, m_key[i]);
        for (int i=0; i<4; i++) {
            QString tp0(tmpL.mid(i*8, 8));
            QString tp1(tp.mid(i*8, 8));
            int val0 = tp0.toInt(nullptr, 2);
            int val1 = tp1.toInt(nullptr, 2);
            R.append(dec2b(val0 xor val1, 1));
        }
        tmpL = L;
        tmpR = R;
        R = "";
        // qDebug()<<"LR_"<<(i+1)<<": "<<tmpL<<" "<<tmpR;
    }
    res = UnIPmc(tmpR + tmpL);
    return res;
}

bool DesOperate::makeKey() {
    QString IkK = Ik(key2b(m_K));
    QString UV[16];
    for (int i=0; i<16; i++) {
        UV[i] = Lk(IkK, i);
        IkK = UV[i];
    }
    for (int i=0; i<16; i++)
        m_key[i] = Pk(UV[i]);
    return true;
}

QString DesOperate::b2ct(QString b) {
    QString res;
    QString tmp;
    for (int i=0; i<b.length(); i+=8) {
        tmp.append(b.mid(i, 8));
        if (tmp.toInt() == 0)
            continue;
        res.append(QChar(tmp.toInt(nullptr, 2)));
        tmp = "";
    }
    // qDebug()<<res;
    return res;
}

QString DesOperate::ct2b(QString ct) {
    QString res;
    for (int i=0; i<ct.length(); i++)
        res.append(dec2b(ct.at(i).unicode(), 1));
    // qDebug()<<"ct: "<<res<<endl;
    int fl = ct.length()%8;
    if (fl==0)
        return res;
    else
        for (int i=0; i<8-fl; i++)
            res.append("00000000");
    // qDebug()<<"ct-00: "<<res<<endl;
    return res;
}

QString DesOperate::key2b(QString key) {
    QString res;
    QString ktmp;
    for (int i=0; i<8; i++)
        ktmp.append(dec2b(key.at(i).unicode(), 1));
    // qDebug()<<"k: "<<ktmp;
    for (int i=0; i<8; i++) {
        int count = 0;
        QString tp = ktmp.mid(i*8+1, 7);
        for (int j=0; j<7; j++) {
            if (tp[j]=='1')
                count++;
        }
        res.append(tp);
        if (count%2==0)
            res.append('1');
        else
            res.append('0');
    }
    // qDebug()<<"kr: "<<res;
    return res;
}

QString DesOperate::makeData(QString M) {
    QString IPM = IPmc(M);
    QString cc = desC(IPM);
    return cc;
}

QString DesOperate::reData(QString C) {
    QString IPC = IPmc(C);
    QString mm = desM(IPC);
    return mm;
}

QString DesOperate::msg_handle(QString msg) {
    QString res;
    QString tmp;
    for (int i=0; i<msg.length(); i+=8) {
        tmp = msg.mid(i, 8);
        QString m_part = ct2b(tmp);
        QString tp = makeData(m_part);
        for (int j=0; j<8; j++) {
            QString substr = tp.mid(j*8, 8);
            int zip = substr.toInt(nullptr, 2);
            res.append(zip);
            // qDebug()<<"zip: "<<zip<<" "<<res;
        }
    }
    // qDebug()<<"come on: "<<res;
    return res;
}

QString DesOperate::remsg(QString msg) {
    QString res;
    QString tmp;
    for (int i=0; i<msg.length(); i++) {
        QString p_msg = dec2b(msg.at(i).unicode(), 1);
        // qDebug()<<"p_msg: "<<p_msg;
        tmp.append(p_msg);
        if (i%8==7) {
            QString m_part = reData(tmp);
            res.append(b2ct(m_part));
            // qDebug()<<"part: "<<res;
            tmp = "";
        }
    }
    return res;
}
