#include "AddHashTableDialog.h"

#include "HashTableDefinition.h"
#include <QRadioButton>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QDialogButtonBox>
#include <QLabel>
#include <QLineEdit>
#include <QIntValidator>


AddHashTableDialog::AddHashTableDialog(QWidget *parent)
    : QDialog(parent)
    , _hashButtonList()
    , _rehashButtonList()
    , _sizeLineEdit(nullptr)
    , _sizeValidator(nullptr)
{
    setWindowTitle("Add hash table");

    QRadioButton *hashIdentityButton = new QRadioButton("Identity");
    QRadioButton *hashSquareButton = new QRadioButton("Square");
    QRadioButton *hashCubeButton = new QRadioButton("Cube");

    _hashButtonList.push_back(hashIdentityButton);
    _hashButtonList.push_back(hashSquareButton);
    _hashButtonList.push_back(hashCubeButton);

    QVBoxLayout *hashLayout = new QVBoxLayout;
    hashLayout->addWidget(hashIdentityButton);
    hashLayout->addWidget(hashSquareButton);
    hashLayout->addWidget(hashCubeButton);

    QGroupBox *hashGroupBox = new QGroupBox("HASH FUNCTIONS");
    hashGroupBox->setLayout(hashLayout);

    QRadioButton *rehashIdentityButton = new QRadioButton("Identity");
    QRadioButton *rehashSquareButton = new QRadioButton("Square");
    QRadioButton *rehashCubeButton = new QRadioButton("Cube");

    _rehashButtonList.push_back(rehashIdentityButton);
    _rehashButtonList.push_back(rehashSquareButton);
    _rehashButtonList.push_back(rehashCubeButton);

    QVBoxLayout *rehashLayout = new QVBoxLayout;
    hashLayout->addWidget(rehashIdentityButton);
    hashLayout->addWidget(rehashSquareButton);
    hashLayout->addWidget(rehashCubeButton);

    QGroupBox *rehashGroupBox = new QGroupBox("REHASH FUNCTIONS");
    rehashGroupBox->setLayout(rehashLayout);

    QLabel *sizeLabel = new QLabel("Size: ");
    QLineEdit *_sizeLineEdit = new QLineEdit;
    _sizeValidator = new QIntValidator(1, 103, this);
    _sizeLineEdit->setValidator(_sizeValidator);
    connect(_sizeLineEdit, SIGNAL(editingFinished()), this, SLOT(updateValidator()));
    QHBoxLayout *sizeLayout = new QHBoxLayout;
    sizeLayout->addWidget(sizeLabel);
    sizeLayout->addWidget(_sizeLineEdit);
    QWidget *sizeWidget = new QWidget;
    sizeWidget->setLayout(sizeLayout);

    QHBoxLayout *modelLayout = new QHBoxLayout;
    modelLayout->addWidget(hashGroupBox);
    modelLayout->addWidget(rehashGroupBox);
    modelLayout->addWidget(sizeWidget);

    QWidget *modelWidget = new QWidget;
    modelWidget->setLayout(modelLayout);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &AddHashTableDialog::verify);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &AddHashTableDialog::reject);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(modelWidget);
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);
}


void AddHashTableDialog::updateValidator()
{
    QString s = _sizeLineEdit->text();
    int i = 0;

    if (_sizeValidator->validate(s, i) == QValidator::Invalid)
    {
        _sizeLineEdit->clear();
    }
    else
    {
        _sizeLineEdit->setText(s);
    }
}


void AddHashTableDialog::verify()
{
    int position = 0;
    bool found = false;
    while (!found && position < _hashButtonList.size())
    {
        if (_hashButtonList.at(position)->isChecked())
        {
            found = true;
        }
        else
        {
            position++;
        }
    }

    HashFunction::Type hashType = HashFunction::Type::Identity;
    switch (position)
    {
    case 0:
        hashType = HashFunction::Type::Identity;
        break;
    case 1:
        hashType = HashFunction::Type::Square;
        break;
    case 2:
        hashType = HashFunction::Type::Cube;
        break;
    default:
        hashType = HashFunction::Type::Identity;
    }


    position = 0;
    found = false;
    while (!found && position < _rehashButtonList.size())
    {
        if (_rehashButtonList.at(position)->isChecked())
        {
            found = true;
        }
        else
        {
            position++;
        }
    }

    RehashFunction::Type rehashType = RehashFunction::Type::Identity;
    switch (position)
    {
    case 0:
        rehashType = RehashFunction::Type::Identity;
        break;
    case 1:
        rehashType = RehashFunction::Type::Square;
        break;
    case 2:
        rehashType = RehashFunction::Type::Cube;
        break;
    default:
        rehashType = RehashFunction::Type::Identity;
    }

    size_t size = static_cast<size_t>(_sizeLineEdit->text().toInt());

    HashTableDefinition definition { hash_table::Type::ARRAY, hashType, rehashType, size };
    emit addHashTable(definition);
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

