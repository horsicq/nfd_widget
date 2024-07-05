/* Copyright (c) 2018-2024 hors<horsicq@gmail.com>
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
#include "dialognfdscandirectory.h"

#include "ui_dialognfdscandirectory.h"

DialogNFDScanDirectory::DialogNFDScanDirectory(QWidget *pParent, const QString &sDirName) : XShortcutsDialog(pParent, true), ui(new Ui::DialogNFDScanDirectory)
{
    ui->setupUi(this);

    connect(this, SIGNAL(resultSignal(QString)), this, SLOT(appendResult(QString)));

    ui->checkBoxScanSubdirectories->setChecked(true);

    if (sDirName != "") {
        ui->lineEditDirectoryName->setText(sDirName);
    }
}

DialogNFDScanDirectory::~DialogNFDScanDirectory()
{
    delete ui;
}

void DialogNFDScanDirectory::on_pushButtonOpenDirectory_clicked()
{
    QString sInitDirectory = ui->lineEditDirectoryName->text();

    QString sDirectoryName = QFileDialog::getExistingDirectory(this, tr("Open directory") + QString("..."), sInitDirectory, QFileDialog::ShowDirsOnly);

    if (!sDirectoryName.isEmpty()) {
        ui->lineEditDirectoryName->setText(QDir().toNativeSeparators(sDirectoryName));
    }
}

void DialogNFDScanDirectory::on_pushButtonScan_clicked()
{
    QString sDirectoryName = ui->lineEditDirectoryName->text().trimmed();

    scanDirectory(sDirectoryName);

    getGlobalOptions()->setLastDirectory(sDirectoryName);
}

void DialogNFDScanDirectory::scanDirectory(const QString &sDirectoryName)
{
    if (sDirectoryName != "") {
        ui->textBrowserResult->clear();

        SpecAbstract::SCAN_OPTIONS options = {};
        options.bIsRecursiveScan = ui->checkBoxRecursiveScan->isChecked();
        options.bIsDeepScan = ui->checkBoxDeepScan->isChecked();
        options.bIsHeuristicScan = ui->checkBoxHeuristicScan->isChecked();
        options.bIsVerbose = ui->checkBoxVerbose->isChecked();
        options.bSubdirectories = ui->checkBoxScanSubdirectories->isChecked();
        options.bAllTypesScan = ui->checkBoxAllTypesScan->isChecked();
        // TODO Filter options
        // |flags|x all|

        DialogNFDScanProcess ds(this);
        ds.setGlobal(getShortcuts(), getGlobalOptions());
        connect(&ds, SIGNAL(scanResult(SpecAbstract::SCAN_RESULT)), this, SLOT(scanResult(SpecAbstract::SCAN_RESULT)), Qt::DirectConnection);
        ds.setData(sDirectoryName, &options);
        ds.showDialogDelay();
    }
}

void DialogNFDScanDirectory::scanResult(SpecAbstract::SCAN_RESULT scanResult)
{
    QString sResult = QString("%1 %2 %3").arg(QDir().toNativeSeparators(scanResult.sFileName), QString::number(scanResult.nScanTime), tr("msec"));
    sResult += "\r\n";  // TODO Linux

    QList<XBinary::SCANSTRUCT> _listRecords = SpecAbstract::convert(&(scanResult.listRecords));

    ScanItemModel model(&_listRecords, 1, false);  // mb TODO colored output

    sResult += model.toString(XBinary::FORMATTYPE_TEXT).toUtf8().data();

    emit resultSignal(sResult);
}

void DialogNFDScanDirectory::appendResult(const QString &sResult)
{
    ui->textBrowserResult->append(sResult);
}

void DialogNFDScanDirectory::on_pushButtonOK_clicked()
{
    this->close();
}

void DialogNFDScanDirectory::on_pushButtonClear_clicked()
{
    ui->textBrowserResult->clear();
}

void DialogNFDScanDirectory::on_pushButtonSave_clicked()
{
    QString sFilter = QString("%1 (*.txt)").arg(tr("Text documents"));
    QString sSaveFileName = ui->lineEditDirectoryName->text() + QDir::separator() + "result";
    QString sFileName = QFileDialog::getSaveFileName(this, tr("Save result"), sSaveFileName, sFilter);

    if (!sFileName.isEmpty()) {
        QFile file;
        file.setFileName(sFileName);

        if (file.open(QIODevice::ReadWrite)) {
            QString sText = ui->textBrowserResult->toPlainText();
            file.write(sText.toUtf8().data());
            file.close();
        }
    }
}
