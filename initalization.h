#ifndef INITALIZATION_H
#define INITALIZATION_H


#include <QSerialPort>
#include <QSerialPortInfo>
#include <QString>
#include <QDebug>
#include <QTimer>

/* Initalization Module
 *
 * - Serial Port
 * - TCP Connection
 *
*/

class Initalization
{
public:
    Initalization();

    /*
     * Serial Connection Fucntions
     */
    int serialConnect(const QString portName, const int baudRate); //Return 1 for succesful connection == 0 for unsuccesfull
    int status(); //checks whether object connected
    void disconnect(); //disconnects if connected
    QString readLine();


private:
    QSerialPort *serial; //initalize serial port
    bool isConnected = false; //Toggle for connected
};

#endif // INITALIZATION_H
