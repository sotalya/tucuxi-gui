#ifndef DOMITEM_H
#define DOMITEM_H

#include "tag.h"

#include <QDomElement>

class DomItem
{

public:

    /** \brief Constructor.
     * @param node The node of the item.
     * @param parent The parent of the item.
     */
    DomItem(QDomElement node, DomItem *parent);

    /** \brief Destructor.
     * The destructor will destruct the children of the item.
     */
    virtual ~DomItem();

    //! Returns the class name of the parser that corresponds the item's node.
    /** This function must return the class name of the parser that corresponds to the node (element) of the item.
     * @return The parser class name.
     */
    virtual QByteArray parserType() const = 0;

    //! Returns the display name of the item.
    /** This function must return a display name that will be returned by the model for the Qt::DisplayRole.
     * By default, the function returns the translation of the element given by the declaration in drugtags.
     * @return The display name of the item.
     */
    virtual QString name() const;

    /** \brief Returns the item node.
     * @return The item node.
     */
    QDomElement node() const;

    /** \brief Returns the row of the item.
     * @return The item's row, or -1 if an error occured.
     */
    int row();

    /** \brief Returns the row of the given item's child.
     * @param child A pointer to one of the item children.
     * @return The corresponding child's row, or -1 if the child cannot be found.
     */
    int childRow(DomItem *child) const;

    /** \brief Returns the item's number of children.
     * @return The number of children.
     */
    int childCount();

    /** \brief Returns a pointer to the item's parent.
     * @return A pointer to the parent, or 0 if the item is top-level.
     */
    DomItem *parent() const;

    /** \brief Returns a pointer to a child of the item.
     * @param row The child's row.
     * @return A pointer to the child, or 0 if there is no child at the given row.
     */
    DomItem *child(int row);

    /** \brief Removes one of the item's children.
     * @param row The child's row.
     * @return Returns true if the child was successfully removed, otherwise returns false.
     */
    bool removeChild(int row);

    /** \brief Inserts a child before the given row.
     * @param childNode The child node.
     * @param row The given row.
     * @return Returns true if the child was successfully added, otherwise returns false.
     */
    bool insertChild(QDomElement childNode, int row);

    /** \brief Appends a child to the item.
     * @param childNode The child node.
     * @return Returns true if the child was successfully added, otherwise returns false.
     */
    bool appendChild(QDomElement childNode);

    /** \brief Prepends a child to the item.
     * @param childNode The child node.
     * @return Returns true if the child was successfully added, otherwise returns false.
     */
    bool prependChild(QDomElement childNode);

private:
    //Base implementation of the children initialization
    void initChildren();

    //Virtual implementation of the children initialization
    virtual void initChildrenImpl() = 0;

    //Item node
    QDomElement _node;

    //Parent item
    DomItem *_parent;

    //Children items
    QList<DomItem *> _children;

    //Children initializer
    bool _initialized;
};

#endif // DOMITEM_H
