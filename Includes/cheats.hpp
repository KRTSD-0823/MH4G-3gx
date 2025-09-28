#ifndef CHEATS_H
#define CHEATS_H

#include <CTRPluginFramework.hpp>
#include "Helpers.hpp"
#include "Unicode.h"

namespace CTRPluginFramework
{
    using StringVector = std::vector<std::string>;

    void SetItemAmount(MenuEntry *entry);

    void SetMAXHP (MenuEntry *entry);

    void SetGreenHP(MenuEntry *entry);

    void SetRedHP(MenuEntry *entry);

    void FixHP(MenuEntry *entry);
}

#endif
