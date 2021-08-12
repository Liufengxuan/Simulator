#include "simworker.h"


Simulator::SimWorker::SimWorker(QObject *parent) : QObject(parent)
{
    workState=WORK_STATE::EXIT;

}

void Simulator::SimWorker::DoWork(int param)
{
    CURRENT_CMD_INDEX=0;
    for (;;) {
        if(workState==WORK_STATE::RUN)
        {
            //检查所有轴是否都移动完毕,移动完毕将当前命令行索引+1
            if(CURRENT_CMD!=nullptr)
            {
                bool cmdIsFinish=true;
                for (int idx = 0; idx < AXIS_MAXNUM; idx++) {
                    if(!realTimePos.contains(CURRENT_CMD->axisFlag[idx]))continue;
                    if( realTimePos[CURRENT_CMD->axisFlag[idx]]!=CURRENT_CMD->loction[idx])
                    {
                        cmdIsFinish=false;
                        break;
                    }
                }
                if(CURRENT_CMD->funcType==NcCode::FuncType::stop)
                {
                    if(timer.hasExpired(CURRENT_CMD->stop))
                    {
                         cmdIsFinish=true;
                    }else{
                        cmdIsFinish=false;
                        continue;
                    }

                }

                //都移动完毕后，将当前索引换到下一行
                if(cmdIsFinish)CURRENT_CMD_INDEX++;
            }


            //如果行索引超出列表数量表示模拟完毕，将当前行索引及当前nc变量恢复默认值，并且暂停
            if(CURRENT_CMD_INDEX>=nc->ncCmds.count())
            {
                workState=WORK_STATE::STOP;
                CURRENT_CMD_INDEX=0;
                CURRENT_CMD=nullptr;
                InitRealTimePos();
                continue;
            }
            //如果当前命令的索引在范围内，则根据索引获取本行命令
            CURRENT_CMD=nc->ncCmds[CURRENT_CMD_INDEX];


            //如果是移动指令
            if(CURRENT_CMD->funcType==NcCode::FuncType::gCode||!CURRENT_CMD->axisFlag[0].isEmpty()){
                QString mainPaw;

                //如果当前m代码中有全部夹紧或者X1夹紧，那就让板件跟随X1
                if(realMAndTCode.contains(McPawsAllClose)||realMAndTCode.contains(McPaw1Close))
                {
                    mainPaw="X1";
                }
                else if(realMAndTCode.contains(McPaw2Close))
                {
                    mainPaw="X2";
                }
                for (int idx = 0; idx < AXIS_MAXNUM; idx++) {
                    //如果指令不存在于realTimePos中表示不需要处理。
                    if(!realTimePos.contains(CURRENT_CMD->axisFlag[idx]))continue;
                    //如果当前坐标是默认坐标-5000，则让该轴直接移动到目标位置。
                    if(realTimePos[CURRENT_CMD->axisFlag[idx]]==DEFAULT_POS)
                    {
                        realTimePos[CURRENT_CMD->axisFlag[idx]]=CURRENT_CMD->loction[idx];
                        //如果是X夹钳，并且板件在默认位置-5000时将板件靠到定位柱的位置
                        if(CURRENT_CMD->axisFlag[idx].contains(mainPaw)&&realTimePos["PART"]==DEFAULT_POS)
                        {
                            realTimePos["PART"]=-xml->part->Length;
                            //qDebug()<<"A X1-PART"<<realTimePos[CURRENT_CMD->axisFlag[idx]]-realTimePos["PART"];
                        }
                    }
                    //否则按STEP_VALUE步进
                    else
                    {
                        //实时坐标和目标坐标的间距。
                        float spacing=  CURRENT_CMD->loction[idx]-realTimePos[CURRENT_CMD->axisFlag[idx]];
                        //需要移动的次数。
                        float moveCount=qAbs(spacing/STEP_VALUE);
                        //当次数小于一次时，当前坐标直接改为目标坐标
                        if(moveCount<1){
                            if(CURRENT_CMD->axisFlag[idx]==mainPaw)
                            {
                                realTimePos["PART"]+=CURRENT_CMD->loction[idx]-realTimePos[CURRENT_CMD->axisFlag[idx]];
                            }
                            realTimePos[CURRENT_CMD->axisFlag[idx]]=CURRENT_CMD->loction[idx];
                        }
                        //步进
                        else
                        {
                            float step_value=spacing/moveCount;//为了得出正负方向
                            realTimePos[CURRENT_CMD->axisFlag[idx]]+=step_value;
                            if(CURRENT_CMD->axisFlag[idx]==mainPaw)
                            {
                                realTimePos["PART"]+=step_value;
                            }
                        }
                    }

                }
            }

            //暂停
            if(CURRENT_CMD->funcType==NcCode::FuncType::stop)
            {
                timer.restart();
              //  QThread::msleep(CURRENT_CMD->stop);
            }
            //钻头打下代码
            if( CURRENT_CMD->funcType==NcCode::FuncType::tCode)
            {
                for (int i=0;i<CURRENT_CMD->tCode->count();i++) {
                    //遍历到空代码直接中断
                    if(CURRENT_CMD->tCode[i].isEmpty())break;
                    //如果不包含这个t代码就添加进列表。
                    if(!realMAndTCode.contains(CURRENT_CMD->tCode[i]))
                    {
                        realMAndTCode.append(CURRENT_CMD->tCode[i]);
                    }

                }
            }
            //如果为M代码
            if(CURRENT_CMD->funcType==NcCode::FuncType::mCode){
                //夹钳的。
                if(CURRENT_CMD->mCode==McPawsAllOpen||CURRENT_CMD->mCode==McPaw1Loosen)
                {
                    realMAndTCode.removeAll(McPaw1Close);
                }
                if(CURRENT_CMD->mCode==McPawsAllOpen||CURRENT_CMD->mCode==McPaw2Loosen)
                {
                    realMAndTCode.removeAll(McPaw2Close);                   
                }

                if(CURRENT_CMD->mCode==McPawsAllClose||CURRENT_CMD->mCode==McPaw2Close||CURRENT_CMD->mCode==Mc_M403)
                {
                    if(!realMAndTCode.contains(McPaw2Close))realMAndTCode.append(McPaw2Close);
                }
                if(CURRENT_CMD->mCode==McPawsAllClose||CURRENT_CMD->mCode==McPaw1Close||CURRENT_CMD->mCode==Mc_M403)
                {
                    if(!realMAndTCode.contains(McPaw1Close))realMAndTCode.append(McPaw1Close);
                }
                //台面
                if(CURRENT_CMD->mCode==McTableUpCode)
                {
                    realMAndTCode.removeAll(McTableDownCode);
                }
                if(CURRENT_CMD->mCode==McTableDownCode)
                {
                    if(!realMAndTCode.contains(McTableDownCode))realMAndTCode.append(McTableDownCode);
                }
                //上主轴是否启动
                if(CURRENT_CMD->mCode==McMainEnd)
                {
                    realMAndTCode.removeAll(McMainStart);
                    realMAndTCode.removeAll(McMainPreStart);
                }
                if(CURRENT_CMD->mCode==McMainStart||CURRENT_CMD->mCode==McMainPreStart)
                {
                    if(!realMAndTCode.contains(McMainStart))realMAndTCode.append(McMainStart);
                     if(!realMAndTCode.contains(McMainPreStart))realMAndTCode.append(McMainPreStart);
                }

                //下主轴是否启动
                if(CURRENT_CMD->mCode==McSMainEnd)
                {
                    realMAndTCode.removeAll(McSMainStart);
                    realMAndTCode.removeAll(McSMainPreStart);
                }
                if(CURRENT_CMD->mCode==McSMainPreStart||CURRENT_CMD->mCode==McSMainStart)
                {
                    if(!realMAndTCode.contains(McSMainPreStart))realMAndTCode.append(McSMainPreStart);
                     if(!realMAndTCode.contains(McSMainStart))realMAndTCode.append(McSMainStart);
                }


                //因为有M350 所以得等循环跑完
                for (int i=0;i<xml->config->MCodeSetups.count();i++) {
                    if(xml->config->MCodeSetups[i].BackCode==CURRENT_CMD->mCode)
                    {
                        realMAndTCode.removeAll(xml->config->MCodeSetups[i].SelectCode);
                    }
                }

                for (int i=0;i<xml->config->PressUpDownSetups.count();i++) {
                    if(xml->config->PressUpDownSetups[i].UpCode==CURRENT_CMD->mCode)
                    {
                        realMAndTCode.removeAll(xml->config->PressUpDownSetups[i].DownCode);
                    }
                    else if(xml->config->PressUpDownSetups[i].DownCode==CURRENT_CMD->mCode)
                    {
                        if(!realMAndTCode.contains(xml->config->PressUpDownSetups[i].DownCode))realMAndTCode.append(xml->config->PressUpDownSetups[i].DownCode);
                    }
                }


            }

            emit SimReady(realTimePos,CURRENT_CMD,realMAndTCode);
            if(CURRENT_CMD->speed>0)
            {
                QThread::msleep(25);
            }

           // 当前指令为移动指令&&包含Z轴移动&&调刀指令中存在T代码    延迟一些时间表示正在打孔
//                        if(CURRENT_CMD->funcType==FuncType::gCode)
//                        {
//                            for (int a=0;a<CURRENT_CMD->axisFlag->count();a++) {
//                                if(CURRENT_CMD->axisFlag[a].contains("Z")){
//                                    for (int i=0;i<realMAndTCode.count();i++) {
//                                        if(realMAndTCode[i].contains("T")||realMAndTCode[i].contains(McMainDown))
//                                        {
//                                            QThread::msleep(250);
//                                        }
//                                    }
//                                    break;
//                                }

//                            }
//                        }


        }
        if(workState==WORK_STATE::STOP)
        {

        }
        if(workState==WORK_STATE::EXIT)
        {
            return;
        }
        if(workState==WORK_STATE::RESET)
        {

            workState=WORK_STATE::STOP;
            CURRENT_CMD_INDEX=0;
            CURRENT_CMD=nullptr;
            InitRealTimePos();
            continue;
        }
        QThread::msleep(WORK_INTRTVAL);
    }

}

