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
#include "nfdoptionswidget.h"

#include "ui_nfdoptionswidget.h"

NFDOptionsWidget::NFDOptionsWidget(QWidget *pParent) : XShortcutsWidget(pParent), ui(new Ui::NFDOptionsWidget)
{
    ui->setupUi(this);

    m_pOptions = nullptr;

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
    m_pOptions = pOptions;

    reload();
}

void NFDOptionsWidget::save()
{
    m_pOptions->setCheckBox(ui->checkBoxScanAfterOpen, XOptions::ID_SCAN_SCANAFTEROPEN);
    m_pOptions->setCheckBox(ui->checkBoxRecursiveScan, XOptions::ID_SCAN_FLAG_RECURSIVE);
    m_pOptions->setCheckBox(ui->checkBoxDeepScan, XOptions::ID_SCAN_FLAG_DEEP);
    m_pOptions->setCheckBox(ui->checkBoxHeuristicScan, XOptions::ID_SCAN_FLAG_HEURISTIC);
    m_pOptions->setCheckBox(ui->checkBoxVerbose, XOptions::ID_SCAN_FLAG_VERBOSE);
    m_pOptions->setCheckBox(ui->checkBoxAllTypesScan, XOptions::ID_SCAN_FLAG_ALLTYPES);
    m_pOptions->setCheckBox(ui->checkBoxFormatResult, XOptions::ID_SCAN_FORMATRESULT);
    m_pOptions->setCheckBox(ui->checkBoxHighlight, XOptions::ID_SCAN_HIGHLIGHT);
    m_pOptions->setComboBox(ui->comboBoxBufferSize, XOptions::ID_SCAN_BUFFERSIZE);
}

void NFDOptionsWidget::setDefaultValues(XOptions *pOptions)
{
    pOptions->addID(XOptions::ID_SCAN_SCANAFTEROPEN, true);
    pOptions->addID(XOptions::ID_SCAN_FLAG_RECURSIVE, true);
    pOptions->addID(XOptions::ID_SCAN_FLAG_DEEP, true);
    pOptions->addID(XOptions::ID_SCAN_FLAG_HEURISTIC, false);
    pOptions->addID(XOptions::ID_SCAN_FLAG_VERBOSE, false);
    pOptions->addID(XOptions::ID_SCAN_FLAG_ALLTYPES, false);
    pOptions->addID(XOptions::ID_SCAN_FORMATRESULT, true);
    pOptions->addID(XOptions::ID_SCAN_HIGHLIGHT, true);
    pOptions->addID(XOptions::ID_SCAN_SORT, true);
    pOptions->addID(XOptions::ID_SCAN_BUFFERSIZE, 2 * 1024 * 1024);
}

void NFDOptionsWidget::reload()
{
    m_pOptions->setCheckBox(ui->checkBoxScanAfterOpen, XOptions::ID_SCAN_SCANAFTEROPEN);
    m_pOptions->setCheckBox(ui->checkBoxRecursiveScan, XOptions::ID_SCAN_FLAG_RECURSIVE);
    m_pOptions->setCheckBox(ui->checkBoxDeepScan, XOptions::ID_SCAN_FLAG_DEEP);
    m_pOptions->setCheckBox(ui->checkBoxHeuristicScan, XOptions::ID_SCAN_FLAG_HEURISTIC);
    m_pOptions->setCheckBox(ui->checkBoxVerbose, XOptions::ID_SCAN_FLAG_VERBOSE);
    m_pOptions->setCheckBox(ui->checkBoxAllTypesScan, XOptions::ID_SCAN_FLAG_ALLTYPES);
    m_pOptions->setCheckBox(ui->checkBoxFormatResult, XOptions::ID_SCAN_FORMATRESULT);
    m_pOptions->setCheckBox(ui->checkBoxHighlight, XOptions::ID_SCAN_HIGHLIGHT);
    m_pOptions->setComboBox(ui->comboBoxBufferSize, XOptions::ID_SCAN_BUFFERSIZE);
}

void NFDOptionsWidget::registerShortcuts(bool bState)
{
    Q_UNUSED(bState)
}
