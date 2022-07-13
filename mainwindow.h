#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPortInfo>
#include <QSerialPort>
#include <QDebug>

#include <QMessageBox>
#include <QTimer>

#include "initalization.h"
#include "commandData.h"

#include <QStandardItemModel>
#include <QTcpSocket>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setSerialBoxes();
    void setCommandTree();

    /*
     * Startup Setup Functions
     */
    void setTCPConnection();



    /*
     * External Acces Methods
     */
    void displayMessageBox(QString message,QString color); //Displaying message at QT Message box
    void displayMessageConsole(QString message, QString color); //Displaing message at Console

private slots:
    void on_Connect_pushButton_clicked();

    void on_ConsoleSend_pushButton_clicked();

    void updateConsoleText();
    void onReadyRead();


    void on_Commands_treeView_doubleClicked(const QModelIndex &index);

    void on_Command_Import_pushButton_clicked();

    void on_TCP_Connect_pushButton_clicked();

    void on_TCP_EnableManualInput_checkBox_stateChanged(int arg1);

private:
    QTimer *serialTimer;
    Initalization *init = new Initalization;
    Ui::MainWindow *ui;
    QTcpSocket  socket;

    //Command data structure

    QVector<commandDataStruct> *dataStruct;

    //Private TCP Variables
    QString targetHostAdress = "10.58.0.81";
    QString targetPortAdress = "22";

    //command Tree item model instantiation
    QStandardItemModel *mainItemModel;



};

#endif // MAINWINDOW_H
