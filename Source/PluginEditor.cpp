/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginEditor.h"

#include "PluginProcessor.h"

//==============================================================================
BusyBoxAudioProcessorEditor::BusyBoxAudioProcessorEditor(BusyBoxAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor_(p) {
  // Make sure that before the constructor has finished, you've set the
  // editor's size to whatever you need it to be.
  setSize(400, 300);
  startTimer(500);
}

BusyBoxAudioProcessorEditor::~BusyBoxAudioProcessorEditor() {}

//==============================================================================
void BusyBoxAudioProcessorEditor::paint(juce::Graphics& g) {
  // (Our component is opaque, so we must completely fill the background with a solid colour)
  g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

  g.setColour(juce::Colours::white);
  g.setFont(15.0f);

  auto const load = audioProcessor_.getLoadMeasurer().getLoadAsPercentage();

  g.drawFittedText(juce::String(load, 1) + "%", getLocalBounds(), juce::Justification::centred, 1);
}

void BusyBoxAudioProcessorEditor::resized() {
  // This is generally where you'll want to lay out the positions of any
  // subcomponents in your editor..
}

void BusyBoxAudioProcessorEditor::timerCallback() { repaint(); }
