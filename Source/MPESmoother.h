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
/*
  ==============================================================================

    MPESmoother.h
    Created: 4 Aug 2021 9:45:25pm
    Author:  Ryan Challinor

  ==============================================================================
*/

#pragma once

#include "NoteEffectBase.h"
#include "IDrawableModule.h"
#include "Slider.h"
#include "Checkbox.h"
#include "ModulationChain.h"
#include "Transport.h"

class MPESmoother : public NoteEffectBase, public IDrawableModule, public IFloatSliderListener, public IAudioPoller
{
public:
   MPESmoother();
   virtual ~MPESmoother();
   static IDrawableModule* Create() { return new MPESmoother(); }


   void CreateUIControls() override;
   void SetEnabled(bool enabled) override { mEnabled = enabled; }
   void Init() override;

   //INoteReceiver
   void PlayNote(double time, int pitch, int velocity, int voiceIdx = -1, ModulationParameters modulation = ModulationParameters()) override;

   void OnTransportAdvanced(float amount) override;

   void FloatSliderUpdated(FloatSlider* slider, float oldVal) override;
   void CheckboxUpdated(Checkbox* checkbox) override;

   void LoadLayout(const ofxJSONElement& moduleInfo) override;
   void SetUpFromSaveData() override;

private:
   //IDrawableModule
   void DrawModule() override;
   void GetModuleDimensions(float& width, float& height) override
   {
      width = mWidth;
      height = mHeight;
   }
   bool Enabled() const override { return mEnabled; }

   float mWidth{ 200 };
   float mHeight{ 20 };

   float mPitchSmooth{ .02 };
   FloatSlider* mPitchSmoothSlider{ nullptr };
   float mPressureSmooth{ .02 };
   FloatSlider* mPressureSmoothSlider{ nullptr };
   float mModWheelSmooth{ .02 };
   FloatSlider* mModWheelSmoothSlider{ nullptr };

   std::array<ModulationParameters, kNumVoices> mModulationInput;
   std::array<ModulationCollection, kNumVoices> mModulationOutput;
};
