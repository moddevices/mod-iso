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

#include "../juce_amalgamated.h"
#include <map>

class TwindyWindow;
class TwindyPreferences;

struct DeviceInfo {
    uint minChansOut;
    uint maxChansOut;
    uint minChansIn;
    uint maxChansIn;
    Array<uint> bufferSizes;
    Array<double> sampleRates;

    DeviceInfo()
        : minChansOut(0),
          maxChansOut(0),
          minChansIn(0),
          maxChansIn(0) {}
};

typedef std::map<String,DeviceInfo> DeviceInfoMapping;

/// Preferences page for audio.
class AudioPreferences : public Component,
                         private ButtonListener,
                         private ComboBoxListener
{
public:
    // Constructor.
    AudioPreferences(TwindyPreferences* const p);

    // Destructor.
    ~AudioPreferences() override;

    // Rescan audio devices.
    void rescanDevices();

    // Places and sizes the various components.
    void resized() override;

    // Draws the component.
    void paint(Graphics& g) override;

    // Called when the button is clicked.
    void buttonClicked(Button* button) override;

    // Called when a ComboBox has its selected item changed.
    void comboBoxChanged(ComboBox* comboBoxThatHasChanged) override;

private:
    // Parent
    TwindyPreferences* const prefs;

    // Array for cached info
    DeviceInfoMapping cachedDevInfo;

    // Widgets.
    TextButton applyButton;
    ComboBox deviceBox, sampleRateBox, bufferSizeBox;
    DrawableText title, subtitle1, subtitle2, labelAdvanced, labelBufSize, labelSampleRate;

    // StringArrays for device discovery.
    StringArray inputNames, outputNames, inputIds, outputIds;
};

#endif
