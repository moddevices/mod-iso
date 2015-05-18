//	MiscPreferences.h - Preferences page for miscellaneous options.
//	----------------------------------------------------------------------------
//	This file is part of the Tracktion-themed Twindy window manager.
//	Copyright (c) 2005-2007 Niall Moody.
//
//	This program is free software; you can redistribute it and/or modify
//	it under the terms of the GNU General Public License as published by
//	the Free Software Foundation; either version 2 of the License, or
//	(at your option) any later version.
//
//	This program is distributed in the hope that it will be useful,
//	but WITHOUT ANY WARRANTY; without even the implied warranty of
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//	GNU General Public License for more details.
//
//	You should have received a copy of the GNU General Public License
//	along with this program; if not, write to the Free Software
//	Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//	----------------------------------------------------------------------------

#ifndef MISCPREFERENCES_H_
#define MISCPREFERENCES_H_

class TwindyWindow;

#include "../juce_amalgamated.h"

/// Preferences page for audio.
class AudioPreferences : public Component,
                         public ComboBoxListener/*,
                         public ListBoxModel,
                         public ButtonListener,
                         public TextEditorListener*/
{
public:
    // Constructor.
    AudioPreferences();

    // Destructor.
    ~AudioPreferences() override;

    // Rescan audio devices.
    void rescanDevices();

    // Places and sizes the various components.
    void resized() override;

    // Draws the component.
    void paint(Graphics& g) override;

    // Called when a ComboBox has its selected item changed.
    void comboBoxChanged(ComboBox* comboBoxThatHasChanged) override;

private:
    // Widgets.
    ComboBox deviceBox, sampleRateBox, bufferSizeBox;
    DrawableText title, subtitle1, subtitle2, labelAdvanced, labelBufSize, labelSampleRate;

    // Used for mixer
    pid_t mixerPid;
    TwindyWindow* mixerWindow;

    // StringArrays for device discovery.
    StringArray inputNames, outputNames, inputIds, outputIds;
};

#endif
