//	TwindyUpperPanel.cpp - Component to hold windows in the upper panel.
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

#include "juce_amalgamated.h"
#include "TwindyErrorDisplay.h"
#include "TwindyUpperPanel.h"
#include "TwindyRootWindow.h"
#include "TracktionScheme.h"

#include <iostream>
#include <unistd.h>

using namespace std;

//------------------------------------------------------------------------------
TwindyUpperPanel::TwindyUpperPanel(bool isMOD_)
    : TwindyTabbedComponent(T("Upper Panel"), isMOD_ ? TwindyTabbedComponent::ModeMOD : TwindyTabbedComponent::ModeDev),
      workspaceVisible(false),
      dontShowHideWindows(false),
      visibleWindow(-1),
      isMOD(isMOD_) {}

//------------------------------------------------------------------------------
TwindyUpperPanel::~TwindyUpperPanel()
{
	int i;
	//The contents of upperWindows get deleted when it's deleted, so we don't
	//have to explicitly delete them here.
	for(i=0;i<windows.size();i++)
	{
		windows[i]->closeWindow();
		Thread::yield();
	}
}

//------------------------------------------------------------------------------
void TwindyUpperPanel::setWorkspaceIsVisible(bool val)
{
    workspaceVisible = val;

    if (windows.size() == 0)
        return;

    if (workspaceVisible)
    {
        if (isMOD)
        {
            visibleWindow = windows.size()-1;
            setCurrentlySelectedTab(visibleWindow);
            for (int i=0; i<=visibleWindow; ++i)
                windows[i]->show();
        }
        else
        {
            visibleWindow = 0;
            setCurrentlySelectedTab(0);
            windows[visibleWindow]->show();
        }
    }
    else
    {
        for (int i=windows.size(); --i>=0;)
            windows[i]->hide();

        // Hide visible window.
        //windows[visibleWindow]->hide();
    }
}

//------------------------------------------------------------------------------
bool TwindyUpperPanel::containsWindow(::Window window)
{
    for (int i=windows.size();--i>=0;)
        if (windows[i]->getWindow() == window)
            return true;
    return false;
}

//------------------------------------------------------------------------------
void TwindyUpperPanel::addWindow(TwindyWindow *newWindow, TwindyUpperTab *newTab)
{
	addTab(getNumTabs(), newTab);
	windows.add(newWindow);
	visibleWindow = windows.size()-1;
	setCurrentlySelectedTab(getNumTabs()-1);
	if(!workspaceVisible)
		newWindow->hide();
}

//------------------------------------------------------------------------------
void TwindyUpperPanel::removeWindow(Window window)
{
    for (int i=windows.size();--i>=0;)
    {
        if (windows[i]->getWindow() == window)
        {
            if ((windows.size()-1) > 0)
            {
                visibleWindow--;
                if (visibleWindow == -1)
                    visibleWindow = 0;
            }
            else
            {
                visibleWindow = -1;
            }

            removeTab(i);
            windows.remove(i);

            if (windows.size() > 0)
            {
                windows[visibleWindow]->show();
                setCurrentlySelectedTab(visibleWindow);
            }
            break;
        }
    }
}

//------------------------------------------------------------------------------
void TwindyUpperPanel::removeIndexedWindow(int index)
{
	windows.remove(index, false);
}

//------------------------------------------------------------------------------
void TwindyUpperPanel::removeWindowNoDelete(TwindyWindow *window)
{
	int i;

	for(i=0;i<windows.size();++i)
	{
		if(windows[i] == window)
		{
			if((windows.size()-1) > 0)
			{
				visibleWindow--;
				if(visibleWindow == -1)
					visibleWindow = 0;
			}
			else
				visibleWindow = -1;
			removeTab(i);
			windows.remove(i, false);

			if(windows.size() > 0)
			{
				windows[visibleWindow]->show();
				setCurrentlySelectedTab(visibleWindow);
			}
		}
	}
}

//------------------------------------------------------------------------------
TwindyWindow* TwindyUpperPanel::getCurrentWindow() const
{
    if (workspaceVisible && (visibleWindow > -1))
        return windows[visibleWindow];

    return nullptr;
}

