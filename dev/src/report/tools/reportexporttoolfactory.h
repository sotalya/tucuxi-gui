#ifndef REPORTEXPORTTOOLFACTORY_H
#define REPORTEXPORTTOOLFACTORY_H

class Report;
class ReportExportTool;

#include <QHash>
#include <QStringList>
#include <functional>

class ReportExportToolFactory
{

public:
    ReportExportToolFactory();

    ReportExportTool *createFromExtension(const QString &extension);
    ReportExportTool *createFromFileType(const QString &fileType);
    ReportExportTool *create(const QString &filePath, const Report *report);

    QStringList supportedExtensions() const;
    QStringList supportedFilesTypes() const;

private:
    void registerExtension(const QString &id, std::function<ReportExportTool *(void)> function);
    void registerFileType(const QString &id, std::function<ReportExportTool *(void)> function);

    QHash<QString, std::function<ReportExportTool *(void)>> _extensionRegistery;
    QHash<QString, std::function<ReportExportTool *(void)>> _fileTypeRegistery;
};

#endif // REPORTEXPORTTOOLFACTORY_H
