//@@license@@

#ifndef LISTTESTWIDGET_H
#define LISTTESTWIDGET_H

class QDomDocument;
class QStandardItemModel;
class QItemSelectionModel;

#include "abstracttestwidget.h"

namespace Ui {
    class ListTestWidget;
}

class ListTestWidget : public AbstractTestWidget
{
    Q_OBJECT

public:
    explicit ListTestWidget(QWidget *parent = 0);
    virtual ~ListTestWidget() Q_DECL_OVERRIDE;

    virtual void enable() Q_DECL_OVERRIDE;
    virtual void disable() Q_DECL_OVERRIDE;
    virtual void reset() Q_DECL_OVERRIDE;

signals:
    void requestSelected(const QString &requestId);

protected:
    virtual QPushButton *pushButton();
    virtual QProgressBar *progressBar();
    virtual QPlainTextEdit *textArea();

private:
    Ui::ListTestWidget *_ui;
    QStandardItemModel *_model;
    QItemSelectionModel *_selector;

private slots:
    virtual void displayError(const QString &errorMsg) Q_DECL_OVERRIDE;
    virtual void displaySuccess() Q_DECL_OVERRIDE;

    void parseRequests(const QDomDocument &doc);
    void selectionChanged(const QModelIndex &current, const QModelIndex &previous);
};

#endif // LISTTESTWIDGET_H
