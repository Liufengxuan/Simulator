#include "mainform.h"



MainForm::MainForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainForm)
{
    ui->setupUi(this);
    this->resize( INIT_FORM_SIZE);
    QSize formsize= this->size();
    LeftTopPos=QPointF(formsize.width()*0.3,formsize.height()*0.4);
}

void MainForm::showEvent(QShowEvent *event)
{
    static  bool executed=false;
    if(executed)return;

 // QMessageBox::information(NULL, "Title",qApp->applicationDirPath(), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);

    //实例化仿真解析线程
    workThread=new QThread;
    //实例化仿真工作类
    simWork=new Simulator::SimWorker;
    //加载仿真支撑数据
    if (parm_argc<4) {
        QMessageBox::information(NULL, "Title", "参数个数不正确", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        QApplication* app;
        app->exit(0);
    }

    ALLOW_PAINT=false;
    ALLOW_PAINT= simWork->LoadSupportingData(parm_argv[1],parm_argv[2],parm_argv[3]);
    QStringList ncList= simWork->nc->ncCodeSource.split(QRegExp("[\r\n]"),QString::SkipEmptyParts);
    for (int var = 0; var < ncList.count(); ++var) {
        ui->listWidget_NC->addItem(ncList[var]);
    }

    //按钮点击-开始工作
    connect(ui->btn_start,&QPushButton::clicked, this, &MainForm::btn_start_clicked);
    //按钮点击-暂停
    connect(ui->btn_stop,&QPushButton::clicked, this, &MainForm::btn_stop_clicked);
    connect(ui->btn_exit,&QPushButton::clicked, this, &MainForm::btn_reset_clicked);
    //执行工作方法
    connect(this,&MainForm::doWork, simWork, &Simulator::SimWorker::DoWork);
    //线程析构
    connect(workThread,&QThread::finished,this,&QObject::deleteLater);
    connect(simWork,& Simulator::SimWorker::SimReady,this,&MainForm::Sim_Handle,Qt::ConnectionType::BlockingQueuedConnection);
    connect(ui->sb_speed,static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged),this,&MainForm::EditSimSpeed);




    //static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged)

    //仿真工作类移交给线程处理
    simWork->moveToThread(workThread);
    workThread->start();
    executed=true;
}
void MainForm::Sim_Handle(QMap<QString,float> pos,const NCCommand* nc,QStringList mAndTCode){

    REAL_M_T_CODE=mAndTCode;
    REAL_TIME_POS=pos;
    if(nc!=nullptr)
    {
        int row=0;
        QString line;
        while(row<ui->listWidget_NC->count())
        {
            line=ui->listWidget_NC->item(row)->text();
            //qDebug()<<line<<"\n";
            if(line==nc->source){
                ui->listWidget_NC->setCurrentRow(row);
                break;
            }
            row++;
        }

    }
    repaint();
    //qDebug()<<"sim1";
}
void MainForm::EditSimSpeed(int i){
    simWork->STEP_VALUE=i;


}

void MainForm::paintEvent(QPaintEvent *event){

    tempPix=QPixmap(this->size());
    tempPix.fill(BACK_COLOR);
    QPainter tempPainter(&tempPix);
    if(  ALLOW_PAINT)
    {
        DrawDatumPoint(tempPainter);
        DrawPart(tempPainter);
        DrawClamp(tempPainter);
        DrawDrills(tempPainter);

        DrawWord(tempPainter);
    }

    QPainter painter(this);
    if(ANTI_ALIASING)painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    painter.drawPixmap(0,0,tempPix);


    //qDebug()<<"sim2";
}

