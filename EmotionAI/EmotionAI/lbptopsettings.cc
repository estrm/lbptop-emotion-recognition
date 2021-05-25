#include "lbptopsettings.h"

#include "global.h"

LbpTopSettings::LbpTopSettings(QWidget *parent) :
    QDialog(parent),
    ui_(new Ui::LbpTopSettings)
{
    ui_->setupUi(this);

    ui_->PxySpinBox->setRange(1, 16);
    ui_->PxtSpinBox->setRange(1, 16);
    ui_->PytSpinBox->setRange(1, 16);
    ui_->PxySpinBox->setValue(8);
    ui_->PxtSpinBox->setValue(8);
    ui_->PytSpinBox->setValue(8);

    ui_->PxySpinBox->setDisabled(true);
    ui_->PxtSpinBox->setDisabled(true);
    ui_->PytSpinBox->setDisabled(true);

    ui_->RxSpinBox->setRange(0, 50);
    ui_->RySpinBox->setRange(0, 50);
    ui_->RtSpinBox->setRange(0, 50);
    ui_->RxSpinBox->setValue(3);
    ui_->RySpinBox->setValue(3);
    ui_->RtSpinBox->setValue(3);

    connect(ui_->cancelButton, SIGNAL(clicked()), SLOT(close()));
    connect(ui_->okButton, SIGNAL(clicked()), SLOT(OkButtonPressed()));
}

LbpTopSettings::~LbpTopSettings()
{
    delete ui_;
}

void LbpTopSettings::OkButtonPressed()
{
    g_Pxy = ui_->PxySpinBox->value();
    g_Pxt = ui_->PxtSpinBox->value();
    g_Pyt = ui_->PytSpinBox->value();

    g_Rx = ui_->RxSpinBox->value();
    g_Ry = ui_->RySpinBox->value();
    g_Rt = ui_->RtSpinBox->value();

    this->close();
}