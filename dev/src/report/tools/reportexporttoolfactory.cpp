#include "reportexporttoolfactory.h"
#include "reportexporttool.h"
#include "reportpdfexporttool.h"

#define REGISTER_EXPORT_TOOL(CLASS)                                                                     \
    CLASS exportTool;                                                                                   \
    registerExtension(exportTool.extension(), [](void) -> ReportExportTool * { return new CLASS();});   \
    registerFileType(exportTool.fileType(), [](void) -> ReportExportTool * { return new CLASS();});

ReportExportToolFactory::ReportExportToolFactory()
{
    REGISTER_EXPORT_TOOL(ReportPdfExportTool)
}

ReportExportTool *ReportExportToolFactory::createFromExtension(const QString &extension)
{
    if (!_extensionRegistery.contains(extension))
        return 0;

    return _extensionRegistery.value(extension)();
}

ReportExportTool *ReportExportToolFactory::createFromFileType(const QString &fileType)
{
    if (!_fileTypeRegistery.contains(fileType))
        return 0;

    return _fileTypeRegistery.value(fileType)();
}

ReportExportTool *ReportExportToolFactory::create(const QString &filePath, const Report *report)
{
    ReportExportTool *exportTool = createFromExtension("." + filePath.section('.', -1));

    if (exportTool == 0)
        return 0;

    exportTool->setFilePath(filePath);
    exportTool->setReport(report);

    return exportTool;
}

QStringList ReportExportToolFactory::supportedExtensions() const
{
    QStringList extensions = _extensionRegistery.uniqueKeys();
    extensions.sort();

    return extensions;
}

QStringList ReportExportToolFactory::supportedFilesTypes() const
{
    QStringList types = _fileTypeRegistery.uniqueKeys();
    types.sort();

    return types;
}

void ReportExportToolFactory::registerExtension(const QString &id, std::function<ReportExportTool *(void)> function)
{
    _extensionRegistery.insert(id, function);
}

void ReportExportToolFactory::registerFileType(const QString &id, std::function<ReportExportTool *(void)> function)
{
    _fileTypeRegistery.insert(id, function);
}