///定位柱、壕沟、夹钳滑轨
void MainForm::DrawDatumPoint(QPainter &painter)
{
    //箭头长度为线段长度的7%
    float headerLen=COOR_LEN*0.07*zoom;
    float halfLineLen=COOR_LEN*zoom*0.5;

    //夹钳滑轨
    float slide_Start_Y=simWork->MachineMirrorType==1?LeftTopPos.y():LeftTopPos.y()-SLIDE_RAIL_WIDTH*zoom;
    painter.setPen(SLIDE_RAIL_PEN);
    painter.setBrush(SLIDE_RAIL_BRUSH);
    QPointF slide_Start= QPointF(LeftTopPos.x()-SLIDE_RAIL/2*zoom,slide_Start_Y);
    painter.drawRect(slide_Start.x(),slide_Start.y(),SLIDE_RAIL*zoom,SLIDE_RAIL_WIDTH*zoom);

    //壕沟

    // if(REAL_M_T_CODE.contains(simWork->McTableDownCode))painter.setBrush(DEFAULT_BRUSH);
    float bili=simWork->MachineMirrorType==1?0.7:0.3;
    QPointF trench= QPointF(LeftTopPos.x()+simWork->TrenchMin*zoom,LeftTopPos.y()-TRENCH_LEN*bili*zoom );
    painter.drawRect(trench.x(),trench.y(),(simWork->TrenchMax-simWork->TrenchMin)*zoom,TRENCH_LEN*zoom);




    painter.setPen(COOR_PEN);

    //X
    painter.drawEllipse(LeftTopPos,10*zoom,10*zoom);
    QPointF line1Start= QPointF(LeftTopPos.x()-halfLineLen,LeftTopPos.y());
    QPointF line1End= QPointF(LeftTopPos.x()+halfLineLen,LeftTopPos.y());
    float angle=135*PI/180;
    float x1=line1End.x()+cos(angle)*(headerLen);
    float y1=line1End.y()+sin(angle)*(headerLen);
    float x2=line1End.x()+cos(-angle)*(headerLen);
    float y2=line1End.y()+sin(-angle)*(headerLen);
    painter.drawLine(line1Start,line1End);
    painter.drawLine(x1,y1,line1End.x(),line1End.y());
    painter.drawLine(x2,y2,line1End.x(),line1End.y());




    //Y
    QPointF line2Start= QPointF(LeftTopPos.x(),LeftTopPos.y()-halfLineLen);
    QPointF line2End= QPointF(LeftTopPos.x(),LeftTopPos.y()+halfLineLen);
    angle=45*PI/180;
    float x3=line2Start.x()+sin(angle)*(headerLen);
    float y3=line2Start.y()+cos(angle)*(headerLen);
    float x4=line2Start.x()+sin(-angle)*(headerLen);
    float y4=line2Start.y()+cos(-angle)*(headerLen);
    painter.drawLine(line2Start,line2End);
    painter.drawLine(x3,y3,line2Start.x(),line2Start.y());
    painter.drawLine(x4,y4,line2Start.x(),line2Start.y());






    painter.setBrush(DEFAULT_BRUSH);
    painter.setPen(DEFAULT_PEN);
    //X
    //    painter.drawLine(LeftTopPos.x()-COOR_LEN*zoom*0.5,LeftTopPos.y(),LeftTopPos.x()+COOR_LEN*zoom*0.5,LeftTopPos.y());
    //    painter.drawLine(LeftTopPos.x()+COOR_LEN*zoom*0.4,LeftTopPos.y()-COOR_LEN*0.05*zoom,LeftTopPos.x()+COOR_LEN*zoom/2,LeftTopPos.y());
    //    painter.drawLine(LeftTopPos.x()+COOR_LEN*zoom*0.4,LeftTopPos.y()+COOR_LEN*0.05*zoom,LeftTopPos.x()+COOR_LEN*zoom/2,LeftTopPos.y());
    //    //Y
    //    painter.drawLine(LeftTopPos.x(),LeftTopPos.y()-COOR_LEN*zoom*0.5,LeftTopPos.x(),LeftTopPos.y()+COOR_LEN*zoom*0.5);
    //    painter.drawLine(LeftTopPos.x()-COOR_LEN*0.05*zoom,LeftTopPos.y()-COOR_LEN*zoom*0.4,LeftTopPos.x(),LeftTopPos.y()-COOR_LEN*zoom*0.5);
    //    painter.drawLine(LeftTopPos.x()+COOR_LEN*0.05*zoom,LeftTopPos.y()-COOR_LEN*zoom*0.4,LeftTopPos.x(),LeftTopPos.y()-COOR_LEN*zoom*0.5);
}
void MainForm::DrawDrills(QPainter &painter){
    m_Drill *t1_BaseDrill=nullptr;
    m_Drill *t2_BaseDrill=nullptr;
    m_Drill *b1_BaseDrill=nullptr;
    for(auto Dit = simWork->drills .begin(); Dit != simWork->drills.end(); ++Dit)
    {
        if( (*Dit)->BasedDrill&&(*Dit)->PackageID==PACKAGE_TOP1  )t1_BaseDrill=(*Dit);
        if( (*Dit)->BasedDrill&&(*Dit)->PackageID==PACKAGE_TOP2  )t2_BaseDrill=(*Dit);
        if( (*Dit)->BasedDrill&&(*Dit)->PackageID==PACKAGE_BOTTOM1  )b1_BaseDrill=(*Dit);
        if(t1_BaseDrill!=nullptr&&t2_BaseDrill!=nullptr&&b1_BaseDrill!=nullptr )break;
    }
    //获得基准钻实时Y坐标
    int t1_PosY=REAL_TIME_POS.contains("Y3")?REAL_TIME_POS["Y3"]:simWork->DEFAULT_POS;
    int t2_PosY=REAL_TIME_POS.contains("Y1")?REAL_TIME_POS["Y1"]:simWork->DEFAULT_POS;
    int b1_PosY=REAL_TIME_POS.contains("Y2")?REAL_TIME_POS["Y2"]:simWork->DEFAULT_POS;
    //    t1_PosY=simWork->MachineMirrorType==1?t1_PosY*-1:t1_PosY;
    //    t2_PosY=simWork->MachineMirrorType==1?t2_PosY*-1:t2_PosY;
    //    b1_PosY=simWork->MachineMirrorType==1?b1_PosY*-1:b1_PosY;
    //基准钻绘图圆心位置Map
    QMap<QString,QPointF> posMap;
    posMap.insert(PACKAGE_TOP1,QPointF(LeftTopPos.x()+t1_BaseDrill->PosX*zoom,LeftTopPos.y()-t1_PosY*zoom));
    posMap.insert(PACKAGE_BOTTOM1,QPointF(LeftTopPos.x()+b1_BaseDrill->PosX*zoom,LeftTopPos.y()-b1_PosY*zoom));
    posMap.insert(PACKAGE_TOP2,QPointF(LeftTopPos.x()+t2_BaseDrill->PosX*zoom,LeftTopPos.y()-t2_PosY*zoom));

    painter.setPen(VER_DRILL_PEN);

    //钻头筒圆的半径 应该为16，为了好看改14
    int drillSpacing=14;
    //基准钻是否打下状态
    bool t1Select= REAL_M_T_CODE.contains(t1_BaseDrill->SelectCode);
    bool b1Select= REAL_M_T_CODE.contains(b1_BaseDrill->SelectCode);
    bool t2Select= REAL_M_T_CODE.contains(t2_BaseDrill->SelectCode);
    if(t1Select)painter.setPen(DRILL_DOWN_PEN);
    painter.drawEllipse(posMap[PACKAGE_TOP1],t1_BaseDrill->Diameter/2*zoom,t1_BaseDrill->Diameter/2*zoom);
    painter.drawEllipse(posMap[PACKAGE_TOP1],drillSpacing*zoom,drillSpacing*zoom);
    if(t1Select)painter.setPen(VER_DRILL_PEN);

    if(b1Select)painter.setPen(DRILL_DOWN_PEN);
    painter.drawEllipse(posMap[PACKAGE_BOTTOM1],b1_BaseDrill->Diameter/2*zoom,b1_BaseDrill->Diameter/2*zoom);
    painter.drawEllipse(posMap[PACKAGE_BOTTOM1],drillSpacing*zoom,drillSpacing*zoom);
    if(b1Select)painter.setPen(VER_DRILL_PEN);

    if(t2Select)painter.setPen(DRILL_DOWN_PEN);
    painter.drawEllipse(posMap[PACKAGE_TOP2],t2_BaseDrill->Diameter/2*zoom,t2_BaseDrill->Diameter/2*zoom);
    painter.drawEllipse(posMap[PACKAGE_TOP2],drillSpacing*zoom,drillSpacing*zoom);
    if(t2Select)painter.setPen(VER_DRILL_PEN);


    for(auto Dit = simWork->xml->config->PressUpDownSetups.begin(); Dit != simWork->xml->config->PressUpDownSetups.end(); ++Dit)
    {
        //PRESS_DOWN_BRUSH
        painter.setPen(PRESS_PEN);
        bool isDown=REAL_M_T_CODE.contains(Dit->DownCode);
        if(isDown)painter.setBrush(PRESS_DOWN_BRUSH);

        QPointF PressCenterPos;
        QRectF  PressRect;
        QString pkgID=Dit->PackageID;
        if(Dit->Type==MAIN_PRESS)pkgID=PACKAGE_TOP2;
        if(Dit->Type==UNDERLAY)pkgID=PACKAGE_BOTTOM1;
        if(posMap.contains(pkgID))
        {
            float PressCenterPos_Y=simWork->MachineMirrorType==1?posMap[pkgID].y()+Dit->CenterY*zoom-Dit->Width/2*zoom:posMap[pkgID].y()-Dit->CenterY*zoom-Dit->Width/2*zoom;
            PressCenterPos=QPointF(posMap[pkgID].x()+Dit->CenterX*zoom-Dit->Length/2*zoom,PressCenterPos_Y);
            PressRect=QRectF(PressCenterPos.x(),PressCenterPos.y(),Dit->Length*zoom,Dit->Width*zoom);
            painter.drawRect(PressRect);
        }
        if(isDown)painter.setBrush(DEFAULT_BRUSH);
    }





    for(auto Dit = simWork->drills .begin(); Dit != simWork->drills.end(); ++Dit)
    {
        m_Drill *drill= (*Dit);
        DrawSideView(painter,drill);//画侧视图

        QPointF drillPos;
        QRectF horDrill;
        bool isDown=REAL_M_T_CODE.contains(drill->SelectCode);


        if(isDown)
        {
            painter.setPen(DRILL_DOWN_PEN);
            painter.setBrush(DRILL_DOWN_BRUSH);
        }
        else if(drill->Type==VER_DRILL||drill->Type==SPINDLE)
        {
            painter.setPen(VER_DRILL_PEN);
        }else {
            painter.setPen(HOR_DRILL_PEN);
        }
        //普通钻头
        if(!drill->BasedDrill&&drill->Type==VER_DRILL&&posMap.contains(drill->PackageID))
        {
            float drillPos_Y=simWork->MachineMirrorType==1?posMap[drill->PackageID].y()+drill->RelPosY*zoom:posMap[drill->PackageID].y()-drill->RelPosY*zoom;
            drillPos=QPointF(posMap[drill->PackageID].x()+drill->RelPosX*zoom,drillPos_Y);
            painter.drawEllipse(drillPos,drill->Diameter/2*zoom,drill->Diameter/2*zoom);
            painter.drawEllipse(drillPos,drillSpacing*zoom,drillSpacing*zoom);
        }
        //上主轴
        else if(drill->PackageID==PACKAGE_TOPMAIN&&posMap.contains(PACKAGE_TOP2)&&drill->Type==SPINDLE)
        {
            if(REAL_M_T_CODE.contains(simWork-> McMainStart)||REAL_M_T_CODE.contains(simWork-> McMainPreStart))   {
                painter.setPen(DRILL_DOWN_PEN);
                painter.setBrush(DRILL_DOWN_BRUSH);
            }
            float drillPos_Y=simWork->MachineMirrorType==1?posMap[PACKAGE_TOP2].y()+drill->RelPosY*zoom:posMap[PACKAGE_TOP2].y()-drill->RelPosY*zoom;
            drillPos=QPointF(posMap[PACKAGE_TOP2].x()+drill->RelPosX*zoom,drillPos_Y);
            painter.drawEllipse(drillPos,drill->Diameter/2*zoom,drill->Diameter/2*zoom);
            painter.drawEllipse(drillPos,drillSpacing*zoom,drillSpacing*zoom);
        }
        //下主轴
        else if(drill->PackageID==PACKAGE_BOTTOM_MAIN&&posMap.contains(PACKAGE_BOTTOM1)&&drill->Type==SPINDLE)
        {
            if(REAL_M_T_CODE.contains(simWork-> McSMainPreStart)||REAL_M_T_CODE.contains(simWork-> McSMainStart))   {
                painter.setPen(DRILL_DOWN_PEN);
                painter.setBrush(DRILL_DOWN_BRUSH);
            }
            float drillPos_Y=simWork->MachineMirrorType==1?posMap[PACKAGE_BOTTOM1].y()+drill->RelPosY*zoom:posMap[PACKAGE_BOTTOM1].y()-drill->RelPosY*zoom;
            drillPos=QPointF(posMap[PACKAGE_BOTTOM1].x()+drill->RelPosX*zoom,drillPos_Y);
            painter.drawEllipse(drillPos,drill->Diameter/2*zoom,drill->Diameter/2*zoom);
            painter.drawEllipse(drillPos,drillSpacing*zoom,drillSpacing*zoom);
        }
        //水平向左
        else if(!drill->BasedDrill&&drill->Type==HOR_LEFT_DIRECTION&&posMap.contains(drill->PackageID))
        {
            //钻头半径
            float r=drill->Diameter/2*zoom;
            float drillPos_Y=simWork->MachineMirrorType==1?posMap[drill->PackageID].y()+drill->RelPosY*zoom:posMap[drill->PackageID].y()-drill->RelPosY*zoom;
            drillPos=QPointF(posMap[drill->PackageID].x()+drill->RelPosX*zoom,drillPos_Y);
            //这个点要画矩形，所以往屏幕坐标y-方向偏移个钻头半径
            horDrill=QRectF(drillPos.x(),drillPos.y()- r,HOR_DRILL_LEN*zoom,drill->Diameter*zoom);
            // painter.setBrush(HOR_DRILL_BRUSH);
            painter.drawRect(horDrill);
            drillPos.setX(drillPos.x()+r);
            painter.drawEllipse(drillPos,r,r);
            //painter.setBrush(DEFAULT_BRUSH);
        }
        //水平向右
        else if(!drill->BasedDrill&&drill->Type==HOR_RIGHT_DIRECTION&&posMap.contains(drill->PackageID))
        {
            //钻头半径
            float r=drill->Diameter/2*zoom;
            float drillPos_Y=simWork->MachineMirrorType==1?posMap[drill->PackageID].y()+drill->RelPosY*zoom:posMap[drill->PackageID].y()-drill->RelPosY*zoom;
            drillPos=QPointF(posMap[drill->PackageID].x()+drill->RelPosX*zoom,drillPos_Y);
            //这个点要画矩形，所以往屏幕坐标y-方向偏移个钻头半径
            horDrill=QRectF(drillPos.x(),drillPos.y()- r,-HOR_DRILL_LEN*zoom,drill->Diameter*zoom);
            // painter.setBrush(HOR_DRILL_BRUSH);
            painter.drawRect(horDrill);
            drillPos.setX(drillPos.x()-r);
            painter.drawEllipse(drillPos,r,r);
            //painter.setBrush(DEFAULT_BRUSH);
        }
        //水平向上
        else if(!drill->BasedDrill&&drill->Type==HOR_UP_DIRECTION&&posMap.contains(drill->PackageID))
        {
            //钻头半径
            float r=drill->Diameter/2*zoom;
            float drillPos_Y,drill_len;
            if(simWork->MachineMirrorType==1)
            {
                drill_len=-HOR_DRILL_LEN;
                drillPos_Y=posMap[drill->PackageID].y()+drill->RelPosY*zoom;
            }else {
                drillPos_Y=posMap[drill->PackageID].y()-drill->RelPosY*zoom;
                drill_len=HOR_DRILL_LEN;
            }

            drillPos=QPointF(posMap[drill->PackageID].x()+drill->RelPosX*zoom,drillPos_Y);
            //x也要往屏幕坐标x-方向偏移一个半径
            horDrill=QRectF(drillPos.x()- r,drillPos.y(),drill->Diameter*zoom,drill_len*zoom);
            //painter.setBrush(HOR_DRILL_BRUSH);
            painter.drawRect(horDrill);
            drillPos.setY(drillPos.y()+r);
            painter.drawEllipse(drillPos,r,r);
            //painter.setBrush(DEFAULT_BRUSH);
        }
        //水平向下
        else if(!drill->BasedDrill&&drill->Type==HOR_DOWN_DIRECTION&&posMap.contains(drill->PackageID))
        {
            //钻头半径
            float r=drill->Diameter/2*zoom;
            float drillPos_Y,drill_len;
            if(simWork->MachineMirrorType==1)
            {
                drill_len=HOR_DRILL_LEN;
                drillPos_Y=posMap[drill->PackageID].y()+drill->RelPosY*zoom;
            }else {
                drillPos_Y=posMap[drill->PackageID].y()-drill->RelPosY*zoom;
                drill_len=-HOR_DRILL_LEN;
            }
            drillPos=QPointF(posMap[drill->PackageID].x()+drill->RelPosX*zoom,drillPos_Y);
            //x也要往屏幕坐标x-方向偏移一个半径
            horDrill=QRectF(drillPos.x()- r,drillPos.y(),drill->Diameter*zoom,drill_len*zoom);
            //painter.setBrush(HOR_DRILL_BRUSH);
            painter.drawRect(horDrill);
            drillPos.setY(drillPos.y()-r);
            painter.drawEllipse(drillPos,r,r);
            //painter.setBrush(DEFAULT_BRUSH);
        }
        painter.setPen(DEFAULT_PEN);
        painter.setBrush(DEFAULT_BRUSH);
    }



    painter.setBrush(DEFAULT_BRUSH);
    painter.setPen(DEFAULT_PEN);


}
void MainForm::DrawClamp(QPainter &painter){
    bool x1Close=REAL_M_T_CODE.contains(simWork->McPaw2Close);
    bool x2Close=REAL_M_T_CODE.contains(simWork->McPaw1Close);
    painter.setPen(CLAMP_OPEN_PEN);
    int X2_PosX=REAL_TIME_POS.contains("X2")?REAL_TIME_POS["X2"]:0;
    int X1_PosX=REAL_TIME_POS.contains("X1")?REAL_TIME_POS["X1"]:0;
    QPointF X2Pos= QPointF(LeftTopPos.x()+X2_PosX*zoom-simWork->ClampLength*zoom,LeftTopPos.y());
    QPointF X1Pos= QPointF(LeftTopPos.x()+X1_PosX*zoom-simWork->ClampLength*zoom,LeftTopPos.y());

    float clampHeight=simWork->MachineMirrorType==1? -simWork->ClampWidth*zoom:simWork->ClampWidth*zoom;

    if(x1Close)painter.setBrush(CLAMP_CLOSE_BRUSH);
    painter.drawRect(X2Pos.x(),X2Pos.y(),simWork->ClampLength*zoom,clampHeight);
    if(x1Close)painter.setBrush(DEFAULT_BRUSH);

    if(x2Close)painter.setBrush(CLAMP_CLOSE_BRUSH);
    painter.drawRect(X1Pos.x(),X1Pos.y(),simWork->ClampLength*zoom,clampHeight);
    if(x2Close)painter.setBrush(DEFAULT_BRUSH);
    painter.setPen(DEFAULT_PEN);
}
void MainForm::DrawWord(QPainter &painter){

    if(ui->cb_ShowStatus ->checkState()!=Qt::CheckState::Checked)return;
    int h= QFontInfo(WORD_FONT).pixelSize();
    painter.setFont(WORD_FONT);

    QString  txt;
    int i=1;

    painter.drawText(5,size().height()-i*h, QString("{%1}/{%2}")
                     .arg(simWork->workStateString[simWork->workState])
            .arg(simWork->MachineMirrorType));
    i++;
    painter.drawText(5,size().height()-i*h, QString("调刀: %1").arg(REAL_M_T_CODE.join("/")));
    i++;
    QMap<QString,float>::iterator iter= REAL_TIME_POS.begin();
    while ( iter != REAL_TIME_POS.end())
    {
        txt=QString("%1 : %2").arg(iter.key()).arg(iter.value());
        painter.drawText(5,size().height()-i*h,txt);
        iter++;
        i++;
    }

}
void MainForm::DrawPart(QPainter &painter){

    painter.setPen(PANEL_PEN);
    painter.setBrush(PANEL_BRUSH);
    Parameters::Part *part=simWork->xml->part;
    //板件原点
    float part_Y=simWork->MachineMirrorType==1? LeftTopPos.y():LeftTopPos.y()+part->width*zoom;
    QPointF o=  QPointF(LeftTopPos.x()+REAL_TIME_POS["PART"]*zoom,part_Y);
    painter.drawRect(o.x(),o.y(),part->Length*zoom,-part->width*zoom);


    for(auto m= part->Machines.begin();m!=part->Machines.end();++m)
    {
        if(m->Face==6)
        {
            painter.setBrush(MACHINE_BACK_Brush);
        }else {
            painter.setBrush(MACHINE_FRONT_Brush);
        }

        //垂直孔
        if(m->Type==2)
        {
            painter.drawEllipse(QPointF(o.x()+m->X*zoom,o.y()-m->Y*zoom),m->Diameter/2*zoom,m->Diameter/2*zoom);
        }
        //榫槽
        if(m->Type==4)
        {
            //竖直方向槽
            if(m->X==m->EndX)
            {
                painter.drawRect(o.x()+m->X*zoom-m->Width/2*zoom,o.y()-m->Y*zoom,m->Width*zoom,m->Y*zoom-m->EndY*zoom);
            }
            //水平方向槽
            if(m->Y==m->EndY)
            {
                painter.drawRect(o.x()+m->X*zoom,o.y()-m->Y*zoom-m->Width/2*zoom,m->EndX*zoom-m->X*zoom,m->Width*zoom);
            }
        }
        //水平孔
        if(m->Type==1){
            float  hole_r=m->Diameter/2*zoom;
            switch (m->Face)
            {
            case 1:painter.drawRect(o.x()+m->X*zoom-hole_r,o.y()-m->Y*zoom,m->Diameter*zoom,m->Depth*zoom);
                break;
            case 2:painter.drawRect(o.x()+m->X*zoom-hole_r,o.y(),m->Diameter*zoom,-m->Depth*zoom);
                break;
            case 4:painter.drawRect(o.x(),o.y()-m->Y*zoom-hole_r,m->Depth*zoom,m->Diameter*zoom);
                break;
            case 3:painter.drawRect(o.x()+m->X*zoom,o.y()-m->Y*zoom-hole_r,-m->Depth*zoom,m->Diameter*zoom);
                break;
            }
        }
    }

    painter.setBrush(DEFAULT_BRUSH);
    painter.setPen(DEFAULT_PEN);
}
void MainForm::DrawSideView(QPainter &painter,m_Drill *drill){

    if(ui->cb_ShowSideView->checkState()!=Qt::CheckState::Checked)return;
    Parameters::Part *part=simWork->xml->part;
    float svZoom=0.7;
    float dwz_w=20;//定位柱宽
    float dwz_h=40;//定位柱高
    float defaultDrillLen=30;
    QPointF o=QPointF(size().width() *0.60,size().height()*0.90);
    painter.setPen(COOR_PEN);
    painter.drawRect(o.x()-dwz_w/2*svZoom,o.y(),dwz_w*svZoom,-dwz_h*svZoom);
    painter.setPen(PANEL_PEN);
    painter.drawRect(o.x()+REAL_TIME_POS["PART"]*svZoom,o.y(),part->Length*svZoom,-part->Thickness*svZoom);



    //    ||REAL_M_T_CODE.contains(simWork->McMainStart)
    //    ||REAL_M_T_CODE.contains(simWork->McMainPreStart)
    //    ||REAL_M_T_CODE.contains(simWork->McSMainStart)
    //    ||REAL_M_T_CODE.contains(simWork->McSMainPreStart)
    //普通钻头
    painter.setPen(DRILL_DOWN_PEN);
    if(REAL_M_T_CODE.contains(drill->SelectCode)){



        int posZ=0;
        if(drill->PackageID==PACKAGE_TOP1)posZ=REAL_TIME_POS.contains("Z3")?REAL_TIME_POS["Z3"]:0;
        else if(drill->PackageID==PACKAGE_TOP2)posZ=REAL_TIME_POS.contains("Z1")?REAL_TIME_POS["Z1"]:0;
        else if(drill->PackageID==PACKAGE_BOTTOM1)posZ=REAL_TIME_POS.contains("Z2")?REAL_TIME_POS["Z2"]:0;

        //下钻包垂直钻
        if(drill->PackageID==PACKAGE_BOTTOM1){
            QPointF dp=QPointF(o.x()+svZoom*drill->PosX,o.y()-svZoom*posZ+svZoom*drill->PosZ);
            painter.drawRect(dp.x()-drill->Diameter/2*svZoom,dp.y(),drill->Diameter*svZoom,defaultDrillLen*svZoom);

        }
        ///上钻包垂直钻
        else if(drill->Type==VER_DRILL)
        {
            QPointF dp=QPointF(o.x()+svZoom*drill->PosX,o.y()-svZoom*posZ+svZoom*drill->PosZ);
            painter.drawRect(dp.x()-drill->Diameter/2*svZoom,dp.y(),drill->Diameter*svZoom,-defaultDrillLen*svZoom);
        }
        ///向前向后的水平钻
        else if(drill->Type==HOR_UP_DIRECTION||drill->Type==HOR_DOWN_DIRECTION)
        {
            QPointF dp=QPointF(o.x()+svZoom*drill->PosX,o.y()-svZoom*posZ+svZoom*drill->PosZ);
            painter.drawEllipse(dp,drill->Diameter*svZoom,drill->Diameter*svZoom);
        }
        ///向左水平钻
        else if(drill->Type==HOR_LEFT_DIRECTION )
        {
            QPointF dp=QPointF(o.x()+svZoom*drill->PosX,o.y()-svZoom*posZ+svZoom*drill->PosZ);
            painter.drawRect(dp.x(),dp.y()- drill->Diameter/2*svZoom,defaultDrillLen*svZoom,drill->Diameter*svZoom);
        }
        ///向右水平钻
        else if(drill->Type==HOR_RIGHT_DIRECTION )
        {
            QPointF dp=QPointF(o.x()+svZoom*drill->PosX,o.y()-svZoom*posZ+svZoom*drill->PosZ);
            painter.drawRect(dp.x(),dp.y()- drill->Diameter/2*svZoom,-defaultDrillLen*svZoom,drill->Diameter*svZoom);
        }
    }
    else if(drill->PackageID==PACKAGE_TOPMAIN&&(REAL_M_T_CODE.contains(simWork->McMainStart)||REAL_M_T_CODE.contains(simWork->McMainPreStart))){
        int posZ=REAL_TIME_POS.contains("Z1")?REAL_TIME_POS["Z1"]:0;
        QPointF dp=QPointF(o.x()+svZoom*drill->PosX,o.y()-svZoom*posZ+svZoom*drill->PosZ);
        painter.drawRect(dp.x()-drill->Diameter/2*svZoom,dp.y(),drill->Diameter*svZoom,-defaultDrillLen*svZoom);
    }
    else if(drill->PackageID==PACKAGE_BOTTOM_MAIN&&(REAL_M_T_CODE.contains(simWork->McSMainStart)||REAL_M_T_CODE.contains(simWork->McSMainPreStart))){
        int posZ=REAL_TIME_POS.contains("Z2")?REAL_TIME_POS["Z2"]:0;
        QPointF dp=QPointF(o.x()+svZoom*drill->PosX,o.y()-svZoom*posZ+svZoom*drill->PosZ);
        painter.drawRect(dp.x()-drill->Diameter/2*svZoom,dp.y(),drill->Diameter*svZoom,-defaultDrillLen*svZoom);
    }

    painter.setBrush(DEFAULT_BRUSH);
    painter.setPen(DEFAULT_PEN);




}

