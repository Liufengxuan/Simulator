#ifndef NCPARSING_H
#define NCPARSING_H

#include <QObject>
#include "QVector"
#include "nccodemodel.h"
#include "QFile"
#include "qdebug.h"
#include "qtextcodec.h"




namespace NcCode{


class NcParsing : public QObject
{
    Q_OBJECT
public:
    // explicit NcParsing(QObject *parent = nullptr);
    explicit NcParsing(QObject *parent = nullptr);
    ~NcParsing();
    int Parsing(const QString& filePath);

private:
    QString DelCodeComment( QString nc);
    void SingleLineParsing(const QString &nc,NCCommand* cmd);
public:
    QString ncCodeSource;
    QVector<NCCommand*> ncCmds;

signals:

public slots:
};


}
#endif // NCPARSING_H
