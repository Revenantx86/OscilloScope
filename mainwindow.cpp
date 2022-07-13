#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "treeviewcommands.cpp"
#include <QHostAddress>

#include "definitions.h"


#include <QFiledialog>



extern QStringList bauds;
extern QStringList parities;
extern QStringList DataBitsSizes;
extern QStringList StopBitsSizes;

MainWindow::MainWindow(QWidget *parent) :QMainWindow(parent),ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    serialTimer = new QTimer(this);
    mainItemModel = new QStandardItemModel(this);


    //#Initalize boxes
    setSerialBoxes();
    setCommandTree();
    setTCPConnection();


}



MainWindow::~MainWindow()
{
    delete ui;
}

/*
 * Initalizaiton layout
 */
void MainWindow::setSerialBoxes()
{
    // Adding serial available serial ports //
    foreach (const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts())
        {
        ui->PortName_comboBox->addItem(serialPortInfo.portName());
        }

    // Adding available baud rates //
   for(int i = 0; i < bauds.size(); i++)
   {
       ui->BaudRate_comboBox->addItem(bauds[i]);
   }

   // Adding Data Bits size selections //
   for(int i = 0; i < DataBitsSizes.size(); i++)
   {
       ui->DataBits_comboBox->addItem(DataBitsSizes[i]);
   }

   // Adding Stop Bits size selections //

   for(int i = 0; i < StopBitsSizes.size(); i++)
   {
       ui->StopBits_comboBox->addItem(StopBitsSizes[i]);
   }

   //adding Avalilale parity bits selections //
   for(int i = 0; i < parities.size(); i++)
   {
        ui->Parity_comboBox->addItem(parities[i]);
   }
}



//Setting the TCP connection Boxes
void MainWindow::setTCPConnection()
{

    //adding default adresses to the combo boxs
    ui->TCP_Select_IP_comboBox->addItem(targetHostAdress);
    ui->TCP_Select_Port_comboBox->addItem(targetPortAdress);

    //disabling the manual entry
    ui->TCP_ManualInput_IP_lineEdit->setEnabled(0);
    ui->TCP_ManualInput_IP_label->setEnabled(0);

    ui->TCP_ManualInput_Port_lineEdit->setEnabled(0);
    ui->TCP_ManualInput_Port_label->setEnabled(0);
}



/*
 *  Custom Functionalities
 */
//set up command tree with initial parameters
void MainWindow::setCommandTree()
{
    /*
    QStandardItem* item1 = new QStandardItem("firstItem");
    QStandardItem* item1Sub = new QStandardItem("sub of first Item");
    QStandardItem* item1Subcolumn = new QStandardItem("column of first Item");
    QList< QStandardItem> *itemDeneme;
    itemDeneme->append(*item1);
    itemDeneme->append(*item1Subcolumn);
    item1->appendRow(item1Sub);
    //item1Sub->appendColumn(item1Subcolumn);
    QStringList header;
    header << "Comands";
    mainItemModel->appendRow(item1);
    //mainItemModel->appendRow(itemDeneme);
    mainItemModel->setItem(0,1,item1Subcolumn);

    mainItemModel->setHorizontalHeaderItem(0,new QStandardItem("Commands"));
    mainItemModel->setHorizontalHeaderItem(1,new QStandardItem("Property"));
    ui->Commands_treeView->setModel(mainItemModel);
    ui->Commands_treeView->setHeaderHidden(0);
    */

    QList<QStandardItem *> items;

    items.append(new QStandardItem("Column 1 Text"));
    items.append(new QStandardItem("Column 2 Text"));

    QStandardItemModel* model = new QStandardItemModel(this);

    QStandardItem* item1 = new QStandardItem("firstItem");
    item1->appendRow(items);
    model->setColumnCount(2);
    model->appendRow(item1);

    ui->Commands_treeView->setModel(model);
}



//updates console text -> used for tcp connection
void MainWindow::updateConsoleText()
{
    ui->Console_textEdit->append(init->readLine());
}


/*
 *  Button Functionalities
 */

