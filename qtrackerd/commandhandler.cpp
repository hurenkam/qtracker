#include <QVariant>
#include <QVariantList>
#include "commandhandler.h"

#define ENABLE_DEBUG
#include "helpers.h"

void CommandHandler::onCommand(QVariant cmd)
{
    ENTER("")

    QVariantMap map = cmd.toMap();
    if (!map.contains("method")) return;

    QString method = map["method"].toString();
    QVariantList args;
    if (map.contains("args"))
        args = map["args"].toList();

    if (methods.contains(method))
        onCommand(methods.indexOf(method),args);
    else
        LOG("unsupported method " << method);

    EXIT("")
}