//------------------------------------------------------------------------------
TwindyWindow* TwindyUpperPanel::getWindow(int index) const
{
    if (index < windows.size())
        return windows[index];

    return nullptr;
}

//------------------------------------------------------------------------------
void TwindyUpperPanel::setName(const String& newName)
{
    name = newName;
}

//------------------------------------------------------------------------------
void TwindyUpperPanel::tabWasClicked(int newSelectedIndex,
                                     bool clickedTabWasAlreadySelected,
                                     const ModifierKeys &currentModifiers)
{
	if(!clickedTabWasAlreadySelected)
	{
		if((visibleWindow != newSelectedIndex) && !dontShowHideWindows)
		{
			windows[visibleWindow]->hide();
			visibleWindow = newSelectedIndex;
			windows[newSelectedIndex]->show();
		}
		else if(dontShowHideWindows)
			dontShowHideWindows = false;
	}
}

//------------------------------------------------------------------------------
void TwindyUpperPanel::mouseMove(const MouseEvent& e)
{
	getParentComponent()->mouseMove(e);
}

//------------------------------------------------------------------------------
void TwindyUpperPanel::closeTab(TwindyUpperTab *tab)
{
	int i;

	for(i=0;i<getNumTabs();i++)
	{
		if(getTabComponent(i) == tab)
		{
			closeWindow(tab, i);
			break;
		}
	}
}

//------------------------------------------------------------------------------
void TwindyUpperPanel::closeWindow(TwindyUpperTab *tab, int index)
{
	bool reselectTab = false;

	//I think there's probably some redundant code here...

	if((windows.size()-1) > 0)
	{
		if(visibleWindow != 0)
		{
			visibleWindow--;
			windows[visibleWindow]->show();
			setCurrentlySelectedTab(visibleWindow);
		}
		else
		{
			windows[visibleWindow+1]->show();
			setCurrentlySelectedTab(visibleWindow+1);
			reselectTab = true;
		}
	}
	else
		visibleWindow = -1;
	windows[index]->closeWindow();
	removeTab(static_cast<TwindyTabbedComponentBasicTab *>(tab));
	windows.remove(index);

	if((windows.size()-1) > 0)
		windows[getCurrentlySelectedTab()]->show();

	if(reselectTab)
	{
		dontShowHideWindows = true;
		setCurrentlySelectedTab(visibleWindow);
	}
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
TwindyUpperTab::TwindyUpperTab(const String& text, Colour textCol, Colour fillCol)
    : TwindyTabbedComponentBasicTab(text),
      textColor(textCol),
      fillColor(fillCol)
{
    addAndMakeVisible(closeButton = new ShapeButton(T("closeButton"), textCol, textCol, fillCol.darker()));

    Path x;
    x.addLineSegment(0.0f, 0.0f, 10.0f, 10.0f, 3.0f);
    x.addLineSegment(10.0f, 0.0f, 0.0f, 10.0f, 3.0f);
    closeButton->setShape(x, false, true, true);
    closeButton->addButtonListener(this);
}

//------------------------------------------------------------------------------
TwindyUpperTab::~TwindyUpperTab()
{
    deleteAllChildren();
}

//------------------------------------------------------------------------------
void TwindyUpperTab::paintTab(Graphics &g, bool, const TwindyTabbedComponent*)
{
    g.setFont(jmin(20.0f, getHeight() * 0.8f));
    g.setColour(isMouseOver() ? Colour::contrasting(textColor, fillColor) : textColor);

    g.drawText(getName(), 4, 2, getWidth() - 8, getHeight() - 4, Justification::centred, true);
}

//------------------------------------------------------------------------------
void TwindyUpperTab::resized()
{
    closeButton->setBounds(6, 6, 15, 15);
}

//------------------------------------------------------------------------------
void TwindyUpperTab::buttonClicked(Button* button)
{
    if (button != closeButton)
        return;

    if (Component* const parent1 = getParentComponent())
        if (TwindyUpperPanel* const parent2 = static_cast<TwindyUpperPanel*>(parent1->getParentComponent()))
            parent2->closeTab(this);
}
