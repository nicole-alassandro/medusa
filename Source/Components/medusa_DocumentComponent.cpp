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

#include "medusa_DocumentComponent.h"

#include "../Windows/medusa_DocumentWindow.h"

medusa::DocumentComponent::DocumentComponent(
    medusa::DocumentWindow& parent) :
        imageViewport(parent.document.image),
        pluginListModel(parent.imageProcessor, pluginList)
{
    addAndMakeVisible(imageViewport);

    const int width  = juce::jmax(imageViewport.getWidth(),  300);
    const int height = juce::jmax(imageViewport.getHeight(), 300);

    pluginList.setModel(&pluginListModel);
    pluginList.setSize(150, height);
    pluginList.setTopLeftPosition(width, 0);
    addAndMakeVisible(pluginList);

    setSize(imageViewport.getWidth() + 150, height);
}

juce::ApplicationCommandTarget*
medusa::DocumentComponent::getNextCommandTarget()
{
    return nullptr;
}

void
medusa::DocumentComponent::getAllCommands(
    juce::Array<juce::CommandID>&)
{

}

void
medusa::DocumentComponent::getCommandInfo(
    juce::CommandID,
    juce::ApplicationCommandInfo&)
{

}

bool
medusa::DocumentComponent::perform(
    const InvocationInfo&)
{
    return false;
}

void
medusa::DocumentComponent::resized()
{
    const int width  = getWidth();
    const int height = getHeight();

    imageViewport.setSize(width - 100, height);
    pluginList.setSize(150, height);
    pluginList.setTopLeftPosition(width - 100, 0);
}
