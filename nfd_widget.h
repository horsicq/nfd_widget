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
#ifndef NFD_WIDGET_H
#define NFD_WIDGET_H

#include <QFutureWatcher>
#include <QWidget>
#include <QtConcurrent>

#include "dialognfdscandirectory.h"
#include "dialogtextinfo.h"
#include "scanitemmodel.h"
#include "xshortcutswidget.h"
#include "dialognfdwidgetadvanced.h"

namespace Ui {
class NFD_Widget;
}

class NFD_Widget : public XShortcutsWidget {
    Q_OBJECT

public:
    enum ST {
        ST_UNKNOWN = 0,
        ST_FILE
    };

    //    struct OPTIONS
    //    {
    //        bool bRecursiveScan;
    //        bool bDeepScan;
    //        bool bHeuristicScan;
    //        bool bAllTypesScan;
    //    };

    enum COLUMN {
        COLUMN_TYPE = 0,
        COLUMN_STRING,
        COLUMN_SIGNATURE,
        COLUMN_INFO
    };

    explicit NFD_Widget(QWidget *pParent = nullptr);
    ~NFD_Widget();

    void setData(const QString &sFileName, bool bScan = false, XBinary::FT fileType = XBinary::FT_UNKNOWN);
    void setGlobal(XShortcuts *pShortcuts, XOptions *pXOptions);
    virtual void adjustView();
    virtual void reloadData(bool bSaveSelection);

private slots:
    void clear();
    void process();
    void scan();
    void stop();
    void on_scanFinished();
    void on_pushButtonNfdExtraInformation_clicked();
    void enableControls(bool bState);
    void on_pushButtonNfdDirectoryScan_clicked();
    void on_pushButtonNfdInfo_clicked();
    void on_pushButtonNfdScanStart_clicked();
    void on_pushButtonNfdScanStop_clicked();
    void timerSlot();

protected:
    virtual void registerShortcuts(bool bState);

signals:
    void scanStarted();
    void scanFinished();
    void currentFileType(qint32 nFT);
    void showInfo();

private:
    Ui::NFD_Widget *ui;
    ST g_scanType;
    SpecAbstract g_pSpecAbstract;
    XScanEngine::SCAN_OPTIONS g_scanOptions;
    XScanEngine::SCAN_RESULT g_scanResult;
    QFutureWatcher<void> g_watcher;
    QString g_sFileName;
    XBinary::FT g_fileType;
    XBinary::PDSTRUCT g_pdStruct;
    QTimer *g_pTimer;
    bool g_bProcess;
};

#endif  // NFD_WIDGET_H
