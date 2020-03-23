#ifndef CLONEBUILDER_H
#define CLONEBUILDER_H

class ReportData;

#include "reportbuilder.h"

class CloneBuilder : public ReportBuilder
{
    Q_OBJECT

public:
    CloneBuilder(const ReportData *data, QObject *parent = 0);
    virtual Report *build();

private:
    const ReportData *_data;
};

#endif // CLONEBUILDER_H
