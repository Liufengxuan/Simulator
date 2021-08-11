#ifndef NCCODEMODEL_H
#define NCCODEMODEL_H
#include "QString"
#include"qdebug.h"


namespace NcCode{

#pragma once
#define AXIS_MAXNUM 8

enum FuncType
{
    ///空行
    none=1,
    ///移动指令
    gCode=2,
    ///m代码指令
    mCode=4,
    ///钻头调用指令
    tCode=8,
    ///暂停指令
    stop=16
};

///这个结构体代表一行nc的解析结果
///
struct NCCommand {
    QString source;
    QString lineNum;
    FuncType funcType=FuncType::none;
    QString axisFlag[AXIS_MAXNUM];   
    float loction[AXIS_MAXNUM]={0};
    QString tCode[AXIS_MAXNUM];
    QString mCode;
    QString gCode1;
    QString gCode2;
    float speed=0;
    float stop=0;
    ~NCCommand(){
        //qDebug()<<lineNum;
    }


};
}
#endif // NCCODEMODEL_H
