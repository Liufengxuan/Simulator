/********************************************************************************
** Form generated from reading UI file 'mainfrom.ui'
**
** Created by: Qt User Interface Compiler version 5.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINFROM_H
#define UI_MAINFROM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainForm
{
public:
    QFrame *frame;
    QSpinBox *sb_speed;
    QListWidget *listWidget_NC;
    QLabel *label;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QCheckBox *cb_ShowStatus;
    QCheckBox *cb_ShowSideView;
    QWidget *widget1;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *btn_start;
    QPushButton *btn_stop;
    QPushButton *btn_exit;

    void setupUi(QWidget *MainForm)
    {
        if (MainForm->objectName().isEmpty())
            MainForm->setObjectName(QString::fromUtf8("MainForm"));
        MainForm->resize(1108, 717);
        frame = new QFrame(MainForm);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setGeometry(QRect(0, 0, 361, 211));
        frame->setMouseTracking(true);
        frame->setAcceptDrops(true);
        frame->setStyleSheet(QString::fromUtf8("background-color: rgb(221, 244, 255);"));
        frame->setFrameShape(QFrame::NoFrame);
        frame->setFrameShadow(QFrame::Plain);
        sb_speed = new QSpinBox(frame);
        sb_speed->setObjectName(QString::fromUtf8("sb_speed"));
        sb_speed->setGeometry(QRect(300, 160, 51, 41));
        QFont font;
        font.setFamily(QString::fromUtf8("\351\273\221\344\275\223"));
        font.setPointSize(16);
        sb_speed->setFont(font);
        sb_speed->setFocusPolicy(Qt::NoFocus);
        sb_speed->setReadOnly(false);
        sb_speed->setMinimum(1);
        sb_speed->setMaximum(16);
        sb_speed->setSingleStep(1);
        listWidget_NC = new QListWidget(frame);
        listWidget_NC->setObjectName(QString::fromUtf8("listWidget_NC"));
        listWidget_NC->setGeometry(QRect(10, 10, 341, 151));
        QFont font1;
        font1.setFamily(QString::fromUtf8("\345\256\213\344\275\223"));
        listWidget_NC->setFont(font1);
        listWidget_NC->setLayoutDirection(Qt::LeftToRight);
        listWidget_NC->setAutoFillBackground(false);
        listWidget_NC->setFrameShape(QFrame::StyledPanel);
        label = new QLabel(frame);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(260, 170, 31, 16));
        widget = new QWidget(frame);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(10, 190, 150, 18));
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        cb_ShowStatus = new QCheckBox(widget);
        cb_ShowStatus->setObjectName(QString::fromUtf8("cb_ShowStatus"));
        cb_ShowStatus->setChecked(true);

        horizontalLayout->addWidget(cb_ShowStatus);

        cb_ShowSideView = new QCheckBox(widget);
        cb_ShowSideView->setObjectName(QString::fromUtf8("cb_ShowSideView"));
        cb_ShowSideView->setChecked(true);

        horizontalLayout->addWidget(cb_ShowSideView);

        widget1 = new QWidget(frame);
        widget1->setObjectName(QString::fromUtf8("widget1"));
        widget1->setGeometry(QRect(11, 161, 239, 27));
        horizontalLayout_2 = new QHBoxLayout(widget1);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        btn_start = new QPushButton(widget1);
        btn_start->setObjectName(QString::fromUtf8("btn_start"));
        QFont font2;
        font2.setFamily(QString::fromUtf8("Arial"));
        font2.setPointSize(11);
        btn_start->setFont(font2);
        btn_start->setCheckable(false);

        horizontalLayout_2->addWidget(btn_start);

        btn_stop = new QPushButton(widget1);
        btn_stop->setObjectName(QString::fromUtf8("btn_stop"));
        btn_stop->setFont(font2);

        horizontalLayout_2->addWidget(btn_stop);

        btn_exit = new QPushButton(widget1);
        btn_exit->setObjectName(QString::fromUtf8("btn_exit"));
        btn_exit->setFont(font2);

        horizontalLayout_2->addWidget(btn_exit);


        retranslateUi(MainForm);

        QMetaObject::connectSlotsByName(MainForm);
    } // setupUi

    void retranslateUi(QWidget *MainForm)
    {
        MainForm->setWindowTitle(QApplication::translate("MainForm", "\345\205\255\351\235\242\351\222\273\345\212\250\344\275\234\346\250\241\346\213\237", nullptr));
        label->setText(QApplication::translate("MainForm", "\351\200\237\345\272\246:", nullptr));
        cb_ShowStatus->setText(QApplication::translate("MainForm", "\346\230\276\347\244\272\347\212\266\346\200\201", nullptr));
        cb_ShowSideView->setText(QApplication::translate("MainForm", "\346\230\276\347\244\272\344\276\247\350\247\206", nullptr));
        btn_start->setText(QApplication::translate("MainForm", "start", nullptr));
        btn_stop->setText(QApplication::translate("MainForm", "stop", nullptr));
        btn_exit->setText(QApplication::translate("MainForm", "reset", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainForm: public Ui_MainForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINFROM_H
