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

    this->g_pDevice = nullptr;
    this->g_fileType = XBinary::FT_UNKNOWN;

    ui->checkBoxDeepScan->setChecked(true);
    ui->checkBoxRecursiveScan->setChecked(true);
    ui->checkBoxHeuristicScan->setChecked(false);
    ui->checkBoxVerbose->setChecked(false);
    ui->checkBoxAllTypesScan->setChecked(false);
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

void NFDWidgetAdvanced::setData(const QString &sFileName, SpecAbstract::SCAN_OPTIONS options, bool bScan)
{
    this->g_sFileName = sFileName;
    this->g_fileType = options.fileType;

    ui->checkBoxRecursiveScan->setChecked(options.bIsRecursiveScan);
    ui->checkBoxDeepScan->setChecked(options.bIsDeepScan);
    ui->checkBoxHeuristicScan->setChecked(options.bIsHeuristicScan);
    ui->checkBoxVerbose->setChecked(options.bIsVerbose);
    ui->checkBoxAllTypesScan->setChecked(options.bAllTypesScan);

    XFormats::setFileTypeComboBox(options.fileType, sFileName, ui->comboBoxType, XBinary::TL_OPTION_ALL);

    if (bScan) {
        process();
    }
}

void NFDWidgetAdvanced::adjustView()
{
}

void NFDWidgetAdvanced::setGlobal(XShortcuts *pShortcuts, XOptions *pXOptions)
{
    ui->checkBoxAllTypesScan->setChecked(pXOptions->getValue(XOptions::ID_SCAN_ALLTYPES).toBool());
    ui->checkBoxDeepScan->setChecked(pXOptions->getValue(XOptions::ID_SCAN_DEEP).toBool());
    ui->checkBoxRecursiveScan->setChecked(pXOptions->getValue(XOptions::ID_SCAN_RECURSIVE).toBool());
    ui->checkBoxHeuristicScan->setChecked(pXOptions->getValue(XOptions::ID_SCAN_HEURISTIC).toBool());
    ui->checkBoxVerbose->setChecked(pXOptions->getValue(XOptions::ID_SCAN_VERBOSE).toBool());

    XShortcutsWidget::setGlobal(pShortcuts, pXOptions);
}

void NFDWidgetAdvanced::on_pushButtonScan_clicked()
{
    process();
}

void NFDWidgetAdvanced::registerShortcuts(bool bState)
{
    Q_UNUSED(bState)
}

void NFDWidgetAdvanced::on_pushButtonSave_clicked()
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
    SpecAbstract::SCAN_RESULT scanResult = {};
    SpecAbstract::SCAN_OPTIONS options = {};

    options.bIsRecursiveScan = ui->checkBoxRecursiveScan->isChecked();
    options.bIsDeepScan = ui->checkBoxDeepScan->isChecked();
    options.bIsHeuristicScan = ui->checkBoxHeuristicScan->isChecked();
    options.bIsVerbose = ui->checkBoxVerbose->isChecked();
    options.bAllTypesScan = ui->checkBoxAllTypesScan->isChecked();
    options.bShowDetects = true;
    options.fileType = (XBinary::FT)(ui->comboBoxType->currentData().toInt());

    DialogNFDScanProcess dialogStaticScanProcess(this);
    if (g_pDevice) {
        dialogStaticScanProcess.setData(g_pDevice, &options, &scanResult);
    } else {
        dialogStaticScanProcess.setData(g_sFileName, &options, &scanResult);
    }

    dialogStaticScanProcess.showDialogDelay();

    QAbstractItemModel *pOldTreeModel = ui->treeViewScan->model();

    QList<XBinary::SCANSTRUCT> _listRecords = SpecAbstract::convert(&(scanResult.listRecords));

    ScanItemModel *pModel = new ScanItemModel(&_listRecords, 1, getGlobalOptions()->getValue(XOptions::ID_SCAN_HIGHLIGHT).toBool());
    ui->treeViewScan->setModel(pModel);
    ui->treeViewScan->expandAll();

    deleteOldAbstractModel(&pOldTreeModel);

    qint32 nNumberOfHeurs = scanResult.listHeurs.count();

    QAbstractItemModel *pOldTableModel = ui->tableViewHeur->model();

    QStandardItemModel *pHeurModel = new QStandardItemModel(nNumberOfHeurs, 3, this);

    pHeurModel->setHeaderData(0, Qt::Horizontal, tr("Type"));
    pHeurModel->setHeaderData(1, Qt::Horizontal, tr("Name"));
    pHeurModel->setHeaderData(2, Qt::Horizontal, tr("Value"));

    for (qint32 i = 0; i < nNumberOfHeurs; i++) {
        QStandardItem *pItemHeurType = new QStandardItem;
        pItemHeurType->setText(SpecAbstract::heurTypeIdToString(scanResult.listHeurs.at(i).detectType));
        pHeurModel->setItem(i, 0, pItemHeurType);

        QStandardItem *pItemName = new QStandardItem;
        pItemName->setText(
            QString("%1(%2)[%3]")
                .arg(SpecAbstract::recordNameIdToString(scanResult.listHeurs.at(i).name), scanResult.listHeurs.at(i).sVersion, scanResult.listHeurs.at(i).sInfo));
        pHeurModel->setItem(i, 1, pItemName);

        QStandardItem *pItemValue = new QStandardItem;
        pItemValue->setText(scanResult.listHeurs.at(i).sValue);
        pHeurModel->setItem(i, 2, pItemValue);
    }

    ui->tableViewHeur->setModel(pHeurModel);

    ui->tableViewHeur->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Interactive);
    ui->tableViewHeur->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Interactive);
    ui->tableViewHeur->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);

    deleteOldAbstractModel(&pOldTableModel);
}
