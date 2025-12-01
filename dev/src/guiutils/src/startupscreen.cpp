/* 
 * Tucuxi - Tucuxi-gui software. 
 * This software is able to perform prediction of drug concentration in blood 
 * and to propose dosage adaptations.
 * It has been developed by HEIG-VD, in close collaboration with CHUV. 
 * Copyright (C) 2024 HEIG-VD, maintained by Yann Thoma  <yann.thoma@heig-vd.ch>
 * 
 * This program is free software: you can redistribute it and/or modify 
 * it under the terms of the GNU Affero General Public License as 
 * published by the Free Software Foundation, either version 3 of the 
 * License, or any later version. 
 * 
 * This program is distributed in the hope that it will be useful, 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * GNU Affero General Public License for more details. 
 * 
 * You should have received a copy of the GNU Affero General Public License 
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */


#include "startupscreen.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QFile>
#include <QTextStream>

#ifdef COMPILE_WITH_TUCUCORE
#include "tucucore/version.h"
#endif // COMPILE_WITH_TUCUCORE


// The following lines are taken from here:
// https://stackoverflow.com/questions/11697820/how-to-use-date-and-time-predefined-macros-in-as-two-integers-then-stri

#define COMPUTE_BUILD_YEAR \
    ( \
        (__DATE__[ 7] - '0') * 1000 + \
        (__DATE__[ 8] - '0') *  100 + \
        (__DATE__[ 9] - '0') *   10 + \
        (__DATE__[10] - '0') \
    )


#define COMPUTE_BUILD_DAY \
    ( \
        ((__DATE__[4] >= '0') ? (__DATE__[4] - '0') * 10 : 0) + \
        (__DATE__[5] - '0') \
    )


#define BUILD_MONTH_IS_JAN (__DATE__[0] == 'J' && __DATE__[1] == 'a' && __DATE__[2] == 'n')
#define BUILD_MONTH_IS_FEB (__DATE__[0] == 'F')
#define BUILD_MONTH_IS_MAR (__DATE__[0] == 'M' && __DATE__[1] == 'a' && __DATE__[2] == 'r')
#define BUILD_MONTH_IS_APR (__DATE__[0] == 'A' && __DATE__[1] == 'p')
#define BUILD_MONTH_IS_MAY (__DATE__[0] == 'M' && __DATE__[1] == 'a' && __DATE__[2] == 'y')
#define BUILD_MONTH_IS_JUN (__DATE__[0] == 'J' && __DATE__[1] == 'u' && __DATE__[2] == 'n')
#define BUILD_MONTH_IS_JUL (__DATE__[0] == 'J' && __DATE__[1] == 'u' && __DATE__[2] == 'l')
#define BUILD_MONTH_IS_AUG (__DATE__[0] == 'A' && __DATE__[1] == 'u')
#define BUILD_MONTH_IS_SEP (__DATE__[0] == 'S')
#define BUILD_MONTH_IS_OCT (__DATE__[0] == 'O')
#define BUILD_MONTH_IS_NOV (__DATE__[0] == 'N')
#define BUILD_MONTH_IS_DEC (__DATE__[0] == 'D')


#define COMPUTE_BUILD_MONTH \
    ( \
        (BUILD_MONTH_IS_JAN) ?  1 : \
        (BUILD_MONTH_IS_FEB) ?  2 : \
        (BUILD_MONTH_IS_MAR) ?  3 : \
        (BUILD_MONTH_IS_APR) ?  4 : \
        (BUILD_MONTH_IS_MAY) ?  5 : \
        (BUILD_MONTH_IS_JUN) ?  6 : \
        (BUILD_MONTH_IS_JUL) ?  7 : \
        (BUILD_MONTH_IS_AUG) ?  8 : \
        (BUILD_MONTH_IS_SEP) ?  9 : \
        (BUILD_MONTH_IS_OCT) ? 10 : \
        (BUILD_MONTH_IS_NOV) ? 11 : \
        (BUILD_MONTH_IS_DEC) ? 12 : \
        /* error default */  99 \
    )

using namespace Tucuxi::Gui::GuiUtils;

StartupScreen::StartupScreen(QWidget *parent) :
    QDialog(parent)
{
#ifdef CONFIG_DEMO
    QFile mFile(":/text/demo_startup_text.txt");
#else
    QFile mFile(":/text/startup_text.txt");
#endif
    QString mText;

    mText ="<style>p{line-height: 20px;}\
            li{ \
            margin: 10px 0; \
          }</style>";

    if(!mFile.open(QFile::ReadOnly | QFile::Text)){
        mText = "Error with the startup screen";
    }
    else {
        QTextStream in(&mFile);
        mText += in.readAll();
        mFile.close();
    }

    QString date = QString("%1.%2.%3").arg(COMPUTE_BUILD_DAY).arg(COMPUTE_BUILD_MONTH).arg(COMPUTE_BUILD_YEAR);
    mText += QString("<p>This software was built on the %1 at %2 from : <br/>Gui Git revision %3 <br/>").arg(date).arg(__TIME__).arg(GIT_REVISION);
#ifdef COMPILE_WITH_TUCUCORE
    mText += QString("Tucucore Git revision %1").arg(QString::fromStdString(Tucuxi::Core::Version::getGitRevision()));
#endif // COMPILE_WITH_TUCUCORE
    mText += QString("</p>");

    auto *layout = new QVBoxLayout(this);
    text = new QTextEdit(mText, this);
    text->setReadOnly(true);

    auto *hLayout = new QHBoxLayout();
    layout->addWidget(text);
    layout->addWidget(new QLabel(tr("Do you agree with the above restrictions?"), this));
    layout->addLayout(hLayout);

    buttonYes = new QPushButton(tr("Yes"), this);
    buttonNo = new QPushButton(tr("No"), this);
    hLayout->addWidget(buttonYes);
    hLayout->addWidget(buttonNo);
    hLayout->addStretch();

    connect(buttonYes, SIGNAL(clicked()), this, SLOT(accept()));
    connect(buttonNo, SIGNAL(clicked()), this, SLOT(reject()));

    setMinimumHeight(600);
    setMinimumWidth(600);

    setLayout(layout);
}
