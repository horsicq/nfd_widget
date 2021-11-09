// Copyright (c) 2020-2021 hors<horsicq@gmail.com>
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
#ifndef NFD_WIDGET_H
#define NFD_WIDGET_H

#include <QWidget>
#include <QFutureWatcher>
#include <QtConcurrent>
#include "staticscanitemmodel.h"
#include "dialogtextinfo.h"
#include "dialogstaticscandirectory.h"

namespace Ui {
class NFD_Widget;
}

class NFD_Widget : public QWidget
{
    Q_OBJECT

public:
    enum ST
    {
        ST_UNKNOWN=0,
        ST_FILE
    };

    struct OPTIONS
    {
        bool bRecursiveScan;
        bool bDeepScan;
        bool bHeuristicScan;
    };

    enum COLUMN
    {
        COLUMN_TYPE=0,
        COLUMN_STRING,
        COLUMN_SIGNATURE,
        COLUMN_INFO
    };

    explicit NFD_Widget(QWidget *pParent=nullptr);
    ~NFD_Widget();

    void setOptions(OPTIONS *pOptions);
    void setData(QString sFileName,bool bScan=false,XBinary::FT fileType=XBinary::FT_UNKNOWN);
    void setDatabase(QString sDatabasePath);

private slots:
    void on_pushButtonNfdScan_clicked();
    void clear();
    void process();
    void scan();
    void stop();
    void onScanFinished();
    void on_pushButtonNfdExtraInformation_clicked();
    void enableControls(bool bState);
    void on_pushButtonNfdDirectoryScan_clicked();

signals:
    void scanStarted();
    void scanFinished();

private:
    Ui::NFD_Widget *ui;
    ST scanType;
    StaticScan staticScan;
    SpecAbstract::SCAN_OPTIONS scanOptions;
    SpecAbstract::SCAN_RESULT scanResult;
    QFutureWatcher<void> watcher;
    QString sFileName;
    XBinary::FT fileType;
    bool bProcess;
};

#endif // NFD_WIDGET_H
