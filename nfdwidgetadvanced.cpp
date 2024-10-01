/* Copyright (c) 2020-2024 hors<horsicq@gmail.com>
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
#include "nfdwidgetadvanced.h"

#include "ui_nfdwidgetadvanced.h"

NFDWidgetAdvanced::NFDWidgetAdvanced(QWidget *pParent) : XShortcutsWidget(pParent), ui(new Ui::NFDWidgetAdvanced)
{
    ui->setupUi(this);

    XOptions::adjustToolButton(ui->toolButtonScan, XOptions::ICONTYPE_SCAN);
    XOptions::adjustToolButton(ui->toolButtonSave, XOptions::ICONTYPE_SAVE);

    ui->comboBoxType->setToolTip(tr("Type"));
    ui->comboBoxFlags->setToolTip(tr("Flags"));
    ui->treeViewScan->setToolTip(tr("Scan"));
    ui->tableViewHeur->setToolTip(tr("Heuristics"));
    ui->toolButtonSave->setToolTip(tr("Save"));
    ui->toolButtonScan->setToolTip(tr("Scan"));

    this->g_pDevice = nullptr;
    this->g_fileType = XBinary::FT_UNKNOWN;

    ui->comboBoxFlags->setData(XScanEngine::getScanFlags(), XComboBoxEx::CBTYPE_FLAGS, 0, tr("Flags"));
}

NFDWidgetAdvanced::~NFDWidgetAdvanced()
{
    delete ui;
}

void NFDWidgetAdvanced::setData(QIODevice *pDevice, bool bScan, XBinary::FT fileType)
{
    this->g_pDevice = pDevice;
    this->g_fileType = fileType;

    XFormats::setFileTypeComboBox(fileType, pDevice, ui->comboBoxType, XBinary::TL_OPTION_ALL);

    if (bScan) {
        process();
    }
}

void NFDWidgetAdvanced::setData(const QString &sFileName, const XScanEngine::SCAN_OPTIONS &scanOptions, bool bScan)
{
    this->g_sFileName = sFileName;
    this->g_fileType = scanOptions.fileType;

    XFormats::setFileTypeComboBox(scanOptions.fileType, sFileName, ui->comboBoxType, XBinary::TL_OPTION_ALL);

    if (bScan) {
        process();
    }
}

void NFDWidgetAdvanced::adjustView()
{
    getGlobalOptions()->adjustTreeView(ui->treeViewScan, XOptions::ID_VIEW_FONT_TREEVIEWS);
    getGlobalOptions()->adjustTableView(ui->tableViewHeur, XOptions::ID_VIEW_FONT_TABLEVIEWS);

    quint64 nFlags = XScanEngine::getScanFlagsFromGlobalOptions(getGlobalOptions());
    ui->comboBoxFlags->setValue(nFlags);
}

void NFDWidgetAdvanced::setGlobal(XShortcuts *pShortcuts, XOptions *pXOptions)
{
    XShortcutsWidget::setGlobal(pShortcuts, pXOptions);
}

void NFDWidgetAdvanced::on_toolButtonScan_clicked()
{
    process();
}

void NFDWidgetAdvanced::registerShortcuts(bool bState)
{
    Q_UNUSED(bState)
}

void NFDWidgetAdvanced::on_toolButtonSave_clicked()
{
    QAbstractItemModel *pModel = ui->treeViewScan->model();

    if (pModel) {
        QString sSaveFileName;

        if (g_pDevice) {
            sSaveFileName = XBinary::getResultFileName(g_pDevice, QString("%1.txt").arg(tr("Result")));
        } else {
            sSaveFileName = XBinary::getResultFileName(g_sFileName, QString("%1.txt").arg(tr("Result")));
        }

        DialogNFDScanProcess::saveResult(this, (ScanItemModel *)pModel, sSaveFileName);
    }
}

void NFDWidgetAdvanced::on_comboBoxType_currentIndexChanged(int nIndex)
{
    Q_UNUSED(nIndex)

    process();
}

void NFDWidgetAdvanced::process()
{
    XScanEngine::SCAN_RESULT scanResult = {};
    XScanEngine::SCAN_OPTIONS scanOptions = {};

    scanOptions.bShowType = true;
    scanOptions.bShowVersion = true;
    scanOptions.bShowInfo = true;
    scanOptions.bShowInternalDetects = true;
    scanOptions.fileType = (XBinary::FT)(ui->comboBoxType->currentData().toInt());
    scanOptions.nBufferSize = getGlobalOptions()->getValue(XOptions::ID_SCAN_BUFFERSIZE).toULongLong();
    scanOptions.bIsHighlight = getGlobalOptions()->getValue(XOptions::ID_SCAN_HIGHLIGHT).toBool();
    scanOptions.bIsSort = getGlobalOptions()->getValue(XOptions::ID_SCAN_SORT).toBool();

    quint64 nFlags = ui->comboBoxFlags->getValue().toULongLong();
    XScanEngine::setScanFlags(&scanOptions, nFlags);

    DialogNFDScanProcess dialogStaticScanProcess(this);
    dialogStaticScanProcess.setGlobal(getShortcuts(), getGlobalOptions());
    if (g_pDevice) {
        dialogStaticScanProcess.setData(g_pDevice, &scanOptions, &scanResult);
    } else {
        dialogStaticScanProcess.setData(g_sFileName, &scanOptions, &scanResult);
    }

    dialogStaticScanProcess.showDialogDelay();

    QAbstractItemModel *pOldTreeModel = ui->treeViewScan->model();

    ScanItemModel *pModel = new ScanItemModel(&scanOptions, &(scanResult.listRecords), 1);
    ui->treeViewScan->setModel(pModel);
    ui->treeViewScan->expandAll();

    deleteOldAbstractModel(&pOldTreeModel);

    qint32 nNumberOfHeurs = scanResult.listDebugRecords.count();

    QAbstractItemModel *pOldTableModel = ui->tableViewHeur->model();

    QStandardItemModel *pHeurModel = new QStandardItemModel(nNumberOfHeurs, 3, this);

    pHeurModel->setHeaderData(0, Qt::Horizontal, tr("Type"));
    pHeurModel->setHeaderData(1, Qt::Horizontal, tr("Name"));
    pHeurModel->setHeaderData(2, Qt::Horizontal, tr("Value"));

    for (qint32 i = 0; i < nNumberOfHeurs; i++) {
        QStandardItem *pItemHeurType = new QStandardItem;
        pItemHeurType->setText(scanResult.listDebugRecords.at(i).sType);
        pHeurModel->setItem(i, 0, pItemHeurType);

        QStandardItem *pItemName = new QStandardItem;
        pItemName->setText(scanResult.listDebugRecords.at(i).sName);
        pHeurModel->setItem(i, 1, pItemName);

        QStandardItem *pItemValue = new QStandardItem;
        pItemValue->setText(scanResult.listDebugRecords.at(i).sValue);
        pHeurModel->setItem(i, 2, pItemValue);
    }

    ui->tableViewHeur->setModel(pHeurModel);

    ui->tableViewHeur->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Interactive);
    ui->tableViewHeur->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Interactive);
    ui->tableViewHeur->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);

    deleteOldAbstractModel(&pOldTableModel);
}
