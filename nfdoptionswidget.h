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
#ifndef NFDOPTIONSWIDGET_H
#define NFDOPTIONSWIDGET_H

#include "xshortcutswidget.h"

namespace Ui {
class NFDOptionsWidget;
}

// TOIDO remove, use ScanOptionsWidget
class NFDOptionsWidget : public XShortcutsWidget {
    Q_OBJECT

public:
    explicit NFDOptionsWidget(QWidget *pParent = nullptr);
    ~NFDOptionsWidget();

    virtual void adjustView();

    void setOptions(XOptions *pOptions);
    static void setDefaultValues(XOptions *pOptions);

public slots:
    void save();
    void reload();

protected:
    virtual void registerShortcuts(bool bState);

private:
    Ui::NFDOptionsWidget *ui;
    XOptions *g_pOptions;
};

#endif  // NFDOPTIONSWIDGET_H
