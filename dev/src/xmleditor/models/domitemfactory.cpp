#include "xmleditormessagehandler.h"
#include "core_errors.h"
#include "tag.h"
#include "domitemfactory.h"

#include "historyitem.h"
#include "creatoritem.h"
#include "revisionsitem.h"
#include "revisionitem.h"
#include "drugitem.h"
#include "dispositionsitem.h"
#include "dispositionitem.h"
#include "intakesitem.h"
#include "intakeitem.h"
#include "comethsitem.h"
#include "comethitem.h"
#include "headitem.h"
#include "dosagesitem.h"
#include "dosageitem.h"
#include "intervalsitem.h"
#include "targetsitem.h"
#include "targetitem.h"
#include "errormodelitem.h"
#include "parametersitem.h"
#include "parameteritem.h"
#include "correlationsitem.h"
#include "correlationitem.h"
#include "covariatesitem.h"
#include "covariateitem.h"
#include "operationsitem.h"
#include "operationitem.h"
#include "commentitem.h"

//Build a new dom item
DomItem *DomItemFactory::build(QDomElement node, DomItem *parent)
{
    if (node.tagName() == Tag::name(DrugTag::History))
        return new HistoryItem(node, parent);
    if (node.tagName() == Tag::name(DrugTag::Creator))
        return new CreatorItem(node, parent);
    if (node.tagName() == Tag::name(DrugTag::Revisions))
        return new RevisionsItem(node, parent);
    if (node.tagName() == Tag::name(DrugTag::Revision))
        return new RevisionItem(node, parent);
    if (node.tagName() == Tag::name(DrugTag::Drug))
        return new DrugItem(node, parent);
    if (node.tagName() == Tag::name(DrugTag::Dispositions))
        return new DispositionsItem(node, parent);
    if (node.tagName() == Tag::name(DrugTag::Disposition))
        return new DispositionItem(node, parent);
    if (node.tagName() == Tag::name(DrugTag::Intakes))
        return new IntakesItem(node, parent);
    if (node.tagName() == Tag::name(DrugTag::Intake))
        return new IntakeItem(node, parent);
    if (node.tagName() == Tag::name(DrugTag::Comeths))
        return new ComethsItem(node, parent);
    if (node.tagName() == Tag::name(DrugTag::Cometh))
        return new ComethItem(node, parent);
    if (node.tagName() == Tag::name(DrugTag::Head))
        return new HeadItem(node, parent);
    if (node.tagName() == Tag::name(DrugTag::Dosages))
        return new DosagesItem(node, parent);
    if (node.tagName() == Tag::name(DrugTag::Dosage))
        return new DosageItem(node, parent);
    if (node.tagName() == Tag::name(DrugTag::Intervals))
        return new IntervalsItem(node, parent);
    if (node.tagName() == Tag::name(DrugTag::Targets))
        return new TargetsItem(node, parent);
    if (node.tagName() == Tag::name(DrugTag::Target))
        return new TargetItem(node, parent);
    if (node.tagName() == Tag::name(DrugTag::ErrorModel))
        return new ErrorModelItem(node, parent);
    if (node.tagName() == Tag::name(DrugTag::Parameters))
        return new ParametersItem(node, parent);
    if (node.tagName() == Tag::name(DrugTag::Parameter))
        return new ParameterItem(node, parent);
    if (node.tagName() == Tag::name(DrugTag::Correlations))
        return new CorrelationsItem(node, parent);
    if (node.tagName() == Tag::name(DrugTag::Correlation))
        return new CorrelationItem(node, parent);
    if (node.tagName() == Tag::name(DrugTag::Covariates))
        return new CovariatesItem(node, parent);
    if (node.tagName() == Tag::name(DrugTag::Covariate))
        return new CovariateItem(node, parent);
    if (node.tagName() == Tag::name(DrugTag::Operations))
        return new OperationsItem(node, parent);
    if (node.tagName() == Tag::name(DrugTag::Operation))
        return new OperationItem(node, parent);
    if (node.tagName() == Tag::name(DrugTag::Comment))
        return new CommentItem(node, parent);

    //Error on unknown tag name
    ERROR(DATAERROR, tr("Unkown tag name '%1' encountered").arg(node.tagName())); return 0;
}
