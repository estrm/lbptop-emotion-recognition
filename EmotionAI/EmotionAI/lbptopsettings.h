/*
@LBP-TOP参数设置(P, R)，P:邻域数量，R:搜索半径
*/
#ifndef LBPTOPSETTINGS_H
#define LBPTOPSETTINGS_H

#include <QDialog>
#include "ui_lbptopsettings.h"

namespace Ui {
    class LbpTopSettings;
}

class LbpTopSettings : public QDialog
{
    Q_OBJECT

public:
    explicit LbpTopSettings(QWidget *parent = 0);
    virtual ~LbpTopSettings();

private slots:
    void OkButtonPressed();

private:
    Ui::LbpTopSettings *ui_;
};

#endif // LBPTOPSETTINGS_H