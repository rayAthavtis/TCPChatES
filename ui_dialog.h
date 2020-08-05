/********************************************************************************
** Form generated from reading UI file 'dialog.ui'
**
** Created by: Qt User Interface Compiler version 5.13.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOG_H
#define UI_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextBrowser>

QT_BEGIN_NAMESPACE

class Ui_Dialog
{
public:
    QPushButton *m_start;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *icon;
    QTextBrowser *m_browser;
    QPushButton *m_send;
    QLineEdit *m_msg;

    void setupUi(QDialog *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName(QString::fromUtf8("Dialog"));
        Dialog->resize(762, 508);
        m_start = new QPushButton(Dialog);
        m_start->setObjectName(QString::fromUtf8("m_start"));
        m_start->setGeometry(QRect(630, 20, 112, 32));
        m_start->setAutoDefault(true);
        label_2 = new QLabel(Dialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(240, 10, 251, 31));
        label_3 = new QLabel(Dialog);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(250, 40, 231, 20));
        icon = new QLabel(Dialog);
        icon->setObjectName(QString::fromUtf8("icon"));
        icon->setGeometry(QRect(20, 10, 50, 45));
        m_browser = new QTextBrowser(Dialog);
        m_browser->setObjectName(QString::fromUtf8("m_browser"));
        m_browser->setGeometry(QRect(20, 60, 721, 381));
        m_send = new QPushButton(Dialog);
        m_send->setObjectName(QString::fromUtf8("m_send"));
        m_send->setGeometry(QRect(630, 460, 112, 32));
        m_send->setAutoDefault(false);
        m_msg = new QLineEdit(Dialog);
        m_msg->setObjectName(QString::fromUtf8("m_msg"));
        m_msg->setGeometry(QRect(20, 460, 601, 31));

        retranslateUi(Dialog);

        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dialog)
    {
        Dialog->setWindowTitle(QCoreApplication::translate("Dialog", "SoChat S", nullptr));
        m_start->setText(QCoreApplication::translate("Dialog", "Start", nullptr));
        label_2->setText(QCoreApplication::translate("Dialog", "<html><head/><body><p><span style=\" font-size:24pt;\">SO CHAT SERVER V2.0</span></p></body></html>", nullptr));
        label_3->setText(QCoreApplication::translate("Dialog", "<html><head/><body><p><span style=\" font-family:'Menlo'; color:#5d6c79;\">Copyright \302\251 2020 Vodka(\344\273\273\350\225\276).</span></p></body></html>", nullptr));
        icon->setText(QCoreApplication::translate("Dialog", "<html><head/><body><p><img src=\":/new/prefix1/TcpChat.png\"/></p></body></html>", nullptr));
        m_send->setText(QCoreApplication::translate("Dialog", "Send", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Dialog: public Ui_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOG_H
