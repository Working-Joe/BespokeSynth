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
//  NoteDisplayer.h
//  Bespoke
//
//  Created by Ryan Challinor on 6/17/15.
//
//

#ifndef __Bespoke__NoteDisplayer__
#define __Bespoke__NoteDisplayer__

#include "IDrawableModule.h"
#include "NoteEffectBase.h"

class NoteDisplayer : public NoteEffectBase, public IDrawableModule
{
public:
   NoteDisplayer() = default;
   static IDrawableModule* Create() { return new NoteDisplayer(); }
   static bool AcceptsAudio() { return false; }
   static bool AcceptsNotes() { return true; }
   static bool AcceptsPulses() { return false; }

   //INoteReceiver
   void PlayNote(double time, int pitch, int velocity, int voiceIdx = -1, ModulationParameters modulation = ModulationParameters()) override;

   void LoadLayout(const ofxJSONElement& moduleInfo) override;
   void SetUpFromSaveData() override;

   bool IsResizable() const override { return true; }
   void Resize(float w, float h) override;

private:
   //IDrawableModule
   void DrawModule() override;
   void GetModuleDimensions(float& width, float& height) override
   {
      width = mWidth;
      height = mHeight;
   }
   bool Enabled() const override { return true; }

   void DrawNoteName(int pitch, float y) const;

   float mWidth{ 110 };
   float mHeight{ 60 };
   int mVelocities[127]{};
};

#endif /* defined(__Bespoke__NoteDisplayer__) */
