/* Copyright (c) 2017-2024 hors<horsicq@gmail.com>
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
#include "nfdoptionswidget.h"

#include "ui_nfdoptionswidget.h"

NFDOptionsWidget::NFDOptionsWidget(QWidget *pParent) : XShortcutsWidget(pParent), ui(new Ui::NFDOptionsWidget)
{
    ui->setupUi(this);

    g_pOptions = nullptr;

    setProperty("GROUPID", XOptions::GROUPID_SCAN);
}

NFDOptionsWidget::~NFDOptionsWidget()
{
    delete ui;
}

void NFDOptionsWidget::adjustView()
{
    // TODO
}

void NFDOptionsWidget::setOptions(XOptions *pOptions)
{
    g_pOptions = pOptions;

    reload();
}

void NFDOptionsWidget::save()
{
    g_pOptions->getCheckBox(ui->checkBoxDeepScan, XOptions::ID_SCAN_SCANAFTEROPEN);
    g_pOptions->getCheckBox(ui->checkBoxScanAfterOpen, XOptions::ID_SCAN_FLAG_RECURSIVE);
    g_pOptions->getCheckBox(ui->checkBoxRecursiveScan, XOptions::ID_SCAN_FLAG_DEEP);
    g_pOptions->getCheckBox(ui->checkBoxHeuristicScan, XOptions::ID_SCAN_FLAG_HEURISTIC);
    g_pOptions->getCheckBox(ui->checkBoxVerbose, XOptions::ID_SCAN_FLAG_VERBOSE);
    g_pOptions->getCheckBox(ui->checkBoxAllTypesScan, XOptions::ID_SCAN_FLAG_ALLTYPES);
    g_pOptions->getCheckBox(ui->checkBoxHighlight, XOptions::ID_SCAN_HIGHLIGHT);
    g_pOptions->getComboBox(ui->comboBoxBufferSize, XOptions::ID_SCAN_BUFFERSIZE);
}

void NFDOptionsWidget::setDefaultValues(XOptions *pOptions)
{
    pOptions->addID(XOptions::ID_SCAN_SCANAFTEROPEN, true);
    pOptions->addID(XOptions::ID_SCAN_FLAG_RECURSIVE, true);
    pOptions->addID(XOptions::ID_SCAN_FLAG_DEEP, true);
    pOptions->addID(XOptions::ID_SCAN_FLAG_HEURISTIC, false);
    pOptions->addID(XOptions::ID_SCAN_FLAG_VERBOSE, false);
    pOptions->addID(XOptions::ID_SCAN_FLAG_ALLTYPES, false);
    pOptions->addID(XOptions::ID_SCAN_HIGHLIGHT, true);
    pOptions->addID(XOptions::ID_SCAN_BUFFERSIZE, 2 * 1024 * 1024);
}

void NFDOptionsWidget::reload()
{
    g_pOptions->setCheckBox(ui->checkBoxScanAfterOpen, XOptions::ID_SCAN_SCANAFTEROPEN);
    g_pOptions->setCheckBox(ui->checkBoxRecursiveScan, XOptions::ID_SCAN_FLAG_RECURSIVE);
    g_pOptions->setCheckBox(ui->checkBoxDeepScan, XOptions::ID_SCAN_FLAG_DEEP);
    g_pOptions->setCheckBox(ui->checkBoxHeuristicScan, XOptions::ID_SCAN_FLAG_HEURISTIC);
    g_pOptions->setCheckBox(ui->checkBoxVerbose, XOptions::ID_SCAN_FLAG_VERBOSE);
    g_pOptions->setCheckBox(ui->checkBoxAllTypesScan, XOptions::ID_SCAN_FLAG_ALLTYPES);
    g_pOptions->setCheckBox(ui->checkBoxHighlight, XOptions::ID_SCAN_HIGHLIGHT);
    g_pOptions->setComboBox(ui->comboBoxBufferSize, XOptions::ID_SCAN_BUFFERSIZE);
}

void NFDOptionsWidget::registerShortcuts(bool bState)
{
    Q_UNUSED(bState)
}
