#include <cstdlib>
#include <unistd.h>

#include <QDebug>
#include <QProcess>
#include <QTime>
#include <QTemporaryFile>
#include <QDataStream>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <libindi/basedevice.h>

#define MAX_FILENAME_LEN    1024

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    BLOBDirty = false;

    kstProcess = new QProcess();

    specTempFile = new QTemporaryFile();

    out = new QDataStream();

    QObject::connect(ui->connectB, SIGNAL(clicked()), this, SLOT(connectServer()));
    QObject::connect(ui->disconnectB, SIGNAL(clicked()), this, SLOT(disconnectServer()));
    QObject::connect(this, SIGNAL(modeUpdated(int)), this, SLOT(updateModeButtons(int)));

}

MainWindow::~MainWindow()
{
    kstProcess->kill();
    delete kstProcess;
    delete specTempFile;
}

void MainWindow::connectServer()
{
    qDebug() << "Connecting ...." << endl;
    bool portOK=false;
    int serverPort = ui->serverPort->text().toInt(&portOK);

    if (portOK == false)
    {
        ui->msgQueue->append(QString("KNRO: %1 is an invalid port, please try again...").arg(ui->serverPort->text()));
        return;
    }

    QFont cf = ui->connectB->font();
    cf.setBold(true);
    ui->connectB->setFont(cf);

    QFont df = ui->disconnectB->font();
    df.setBold(false);
    ui->disconnectB->setFont(df);

    // in KNRO Lab, we preselected port 8000 for Spectracyber driver. In the XML file under /usr/share/indi, the port is also specified.
    // User may still change it, but 8000 is default
    setServer("localhost", serverPort);

    if (INDI::BaseClient::connect() == false)
        ui->msgQueue->append(QString("KNRO: connection to server on port %1 is refused...").arg(ui->serverPort->text()));

}

void MainWindow::disconnectServer()
{
    qDebug() << "Disconnecting ..." << endl;

    QFont cf = ui->connectB->font();
    cf.setBold(false);
    ui->connectB->setFont(cf);

    QFont df = ui->disconnectB->font();
    df.setBold(true);
    ui->disconnectB->setFont(df);

    INDI::BaseClient::disconnect();

    BLOBDirty = true;

    ui->msgQueue->append(QString("Disconnecting..."));

}

void MainWindow::newDevice()
{
    std::vector<INDI::BaseDevice *>::const_iterator devicei;

    for (devicei = getDevices().begin(); devicei != getDevices().end(); devicei++)
            ui->msgQueue->append(QString("Connection to %1 is successful.").arg((*devicei)->deviceName()));

    setBLOBMode(B_ALSO);

}

void MainWindow::newBLOB(IBLOB *bp)
{
   int nr=0,n=0;

    if (currentSMode != QString(bp->format) || BLOBDirty)
    {
        BLOBDirty = false;
        currentSMode = QString(bp->format);

        delete (specTempFile);

        specTempFile = new QTemporaryFile();
        kstProcess->kill();
        kstProcess->waitForFinished();

        specTempFile->setFileTemplate(QString("/tmp/XXXXXX%1").arg(currentSMode));

        if (specTempFile->open() == false)
        {
            qDebug() << "Failed to open temp file!" << endl;
            return;
        }

        out->setDevice(specTempFile);
    }

    for (nr=0; nr < bp->size; nr += n)
        n = out->writeRawData( static_cast<const char *>(bp->blob) + nr, bp->size - nr);

    out->writeRawData( (const char *) "\n" , 1);
    specTempFile->flush();


    if (kstProcess->state() == QProcess::NotRunning)
    {
        QStringList argList;

        if (currentSMode == ".ascii_cont")
        {
            kstProcess->start(QString("kst -x 1 -y 2 %1").arg(specTempFile->fileName()));
            ui->msgQueue->append(QString("KNRO: Starting continuum channel monitor..."));
            emit modeUpdated(0);
        }
        else
        {
            kstProcess->start(QString("kst -x 3 -y 2 %1").arg(specTempFile->fileName()));
            ui->msgQueue->append(QString("KNRO: Starting spectral channel monitor..."));
            emit modeUpdated(1);

        }

        kstProcess->waitForStarted();

    }
}

void MainWindow::updateModeButtons(int specMode)
{

    if (specMode == 0)
    {
        QFont cf = ui->contB->font();
        cf.setBold(true);
        ui->contB->setFont(cf);

        QFont df = ui->specB->font();
        df.setBold(false);
        ui->specB->setFont(df);
    }
    else
    {
        QFont cf = ui->contB->font();
        cf.setBold(false);
        ui->contB->setFont(cf);

        QFont df = ui->specB->font();
        df.setBold(true);
        ui->specB->setFont(df);

    }

}

void MainWindow::newSwitch(ISwitchVectorProperty *svp)
{
    if (QString("Scan") == QString(svp->name))
        BLOBDirty = true;
}