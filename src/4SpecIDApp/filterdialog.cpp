#include "filterdialog.h"

FilterDialog::FilterDialog(QStringList header, QList<QStringList> completions, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::FilterDialog)
{
    ui->setupUi(this);
    this->header = header;
    fs = ui->fsarea;
    fs->set_header(header);
    fs->set_completions(completions);
    ui->horizontalLayout->setAlignment(Qt::AlignHCenter);
    connect(ui->addBtn, &QPushButton::clicked,
            fs,[this](){fs->addWidget();});
    fs->addWidget();
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(onButtonboxAccepted()));
    connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(onButtonboxRejected()));
}


FilterDialog::~FilterDialog()
{
    delete ui;
}


void FilterDialog::onButtonboxRejected()
{
    close();
}

void FilterDialog::onButtonboxAccepted()
{
    ok = true;
    close();
}
