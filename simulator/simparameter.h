#ifndef SIMPARAMETER_H
#define SIMPARAMETER_H

#include <QObject>
#include "parametermodel.h"
#include<QPointF>

namespace  Simulator{
struct m_Drill:Parameters::DrillInfo,Parameters::MCodeSetup
{
    QPointF Draw_State_Pos;
    bool    Draw_State_IsSelect;
};


//    class SimParameter : public QObject
//    {
//        Q_OBJECT
//    public:
//        explicit SimParameter(QObject *parent = nullptr);
//        ~SimParameter();



//    public slots:


//    };



}


#endif // SIMPARAMETER_H
