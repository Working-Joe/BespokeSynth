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

    ModulatorCurve.cpp
    Created: 29 Nov 2017 8:56:48pm
    Author:  Ryan Challinor

  ==============================================================================
*/

#include "ModulatorCurve.h"
#include "Profiler.h"
#include "ModularSynth.h"
#include "PatchCableSource.h"
#include "MathUtils.h"

namespace
{
   const int kAdsrTime = 10000;
}

ModulatorCurve::ModulatorCurve()
: mInput(0)
, mEnvelopeControl(ofVec2f(3, 19), ofVec2f(100, 100))
, mInputSlider(nullptr)
{
   mEnvelopeControl.SetADSR(&mAdsr);
   mEnvelopeControl.SetViewLength(kAdsrTime);
   mEnvelopeControl.SetFixedLengthMode(true);
   mAdsr.SetNumStages(2);
   mAdsr.GetHasSustainStage() = false;
   mAdsr.GetStageData(0).target = 0;
   mAdsr.GetStageData(0).time = 0.01f;
   mAdsr.GetStageData(1).target = 1;
   mAdsr.GetStageData(1).time = kAdsrTime - .02f;
}

void ModulatorCurve::CreateUIControls()
{
   IDrawableModule::CreateUIControls();

   mInputSlider = new FloatSlider(this, "input", 3, 2, 100, 15, &mInput, 0, 1);

   mTargetCable = new PatchCableSource(this, kConnectionType_Modulator);
   mTargetCable->SetModulatorOwner(this);
   AddPatchCableSource(mTargetCable);
}

ModulatorCurve::~ModulatorCurve()
{
}

void ModulatorCurve::DrawModule()
{
   if (Minimized() || IsVisible() == false)
      return;

   mInputSlider->Draw();
   mEnvelopeControl.Draw();
}

void ModulatorCurve::PostRepatch(PatchCableSource* cableSource, bool fromUserClick)
{
   OnModulatorRepatch();

   if (mTarget)
      mInput = mTarget->GetValue();
}

float ModulatorCurve::Value(int samplesIn)
{
   ComputeSliders(samplesIn);
   mAdsr.Clear();
   mAdsr.Start(0, 1);
   mAdsr.Stop(kAdsrTime);
   float val = ofClamp(mAdsr.Value(mInput * kAdsrTime), 0, 1);
   if (val != val)
      val = 0;
   return ofLerp(GetMin(), GetMax(), val);
}

void ModulatorCurve::OnClicked(int x, int y, bool right)
{
   IDrawableModule::OnClicked(x, y, right);

   mEnvelopeControl.OnClicked(x, y, right);
}

void ModulatorCurve::MouseReleased()
{
   IDrawableModule::MouseReleased();

   mEnvelopeControl.MouseReleased();
}

bool ModulatorCurve::MouseMoved(float x, float y)
{
   IDrawableModule::MouseMoved(x, y);

   mEnvelopeControl.MouseMoved(x, y);

   return false;
}

void ModulatorCurve::SaveLayout(ofxJSONElement& moduleInfo)
{
   IDrawableModule::SaveLayout(moduleInfo);

   std::string targetPath = "";
   if (mTarget)
      targetPath = mTarget->Path();

   moduleInfo["target"] = targetPath;
}

void ModulatorCurve::LoadLayout(const ofxJSONElement& moduleInfo)
{
   mModuleSaveData.LoadString("target", moduleInfo);

   SetUpFromSaveData();
}

void ModulatorCurve::SetUpFromSaveData()
{
   mTargetCable->SetTarget(TheSynth->FindUIControl(mModuleSaveData.GetString("target")));
}

namespace
{
   const int kSaveStateRev = 1;
}

void ModulatorCurve::SaveState(FileStreamOut& out)
{
   IDrawableModule::SaveState(out);

   out << kSaveStateRev;

   mAdsr.SaveState(out);
}

void ModulatorCurve::LoadState(FileStreamIn& in)
{
   IDrawableModule::LoadState(in);

   int rev;
   in >> rev;
   LoadStateValidate(rev <= kSaveStateRev);

   mAdsr.LoadState(in);
}
