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

#include "medusa_DocumentPluginList.h"
#include "../Application/medusa_Application.h"
#include "../Windows/medusa_DocumentWindow.h"
#include "../Windows/medusa_PluginWindow.h"

medusa::DocumentPluginList::DocumentPluginList(
    juce::AudioProcessorGraph& graph,
    juce::ListBox& list) :
        parentGraph(graph),
        parentListBox(list)
{
    formatManager.addDefaultFormats();
}

int
medusa::DocumentPluginList::getNumRows()
{
    return parentGraph.getNumNodes() - 2;
}

void
medusa::DocumentPluginList::listBoxItemDoubleClicked(
    const int row,
    const juce::MouseEvent& e)
{
    medusa::Application& application = medusa::Application::getInstance();

    medusa::DocumentWindow* activeWindow = nullptr;
    for (auto* const window : application.documentWindows)
    {
        if (not window->isActiveWindow())
            continue;

        activeWindow = window;
        break;
    }
    jassert(activeWindow);

    juce::AudioProcessor* plugin = parentGraph.getNode(row + 2)->getProcessor();

    auto* const pluginWindow = new PluginWindow(plugin->createEditor());
    activeWindow->pluginWindows.add(pluginWindow);

    pluginWindow->addToDesktop();
    pluginWindow->setVisible(true);
}

void
medusa::DocumentPluginList::backgroundClicked(
    const juce::MouseEvent& e)
{
    parentListBox.deselectAllRows();

    if (e.mods.isRightButtonDown())
    {
        auto& application = medusa::Application::getInstance();

        if (application.pluginListCache)
        {
            application.knownPluginList.recreateFromXml(
                *application.pluginListCache
            );
        }
        else
        {
            application.scanForPlugins();
        }

        juce::PopupMenu m;
        const auto types = application.knownPluginList.getTypes();

        juce::KnownPluginList::addToMenu(m, types, juce::KnownPluginList::sortByManufacturer);
        const int pluginIndex = juce::KnownPluginList::getIndexChosenByMenu(types, m.show());

        if (pluginIndex >= 0 && pluginIndex < types.size())
        {
            const auto pluginDescription = types[pluginIndex];
            const auto xml = pluginDescription.createXml();
            std::cout << xml->toString() << std::endl;

            juce::String error;
            auto plugin = formatManager.createPluginInstance(
                pluginDescription, 44100, 512, error
            );

            if (plugin.get())
            {
                const auto newNode = parentGraph.addNode(
                    std::unique_ptr<juce::AudioProcessor>(
                        dynamic_cast<juce::AudioProcessor*>(plugin.release())
                    )
                );

                if (parentGraph.getNumNodes() > 3)
                {
                    const auto lastNode = parentGraph.getNode(
                        parentGraph.getNumNodes() - 2
                    );

                    if (lastNode)
                    {
                        juce::AudioProcessorGraph::Node* outputNode = parentGraph.getNode(1);

                        parentGraph.removeConnection({
                            {lastNode->nodeID, 0}, {outputNode->nodeID, 0}
                        });
                        parentGraph.addConnection({
                            {lastNode->nodeID, 0}, {newNode->nodeID, 0}
                        });
                        parentGraph.addConnection({
                            {newNode->nodeID, 0}, {outputNode->nodeID, 0}
                        });
                    }
                }
                else
                {
                    const auto inputNode  = parentGraph.getNode(0);
                    const auto outputNode = parentGraph.getNode(1);

                    parentGraph.addConnection({
                        {inputNode->nodeID, 0}, {newNode->nodeID, 0}
                    });
                    parentGraph.addConnection({
                        {newNode->nodeID, 0}, {outputNode->nodeID, 0}
                    });
                }

                parentListBox.updateContent();
            }
            else
            {
                juce::NativeMessageBox::showMessageBox(
                    juce::AlertWindow::WarningIcon,
                    "Error",
                    "Could not load plugin: " + error
                );
            }
        }
    }
}

void
medusa::DocumentPluginList::paintListBoxItem(
    const int rowNumber,
    juce::Graphics& g,
    const int width,
    const int height,
    const bool rowIsSelected)
{
    if (rowIsSelected)
    {
        g.fillAll(juce::Colours::purple);
        g.setColour(juce::Colours::white);
    }
    else
    {
        g.fillAll(juce::Colours::darkgrey);
        g.setColour(juce::Colours::black);
    }

    const auto& pluginName = parentGraph
        .getNode(rowNumber + 2)
       ->getProcessor()
       ->getName();

    g.drawFittedText(
        pluginName,
        0, 0,
        width, height,
        juce::Justification::centredLeft,
        1
    );
}
