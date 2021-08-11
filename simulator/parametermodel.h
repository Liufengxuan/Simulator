#ifndef PARAMETERMODEL_H
#define PARAMETERMODEL_H

#include <string>
#include<ostream>
#include <qvector.h>
#include <qdebug.h>


namespace  Parameters {

enum MachineType{
    HHole=1,
    VHole=2,
    Groove=4,
};
struct Machining{
     int Type;
     int Face;
     float X=0;
     float Y=0;
     float Depth=0;
     float Diameter=0;
     float  Width=0;
      float  EndX=0;
      float  EndY=0;
};

struct Part{
     QString Name;
     float width=0;
     float Length=0;
     float Thickness=0;
     QVector<Machining> Machines;
};


struct BasicSetup {
    QString Name;
    QString Value;
    QString Description;
    bool ReadOnly;

    BasicSetup(QString name,QString value,QString description,bool readOnly)
        :Name(std::move(name)),Value(std::move(value)),Description(std::move(description)),ReadOnly(readOnly)
    {}
    BasicSetup(){}
};

struct MCodeSetup {
    QString MCode;
    QString SelectCode;
    QString BackCode;

    MCodeSetup(QString mCode,QString selectCode,QString backCode)
        :MCode(std::move(mCode)),SelectCode(std::move(selectCode)),BackCode(std::move(backCode))
    {}
    MCodeSetup(){}

};

struct PressUpDownSetup {
    QString PackageID;
    QString Type;
    QString Description;
    float Length=0;
    float Width=0;
    float CenterX=0;
    float CenterY=0;
    float PressedZ=0;
    QString UpCode;
    QString DownCode;
    int UpDelay=0;
    int DownDelay=0;
    QString PressID;
    bool ReadOnly=0;
    int UpDown=0;
    PressUpDownSetup(QString packageID,QString type,QString description,float length,
                     float width,float centerX,float centerY,float pressedZ,QString upCode,
                     QString downCode,int upDelay,int downDelay,QString pressId,bool readOnly,int upDown)
        :PackageID(std::move(packageID)),Type(std::move(type)),Description(std::move(description)),Length(length),
         Width(width),CenterX(centerX),CenterY(centerY),PressedZ(pressedZ),UpCode(std::move(upCode)),DownCode(std::move(downCode)),UpDelay(upDelay),
         DownDelay(downDelay),PressID(std::move(pressId)),ReadOnly(readOnly),UpDown(upDown)
    {

    }
    PressUpDownSetup(){}


};

struct DrillInfo{
    QString ID;
    QString Type;
    int Diameter=0;
    float PosX=0;
    float PosY=0;
    float PosZ=0;
    float RelPosX=0;
    float RelPosY=0;
    float RelPosZ=0;
    float SafeDistance=0;
    int DownVel=0;
    int MachiningVel=0;
    int MillingVel=0;
    int DelayTime=0;
    bool Enable=0;
    bool IsLone=0;
    QString PackageID;
    bool BasedDrill=0;
    bool PackageEnable=0;
    bool Throughdrill=0;
    int LastLen=0;
    int InitialVel=0;
    int DrillLen=0;
    int WorkStatus=0;




    DrillInfo(QString id,QString type,int diameter,float posX,float posY,float posZ,float relPosX,float relPosY,float relPosZ,
              float safeDistance,int downVel,int machiningVel,int millingVel,int delayTime,bool enable,bool isLone,QString packageID,
              bool basedDrill,bool packageEnable,bool throughdrill,int lastLen,int initialVel,int drillLen,int workStatus)
        :ID(std::move(id)),Type(std::move(type)),Diameter(diameter),PosX(posX),PosY(posY),PosZ(posZ),RelPosX(relPosX),RelPosY(relPosY),RelPosZ(relPosZ),
          SafeDistance(safeDistance),DownVel(downVel),MachiningVel(machiningVel),MillingVel(millingVel),DelayTime(delayTime),Enable(enable),
          IsLone(isLone),PackageID(std::move(packageID)),BasedDrill(basedDrill),PackageEnable(packageEnable),Throughdrill(throughdrill),
          LastLen(lastLen),InitialVel(initialVel),DrillLen(drillLen),WorkStatus(workStatus)
    {

    }
    DrillInfo(){}

};

struct  Config{
    QVector<BasicSetup> BasicSetups;
    QVector<MCodeSetup> MCodeSetups;
    QVector<PressUpDownSetup> PressUpDownSetups;
    QVector<DrillInfo> DrillInfos;
//    ~Config(){
//        qDebug()<<"132";
//    }
};

}
#endif // PARAMETERMODEL_H
