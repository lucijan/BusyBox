/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "PluginProcessor.h"

//==============================================================================
/**
 */
class BusyBoxAudioProcessorEditor : public juce::AudioProcessorEditor, private juce::Timer {
 public:
  BusyBoxAudioProcessorEditor(BusyBoxAudioProcessor&);
  ~BusyBoxAudioProcessorEditor() override;

  //==============================================================================
  void paint(juce::Graphics&) override;
  void resized() override;

 private:
  void timerCallback() override;
  // This reference is provided as a quick way for your editor to
  // access the processor object that created it.
  BusyBoxAudioProcessor& audioProcessor_;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BusyBoxAudioProcessorEditor)
};
