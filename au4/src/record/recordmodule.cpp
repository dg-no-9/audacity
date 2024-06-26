/*
* Audacity: A Digital Audio Editor
*/
#include "recordmodule.h"

#include <QQmlEngine>
#include <QtQml>

#include "modularity/ioc.h"

#include "ui/iuiactionsregister.h"

#include "internal/recordcontroller.h"
#include "internal/recorduiactions.h"

using namespace au::record;
using namespace muse;
using namespace muse::modularity;
using namespace muse::ui;
using namespace muse::actions;

static void record_init_qrc()
{
    Q_INIT_RESOURCE(record);
}

std::string RecordModule::moduleName() const
{
    return "record";
}

void RecordModule::registerExports()
{
    m_controller = std::make_shared<RecordController>();
    m_uiActions = std::make_shared<RecordUiActions>(m_controller);

    ioc()->registerExport<IRecordController>(moduleName(), m_controller);
}

void RecordModule::resolveImports()
{
    auto ar = ioc()->resolve<IUiActionsRegister>(moduleName());
    if (ar) {
        ar->reg(m_uiActions);
    }
}

void RecordModule::registerResources()
{
    record_init_qrc();
}

void RecordModule::onInit(const IApplication::RunMode& mode)
{
    if (mode == IApplication::RunMode::AudioPluginRegistration) {
        return;
    }

    m_controller->init();
}

void RecordModule::onDeinit()
{
    m_controller->deinit();
}
