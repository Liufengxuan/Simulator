#ifndef SIMWORKER_H
#define SIMWORKER_H

#include <QObject>
#include <windows.h>
#include<QtDebug>
#include <QThread>
#include <QObject>
#include <ncparsing.h>
#include<xmlparsing.h>
#include"simparameter.h"
using namespace Parameters;
using namespace NcCode;


namespace  Simulator{
///run:线程运行，Stop：线程暂停，Exit线程工作退出。
enum WORK_STATE
 {
     RUN = 0,
     STOP,
     EXIT,
    RESET,
 };

class SimWorker : public QObject
{
    Q_OBJECT
public:
    explicit SimWorker(QObject *parent = nullptr);
    ~SimWorker();
     ///加载仿真所需的支撑数据
     bool LoadSupportingData(const QString &p1,const QString &p2,const QString &p3);

private:
       void InitRealTimePos();
signals:
     void SimReady( QMap<QString,float> pos,const NCCommand* nc,QStringList realMAndTCode);
public slots:
    ///工作线程方法
    void DoWork(int param);

public:

    const QString workStateString[4] = {"RUN","STOP","EXIT","RESET"};
    ///获取及设置线程的工作状态
    WORK_STATE workState;
    ///xml配置文件!!!!!!运行时这个数据不可修改
    XMLParsing *xml;
    ///解析完成的nc代码!!!!!!运行时这个数据不可修改
    NcParsing *nc;
    ///未执行nc时 各轴的默认位置
    float DEFAULT_POS=-5000;

    //以下为全局参数
    ///夹钳y方向长度   初始化后不可修改
    float ClampWidth=30;
    ///夹钳x方向长度   初始化后不可修改
    float ClampLength=130;
    ///测量+双夹钳夹
    QString Mc_M403="M403";
    ///双夹钳松开
    QString McPawsAllOpen="M308";
    ///双夹钳夹紧
    QString McPawsAllClose="M307";
    ///夹钳1松开
    QString McPaw1Loosen="M310";
    ///夹钳1夹紧
    QString McPaw1Close="M309";
    ///夹钳2松开
    QString McPaw2Loosen="M312";
    ///夹钳2夹紧
    QString McPaw2Close="M311";

    ///机器壕沟最小值对应的X坐标
    float TrenchMin=496;
    ///机器壕沟最da值对应的X坐标
    float TrenchMax=639;
    ///上主轴上（启用主轴压料时）
    QString McMainUp="M301";
    ///上主轴下（启用主轴压料时）
    QString McMainDown="M302";
    ///台面上升
    QString McTableUpCode="M348";
    ///台面下降
    QString McTableDownCode="M347";

    ///上主轴启动代码
    QString McMainStart="M43";
    ///上主轴停止代码
    QString McMainEnd="M45";
    ///下主轴启动代码
    QString McSMainStart="M53";
    ///下主轴停止代码
    QString McSMainEnd="M55";
    ///上主轴预启动
    QString McMainPreStart="M43";
    ///下主轴预启动
    QString McSMainPreStart="M53";
    ///是否Y轴镜像
    int MachineMirrorType=0;


    ///机床上所有钻头的信息。
    QVector<m_Drill*> drills;
    ///步进值
    int STEP_VALUE=4;
private:
    ///存储实时坐标
    QMap<QString,float> realTimePos;
    ///存储实时M代码
    QStringList realMAndTCode;


    ///刷新间隔 毫秒
    int WORK_INTRTVAL=15;

    NcCode::NCCommand *CURRENT_CMD=nullptr;
    int CURRENT_CMD_INDEX=0;



};
}
#endif // SIMWORKER_H
