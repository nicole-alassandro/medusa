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

#include "medusa_PluginWindow.h"
#include "../Application/medusa_Application.h"

medusa::PluginWindow::PluginWindow(
    juce::AudioProcessorEditor* editor) :
        juce::DocumentWindow(
            editor->processor.getName(),
            juce::Colours::black,
            4
        )
{
    setUsingNativeTitleBar(true);
    setContentOwned(editor, true);
}

void
medusa::PluginWindow::closeButtonPressed()
{
    removeFromDesktop();
    delete this;
}
