#pragma once

#include "../module.h"
#include "../globals.h"
#include "../faust-module.h"
#include "../ui.h"

/**
 * Generates a square wave
 */
class TestSynth : public FaustSynthModule {
  ui::ModuleScreen<TestSynth> *screen;
public:
  float *frequency;

  TestSynth();
  ~TestSynth();
};

class TestSynthScreen : public ui::ModuleScreen<TestSynth> {
private:
  virtual void draw(NanoCanvas::Canvas& ctx) override;

  virtual bool keypress(ui::Key key, bool shift) override;

public:

  TestSynthScreen(TestSynth *module) : ui::ModuleScreen<TestSynth>(module) {}
};