/* Copyright (c) 2020-2025 hors<horsicq@gmail.com>
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

NFD_Widget::NFD_Widget(QWidget *pParent) : XShortcutsWidget(pParent), ui(new Ui::NFD_Widget)
{
    ui->setupUi(this);

    m_pdStruct = XBinary::createPdStruct();

    connect(&m_watcher, SIGNAL(finished()), this, SLOT(on_scanFinished()));

    m_pTimer = new QTimer(this);
    connect(m_pTimer, SIGNAL(timeout()), this, SLOT(timerSlot()));

    ui->comboBoxFlags->setData(XScanEngine::getScanFlags(), XComboBoxEx::CBTYPE_FLAGS, 0, tr("Flags"));

    clear();

    ui->lineEditElapsedTime->setText(QString("%1 %2").arg(0).arg(tr("msec")));  // TODO Function
}

NFD_Widget::~NFD_Widget()
{
    if (m_bProcess) {
        stop();
        m_watcher.waitForFinished();
    }

    delete ui;
}

void NFD_Widget::setData(const QString &sFileName, bool bScan, XBinary::FT fileType)
{
    clear();

    this->m_sFileName = sFileName;
    this->m_fileType = fileType;
    m_scanType = ST_FILE;

    if (bScan) {
        process();
    }
}

void NFD_Widget::setGlobal(XShortcuts *pShortcuts, XOptions *pXOptions)
{
    XShortcutsWidget::setGlobal(pShortcuts, pXOptions);
}

void NFD_Widget::adjustView()
{
    quint64 nFlags = XScanEngine::getScanFlagsFromGlobalOptions(getGlobalOptions());
    ui->comboBoxFlags->setValue(nFlags);
}

void NFD_Widget::reloadData(bool bSaveSelection)
{
    Q_UNUSED(bSaveSelection)
    process();
}

void NFD_Widget::clear()
{
    m_scanType = ST_UNKNOWN;
    m_bProcess = false;
    m_scanOptions = {};
    m_scanResult = {};
}

void NFD_Widget::process()
{
    if (!m_bProcess) {
        m_bProcess = true;
        enableControls(false);

        ui->pushButtonNfdScanStart->setText(tr("Stop"));

        m_scanOptions.bShowType = true;
        m_scanOptions.bShowVersion = true;
        m_scanOptions.bShowInfo = true;
        m_scanOptions.fileType = m_fileType;
        m_scanOptions.nBufferSize = getGlobalOptions()->getValue(XOptions::ID_SCAN_BUFFERSIZE).toULongLong();
        m_scanOptions.bIsHighlight = getGlobalOptions()->getValue(XOptions::ID_SCAN_HIGHLIGHT).toBool();
        m_scanOptions.bHideUnknown = getGlobalOptions()->getValue(XOptions::ID_SCAN_HIDEUNKNOWN).toBool();
        m_scanOptions.bIsSort = getGlobalOptions()->getValue(XOptions::ID_SCAN_SORT).toBool();
        //    scanOptions.bDebug=true;

        quint64 nFlags = ui->comboBoxFlags->getValue().toULongLong();
        XScanEngine::setScanFlags(&m_scanOptions, nFlags);

        XScanEngine::setScanFlagsToGlobalOptions(getGlobalOptions(), nFlags);

        m_pTimer->start(200);  // TODO const

        ui->progressBar0->hide();
        ui->progressBar1->hide();
        ui->progressBar2->hide();
        ui->progressBar3->hide();
        ui->progressBar4->hide();

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        QFuture<void> future = QtConcurrent::run(&NFD_Widget::scan, this);
#else
        QFuture<void> future = QtConcurrent::run(this, &NFD_Widget::scan);
#endif

        m_watcher.setFuture(future);
    } else {
        ui->pushButtonNfdScanStart->setEnabled(false);
        stop();
        m_watcher.waitForFinished();
        ui->pushButtonNfdScanStart->setText(tr("Scan"));
        enableControls(true);
    }
}

void NFD_Widget::scan()
{
    if (m_scanType != ST_UNKNOWN) {
        if (m_scanType == ST_FILE) {
            emit scanStarted();

            m_pdStruct = XBinary::createPdStruct();

            m_pSpecAbstract.setData(m_sFileName, &m_scanOptions, &m_scanResult, &m_pdStruct);
            m_pSpecAbstract.process();

            if (m_scanResult.ftInit == XBinary::FT_COM) {
                emit currentFileType(m_scanResult.ftInit);
            }

            emit scanFinished();
        }
    }
}

void NFD_Widget::stop()
{
    m_pdStruct.bIsStop = true;
}

void NFD_Widget::on_scanFinished()
{
    enableControls(true);

    m_pTimer->stop();

    QAbstractItemModel *pOldModel = ui->treeViewResult->model();

    ScanItemModel *pModel = new ScanItemModel(&m_scanOptions, &(m_scanResult.listRecords), 1);
    ui->treeViewResult->setModel(pModel);
    ui->treeViewResult->expandAll();

    deleteOldAbstractModel(&pOldModel);

    ui->lineEditElapsedTime->setText(QString("%1 %2").arg(QString::number(m_scanResult.nScanTime), tr("msec")));

    m_bProcess = false;

    ui->pushButtonNfdScanStart->setEnabled(true);
    ui->pushButtonNfdScanStart->setText(tr("Scan"));
}

void NFD_Widget::on_pushButtonNfdExtraInformation_clicked()
{
    ScanItemModel *pModel = static_cast<ScanItemModel *>(ui->treeViewResult->model());

    if (pModel) {
        DialogTextInfo dialogInfo(this);
        dialogInfo.setGlobal(getShortcuts(), getGlobalOptions());
        dialogInfo.setText(pModel->toString(XBinary::FORMATTYPE_TEXT));

        dialogInfo.exec();
    }
}

void NFD_Widget::enableControls(bool bState)
{
    if (!bState) {
        QAbstractItemModel *pOldModel = ui->treeViewResult->model();
        ui->treeViewResult->setModel(0);

        deleteOldAbstractModel(&pOldModel);
    }

    ui->treeViewResult->setEnabled(bState);
    ui->comboBoxFlags->setEnabled(bState);
    ui->pushButtonNfdDirectoryScan->setEnabled(bState);
    ui->pushButtonNfdExtraInformation->setEnabled(bState);
    ui->pushButtonNfdInfo->setEnabled(bState);
    ui->lineEditElapsedTime->setEnabled(bState);

    if (bState) {
        ui->stackedWidgetNfdScan->setCurrentIndex(0);
    } else {
        ui->stackedWidgetNfdScan->setCurrentIndex(1);
    }
}

void NFD_Widget::on_pushButtonNfdDirectoryScan_clicked()
{
    DialogNFDScanDirectory dds(this, QFileInfo(m_sFileName).absolutePath());
    dds.setGlobal(getShortcuts(), getGlobalOptions());
    dds.exec();
}

void NFD_Widget::registerShortcuts(bool bState)
{
    Q_UNUSED(bState)
}

void NFD_Widget::on_pushButtonNfdInfo_clicked()
{
    if (!m_scanOptions.bHandleInfo) {
        DialogNFDWidgetAdvanced dnwa(this);
        dnwa.setGlobal(getShortcuts(), getGlobalOptions());
        dnwa.setData(m_sFileName, m_scanOptions, true);

        dnwa.exec();
    } else {
        emit showInfo();
    }
}

void NFD_Widget::on_pushButtonNfdScanStart_clicked()
{
    ui->pushButtonNfdScanStart->setEnabled(false);
    process();
    ui->pushButtonNfdScanStart->setEnabled(true);
}

void NFD_Widget::on_pushButtonNfdScanStop_clicked()
{
    ui->pushButtonNfdScanStop->setEnabled(false);
    process();
    ui->pushButtonNfdScanStop->setEnabled(true);
}

void NFD_Widget::timerSlot()
{
    XFormats::setProgressBar(ui->progressBar0, m_pdStruct._pdRecord[0]);
    XFormats::setProgressBar(ui->progressBar1, m_pdStruct._pdRecord[1]);
    XFormats::setProgressBar(ui->progressBar2, m_pdStruct._pdRecord[2]);
    XFormats::setProgressBar(ui->progressBar3, m_pdStruct._pdRecord[3]);
    XFormats::setProgressBar(ui->progressBar4, m_pdStruct._pdRecord[4]);
}
