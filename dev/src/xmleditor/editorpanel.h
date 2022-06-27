#ifndef EDITORPANEL_H
#define EDITORPANEL_H

#include "ezpanel.h"
#include "druglister.h"
#include "domitemmodel.h"

#include <QWidget>
#include <QStandardItemModel>
#include <QItemSelection>

class EzPanel;

namespace Ui {
    class EditorPanel;
}

class EditorPanel : public EzPanel
{
    Q_OBJECT
    
public:

    //! Default constructor.
    /** The default constructor initialize the XML engine and the GUI elements of the application.
     * It implements the EzPanel interface, which means it can be used as any other panel in Tucuxi.
     * @param parent An optional parent widget.
     */
    explicit EditorPanel(QWidget *parent = 0);

    //! Default destructor.
    ~EditorPanel();

    /** \brief Returns the editor panel's menus.
     * @return The panel menus.
     */
    virtual QMenu* menus();

    /** \brief Returns the editor panel's icon.
     * @return The panel icon.
     */
    virtual QIcon icon();

    /** \brief Returns the editor panel's title.
     * @return The panel title.
     */
    virtual QString title();

    /** \brief Returns the editor panel's description.
     * @return The panel description.
     */
    virtual QString description();

private:
    //Sets up the usual GUI objects
    void createActions();
    void createMenus();
    void createToolbar();

    //Sets up the logic and models
    void initDrugList();
    void initDrugView();

    //Editor user interface
    Ui::EditorPanel *_ui;

    //Usual GUI objects
    QMenu *_sharedMenu;

    //Logic objects
    DrugLister _engine;

    //Model objects
    QStandardItemModel _drugModel;
    DomItemModel _domModel;

    //Selection objects
    QItemSelectionModel *_domSelection;

private slots:
    //Manages a drug selection change
    void drugSelected(const QModelIndex &index);

    //Manages a drug item selection change
    void itemSelected(const QModelIndex &index);
};

#endif // EDITORPANEL_H
