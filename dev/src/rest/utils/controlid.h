//@@license@@

#ifndef CONTROLID_H
#define CONTROLID_H

#include <QObject>

class ControlId : public QObject
{
    Q_OBJECT

public:
    static ControlId &getInstance();
    qint64 next();

private:
    ControlId();
    virtual ~ControlId() Q_DECL_OVERRIDE;

    qint64 currentId;
};

#endif // CONTROLID_H
