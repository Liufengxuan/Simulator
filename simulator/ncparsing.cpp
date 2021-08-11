#include "ncparsing.h"



NcCode::NcParsing::NcParsing(QObject *parent)
{



}


int NcCode::NcParsing::Parsing(const QString& filePath)
{
    QFile ncFile(filePath);
    //ncCmds=new QVector<NCCommand>();
    if(ncFile.open(QIODevice::ReadOnly))
    {
        QByteArray t=ncFile.readAll();
        QTextCodec *codec=QTextCodec::codecForName("GBK");
        ncCodeSource=codec->toUnicode(t);
        ncFile.close();
    }
    QStringList list= ncCodeSource.split(QRegExp("[\r\n]"),QString::SkipEmptyParts);
    for (int i=0;i<list.length();i++) {

        //去除注释和末尾空格
        QString singleLine=  this->DelCodeComment(list[i]);
        if(singleLine.isEmpty())continue;
        NCCommand *cmd=new NCCommand();
        cmd->source=list[i];
        //将单行nc代码解析为NCCmd结构。
        this->SingleLineParsing(singleLine,cmd);
        //将解析后的加入到vector中
        ncCmds.push_back(cmd);
    }
    qDebug()<<QString("解析nc文件完成，总共{%1}行。").arg(ncCmds.count());
    return ncCmds.count();


}

///1.将单行nc代码去除注释和末尾空格
QString NcCode::NcParsing::DelCodeComment( QString nc)
{
    int a= nc.indexOf("//");
    if(a>=0)
    {
        return nc.remove(a,nc.length()-a).trimmed();
    }else{
        return nc;
    }

}
///2.将单行nc代码解析为NCCmd结构体。
void NcCode::NcParsing::SingleLineParsing(const QString &nc, NCCommand* cmd)
{
    QStringList list= nc.split(QRegExp("[\\s]"),QString::SkipEmptyParts);
    cmd->funcType=FuncType::none;
    //正则示例 ：G00 、G53
    QRegExp exp1("^[G]\\d{2}$");
    //正则示例 ：W=100.00 、X2=333.11
    QRegExp exp2("^[a-zA-Z][0-9]?=[-.0-9]*");
    //正则示例 ：F18000
    QRegExp exp3("^[F][0-9]*");
    //正则示例 ：P800 M321
    QRegExp exp4("^[P|M][0-9]*");
    //正则示例 ：{W=100.00} 、X2=333.11
    QRegExp exp5("[a-zA-Z][0-9]?=[-.0-9]*");
    //正则示例 ：T33 T12T13
    QRegExp exp6("[T][0-9]+");

    for (int i=0;i<list.length();i++){
        //去除所有空格
        list[i]=list[i].remove(QRegExp("\\s"));
        //如果是空字符串则忽略
        if(list[i].length()<1)continue;

        //1.走到这里代表读取到了第一个非空列，也就是行号那一列
        if(cmd->lineNum.isEmpty()){
            cmd->lineNum=list[i];
            continue;
        }
        //2.走到这里代表已经读取到行号。需要判断本列是否为T,M,G,W开头
        if(cmd->funcType==FuncType::none)
        {
            char head=list[i].at(0).toLatin1();
            switch (head) {
            case 'T':cmd->funcType=FuncType::tCode;break;
            case 'M':cmd->funcType=FuncType::mCode;break;
            case 'W':
            case 'G':cmd->funcType=FuncType::gCode;break;
            }
            //continue;
        }
        if(cmd->funcType==FuncType::gCode)
        {

            if(exp1.indexIn(list[i])>=0){
                if(cmd->gCode1.isEmpty())cmd->gCode1=list[i];
                else if(cmd->gCode2.isEmpty())cmd->gCode2=list[i];
            }
            else if(exp2.indexIn(list[i])>=0){
                QStringList l= exp2.cap(0).split("=");
                for (int f=0;f<AXIS_MAXNUM;f++) {
                    if(cmd->axisFlag[f].isEmpty()){
                        cmd->axisFlag[f]=l[0];
                        cmd->loction[f]=l[1].toFloat();
                        break;
                    }
                }

            }
            else if(exp3.indexIn(list[i])>=0){
                cmd->speed= exp3.cap(0).remove("F").remove("f").toFloat();
            }
            else if(exp4.indexIn(list[i])>=0){
                cmd->funcType=FuncType::stop;
                cmd->stop= exp4.cap(0).remove("P").remove("p").toFloat();
            }
        }
        if(cmd->funcType==FuncType::mCode){
            if(exp4.indexIn(list[i])>=0){
                cmd->mCode=exp4.cap(0);
            }
            else if(exp5.indexIn(list[i])>=0){
                QStringList l= exp5.cap(0).split("=");
                for (int f=0;f<AXIS_MAXNUM;f++) {
                    if(cmd->axisFlag[f].isEmpty()){
                        cmd->axisFlag[f]=l[0];
                        cmd->loction[f]=l[1].toFloat();
                        break;
                    }
                }
            }
        }
        if(cmd->funcType==FuncType::tCode){
            for (int pos=0;exp6.indexIn(list[i],pos)!=-1;pos+=exp6.matchedLength()) {
                for (int j=0;j<AXIS_MAXNUM;j++) {
                    if(cmd->tCode[j].isEmpty()){
                        cmd->tCode[j]=exp6.cap(0);
                        break;
                    }
                }
            }
        }
    }

}

NcCode::NcParsing::~NcParsing(){

    for(auto it = ncCmds.begin(); it != ncCmds.end(); ++it)
    {
        if(*it != nullptr)
        {
            delete (*it);
            (*it) = nullptr;
        }

    }
    ncCmds.clear();
    QVector<NCCommand*>().swap(ncCmds);


}
