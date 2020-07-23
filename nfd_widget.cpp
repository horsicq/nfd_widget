// copyright (c) 2020 hors<horsicq@gmail.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
#include "nfd_widget.h"
#include "ui_nfd_widget.h"

NFD_Widget::NFD_Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NFD_Widget)
{
    ui->setupUi(this);

    connect(&watcher,SIGNAL(finished()),this,SLOT(onScanFinished()));
}

NFD_Widget::~NFD_Widget()
{
    delete ui;
}

void NFD_Widget::setOptions(NFD_Widget::OPTIONS *pOptions)
{
    ui->checkBoxDeepScan->setChecked(pOptions->bDeepScan);
}

void NFD_Widget::setData(QString sFileName, bool bScan)
{
    clear();

    this->sFileName=sFileName;
    scanType=ST_FILE;

    if(bScan)
    {
        process();
    }
}

void NFD_Widget::on_pushButtonScan_clicked()
{
    process();
}

void NFD_Widget::clear()
{
    scanType=ST_UNKNOWN;
    bProcess=false;
    scanOptions={};
}

void NFD_Widget::process()
{
    if(!bProcess)
    {
        bProcess=true;

        ui->pushButtonScan->setText(tr("Stop"));

        scanOptions.bDeepScan=ui->checkBoxDeepScan->isChecked();
        //    scanOptions.bDebug=true;

        QFuture<void> future=QtConcurrent::run(this,&NFD_Widget::scan);

        watcher.setFuture(future);
    }
    else
    {
        stop();
        watcher.waitForFinished();
        ui->pushButtonScan->setText(tr("Scan"));
    }
}

void NFD_Widget::scan()
{
    if(scanType!=ST_UNKNOWN)
    {
        if(scanType==ST_FILE)
        {

        }
    }
}

void NFD_Widget::stop()
{

}

void NFD_Widget::onScanFinished()
{
    bProcess=false;

    ui->progressBarProgress->setMaximum(100);
    ui->progressBarProgress->setValue(100);
    ui->pushButtonScan->setText(tr("Scan"));
}

void NFD_Widget::onProgressMaximumChanged(qint32 nMaximum)
{
    ui->progressBarProgress->setMaximum(nMaximum);
}

void NFD_Widget::onProgressValueChanged(qint32 nValue)
{
    ui->progressBarProgress->setValue(nValue);
}

void NFD_Widget::on_pushButtonExtraInformation_clicked()
{
    // TODO
}
