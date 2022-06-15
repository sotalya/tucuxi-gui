//@@license@@

#ifndef COVARIATEIDTRANSLATOR_H
#define COVARIATEIDTRANSLATOR_H


#include <QString>
#include <QMap>
#include <QSettings>

namespace Tucuxi {
namespace Gui {
namespace Rest {


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

class ExternalCovariateIdTranslator : public CovariateIdTranslator
{
public:
    ExternalCovariateIdTranslator();

    void setFileName(const QString &fileName);

    virtual QString restToInternalId(QString restId);

private:
    QString m_fileName;
};

}
}
}

#endif // COVARIATEIDTRANSLATOR_H
