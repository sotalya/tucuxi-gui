//@@license@@

#ifndef DRUGIDTRANSLATOR_H
#define DRUGIDTRANSLATOR_H

#include <QString>
#include <QMap>

class DrugIdTranslator
{
public:
    DrugIdTranslator();
    virtual ~DrugIdTranslator();

    virtual QString restToInternalId(QString restId) = 0;
};

class DummyDrugIdTranslator : public DrugIdTranslator
{
public:
    DummyDrugIdTranslator();

    virtual QString restToInternalId(QString restId);
};

class ChuvDrugIdTranslator : public DrugIdTranslator
{
public:
    ChuvDrugIdTranslator();

    virtual QString restToInternalId(QString restId);

private:
    QMap<QString,QString> map;
};

class ExternalDrugIdTranslator : public DrugIdTranslator
{
public:
    ExternalDrugIdTranslator();

    void setFileName(const QString &fileName);

    virtual QString restToInternalId(QString restId);

private:
    QString m_fileName;
};

#endif // DRUGIDTRANSLATOR_H