bool Simulator::SimWorker::LoadSupportingData(const QString &p1,const QString &p2,const QString &p3){

    //解析配置文件
//    QString path=QString("%1%2").arg( qApp->applicationDirPath()).arg(p1);
//    QString path2=QString("%1%2").arg( qApp->applicationDirPath()).arg(p2);
    QString path=p1;
    QString path2=p2;
    xml=new XMLParsing();
    bool rst=  xml->Parsing(path,path2);

    //预解析nc代码
    //QString ncPath=QString("%1%2").arg( qApp->applicationDirPath()).arg(p3);
    QString ncPath=p3;
    nc=new NcParsing();
    int count=nc->Parsing(ncPath);

    //初始化实时坐标列表；
    InitRealTimePos();


    if(rst&&count>0)
    {
        for(auto Dit = xml->config->DrillInfos.begin(); Dit != xml->config->DrillInfos.end(); ++Dit)
        {
            m_Drill *New_m_drill=new m_Drill;

            for(auto Mit = xml->config->MCodeSetups.begin(); Mit != xml->config->MCodeSetups.end(); ++Mit)
            {
                if(Dit->ID== Mit->MCode)
                {
                    New_m_drill->MCode=Mit->MCode;
                    New_m_drill->SelectCode=Mit->SelectCode;
                    New_m_drill->BackCode=Mit->BackCode;
                    break;
                }
            }
            New_m_drill->ID=Dit->ID;
            New_m_drill->RelPosX=Dit->RelPosX;
            New_m_drill->RelPosY=Dit->RelPosY;
            New_m_drill->PosY=Dit->PosY;
            New_m_drill->PosX=Dit->PosX;
            New_m_drill->Type=Dit->Type;
            New_m_drill->PackageID=Dit->PackageID;
            New_m_drill->BasedDrill=Dit->BasedDrill;
            New_m_drill->Diameter=Dit->Diameter;
            drills.push_back(New_m_drill);

        }
        for(auto pit = xml->config->BasicSetups.begin(); pit != xml->config->BasicSetups.end(); ++pit){
            if(pit->Name=="PawLength")
                this->ClampLength=pit->Value.toInt();
            if(pit->Name=="PawWidth")
                this->ClampWidth=pit->Value.toInt();
            if(pit->Name=="McPawsAllOpen")
                this->McPawsAllOpen=pit->Value;
            if(pit->Name=="McPawsAllClose")
                this->McPawsAllClose=pit->Value;
            if(pit->Name=="McPaw1Loosen")
                this->McPaw1Loosen=pit->Value;
            if(pit->Name=="McPaw1Close")
                this->McPaw1Close=pit->Value;
            if(pit->Name=="McPaw2Loosen")
                this->McPaw2Loosen=pit->Value;
            if(pit->Name=="McPaw2Close")
                this->McPaw2Close=pit->Value;
            if(pit->Name=="TrenchMin")
                this->TrenchMin=pit->Value.toFloat();
            if(pit->Name=="TrenchMax")
                this->TrenchMax=pit->Value.toFloat();

            if(pit->Name=="McMainUp")
                this->McMainUp=pit->Value;
            if(pit->Name=="McMainDown")
                this->McMainDown=pit->Value;
            if(pit->Name=="McTableUpCode")
                this->McTableUpCode=pit->Value;
            if(pit->Name=="McTableDownCode")
                this->McTableDownCode=pit->Value;


            if(pit->Name=="McMainStart")
                this->McMainStart=pit->Value;
            if(pit->Name=="McMainEnd")
                this->McMainEnd=pit->Value;
            if(pit->Name=="McSMainStart")
                this->McSMainStart=pit->Value;
            if(pit->Name=="McSMainEnd")
                this->McSMainEnd=pit->Value;
            if(pit->Name=="McMainPreStart")
                this->McMainPreStart=pit->Value;
            if(pit->Name=="McSMainPreStart")
                this->McSMainPreStart=pit->Value;
            if(pit->Name=="MachineMirrorType")
                this->MachineMirrorType=pit->Value.isEmpty()?0:pit->Value.toInt();

        }


        return true;
    }
    return false;
}
void Simulator::SimWorker::InitRealTimePos(){
    realTimePos.insert("Y1",DEFAULT_POS);
    realTimePos.insert("Y2",DEFAULT_POS);
    realTimePos.insert("Y3",DEFAULT_POS);
    realTimePos.insert("X1",DEFAULT_POS);
    realTimePos.insert("X2",DEFAULT_POS);
    realTimePos.insert("P",DEFAULT_POS);
    realTimePos.insert("W",DEFAULT_POS);
    realTimePos.insert("Z1",DEFAULT_POS);
    realTimePos.insert("Z2",DEFAULT_POS);
    realTimePos.insert("Z3",DEFAULT_POS);
    realTimePos.insert("PART",DEFAULT_POS);
}
Simulator::SimWorker::~SimWorker(){
    qDebug()<<"线程析构函数";
    delete xml;
    delete nc;

    for(auto it = drills.begin(); it != drills.end(); ++it)
    {
        if(*it != nullptr)
        {
            delete (*it);
            (*it) = nullptr;
        }

    }
    drills.clear();
    QVector<m_Drill*>().swap(drills);
}
