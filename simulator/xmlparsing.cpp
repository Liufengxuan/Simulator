#include "xmlparsing.h"




Parameters::XMLParsing::XMLParsing(QObject *parent) : QObject(parent)
{  
    config=new Config;

}

bool Parameters::XMLParsing::Parsing(const QString& configPath,const QString& panelPath)
{
    QFile xml_File(configPath);
    QString errlog;
    int errCol;
    int errline;

    if(!doc.setContent(&xml_File,&errlog,&errline,&errCol)){
        qDebug()<<"读取config出错,位置:"<<errlog<<errline<<"col"<<errCol<<endl;
        return  false;
    }
    else{

        this->ReadBasicSetup();
        this->ReadMCodeSetup();
        this->ReadPressUpDownSetup();
        this->ReadDrillInfo();
        qDebug()<<QString("PressUpDownSetup读取到{%1}个参数").arg( config->PressUpDownSetups.count());
        qDebug()<<QString("DrillInfo读取到{%1}个参数").arg( config->DrillInfos.count());
        qDebug()<<QString("MCodeSetup读取到{%1}个参数").arg( config->MCodeSetups.count());
        qDebug()<<QString("BasicSetup读取到{%1}个参数").arg(config->BasicSetups.count());
    }

    QFile panel_File(panelPath);
    if(!doc.setContent(&panel_File,&errlog,&errline,&errCol)){
        qDebug()<<"读取panel出错,位置:"<<errlog<<errline<<"col"<<errCol<<endl;
        return  false;
    }
    else{
        this->ReadPanel();
    }



panel_File.close();
    xml_File.close();
    doc.clear();
    return  true;
}