void MainWindow::on_TCP_EnableManualInput_checkBox_stateChanged(int arg1)
{
    if(!arg1)
    {
        //Disable manual input boxes
        ui->TCP_ManualInput_IP_lineEdit->setEnabled(0);
        ui->TCP_ManualInput_IP_label->setEnabled(0);
        ui->TCP_ManualInput_Port_lineEdit->setEnabled(0);
        ui->TCP_ManualInput_Port_label->setEnabled(0);
        //Enable combobox selextion
        ui->TCP_Select_IP_comboBox->setEnabled(1);
        ui->TCP_Select_IP_label->setEnabled(1);
        ui->TCP_Select_Port_comboBox->setEnabled(1);
        ui->TCP_Select_Port_label->setEnabled(1);
    }
    else //if Manual input is enabled
    {
        //enable manual input boxes
        ui->TCP_ManualInput_IP_lineEdit->setEnabled(1);
        ui->TCP_ManualInput_IP_label->setEnabled(1);
        ui->TCP_ManualInput_Port_lineEdit->setEnabled(1);
        ui->TCP_ManualInput_Port_label->setEnabled(1);
        //disable combobox selextion
        ui->TCP_Select_IP_comboBox->setEnabled(0);
        ui->TCP_Select_IP_label->setEnabled(0);
        ui->TCP_Select_Port_comboBox->setEnabled(0);
        ui->TCP_Select_Port_label->setEnabled(0);
    }
}


void MainWindow::on_TCP_Connect_pushButton_clicked()
{
    if( socket.state() != QTcpSocket::ConnectedState) //if Not connected
    {
        socket.connectToHost(QHostAddress(targetHostAdress), targetPortAdress.toInt());
        connect(&socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
        if(socket.state() == QTcpSocket::ConnectedState);
        {
            ui->TCP_EnableManualInput_checkBox->setEnabled(0);
            ui->SerialConnection_groupBox->setEnabled(0);
            ui->TCP_Connect_pushButton->setText("Disconnect");
        }
    }
    else //if Connected
    {
        socket.disconnect();
        if(socket.state() != QTcpSocket::ConnectedState);
        {
            ui->TCP_EnableManualInput_checkBox->setEnabled(1);
            ui->SerialConnection_groupBox->setEnabled(1);
            ui->TCP_Connect_pushButton->setText("Connect");
        }
    }
}


void MainWindow::on_Connect_pushButton_clicked() //Connect to the serial device if not successfull display error
{
    if(init->serialConnect(ui->PortName_comboBox->currentText(), ui->BaudRate_comboBox->currentText().toInt()))
    {
        ui->Connect_pushButton->setText("Disconnect");
        QObject::connect(serialTimer, SIGNAL(timeout()), this, SLOT(updateConsoleText()));
    }
    else {
        QMessageBox::critical(this, tr("Error"), "Could not connect serial Device ! ");
    }
}


//Getting input from command box and writing to tcp and displaying on console
void MainWindow::on_ConsoleSend_pushButton_clicked()
{
    if( ui->Console_lineEdit->text().size() > 0) //if there is actually input
    {
        //writing on the TCP Server
        if(socket.state() == QTcpSocket::ConnectedState)
        {
            socket.write(ui->Console_lineEdit->text().toUtf8());
            socket.flush();
            displayMessageConsole("Sending ->", "magenta");
            displayMessageConsole(ui->Console_lineEdit->text() + "\n", "white"); //displaying on console text box
        }
        else
        {

        }
        ui->Console_lineEdit->clear(); //clear

    }
}




void MainWindow::onReadyRead()
{
    QByteArray datas = socket.readAll();
    QString result = datas;
    displayMessageConsole("EGSE: "+result, "cyan");
    socket.write(QByteArray("ok !\n"));
}





void MainWindow::on_Commands_treeView_doubleClicked(const QModelIndex &index)
{
    ui->Console_lineEdit->setText(mainItemModel->itemFromIndex(index)->text());
    //QStandardItemModel * temp = mainItemModel->itemFromIndex(index)->takeColumn(1);
    //ui->Console_textEdit->insertPlainText(mainItemModel->itemFromIndex(index)->text());
}


/*
 * Importing the commands text file, selecting text file at qDialog,
 * it will automatically import to the command tree
 */
void MainWindow::on_Command_Import_pushButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Select Text File "),QDir::rootPath(),"Text File (*.txt)");
    if(fileName.length() > 0) //if text selected
    {
        ui->Console_textEdit->setTextColor(QColor("magenta"));
        ui->Console_textEdit->insertPlainText("Opening text file at -> \" ");
        ui->Console_textEdit->setTextColor(QColor("white"));
        ui->Console_textEdit->insertPlainText(fileName);
        ui->Console_textEdit->insertPlainText("\" \n");
        mainItemModel = readCommandsFromFile(fileName,dataStruct);
        ui->Commands_treeView->setModel(mainItemModel);
    }

}


/*
 * External Methods
 */
void MainWindow::displayMessageBox(QString message, QString color)
{
    QMessageBox::about(this,"Warning !", message);
}

void MainWindow::displayMessageConsole(QString message,QString color)
{
    ui->Console_textEdit->setTextColor(QColor(color));
    ui->Console_textEdit->insertPlainText(message);
    ui->Console_textEdit->setTextColor(QColor("white"));
}





