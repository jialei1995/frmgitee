/********************************************************************************
** Form generated from reading UI file 'smallwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.9.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SMALLWIDGET_H
#define UI_SMALLWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SmallWidget
{
public:
    QHBoxLayout *horizontalLayout;
    QSlider *horizontalSlider;
    QSpinBox *spinBox;

    void setupUi(QWidget *SmallWidget)
    {
        if (SmallWidget->objectName().isEmpty())
            SmallWidget->setObjectName(QStringLiteral("SmallWidget"));
        SmallWidget->resize(273, 48);
        horizontalLayout = new QHBoxLayout(SmallWidget);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalSlider = new QSlider(SmallWidget);
        horizontalSlider->setObjectName(QStringLiteral("horizontalSlider"));
        horizontalSlider->setOrientation(Qt::Horizontal);

        horizontalLayout->addWidget(horizontalSlider);

        spinBox = new QSpinBox(SmallWidget);
        spinBox->setObjectName(QStringLiteral("spinBox"));

        horizontalLayout->addWidget(spinBox);


        retranslateUi(SmallWidget);

        QMetaObject::connectSlotsByName(SmallWidget);
    } // setupUi

    void retranslateUi(QWidget *SmallWidget)
    {
        SmallWidget->setWindowTitle(QApplication::translate("SmallWidget", "Form", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class SmallWidget: public Ui_SmallWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SMALLWIDGET_H
