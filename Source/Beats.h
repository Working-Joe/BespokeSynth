/**
    bespoke synth, a software modular synthesizer
    Copyright (C) 2021 Ryan Challinor (contact: awwbees@gmail.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
**/
//
//  Beats.h
//  modularSynth
//
//  Created by Ryan Challinor on 2/2/14.
//
//

#ifndef __modularSynth__Beats__
#define __modularSynth__Beats__

#include <iostream>
#include "IAudioSource.h"
#include "EnvOscillator.h"
#include "IDrawableModule.h"
#include "Checkbox.h"
#include "Slider.h"
#include "DropdownList.h"
#include "Transport.h"
#include "ClickButton.h"
#include "RadioButton.h"
#include "OpenFrameworksPort.h"
#include "BiquadFilter.h"
#include "Ramp.h"
#include "ChannelBuffer.h"

class Beats;

#define BEAT_COLUMN_WIDTH 150

struct BeatData
{
   BeatData()
   : mBeat(nullptr)
   {}
   void LoadBeat(Sample* sample);
   void RecalcPos(double time, bool doubleTime, int numBars);

   Sample* mBeat;
};

class BeatColumn
{
public:
   BeatColumn(Beats* owner, int index);
   ~BeatColumn();
   void Draw(int x, int y);
   void CreateUIControls();
   void AddBeat(Sample* sample);
   void Process(double time, ChannelBuffer* buffer, int bufferSize);
   int GetNumSamples() { return (int)mSamples.size(); }
   void SaveState(FileStreamOut& out);
   void LoadState(FileStreamIn& in);

   void RadioButtonUpdated(RadioButton* list, int oldVal);

private:
   RadioButton* mSelector{ nullptr };
   int mSampleIndex{ -1 };
   float mVolume{ 0 };
   FloatSlider* mVolumeSlider{ nullptr };
   BeatData mBeatData;
   int mIndex;
   float mFilter{ 0 };
   FloatSlider* mFilterSlider{ nullptr };
   std::array<BiquadFilter, 2> mLowpass;
   std::array<BiquadFilter, 2> mHighpass;
   Beats* mOwner;
   Ramp mFilterRamp;
   bool mDoubleTime{ false };
   Checkbox* mDoubleTimeCheckbox{ nullptr };
   int mNumBars{ 4 };
   IntSlider* mNumBarsSlider{ nullptr };
   std::vector<Sample*> mSamples;
   float mPan{ 0 };
   FloatSlider* mPanSlider{ nullptr };
};

class Beats : public IAudioSource, public IDrawableModule, public IFloatSliderListener, public IIntSliderListener, public IDropdownListener, public ITimeListener, public IButtonListener, public IRadioButtonListener
{
public:
   Beats();
   virtual ~Beats();
   static IDrawableModule* Create() { return new Beats(); }


   void CreateUIControls() override;

   void Init() override;

   //IAudioSource
   void Process(double time) override;
   void SetEnabled(bool enabled) override { mEnabled = enabled; }

   //IDrawableModule
   void FilesDropped(std::vector<std::string> files, int x, int y) override;
   void SampleDropped(int x, int y, Sample* sample) override;
   bool CanDropSample() const override { return true; }

   void CheckboxUpdated(Checkbox* checkbox) override;
   void FloatSliderUpdated(FloatSlider* slider, float oldVal) override;
   void IntSliderUpdated(IntSlider* slider, int oldVal) override;
   void DropdownClicked(DropdownList* list) override;
   void DropdownUpdated(DropdownList* list, int oldVal) override;
   void ButtonClicked(ClickButton* button) override;
   void RadioButtonUpdated(RadioButton* list, int oldVal) override;

   //ITimeListener
   void OnTimeEvent(double time) override;

   void LoadLayout(const ofxJSONElement& moduleInfo) override;
   void SaveLayout(ofxJSONElement& moduleInfo) override;
   void SetUpFromSaveData() override;
   void SaveState(FileStreamOut& out) override;
   void LoadState(FileStreamIn& in) override;

private:
   //IDrawableModule
   void DrawModule() override;
   bool Enabled() const override { return mEnabled; }
   void GetModuleDimensions(float& width, float& height) override;

   ChannelBuffer mWriteBuffer;
   std::array<BeatColumn*, 4> mBeatColumns;
   int mRows;
};

#endif /* defined(__modularSynth__Beats__) */
