#ifndef XMLPARSING_H
#define XMLPARSING_H

#include <QObject>
#include <qvector.h>
#include <parametermodel.h>
#include <QtXml/QDomDocument>
#include "qdebug.h"
#include <QFile>
#include <qapplication.h>

namespace  Parameters {
class XMLParsing : public QObject
{
    Q_OBJECT
public:
   // explicit XMLParsing(QObject *parent = nullptr);
    explicit XMLParsing(QObject *parent = nullptr);
    bool Parsing(const QString& configPath,const QString& panelPath);
    ~XMLParsing();
private:


    void ReadBasicSetup();
    void ReadMCodeSetup();
    void ReadPressUpDownSetup();
    void ReadDrillInfo();
    void ReadPanel();


public:
    Parameters::Config *config;
    Parameters::Part *part;
private:
    QDomDocument doc;




signals:

public slots:
};

}
#endif // XMLPARSING_H