void Parameters::XMLParsing::ReadDrillInfo()
{
    config-> DrillInfos.clear();
    QDomElement root= doc.documentElement();
    QDomNode DrillInfosNode= root.elementsByTagName("DrillInfos").item(0);
    QDomNode paramList=DrillInfosNode.firstChild();
    while (!paramList.isNull()) {
        QDomElement e=paramList.toElement();
        DrillInfo di=DrillInfo(e.attribute("ID"),
                               e.attribute("Type"),
                               e.attribute("Diameter").toInt(),
                               e.attribute("PosX").toFloat(),
                               e.attribute("PosY").toFloat(),
                               e.attribute("PosZ").toFloat(),
                               e.attribute("RelPosX").toFloat(),
                               e.attribute("RelPosY").toFloat(),
                               e.attribute("RelPosZ").toFloat(),
                               e.attribute("SafeDistance").toFloat(),
                               e.attribute("DownVel").toInt(),
                               e.attribute("MachiningVel").toInt(),
                               e.attribute("MillingVel").toInt(),
                               e.attribute("DelayTime").toInt(),
                               e.attribute("Enable")=="true"?true:false,
                               e.attribute("IsLone")=="true"?true:false,
                               e.attribute("PackageID"),
                               e.attribute("BasedDrill")=="true"?true:false,
                               e.attribute("PackageEnable")=="true"?true:false,
                               e.attribute("Throughdrill")=="true"?true:false,
                               e.elementsByTagName("lastLen").at(0).toElement().text().toInt(),
                               e.elementsByTagName("initialVel").at(0).toElement().text().toInt(),
                               e.elementsByTagName("drillLen").at(0).toElement().text().toInt(),
                               e.attribute("WorkStatus").toInt()
                               );


        config->DrillInfos.push_back(di);
        paramList=  paramList.nextSibling();
        // qDebug()<<ms.MCode<<ms.BackCode<<ms.SelectCode;
    }

}
void Parameters::XMLParsing::ReadPressUpDownSetup()
{
    config->PressUpDownSetups.clear();
    QDomElement root= doc.documentElement();
    QDomNode PressUpDownSetupsNode= root.elementsByTagName("PressUpDownSetups").item(0);
    QDomNode paramList=PressUpDownSetupsNode.firstChild();
    while (!paramList.isNull()) {
        QDomElement e=paramList.toElement();
        PressUpDownSetup ms=PressUpDownSetup(e.attribute("PackageID"),
                                             e.attribute("Type"),
                                             e.attribute("Description"),
                                             e.attribute("Length").toFloat(),
                                             e.attribute("Width").toFloat(),
                                             e.attribute("CenterX").toFloat(),
                                             e.attribute("CenterY").toFloat(),
                                             e.attribute("PressedZ").toFloat(),
                                             e.attribute("UpCode"),
                                             e.attribute("DownCode"),
                                             e.attribute("UpDelay").toInt(),
                                             e.attribute("DownDelay").toInt(),
                                             e.attribute("PressID"),
                                             e.attribute("ReadOnly")=="true"?true:false,
                                             e.attribute("UpDown").toInt()
                                             );
        config->PressUpDownSetups.push_back(ms);
        paramList=  paramList.nextSibling();
        // qDebug()<<ms.MCode<<ms.BackCode<<ms.SelectCode;
    }

}
void Parameters::XMLParsing::ReadMCodeSetup()
{
    config->MCodeSetups.clear();
    QDomElement root= doc.documentElement();
    QDomNode MCodeSetupsNode= root.elementsByTagName("MCodeSetups").item(0);
    QDomNode paramList=MCodeSetupsNode.firstChild();
    while (!paramList.isNull()) {
        QDomElement e=paramList.toElement();
        MCodeSetup ms=MCodeSetup(e.attribute("MCode"),e.attribute("SelectCode"),e.attribute("BackCode"));
        config->MCodeSetups.push_back(ms);
        paramList=  paramList.nextSibling();
        // qDebug()<<ms.MCode<<ms.BackCode<<ms.SelectCode;
    }

}
void Parameters::XMLParsing::ReadBasicSetup()
{
    config->BasicSetups.clear();
    QDomElement root= doc.documentElement();
    QDomNode basicSetupsNode= root.elementsByTagName("BasicSetups").item(0);
    QDomNode paramList=basicSetupsNode.firstChild();
    while (!paramList.isNull()) {
        QDomElement e=paramList.toElement();
        BasicSetup bs=BasicSetup(e.attribute("Name"),e.attribute("Value"),e.attribute("Description"),e.attribute("ReadOnly")=="true"?true:false);
        config->BasicSetups.push_back(bs);
        paramList=  paramList.nextSibling();
        // qDebug()<<bs.Name<<bs.Value<<bs.Description;
    }

}
void Parameters::XMLParsing::ReadPanel()
{
    QDomElement root= doc.documentElement();
    QDomNode projectNode= root.elementsByTagName("Project").item(0);
    QDomNode panelsNode=projectNode.firstChild();
    QDomElement panelNode=panelsNode.firstChild().toElement();//当前节点是Panel

    QDomNode machines=panelNode.elementsByTagName("Machines").item(0);

    part=new Part;
    part->Length;
    part->width=panelNode.attribute("Width").toFloat();
    part->Length=panelNode.attribute("Length").toFloat();
    part->Thickness=panelNode.attribute("Thickness").toFloat();
    QDomNode machine= machines.firstChild();
    while (!machine.isNull()) {
        QDomElement e=machine.toElement();
        Machining m;
        m.X=e.attribute("X").toFloat();
        m.Y=e.attribute("Y").toFloat();
        m.EndX=e.attribute("EndX").toFloat();
        m.EndY=e.attribute("EndY").toFloat();
        m.Face=e.attribute("Face").toInt();
        m.Width=e.attribute("Width").toFloat();
        m.Depth=e.attribute("Depth").toFloat();
        m.Diameter=e.attribute("Diameter").toFloat();
         m.Type=e.attribute("Type").toInt();

         part->Machines.push_back(m);
        machine=  machine.nextSibling();

    }
//    while (!panelNode.isNull()) {
//        QDomElement e=panelNode.toElement();
//         part->Name= e.attribute("Name");
//         part->width=e.attribute("Width").toFloat();
//         part->Length=e.attribute("Length").toFloat();

//        panelNode=  panelNode.nextSibling();
//         qDebug()<<ms.MCode<<ms.BackCode<<ms.SelectCode;
//    }

}


Parameters::XMLParsing::~XMLParsing()
{
    delete config;
    delete part;

}
