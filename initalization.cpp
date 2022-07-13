#include "initalization.h"
#include <QSerialPort>
#include <QString>

/*
 * Standard functions
 */

Initalization::Initalization()
{
    isConnected = false;
    serial = new QSerialPort;
}


/*
 * Serial connection Functions
*/

int Initalization::serialConnect(const QString portName, const int baudRate)
{
    if(!isConnected){
        //variables
        serial->setPortName(portName);
        serial->open(QIODevice::ReadWrite);
        serial->setBaudRate(115200);
        //Deafult values can be changed later if needed
        serial->setDataBits(QSerialPort::Data8);
        serial->setParity(QSerialPort::NoParity);
        serial->setStopBits(QSerialPort::OneStop);
        serial->setFlowControl(QSerialPort::NoFlowControl);
        while(!serial->isOpen()) serial->open(QIODevice::ReadWrite);
        if (serial->isOpen() && serial->isWritable())
        {
            qDebug() << "Serial Connection is available" << endl;
            isConnected = true;
            return 1;
        }
        else {
            qDebug() << "Serial connection could not estabblished !!!" << endl;
            return 0;
        }
    }
    else {
        return 0;
    }
}

QString Initalization::readLine()
{
    if(!isConnected)
    {
        return serial->readAll();
    }
}

int Initalization::status() //Return 1 for connected, 0 for not  connected
{
    if(serial->isOpen())
        return 1;
    else
        return 0;
}

void Initalization::disconnect()
{
    if(isConnected)
    {
        serial->close();
        isConnected = 0;
    }
}
