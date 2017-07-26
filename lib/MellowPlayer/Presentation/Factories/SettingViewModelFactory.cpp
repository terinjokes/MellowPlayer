#include "SettingViewModelFactory.hpp"
#include "MellowPlayer/Presentation/ViewModels/Settings/Types/BoolSettingViewModel.hpp"
#include "MellowPlayer/Presentation/ViewModels/Settings/Types/ColorSettingViewModel.hpp"
#include "MellowPlayer/Presentation/ViewModels/Settings/Types/EnumSettingViewModel.hpp"
#include "MellowPlayer/Presentation/ViewModels/Settings/Types/ShortcutSettingViewModel.hpp"
#include "MellowPlayer/Presentation/ViewModels/Settings/Types/StringSettingViewModel.hpp"
#include "MellowPlayer/Presentation/ViewModels/Settings/Types/ThemeSettingViewModel.hpp"
#include "MellowPlayer/Presentation/ViewModels/Settings/Types/TimeLimitSettingViewModel.hpp"

using namespace MellowPlayer::Presentation;
using namespace MellowPlayer::Application;

SettingViewModelFactory::SettingViewModelFactory(ThemeViewModel& themeViewModel) : themeViewModel(themeViewModel)
{
}

SettingViewModel* SettingViewModelFactory::create(Setting& setting, QObject* parent) const
{
    QString type = setting.getType().toLower();
    if (type == "bool")
        return new BoolSettingViewModel(setting, parent);
    else if (type == "color")
        return new ColorSettingViewModel(setting, parent);
    else if (type == "shortcut")
        return new ShortcutSettingViewModel(setting, parent);
    else if (type.startsWith("enum"))
        return new EnumSettingViewModel(setting, parent);
    else if (type.startsWith("time-limit"))
        return new TimeLimitSettingViewModel(setting, parent);
    else if (type.startsWith("theme"))
        return new ThemeSettingViewModel(setting, parent, themeViewModel);
    return new StringSettingViewModel(setting, parent);
}
