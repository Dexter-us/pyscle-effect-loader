#include "PsycleAdapter.h"
#include "IPlugParameter.h"

PsycleAdapter::PsycleAdapter(const InstanceInfo& info)
: Plugin(info, MakeConfig(0, 1)) // 0 Parameters, 1 Preset/Program default
{
  // Allocate Stereo Bus Layouts
  SetChannelConnections(ERoute::kInput, 0, 2, true);
  SetChannelConnections(ERoute::kOutput, 0, 2, true);
}

PsycleAdapter::~PsycleAdapter()
{
  // Final resource extraction safety line
  if (psycleModule != nullptr) {
    FreeLibrary(psycleModule);
  }
}

void PsycleAdapter::LoadPsycleDll(const char* fullPath)
{
  // 1. Thread-safe: Suspend target processing flags briefly
  bool wasActive = IsActivated();
  if (wasActive) OnActivate(false);

  // 2. Clear old handles safely to avoid active process pointer collisions
  if (psycleModule != nullptr) {
    FreeLibrary(psycleModule);
    psycleModule = nullptr;
    remotePsycleProcess = nullptr;
  }

  // 3. Mount fresh target library executable trees
  if (fullPath && fullPath[0] != '\0') {
    strcpy_s(currentDllPath, MAX_PATH, fullPath);
    psycleModule = LoadLibraryA(fullPath);
    if (psycleModule != nullptr) {
      // Map straight to the native entry function block inside the target binary
      remotePsycleProcess = (PsycleProcessFunc)GetProcAddress(psycleModule, "ProcessAudio");
    }
  }

  // 4. Resume processing loops
  if (wasActive) OnActivate(true);
}

void PsycleAdapter::OnActivate(bool active)
{
  // Reload library dynamically if the host wakes up and a target path is cached
  if (active && psycleModule == nullptr && currentDllPath[0] != '\0') {
    LoadPsycleDll(currentDllPath);
  }
}

void PsycleAdapter::ProcessBlock(sample** inputs, sample** outputs, int nFrames)
{
  // iPlug2 splits buffer streams into double pointer channels
  float* leftIn   = inputs[0];
  float* rightIn  = inputs[1];
  
  float* leftOut  = outputs[0];
  float* rightOut = outputs[1];

  // Pass-through processing fallback step (prevents silence if no DLL is linked)
  memcpy(leftOut, leftIn, nFrames * sizeof(float));
  memcpy(rightOut, rightIn, nFrames * sizeof(float));

  // If the dynamic binding interface checks out, route audio streams straight through it
  if (remotePsycleProcess != nullptr) {
    remotePsycleProcess(leftOut, rightOut, nFrames);
  }
}

bool PsycleAdapter::SerializeState(IByteChunk& chunk) const
{
  // Save the full path string straight into the DAW project state chunk
  return chunk.PutStr(currentDllPath) > 0;
}

int PsycleAdapter::UnserializeState(const IByteChunk& chunk, int startPos)
{
  WString pathStr;
  int pos = chunk.GetStr(pathStr, startPos);
  
  if (pos > startPos) {
    sprintf_s(currentDllPath, MAX_PATH, "%s", pathStr.GetAnsi());
    LoadPsycleDll(currentDllPath);
  }
  return pos;
}
