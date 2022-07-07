/* Copyright (c) 2020-2022 hors<horsicq@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include "nfd_widget.h"
#include "ui_nfd_widget.h"

NFD_Widget::NFD_Widget(QWidget *pParent) :
    XShortcutsWidget(pParent),
    ui(new Ui::NFD_Widget)
{
    ui->setupUi(this);

    g_pdStruct={};

    connect(&watcher,SIGNAL(finished()),this,SLOT(onScanFinished()));

    ui->checkBoxDeepScan->setChecked(true);
    ui->checkBoxHeuristicScan->setChecked(true);
    ui->checkBoxRecursiveScan->setChecked(true);
    ui->checkBoxAllTypesScan->setChecked(false);

    ui->progressBarScan->hide();

    clear();
}

NFD_Widget::~NFD_Widget()
{
    if(g_bProcess)
    {
        stop();
        watcher.waitForFinished();
    }

    delete ui;
}

//void NFD_Widget::setOptions(NFD_Widget::OPTIONS *pOptions)
//{
//    ui->checkBoxRecursiveScan->setChecked(pOptions->bRecursiveScan);
//    ui->checkBoxDeepScan->setChecked(pOptions->bDeepScan);
//    ui->checkBoxHeuristicScan->setChecked(pOptions->bHeuristicScan);
//    ui->checkBoxAllTypesScan->setChecked(pOptions->bAllTypesScan);
//}

void NFD_Widget::setData(QString sFileName,bool bScan,XBinary::FT fileType)
{
    clear();

    this->sFileName=sFileName;
    this->fileType=fileType;
    g_scanType=ST_FILE;

    if(bScan)
    {
        process();
    }
}

void NFD_Widget::setGlobal(XShortcuts *pShortcuts,XOptions *pXOptions)
{
    ui->checkBoxAllTypesScan->setChecked(pXOptions->isAllTypesScan());
    ui->checkBoxDeepScan->setChecked(pXOptions->isDeepScan());
    ui->checkBoxRecursiveScan->setChecked(pXOptions->isRecursiveScan());
    ui->checkBoxHeuristicScan->setChecked(pXOptions->isHeuristicScan());
    ui->checkBoxVerbose->setChecked(pXOptions->isVerbose());

    XShortcutsWidget::setGlobal(pShortcuts,pXOptions);
}

void NFD_Widget::on_pushButtonNfdScan_clicked()
{
    process();
}

void NFD_Widget::clear()
{
    g_scanType=ST_UNKNOWN;
    g_bProcess=false;
    scanOptions={};
    scanResult={};
}

void NFD_Widget::process()
{
    if(!g_bProcess)
    {
        g_bProcess=true;
        enableControls(false);

        ui->pushButtonNfdScan->setText(tr("Stop"));

        scanOptions.bRecursiveScan=ui->checkBoxRecursiveScan->isChecked();
        scanOptions.bDeepScan=ui->checkBoxDeepScan->isChecked();
        scanOptions.bHeuristicScan=ui->checkBoxHeuristicScan->isChecked();
        scanOptions.bAllTypesScan=ui->checkBoxAllTypesScan->isChecked();
        scanOptions.fileType=fileType;
        //    scanOptions.bDebug=true;

    #if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
        QFuture<void> future=QtConcurrent::run(&NFD_Widget::scan,this);
    #else
        QFuture<void> future=QtConcurrent::run(this,&NFD_Widget::scan);
    #endif

        watcher.setFuture(future);
    }
    else
    {
        ui->pushButtonNfdScan->setEnabled(false);
        stop();
        watcher.waitForFinished();
        ui->pushButtonNfdScan->setText(tr("Scan"));
        enableControls(true);
    }
}

void NFD_Widget::scan()
{
    if(g_scanType!=ST_UNKNOWN)
    {
        if(g_scanType==ST_FILE)
        {
            emit scanStarted();

            staticScan.setData(sFileName,&scanOptions,&scanResult,&g_pdStruct);
            staticScan.process();

            emit scanFinished();
        }
    }
}

void NFD_Widget::stop()
{
    g_pdStruct.bIsStop=true;
}

void NFD_Widget::onScanFinished()
{
    enableControls(true);

    QAbstractItemModel *pOldModel=ui->treeViewResult->model();

    QList<XBinary::SCANSTRUCT> _listRecords=SpecAbstract::convert(&(scanResult.listRecords));

    ScanItemModel *pModel=new ScanItemModel(&_listRecords,this,1);
    ui->treeViewResult->setModel(pModel);
    ui->treeViewResult->expandAll();

    delete pOldModel;

    ui->lineEditElapsedTime->setText(QString("%1 %2").arg(QString::number(scanResult.nScanTime),tr("msec")));

    g_bProcess=false;

    ui->pushButtonNfdScan->setEnabled(true);
    ui->pushButtonNfdScan->setText(tr("Scan"));
}

void NFD_Widget::on_pushButtonNfdExtraInformation_clicked()
{
    ScanItemModel *pModel=static_cast<ScanItemModel *>(ui->treeViewResult->model());

    if(pModel)
    {
        DialogTextInfo dialogInfo(this);

        dialogInfo.setText(pModel->toString(XBinary::FORMATTYPE_TEXT));

        dialogInfo.exec();
    }
}

void NFD_Widget::enableControls(bool bState)
{
    if(!bState)
    {
        QAbstractItemModel *pOldModel=ui->treeViewResult->model();
        ui->treeViewResult->setModel(0);
        delete pOldModel;
    }

    ui->treeViewResult->setEnabled(bState);
    ui->checkBoxDeepScan->setEnabled(bState);
    ui->checkBoxHeuristicScan->setEnabled(bState);
    ui->checkBoxRecursiveScan->setEnabled(bState);
    ui->pushButtonNfdExtraInformation->setEnabled(bState);
    ui->lineEditElapsedTime->setEnabled(bState);

    if(bState)
    {
        ui->progressBarScan->hide();
    }
    else
    {
        ui->progressBarScan->show();
    }
}

void NFD_Widget::on_pushButtonNfdDirectoryScan_clicked()
{
    DialogStaticScanDirectory dds(this,QFileInfo(sFileName).absolutePath());

    dds.exec();
}

void NFD_Widget::registerShortcuts(bool bState)
{
    Q_UNUSED(bState)
}
