#include "RemoveHashTableDialog.h"

#include "HashTableModel.h"
#include <QRadioButton>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QDialogButtonBox>


RemoveHashTableDialog::RemoveHashTableDialog(const HashTableModelList &tableModelList, QWidget *parent)
    : QDialog(parent)
    , _hashTableRadioButtonList()
{
    setWindowTitle("Remove hash table");

    QVBoxLayout *modelLayout = new QVBoxLayout;
    for (const auto &tableModel : tableModelList)
    {
        QRadioButton *currentModelButton = new QRadioButton(tableModel->getHashTableDefinition());
        _hashTableRadioButtonList.push_back(currentModelButton);
        modelLayout->addWidget(currentModelButton);
    }

    QGroupBox *modelBox = new QGroupBox;
    modelBox->setLayout(modelLayout);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &RemoveHashTableDialog::verify);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &RemoveHashTableDialog::reject);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(modelBox);
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);
}


void RemoveHashTableDialog::verify()
{
    size_t selectedButtonPosition = 0;
    bool found = false;
    while (!found && selectedButtonPosition < _hashTableRadioButtonList.size())
    {
        if (_hashTableRadioButtonList.at(selectedButtonPosition)->isChecked())
        {
            found = true;
        }
        else
        {
            selectedButtonPosition++;
        }
    }

    emit removeHashTable(selectedButtonPosition);
    accept();
    return;


    /*
    QMessageBox::StandardButton answer;
    answer = QMessageBox::warning(this, tr("Incomplete Form"),
        tr("The form does not contain all the necessary information.\n"
           "Do you want to discard it?"),
        QMessageBox::Yes | QMessageBox::No);

    if (answer == QMessageBox::Yes)
        reject();
     */
}
