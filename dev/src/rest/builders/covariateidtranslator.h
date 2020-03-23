#ifndef COVARIATEIDTRANSLATOR_H
#define COVARIATEIDTRANSLATOR_H


#include <QString>
#include <QMap>

class CovariateIdTranslator
{
public:
    CovariateIdTranslator();

    virtual QString restToInternalId(QString restId) = 0;
};

class DummyCovariateIdTranslator : public CovariateIdTranslator
{
public:
    DummyCovariateIdTranslator();

    virtual QString restToInternalId(QString restId);
};

class ChuvCovariateIdTranslator : public CovariateIdTranslator
{
public:
    ChuvCovariateIdTranslator();

    virtual QString restToInternalId(QString restId);

private:
    QMap<QString,QString> map;
};


#endif // COVARIATEIDTRANSLATOR_H
