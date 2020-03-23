#include "stackeddomitemwidget.h"
#include "domitemwidgetfactory.h"
#include "domitemwidget.h"
#include "domitem.h"
#include "emptyitemwidget.h"
#include "drugheaditemwidget.h"
#include "dispositionheaditemwidget.h"
#include "intakeheaditemwidget.h"
#include "comethheaditemwidget.h"
#include "dosageitemwidget.h"
#include "intervalsitemwidget.h"
#include "targetitemwidget.h"
#include "errormodelitemwidget.h"
#include "parameteritemwidget.h"
#include "correlationitemwidget.h"
#include "covariateitemwidget.h"
#include "operationitemwidget.h"
#include "commentitemwidget.h"

//Default constructor
StackedDomItemWidget::StackedDomItemWidget(QWidget *parent) : QStackedWidget(parent)
{
    //Register the item widgets
    DomItemWidgetFactory::registerClass<DrugHeadItemWidget>(DrugHeadParser::staticMetaObject.className());
    DomItemWidgetFactory::registerClass<DispositionHeadItemWidget>(DispositionHeadParser::staticMetaObject.className());
    DomItemWidgetFactory::registerClass<IntakeHeadItemWidget>(IntakeHeadParser::staticMetaObject.className());
    DomItemWidgetFactory::registerClass<ComethHeadItemWidget>(ComethHeadParser::staticMetaObject.className());
    DomItemWidgetFactory::registerClass<DosageItemWidget>(DosageParser::staticMetaObject.className());
    DomItemWidgetFactory::registerClass<IntervalsItemWidget>(IntervalsParser::staticMetaObject.className());
    DomItemWidgetFactory::registerClass<TargetItemWidget>(TargetParser::staticMetaObject.className());
    DomItemWidgetFactory::registerClass<ErrorModelItemWidget>(ErrorModelParser::staticMetaObject.className());
    DomItemWidgetFactory::registerClass<ParameterItemWidget>(ParameterParser::staticMetaObject.className());
    DomItemWidgetFactory::registerClass<CorrelationItemWidget>(CorrelationParser::staticMetaObject.className());
    DomItemWidgetFactory::registerClass<CovariateItemWidget>(CovariateParser::staticMetaObject.className());
    DomItemWidgetFactory::registerClass<OperationItemWidget>(OperationParser::staticMetaObject.className());
    DomItemWidgetFactory::registerClass<CommentItemWidget>(CommentParser::staticMetaObject.className());

    //Add an initial empty item widget
    addWidget(new EmptyItemWidget);
}

//Set the current item widget
void StackedDomItemWidget::setCurrentItemWidget(DomItem *item)
{
    //Get the widget corresponding to this item
    DomItemWidget *widget = _parserToWidget.value(item->parserType());

    //Create it and add it if it doesn't exist
    if (widget == 0) {
        widget = DomItemWidgetFactory::build(item->parserType());

        addWidget(widget);
        _parserToWidget.insert(item->parserType(), widget);
    }

    //Set the item node
    widget->setNode(item->node());

    //Set the current widget
    setCurrentWidget(widget);
}

//Sets the current as empty widget
void StackedDomItemWidget::setEmptyItemWidget()
{
    setCurrentIndex(0);
}
