#ifndef COMMANDDATA_H
#define COMMANDDATA_H



#include <QString>


class commandDataStruct
{
private:
    QString functionName;
    QString functionProperty;
public:
    commandDataStruct();
    ~commandDataStruct();
    commandDataStruct(QString name, QString property) : functionName(name), functionProperty(property) {};
};




#endif // COMMANDDATA_H
