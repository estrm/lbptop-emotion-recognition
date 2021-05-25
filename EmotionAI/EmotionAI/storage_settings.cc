#include "storage_settings.h"

#include <QFileDialog>

#include "global.h"

StorageSettings::StorageSettings(QWidget *parent) :
    QDialog(parent),
    storage_ui_(new Ui::StorageSettings)
{
    storage_ui_->setupUi(this);

    storage_ui_->hourSpinBox->setRange(0, 23);
    storage_ui_->minSpinBox->setRange(0, 59);
    storage_ui_->secSpinBox->setRange(0, 59);
    storage_ui_->hourSpinBox->setValue(0);
    storage_ui_->minSpinBox->setValue(0);
    storage_ui_->secSpinBox->setValue(10);

    storage_ui_->label_savepath->setText(g_save_image_path.c_str());
    storage_ui_->label_snippath->setText(g_snip_image_path.c_str());

    connect(storage_ui_->pushButton_changeSavePath, SIGNAL(clicked()),
        this, SLOT(BrowerSavePathPressed()));
    connect(storage_ui_->pushButton_changeSnipPath, SIGNAL(clicked()),
        this, SLOT(BrowerSnipPathPressed()));
    connect(storage_ui_->cancelButton, SIGNAL(clicked()), SLOT(close()));
    connect(storage_ui_->okButton, SIGNAL(clicked()), SLOT(OkButtonPressed()));
}

StorageSettings::~StorageSettings()
{
    delete storage_ui_;
}

/*
@ÐÞ¸ÄÍ¼Ïñ´æ´¢Â·¾¶.
*/
void StorageSettings::BrowerSavePathPressed()
{
    QFileDialog *fd = new QFileDialog;
    fd->setFileMode(QFileDialog::Directory);
    fd->setOption(QFileDialog::ShowDirsOnly);
    int result = fd->exec();
    if (!result)
        return;
    QString directoryPath = fd->selectedFiles()[0];
    storage_ui_->label_savepath->setText(directoryPath);
    g_save_image_path = directoryPath.toLocal8Bit().toStdString();
}

/*
@ÐÞ¸Ä½ØÍ¼´æ´¢Â·¾¶.
*/
void StorageSettings::BrowerSnipPathPressed()
{
    QFileDialog *fd = new QFileDialog;
    fd->setFileMode(QFileDialog::Directory);
    fd->setOption(QFileDialog::ShowDirsOnly);
    int result = fd->exec();
    if (!result)
        return;
    QString directoryPath = fd->selectedFiles()[0];
    storage_ui_->label_snippath->setText(directoryPath);
    g_snip_image_path = directoryPath.toLocal8Bit().toStdString();
}

void StorageSettings::OkButtonPressed()
{
    int hour = storage_ui_->hourSpinBox->value();
    int min = storage_ui_->minSpinBox->value();
    int sec = storage_ui_->secSpinBox->value();
    g_save_image_time_interval = (3600 * hour + 60 * min + sec * 1) * 1000;
    this->close();
}