void MainForm::btn_start_clicked()
{
    if(simWork->workState==Simulator::EXIT)
    {
        simWork->workState=Simulator::RUN;
        emit doWork(1);
    }
    if(simWork->workState==Simulator::STOP||simWork->workState==Simulator::RESET)
    {
        simWork->workState=Simulator::RUN;
    }
}
void MainForm::btn_stop_clicked(){
    if(simWork->workState==Simulator::RUN)
    {
        simWork->workState=Simulator::STOP;
    }
}
void MainForm::btn_reset_clicked(){
    simWork->workState=Simulator::RESET;
}

void MainForm::mousePressEvent(QMouseEvent *event){

    if(event->button()&Qt::MouseButton::LeftButton){
        mousePos.setX(event->x());
        mousePos.setY(event->y());
        moveButtonPress=true;
        this->setCursor(Qt::SizeAllCursor);
    }
}
void MainForm::mouseReleaseEvent(QMouseEvent *event){
    this->setCursor(Qt::ArrowCursor);
    moveButtonPress=false;
}
void MainForm:: mouseMoveEvent(QMouseEvent *event){
    float offsetX=event->x()-mousePos.x();
    float offsetY=event->y()-mousePos.y();
    if(moveButtonPress)
    {
        LeftTopPos.setY(LeftTopPos.y()+offsetY);
        LeftTopPos.setX(LeftTopPos.x()+offsetX);
        mousePos.setX(event->x());
        mousePos.setY(event->y());
        repaint();
    }
}
void MainForm::wheelEvent(QWheelEvent *event){
    int delta=event->delta();
    if(qAbs(delta)<10){
        return;
    }
    mousePos=QPointF(event->x(),event->y());
    oldZoom=zoom;
    if(delta<0){
        zoom-=STEP_VALUE;
    }else if(delta>0){
        zoom+=STEP_VALUE;
    }

    if(zoom<ZOOM_MIN){
        zoom=ZOOM_MIN;
    }else if(zoom>ZOOM_MAX){
        zoom=ZOOM_MAX;
    }
    float deltaZoomNewToOld = zoom / oldZoom;
    LeftTopPos.setX(mousePos.x()-(mousePos.x()-LeftTopPos.x())*deltaZoomNewToOld);
    LeftTopPos.setY(mousePos.y()-(mousePos.y()-LeftTopPos.y())*deltaZoomNewToOld);
    repaint();

}


MainForm::~MainForm()
{

    simWork->workState=Simulator::EXIT;
    workThread->quit();
    workThread->wait();

    //    delete xml;
    //    delete ui;
    //    delete ncParsing;
    delete simWork;
    delete workThread;
}
