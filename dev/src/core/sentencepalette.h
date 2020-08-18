#ifndef SENTENCEPALETTE_H
#define SENTENCEPALETTE_H

#include <QObject>
#include "core/dal/entity.h"

namespace ezechiel {
namespace core {

class SentencesPalette : public ezechiel::core::Entity
{
    Q_OBJECT

    ADMIN_ENTITY_UTILS(SentencesPalette)

    AUTO_PROPERTY_DECL(QStringList, globalSentences, GlobalSentences)
    AUTO_PROPERTY_DECL(QStringList, drugSpecificSentences, DrugSpecificSentences)

    protected:
        Q_INVOKABLE SentencesPalette(ezechiel::core::AbstractRepository *repository, QObject *parent = nullptr);

public:

    Q_INVOKABLE void addSentenceToGlobal(QString _sentence);
    Q_INVOKABLE void addSentenceToSpecific(QString _sentence);
    Q_INVOKABLE void removeSentenceFromGlobal(int _listIndex);
    Q_INVOKABLE void removeSentenceFromSpecific(int _listIndex);

    void update(void);
};

QML_POINTERLIST_CLASS_DECL(SentencesPalettes, SentencesPalette)

} // namespace core
} // namespace ezechiel
Q_DECLARE_METATYPE(ezechiel::core::SentencesPalette*)

#endif // SENTENCEPALETTE_H
