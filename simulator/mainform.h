#ifndef MAINFROM_H
#define MAINFROM_H
#include <QWidget>
#include <xmlparsing.h>
#include"ncparsing.h"
#include "simworker.h"
#include "ui_mainfrom.h"
#include<qmessagebox.h>
#include <QThread>
#include <QPainter>
#include <QPointF>
#include <QMouseEvent>
#include <QtGlobal>
#include"math.h"
//#include "simparameter.h";
#include "qfont.h"
#include"qmessagebox.h"




using namespace NcCode;
using namespace Parameters;
using namespace Simulator;
namespace Ui {
class MainForm;
}

class MainForm : public QWidget
{
    Q_OBJECT

public:
    explicit MainForm(QWidget *parent = nullptr);
    ~MainForm();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void paintEvent(QPaintEvent *event);
    void showEvent(QShowEvent *event);

    void DrawDatumPoint(QPainter &painter);
    void DrawDrills(QPainter &painter);
    void DrawClamp(QPainter &painter);
    void DrawWord(QPainter &painter);
    void DrawPart(QPainter &painter);
    void DrawSideView(QPainter &painter,m_Drill *drill);



private:
    Ui::MainForm *ui;
    QThread *workThread;
    Simulator::SimWorker *simWork;
    QMap<QString,float> REAL_TIME_POS;
    QStringList REAL_M_T_CODE;





public:
    int parm_argc;
    char **parm_argv;
protected:
    float zoom=0.5;
    float oldZoom=1.0;
    QPointF mousePos;
    QPointF LeftTopPos;//绘图的实时原点
    QPixmap tempPix;//缓冲图片
    bool moveButtonPress=false;//拖动按钮按下

    ///是否允许绘图
    bool ALLOW_PAINT=false;
    ///最大缩放比
    float ZOOM_MAX=4.0;
    ///最小缩放比
    float ZOOM_MIN=0.2;
    ///缩放步进值
    float STEP_VALUE=0.1;
    ///Π
    float PI=3.14159265;
    bool ANTI_ALIASING=false;//抗锯齿
    Qt::GlobalColor BACK_COLOR=Qt::black;//背景色
    ///Y轴镜像
    bool MIRROR_Y_AXIS=false;
    ///窗口初始大小
    QSize INIT_FORM_SIZE=QSize(1366,900);



    ///默认画笔
     QPen DEFAULT_PEN=QPen(Qt::white,1,Qt::PenStyle::SolidLine);
     QBrush DEFAULT_BRUSH=QBrush(Qt::white,Qt::NoBrush);
    ///坐标系线段长度
    float COOR_LEN=120;
    ///夹钳滑轨长度
    float SLIDE_RAIL=3000;
    ///夹钳滑轨宽度
    float SLIDE_RAIL_WIDTH=30;
    ///壕沟长度
    float TRENCH_LEN=1200;
    ///夹钳滑轨画笔
    QPen SLIDE_RAIL_PEN=QPen(QColor(105,105,105),1,Qt::PenStyle::SolidLine);
    QBrush SLIDE_RAIL_BRUSH=QBrush(QColor(70,70,70),Qt::SolidPattern);
    ///坐标系的画笔
    QPen COOR_PEN=QPen(QColor(0,255,0),2,Qt::PenStyle::SolidLine);
    ///钻头打出时的笔和画刷
    QPen DRILL_DOWN_PEN=QPen(QColor(255,0,0),1.5,Qt::PenStyle::SolidLine);
    QBrush DRILL_DOWN_BRUSH=QBrush(QColor(255,0,0),Qt::NoBrush);
    ///垂直钻收回时的颜色
    QPen VER_DRILL_PEN=QPen(QColor(0,245,255),1,Qt::PenStyle::SolidLine);
    ///水平钻收回时的颜色
    QPen HOR_DRILL_PEN=QPen(QColor(0,255,127),1,Qt::PenStyle::SolidLine);
    ///压料收回时的颜色
    QPen PRESS_PEN=QPen(QColor(255,215,0),1,Qt::PenStyle::SolidLine);
    QBrush PRESS_DOWN_BRUSH=QBrush(QColor(255,215,0),Qt::Dense6Pattern);
    ///板子的颜色
    QPen PANEL_PEN=QPen(QColor(220,220,220),1,Qt::PenStyle::SolidLine);
    QBrush PANEL_BRUSH=QBrush(QColor(160,82,45),Qt::SolidPattern);
    ///正面和反面孔的颜色
//    QPen MACHINE_FRONT_PEN=QPen(Qt::white,1,Qt::PenStyle::SolidLine);
//    QPen MACHINE_BACK_PEN=QPen(Qt::blue,1,Qt::PenStyle::SolidLine);
    QBrush MACHINE_FRONT_Brush=QBrush(QColor(186, 186, 186),Qt::SolidPattern);
    QBrush MACHINE_BACK_Brush=QBrush(Qt::blue,Qt::SolidPattern);




    ///夹钳的画笔
    QPen CLAMP_OPEN_PEN=QPen(QColor(220,220,220),1,Qt::PenStyle::SolidLine);
    QBrush CLAMP_CLOSE_BRUSH=QBrush(QColor(105,105,105),Qt::SolidPattern);

    const QString VER_DRILL="VerDrill";//垂直钻
    const QString HOR_LEFT_DIRECTION="HorLeftDirection";// 水平向左
    const QString HOR_RIGHT_DIRECTION="HorRightDirection";// 水平向右
    const QString HOR_UP_DIRECTION="HorUpDirection";// 水平向上
    const QString HOR_DOWN_DIRECTION="HorDownDirection";//水平向下
    const QString SPINDLE="Spindle";//主轴
    const QString SAW="Saw";//锯片

    const QString VERTICAL_ROLLER="VerticalRoller";//垂直压轮
    const QString VERTICAL_CLAMP="VerticalClamp";//垂直压料
    const QString HORIZONTAL_CLAMP="HorizontalClamp";//水平压料
    const QString HORIZONTAL_ROLLER="HorizontalRoller";//水平压轮
    const QString MAIN_PRESS="MainPress";//主轴压料
    const QString UNDERLAY="Underlay";//下托料
    ///水平钻长度
    const float HOR_DRILL_LEN=60;




    const QString PACKAGE_TOP1="Top1";//Y1
    const QString PACKAGE_TOP2="Top2";//Y3
    const QString PACKAGE_BOTTOM1="Bottom1";//Y2
    const QString PACKAGE_BOTTOM_MAIN="BottomMain";
    const QString PACKAGE_TOPMAIN="TopMain";
    const QFont WORD_FONT=QFont("Consoles",14,QFont::Normal,false);

private slots:
    void Sim_Handle(QMap<QString,float> pos,const NCCommand* nc,QStringList mAndTCode);
    void btn_start_clicked();
    void btn_stop_clicked();
    void btn_reset_clicked();
    void EditSimSpeed(int);

signals:
    void doWork(int);


};

#endif // MAINFROM_H
