//@@license@@

#ifndef AUTOPROPERTY
#define AUTOPROPERTY

#include <QObject>
#include <QQmlListProperty>
#include "core/core.h"

#define QML_POINTERLIST_CLASS_DECL(NAME, TYPE) \
    class NAME : public ezechiel::GuiCore::Entity { \
        Q_OBJECT \
        ENTITY_UTILS(NAME) \
        typedef bool (*comparator_t)(const TYPE* , const TYPE*); \
        Q_PROPERTY(QList<QObject*> objlist READ getObjList NOTIFY objListChanged) \
    public: \
        NAME(ezechiel::GuiCore::AbstractRepository* repository, QObject* parent = nullptr); \
        Q_INVOKABLE void append(TYPE* target); \
        Q_INVOKABLE void insert(int index,TYPE* target);\
        Q_INVOKABLE void add(); \
        Q_INVOKABLE void update(); \
        Q_INVOKABLE void remove(const int); \
        Q_INVOKABLE void clear(); \
        Q_INVOKABLE void clearWithoutDeletion(); \
        Q_INVOKABLE void sort(NAME::comparator_t); \
        Q_INVOKABLE int size() const; \
        Q_INVOKABLE bool isEmpty() const; \
        Q_INVOKABLE bool isValid(); \
        Q_INVOKABLE TYPE *first(); \
        Q_INVOKABLE TYPE *last(); \
        Q_INVOKABLE QObject* objat(const int); \
        Q_INVOKABLE TYPE* at(const int); \
        Q_INVOKABLE QList<TYPE*>& getList(); \
        Q_INVOKABLE QList<QObject*>& getObjList(); \
    signals: \
        Q_SIGNAL void objListChanged (QList<QObject*>* objList); \
        Q_SIGNAL void TYPE ## Changed (TYPE& value); \
    private: \
        QList<TYPE*> _list; \
    };

