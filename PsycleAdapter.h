#pragma once

#include "IPlugPlugin.h"
#include <windows.h>

using namespace iplug;

// --- Function signature matching Psycle's ProcessAudio export ---
typedef void (*PsycleProcessFunc)(float* leftChannel, float* rightChannel, int sampleCount);

class PsycleAdapter : public Plugin
{
public:
  PsycleAdapter(const InstanceInfo& info);
  ~PsycleAdapter();

  // Audio Core Loop Execution
  void ProcessBlock(sample** inputs, sample** outputs, int nFrames) override;
  
  // State Initialization and Unloading
  void OnActivate(bool active) override;
  void LoadPsycleDll(const char* fullPath);

  // DAW Project Save/Load Serialization Loops
  bool SerializeState(IByteChunk& chunk) const override;
  int UnserializeState(const IByteChunk& chunk, int startPos) override;

  // Runtime Hooks and Module Instances
  PsycleProcessFunc remotePsycleProcess = nullptr;
  HINSTANCE psycleModule = nullptr;
  char currentDllPath[MAX_PATH] = { 0 };
};
