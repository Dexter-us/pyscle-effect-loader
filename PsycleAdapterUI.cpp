#include "PsycleAdapter.h"
#include "IGraphics.h"
#include "IControls.h"

// Mandatory factory entry point required by iPlug2 to spin up the UI window
IGraphics* CreateGraphics(PsycleAdapter* pPlug)
{
  // 1. Initialize the layout canvas to your exact window size: 450x220 at 60 FPS
  IGraphics* pGraphics = MakeGraphics(*pPlug, 450, 220, 60);
  
  // Attach standard text layout parameters
  pGraphics->AttachFont("Roboto-Bold", "Roboto-Bold.ttf");
  pGraphics->HandleMouseOver(true);
  
  // 2. Build the visual interface tree
  pGraphics->SetLayoutFcn([pPlug](IGraphics* pLayout) {
    
    // Fill background with your solid, vibrant custom Yellow (RGB: 255, 222, 0)
    pLayout->FillAll(IColor(255, 255, 222, 0));

    // Get the base canvas bounds and trim margins for padding
    IRECT mainArea = pLayout->GetBounds().GetPadded(-20);

    // 3. Render Header: "Dexter U.S. Psycle Effect Loader"
    IRECT titleBounds = mainArea.SubRectVertical(4, 0); // Carve out top quarter
    IText titleStyle(20.f, IColor(255, 0, 0, 0), "Roboto-Bold", EAlign::Center);
    pLayout->AttachControl(new ITextControl(titleBounds, "Dexter U.S. Psycle Effect Loader", titleStyle));

    // 4. Draw a dark structural visual divider line under the header
    IRECT lineBounds = titleBounds.SubRectVertical(10, 9).MidPointY(); // Near the bottom of title space
    pLayout->AttachControl(new IVPanelControl(lineBounds, IColor(50, 0, 0, 0))); 

    // 5. Create the "Load Psycle Effect (.dll)" Action Button
    // Centers a 260x40 pixel button right in the middle space
    IRECT buttonBounds = pLayout->GetBounds().GetCentredInside(260, 40).Offset(0, 25);
    
    pLayout->AttachControl(new IVTextButton(buttonBounds, [pPlug](IControl* pCaller) {
      
      // Triggers native Windows File Explorer to search for target binaries
      WString fileStr;
      pCaller->GetUI()->PromptForFile(fileStr, EFileAction::Open, "", "dll");
      
      if (fileStr.GetLength() > 0) {
         pPlug->LoadPsycleDll(fileStr.GetAnsi());
      }
    }, 
    "Load Psycle Effect (.dll)", 
    // Stylistic configuration for high-contrast dark visual face
    IShapeData::kRectangle, 
    IColor(255, 28, 28, 31),    // Button Background (Charcoal Dark)
    IColor(255, 255, 255, 255)  // Label Text Color (White)
    ));
    
  });

  return pGraphics;
}
