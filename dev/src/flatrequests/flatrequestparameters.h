#ifndef FLATREQUESTPARAMETERS_H
#define FLATREQUESTPARAMETERS_H

#include <QString>


class FlatRequestParameters
{
public:
    static FlatRequestParameters *getInstance();

    void setFrenchTag(bool enable);
    bool getIsFrenchTag();

    QString reportNameXml();
    QString fullDataNameXml();
    QString detailsListNameXml();
    QString siteNameXmls();
    QString drugNameXml();
    QString timeNameXml();
    QString unitNameXml();
    QString valueNameXml();
    QString dataNameXml();
    QString encounteridNameXml();
    QString detailsNameXml();


private:
    FlatRequestParameters();
    static FlatRequestParameters* instance;

    bool m_frenchTags;
};

#endif // FLATREQUESTPARAMETERS_H