// Be careful with isValis. It uses isEmpty to return false, but I don't know why it should
#define QML_POINTERLIST_CLASS_IMPL(NAME, TYPE) \
    NAME::NAME(ezechiel::GuiCore::AbstractRepository* repository, QObject* parent) : ezechiel::GuiCore::Entity(repository, parent) {} \
    void NAME::append(TYPE* target) { _list.append(target); setModified(true); emit TYPE ## Changed(*target); update();} \
    void NAME::insert(int index, TYPE* target) { _list.insert(index,target); setModified(true); update();} \
    void NAME::add() { _list.append(ezechiel::GuiCore::CoreFactory::createEntity<TYPE>(ABSTRACTREPO, this)); setModified(true); update();} \
    void NAME::update() { emit objListChanged(&getObjList());} \
    void NAME::remove(const int index) { \
        if (_list.at(index)->parent() == this) {\
            _list.at(index)->deleteLater();\
        } \
        _list.removeAt(index); setModified(true); \
        update(); \
    } \
    void NAME::clear() { \
        for (int i = 0; i < _list.size(); ++i) { \
            if (_list.at(i)->parent() == this) {\
                 _list.at(i)->deleteLater(); \
            } \
        } \
        _list.clear(); setModified(true); \
        update(); \
    } \
    void NAME::clearWithoutDeletion() { _list.clear(); setModified(true); update(); } \
    void NAME::sort(NAME::comparator_t compFunc) { \
       qSort(_list.begin(), _list.end(), compFunc); \
    } \
    int NAME::size() const { return _list.size();} \
    bool NAME::isEmpty() const { return _list.isEmpty(); } \
    bool NAME::isValid() { \
        if (isEmpty()) {return true;} \
        for (int i = 0; i < size(); ++i) { \
            if (!at(i)->isValid()) { return false; } \
           } \
        return true; \
    } \
    TYPE *NAME::first() { return _list.first(); } \
    TYPE *NAME::last() { return _list.last(); } \
    TYPE *NAME::at(const int i) { return _list.at(i); } \
    QObject *NAME::objat(const int i) { return _list.at(i); } \
    QList<TYPE*>& NAME::getList() { return _list; } \
    QList<QObject*>& NAME::getObjList() { return *reinterpret_cast<QList<QObject*>*>(&_list); }

#define POINTERLIST_CLASS_DECL(NAME, TYPE) \
    class NAME : public ezechiel::GuiCore::Entity { \
        Q_OBJECT \
        ENTITY_UTILS(NAME) \
    public: \
        NAME(ezechiel::GuiCore::AbstractRepository* repository, QObject* parent = nullptr); \
        Q_INVOKABLE TYPE *at(int i) const; \
        void append(TYPE* target); \
        Q_INVOKABLE int size() const; \
        bool isEmpty() const; \
        Q_INVOKABLE bool isValid(); \
        TYPE *&first(); \
        Q_INVOKABLE QList<TYPE*>& getList(); \
    private: \
        QList<TYPE*> _list; \
    };


// Be careful with isValis. It uses isEmpty to return false, but I don't know why it should
#define POINTERLIST_CLASS_IMPL(NAME, TYPE) \
    NAME::NAME(ezechiel::GuiCore::AbstractRepository* repository, QObject* parent) : ezechiel::GuiCore::Entity(repository, parent) {} \
    TYPE *NAME::at(int i) const { return _list.at(i);} \
    void NAME::append(TYPE* target) { _list.append(target); setModified(true);} \
    int NAME::size() const { return _list.size();} \
    bool NAME::isEmpty() const { return _list.isEmpty(); } \
    bool NAME::isValid() { \
    if (isEmpty()) {return true;} \
        for (int i = 0; i < size(); ++i) { \
            if (!at(i)->isValid()) { return false; } \
           } \
        return true; \
    } \
    TYPE *&NAME::first() { return _list.first(); } \
    QList<TYPE*>& NAME::getList() { return _list; }

#define VALUELIST_CLASS_DECL(NAME, TYPE) \
    class NAME : public ezechiel::GuiCore::Entity { \
        Q_OBJECT \
        ENTITY_UTILS(NAME) \
    public: \
        NAME(ezechiel::GuiCore::AbstractRepository* repository, QObject* parent = nullptr); \
        NAME(ezechiel::GuiCore::AbstractRepository* repository, QObject* parent, NAME* copyfrom); \
        Q_INVOKABLE TYPE at(const int i) const; \
        Q_INVOKABLE TYPE& first(); \
        Q_INVOKABLE TYPE& last(); \
        Q_INVOKABLE TYPE& operator[](const int i); \
        Q_INVOKABLE void append(TYPE target); \
        Q_INVOKABLE bool isValid(); \
        Q_INVOKABLE int size() const; \
        bool isEmpty() const; \
        Q_INVOKABLE QList<TYPE> getList() const; \
    signals: \
        Q_SIGNAL void TYPE ## ListChanged (QList<TYPE>& value); \
        Q_SIGNAL void TYPE ## Changed (TYPE& value); \
    private: \
        QList<TYPE> _list; \
    };

#define VALUELIST_CLASS_IMPL(NAME, TYPE) \
    NAME::NAME(ezechiel::GuiCore::AbstractRepository* repository, QObject* parent) : ezechiel::GuiCore::Entity(repository, parent) {} \
    NAME::NAME(ezechiel::GuiCore::AbstractRepository* repository, QObject* parent, NAME* copyfrom) : ezechiel::GuiCore::Entity(repository, parent) { _list = copyfrom->getList();} \
    TYPE NAME::at(const int i) const { return _list.at(i);} \
    TYPE& NAME::first() { return _list.first(); } \
    TYPE& NAME::last() { return _list.last(); } \
    TYPE& NAME::operator[](const int i) { return _list[i]; } \
    void NAME::append(TYPE target) { _list.append(target); setModified(true); emit TYPE ## Changed(target);} \
    bool NAME::isValid() { \
        if (!isEmpty()) { return true; } \
        return false; \
    } \
    int NAME::size() const { return _list.size();} \
    bool NAME::isEmpty() const { return _list.isEmpty(); } \
    QList<TYPE> NAME::getList() const { return _list; }


#define ENTITY_UTILS(entity) \
    friend Creator<entity>; \
public: \
    static Factory<entity> *getFactory() \
    { \
        static Factory<entity>* _factory = new Factory<entity>(); \
        return _factory; \
    } \
 \
    static entity *createEntity(ezechiel::GuiCore::AbstractRepository *repository = nullptr, QObject* parent = nullptr) \
    { \
        return getFactory()->createEntity(repository, parent); \
    }

// That could be interesting to see what happens if we delete the copy and assignment operators
// entity(const entity&) = delete; \
// entity(const entity&&) = delete; \
// entity& operator=(entity const&); \


#define ADMIN_ENTITY_UTILS(entity) \
    friend Creator<entity>; \
public: \
    static Factory<entity> *getFactory() \
    { \
        static Factory<entity>* _factory = new Factory<entity>(); \
        return _factory; \
    } \
 \
    static entity *createEntity(ezechiel::GuiCore::AbstractRepository *repository = nullptr, QObject* parent = nullptr) \
    { \
        return getFactory()->createEntity(repository, parent); \
    }


#define SUBENTITY_UTILS(entity, baseclass) \
public: \
    class entity ## Creator: public Creator<baseclass> \
    { \
    public: \
        virtual baseclass *createEntity(ezechiel::GuiCore::AbstractRepository *repository, QObject *parent = nullptr) \
        { \
            return new entity(repository, parent); \
        } \
    }; \
 \
    friend entity ## Creator;

#define AUTO_PROPERTY(TYPE, NAME) \
    Q_PROPERTY(TYPE NAME READ NAME WRITE NAME NOTIFY NAME ## Changed) \
    public: \
        TYPE NAME() const { return _ ## NAME ; } \
        void NAME (TYPE value) { \
            if (_ ## NAME == value) return; \
            _ ## NAME = value; \
            _isModified = true; \
            emit NAME ## Changed(value); \
        } \
    Q_SIGNAL void NAME ## Changed (TYPE value); \
    private: \
        TYPE _ ## NAME;

#define AUTO_PROPERTY2(TYPE, NAME, UPPERNAME) \
    Q_PROPERTY(TYPE NAME READ get ## UPPERNAME WRITE set ## UPPERNAME NOTIFY NAME ## Changed) \
    public: \
        TYPE get ## UPPERNAME() const { return _ ## NAME ; } \
        void set ## UPPERNAME (TYPE value) { \
            if (_ ## NAME == value) return; \
            _ ## NAME = value; \
            _isModified = true; \
            emit NAME ## Changed(value); \
        } \
    Q_SIGNAL void NAME ## Changed (TYPE value); \
    private: \
        TYPE _ ## NAME;

#define Q_AUTO_PROPERTY_IMPL(CLASS,TYPE, NAME, UPPERNAME) \
        TYPE CLASS::get ## UPPERNAME() const { return _ ## NAME ; } \
        void CLASS::set ## UPPERNAME (TYPE value) { \
            if (_ ## NAME == value) return; \
            TYPE tmp = _ ## NAME; \
            _ ## NAME = value; \
            _isModified = true; \
            emit NAME ## Changed(value); \
            if((tmp != nullptr) && (tmp->parent() == this)) { \
                tmp->deleteLater(); \
            }\
        }

#define AUTO_PROPERTY_IMPL(CLASS,TYPE, NAME, UPPERNAME) \
        TYPE CLASS::get ## UPPERNAME() const { return _ ## NAME ; } \
        void CLASS::set ## UPPERNAME (TYPE value) { \
            if (_ ## NAME == value) return; \
            _ ## NAME = value; \
            _isModified = true; \
            emit NAME ## Changed(value); \
        }

#define AUTO_PROPERTY_DECL(TYPE, NAME, UPPERNAME) \
    Q_PROPERTY(TYPE NAME READ get ## UPPERNAME WRITE set ## UPPERNAME NOTIFY NAME ## Changed) \
    public: \
        TYPE get ## UPPERNAME() const; \
        void set ## UPPERNAME (TYPE value); \
    Q_SIGNAL void NAME ## Changed (TYPE value); \
    private: \
        TYPE _ ## NAME;

#define AMOUNT_PROPERTY_IMPL(CLASS,TYPE, NAME, UPPERNAME) \
    TYPE& CLASS::get ## UPPERNAME() { return _ ## NAME ; } \
    const TYPE CLASS::get ## UPPERNAME() const { return _ ## NAME ; } \
        void CLASS::set ## UPPERNAME (TYPE value) { \
            if (_ ## NAME == value) return; \
            _ ## NAME = value; \
            _isModified = true; \
            emit NAME ## Changed(value); \
        }

#define AMOUNT_PROPERTY_DECL(TYPE, NAME, UPPERNAME) \
    Q_PROPERTY(TYPE NAME READ get ## UPPERNAME WRITE set ## UPPERNAME NOTIFY NAME ## Changed) \
    public: \
        TYPE& get ## UPPERNAME(); \
        const TYPE get ## UPPERNAME() const; \
        void set ## UPPERNAME (TYPE value); \
    Q_SIGNAL void NAME ## Changed (TYPE value); \
    private: \
        TYPE _ ## NAME;

#define ACCESSIBLE_VALUE_OBJECT_PROPERTY_IMPL(CLASS,TYPE, NAME, UPPERNAME) \
    TYPE& CLASS::get ## UPPERNAME() { return _ ## NAME ; } \
    const TYPE CLASS::get ## UPPERNAME() const { return _ ## NAME ; } \
        void CLASS::set ## UPPERNAME (TYPE value) { \
            if (_ ## NAME == value) return; \
            _ ## NAME = value; \
            _isModified = true; \
            emit NAME ## Changed(value); \
        }

#define ACCESSIBLE_VALUE_OBJECT_PROPERTY_DECL(TYPE, NAME, UPPERNAME) \
    Q_PROPERTY(TYPE NAME READ get ## UPPERNAME WRITE set ## UPPERNAME NOTIFY NAME ## Changed) \
    public: \
        TYPE& get ## UPPERNAME(); \
        const TYPE get ## UPPERNAME() const; \
        void set ## UPPERNAME (TYPE value); \
    Q_SIGNAL void NAME ## Changed (TYPE value); \
    private: \
        TYPE _ ## NAME;

#define DEFAULTLANGUAGE "en"

#define TRANSLATABLE_STRING_PROPERTY_IMPL(CLASS, NAME, UPPERNAME) \
    QString CLASS::get ## UPPERNAME(const QString &language) const { return _ ## NAME->value(language) ; } \
    QString CLASS::get ## UPPERNAME() const { return _ ## NAME->value(DEFAULTLANGUAGE) ; } \
    ezechiel::GuiCore::TranslatableString* CLASS::get ## UPPERNAME ## TranslationMap() const { return _ ## NAME ; } \
    void CLASS::set ## UPPERNAME (const QString &language, const QString &value) { \
        if (_ ## NAME->value(language) == value) return; \
        _ ## NAME->insert(language, value); \
        _isModified = true; \
        emit NAME ## Changed(); \
    } \
    void CLASS::set ## UPPERNAME (const QString &value) { \
        if (_ ## NAME->value() == value) return; \
        _ ## NAME->insert(value); \
        _isModified = true; \
        emit NAME ## Changed(); \
    } \
void CLASS::set ## UPPERNAME ## TranslationMap (ezechiel::GuiCore::TranslatableString *map) { \
    _ ## NAME = map; \
    _isModified = true; \
}

#define TRANSLATABLE_STRING_PROPERTY_DECL(NAME, UPPERNAME) \
    Q_PROPERTY(QString NAME READ get ## UPPERNAME WRITE set ## UPPERNAME NOTIFY NAME ## Changed) \
    public: \
    QString get ## UPPERNAME(const QString &language) const; \
    QString get ## UPPERNAME() const; \
    ezechiel::GuiCore::TranslatableString* get ## UPPERNAME ## TranslationMap() const; \
    void set ## UPPERNAME (const QString &language, const QString &value); \
    void set ## UPPERNAME (const QString &value); \
    void set ## UPPERNAME ## TranslationMap (ezechiel::GuiCore::TranslatableString *map); \
    Q_SIGNAL void NAME ## Changed(); \
    private: \
        ezechiel::GuiCore::TranslatableString *_ ## NAME;

//#define AUTO_QSHAREDPOINTER_PROPERTY(TYPE, NAME) \
//    Q_PROPERTY(TYPE NAME READ NAME WRITE NAME NOTIFY NAME ## Changed) \
//    public: \
//        TYPE NAME() const { return _ ## NAME .data(); } \
//        void NAME (TYPE value) { \
//            if (_ ## NAME == value) return; \
//            _ ## NAME = QSharedPointer<TYPE>(value); \
//            emit NAME ## Changed(value); \
//        } \
//    Q_SIGNAL void NAME ## Changed (TYPE value); \
//    private: \
//        TYPE _ ## NAME;

#define STD_PROPERTY_IMPL(CLASS,TYPE, NAME, UPPERNAME) \
        TYPE CLASS::get ## UPPERNAME() const { return _ ## NAME ; } \
        void CLASS::set ## UPPERNAME (TYPE value) { \
            if (_ ## NAME == value) return; \
            _ ## NAME = value; \
            emit NAME ## Changed(value); \
        }

#define STD_PROPERTY_DECL(TYPE, NAME, UPPERNAME) \
    Q_PROPERTY(TYPE NAME READ get ## UPPERNAME WRITE set ## UPPERNAME NOTIFY NAME ## Changed) \
    public: \
        TYPE get ## UPPERNAME() const; \
        void set ## UPPERNAME (TYPE value); \
    Q_SIGNAL void NAME ## Changed (TYPE value); \
    private: \
        TYPE _ ## NAME;


#endif // AUTOPROPERTY

