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

#include "medusa_DocumentPluginList.h"
#include "medusa_ImageViewport.h"

namespace medusa
{

class DocumentWindow;

class DocumentComponent : public juce::Component,
                          public juce::ApplicationCommandTarget
{
public:
    DocumentComponent(medusa::DocumentWindow&);

    juce::ApplicationCommandTarget* getNextCommandTarget() override;
    void getAllCommands(juce::Array<juce::CommandID>&) override;
    void getCommandInfo(juce::CommandID, juce::ApplicationCommandInfo&) override;
    bool perform(const InvocationInfo&) override;

    ImageViewport imageViewport;

    DocumentPluginList pluginListModel;
    juce::ListBox      pluginList;

private:
    void resized() override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DocumentComponent)
};

}
