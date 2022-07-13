#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <QString>
#include <QStringList>

#define ONEBITS "ded"

QStringList bauds = {"9600","115200"};
QStringList parities = {"None", "Odd", "Even","Mark", "Space"};
QStringList DataBitsSizes = {"8 Bits", "7 Bits", "6 Bits", "5 Bits"};
QStringList StopBitsSizes = {ONEBITS, "1.5 Bits", "2 Bits"};


/*
 * Operation Modes
 */
#define BOOT 1      //Boot mode state
#define CONF 2      //Configuration mode state
#define OPS 3       //Operational mode state
#define SAFE -1     //Safe operation state
#define STATE 1     //Current state

/*
#define DEFAULTHOSTIP "127.0.0.1"
#define DEFAULTHOSTPORT "9999"
*/




#endif // DEFINITIONS_H
