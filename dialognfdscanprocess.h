/* Copyright (c) 2017-2025 hors<horsicq@gmail.com>
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
#ifndef DIALOGNFDSCANPROCESS_H
#define DIALOGNFDSCANPROCESS_H

#include <QDateTime>
#include <QDialog>
#include <QFileDialog>
#include <QStandardItemModel>
#include <QThread>

#include "scanitemmodel.h"
#include "specabstract.h"
#include "xdialogprocess.h"

class DialogNFDScanProcess : public XDialogProcess {
    Q_OBJECT

public:
    explicit DialogNFDScanProcess(QWidget *pParent = nullptr);
    ~DialogNFDScanProcess();

    void setData(const QString &sFileName, XScanEngine::SCAN_OPTIONS *pOptions, XScanEngine::SCAN_RESULT *pScanResult);
    void setData(QIODevice *pDevice, XScanEngine::SCAN_OPTIONS *pOptions, XScanEngine::SCAN_RESULT *pScanResult);
    void setData(const QString &sDirectoryName, XScanEngine::SCAN_OPTIONS *pOptions);

signals:
    void scanFileStarted(const QString &sFileName);
    void scanResult(const XScanEngine::SCAN_RESULT &scanResult);

private:
    SpecAbstract *g_pSpecAbstract;
    QThread *g_pThread;
};

#endif  // DIALOGNFDSCANPROCESS_H
