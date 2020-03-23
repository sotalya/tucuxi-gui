#ifndef HISTORYITEM_H
#define HISTORYITEM_H

#include "inneritem.h"

class HistoryItem : public InnerItem
{
    friend class DomItemFactory;

public:

    /** \brief Returns the history parser class name.
     * @return The history parser class name.
     */
    virtual QByteArray parserType() const;

private:
    //Private constructor
    HistoryItem(QDomElement node, DomItem *parent);
};

#endif // HISTORYITEM_H
