#ifndef DOMITEMMODEL_H
#define DOMITEMMODEL_H

#include "domitemfactory.h"
#include "domitem.h"

#include <QAbstractItemModel>
#include <QDomDocument>
#include <QScopedPointer>

class DomItemModel : public QAbstractItemModel
{
    Q_OBJECT

public:

    //! Constructor.
    DomItemModel(QObject *parent = 0);

    /** Returns the item flags for the given item's index.
     * @param The index of the item.
     * @return The item flags.
     */
    Qt::ItemFlags flags(const QModelIndex &index) const;

    /** Returns the data stored under the given role for the item referred to by the index.
     * @param index The index of the item.
     * @param role The wanted role.
     * @return The corresponding item's data.
     */
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    //! Returns the data for the given role and section in the header with the specified orientation.
    /** The model returns the name of the first element of the QDomDocument as the horizontal header.
     * The vertical headers are always empty.
     * @param section The given section.
     * @param orientation The given orientation.
     * @param role The wanted role.
     * @return The corresponding header data.
     */
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    //! Returns the index of an item in the model.
    /** Returns the index of the item in the model specified by the given row, column and parent index.
     * @param row The given row.
     * @param column The given column.
     * @param parent The parent's index.
     * @return The index of the item.
     */
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;

    //! Returns the parent of the model item with the given index.
    /** If the item has no parent, an invalid QModelIndex is returned.
     * @param child The child's index.
     * @return The parent's index.
     */
    QModelIndex parent(const QModelIndex &child) const;

    //! Returns the number of rows under the given parent.
    /** When the parent is valid it means that rowCount is returning the number of children of parent.
     * @param parent The parent's index.
     * @return The number of rows/childs.
     */
    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    //! Returns the number of columns of the model.
    /** The model uses a fixed number of column.
     * @param parent Unused parent index.
     * @return The number of columns.
     */
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    //! Sets the model current QDomDocument.
    /** The function will reset the model, removing the previous document.
     * @param document A QDomDocument.
     */
    void setDocument(QDomDocument document);

    //! Clears the model.
    void clear();

private:
    //Model's current root item
    QScopedPointer<DomItem> _root;

    //Model's current QDomDocument
    QDomDocument _document;

    //Model's items factory
    DomItemFactory _factory;

    //Model's number of columns
    static const int _nbColumns;
};

#endif // DOMITEMMODEL_H
