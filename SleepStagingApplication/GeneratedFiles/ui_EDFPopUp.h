/********************************************************************************
** Form generated from reading UI file 'EDFPopUp.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EDFPOPUP_H
#define UI_EDFPOPUP_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_EDFPopUp
{
public:

    void setupUi(QWidget *EDFPopUp)
    {
        if (EDFPopUp->objectName().isEmpty())
            EDFPopUp->setObjectName(QStringLiteral("EDFPopUp"));
        EDFPopUp->resize(400, 300);

        retranslateUi(EDFPopUp);

        QMetaObject::connectSlotsByName(EDFPopUp);
    } // setupUi

    void retranslateUi(QWidget *EDFPopUp)
    {
        EDFPopUp->setWindowTitle(QApplication::translate("EDFPopUp", "EDFPopUp", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class EDFPopUp: public Ui_EDFPopUp {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EDFPOPUP_H
