/*
 *  Copyright 2017 - 2018 Nicole Alassandro
 *
 *  medusa is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  medusa is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#pragma once

#include "../../JuceLibraryCode/JuceHeader.h"

#include "medusa_Commands.h"
#include "../Windows/medusa_DocumentWindow.h"

namespace medusa
{
    class Application;
}

class medusa::Application final : public juce::JUCEApplication
{
public:
    Application()  = default;
    ~Application() = default;

    static Application& getInstance();

    const juce::String getApplicationName() override;
    const juce::String getApplicationVersion() override;
    bool moreThanOneInstanceAllowed() override;

    void initialise(const juce::String& commandLineParameters) override;
    void shutdown() override;

    void systemRequestedQuit() override;

    void createMenu(juce::PopupMenu& menu, const juce::String& menuName);
    void createFileMenu(juce::PopupMenu& menu);
    void createViewMenu(juce::PopupMenu& menu);

    juce::ApplicationCommandTarget* getNextCommandTarget() override;
    void getAllCommands(juce::Array<juce::CommandID>& commands) override;
    void getCommandInfo(juce::CommandID commandID, juce::ApplicationCommandInfo& info) override;
    bool perform(const juce::ApplicationCommandTarget::InvocationInfo& info) override;

    void scanForPlugins();

    juce::ApplicationCommandManager commandManager;

    juce::OwnedArray<medusa::DocumentWindow> documentWindows;

    std::unique_ptr<juce::XmlElement> pluginListCache;
    juce::KnownPluginList knownPluginList;

private:
    struct MenuModel : public juce::MenuBarModel
    {
        MenuModel()
        {
            setApplicationCommandManagerToWatch(&getInstance().commandManager);
        }

        juce::StringArray getMenuBarNames() override
        {
            return {"File", "View"};
        }

        juce::PopupMenu getMenuForIndex(int, const juce::String& menuName) override
        {
            juce::PopupMenu menu;
            getInstance().createMenu(menu, menuName);
            return menu;
        }

        void menuItemSelected(int, int) override {}
    };

    MenuModel menuModel;
    juce::TooltipWindow tooltipWindow;
};
