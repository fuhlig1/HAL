/*
 * CorrFitGUI.cxx
 *
 *  Created on: 11 sty 2021
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "CorrFitGUI.h"
#include "ChiSqMap2D.h"
#include "CorrFitFunc.h"
#include "Std.h"

#include <TCanvas.h>
#include <TColor.h>
#include <TF1.h>

#include <TSystem.h>
#include <TVirtualPad.h>

#include <utility>

namespace Hal {
  CorrFitGUI::CorrFitGUI(CorrFit* f) :
    TGMainFrame(gClient->GetRoot(), 350, 300 + 140), fFunc(f), fNormIndex(fFunc->GetParameterIndex("N")) {
    const Int_t width = 350;
    Int_t nparams     = fFunc->GetParametersNo();
    fInitalNorm       = fFunc->GetParameter(fNormIndex);
    fSliders          = new CorrFitParButton*[nparams];
    Int_t maxL        = 0;
    for (int i = 0; i < nparams; i++) {
      maxL = fFunc->GetParameterName(i).Length();
    }

    for (int i = 0; i < nparams; i++) {
      TString name = fFunc->GetParameterName(i);
      if (name.Length() < maxL) {
        for (int j = name.Length(); j < maxL; j++) {
          name = name + " ";
        }
      }
      fSliders[i] = new CorrFitParButton(this, 200, 40);
      fSliders[i]->Init(this, name, fFunc->GetParamConf(i));
    }

    TGHorizontalFrame* legendFrame = new TGHorizontalFrame(this, width, 40);
    TGLabel* lab1                  = new TGLabel(legendFrame, "Par. Name");
    TGLabel* lab2                  = new TGLabel(legendFrame, "Min");
    TGLabel* lab3                  = new TGLabel(legendFrame, "Max");
    TGLabel* lab4                  = new TGLabel(legendFrame, "Steps");
    legendFrame->AddFrame(lab1, new TGLayoutHints(kLHintsCenterX | kLHintsExpandX, 5, 5, 3, 4));
    legendFrame->AddFrame(lab2, new TGLayoutHints(kLHintsCenterX | kLHintsExpandX, 5, 5, 3, 4));
    legendFrame->AddFrame(lab3, new TGLayoutHints(kLHintsCenterX | kLHintsExpandX, 5, 5, 3, 4));
    legendFrame->AddFrame(lab4, new TGLayoutHints(kLHintsCenterX | kLHintsExpandX, 5, 5, 3, 4));

    AddFrame(legendFrame, new TGLayoutHints(kLHintsCenterX | kLHintsExpandX, 5, 5, 5, 5));


    fPairChi1 = new CorrFitChiSelector(this, width, 100);
    fPairChi2 = new CorrFitChiSelector(this, width, 100);
    fPairChi1->Init(this, fFunc);
    fPairChi2->Init(this, fFunc);

    TGHorizontalFrame* chiFrame = new TGHorizontalFrame(this, width, 40);
    fChiMin                     = new TGCheckButton(chiFrame, "Draw Chi min");
    fChiMin->SetOn(kFALSE);
    fChiLogz = new TGCheckButton(chiFrame, "Draw Chi logZ");
    fChiLogz->SetOn(kFALSE);
    fChiFit = new TGCheckButton(chiFrame, "Draw Fit par");
    fChiFit->SetOn(kTRUE);
    chiFrame->AddFrame(fChiMin, new TGLayoutHints(kLHintsTop | kLHintsRight, 5, 5, 5, 5));
    chiFrame->AddFrame(fChiLogz, new TGLayoutHints(kLHintsTop | kLHintsRight, 5, 5, 5, 5));
    chiFrame->AddFrame(fChiFit, new TGLayoutHints(kLHintsTop | kLHintsRight, 5, 5, 5, 5));
    AddFrame(chiFrame, new TGLayoutHints(kLHintsCenterX | kLHintsExpandX, 5, 5, 5, 5));

    TGHorizontalFrame* applyFrame = new TGHorizontalFrame(this, width, 40);
    TGTextButton* draw            = new TGTextButton(applyFrame, "&Draw chi");
    applyFrame->AddFrame(draw, new TGLayoutHints(kLHintsCenterX, 5, 5, 3, 4));
    draw->Connect("Clicked()", this->ClassName(), this, "DrawChi2()");
    TGTextButton* exit = new TGTextButton(applyFrame, "&Exit", "gApplication->Terminate(0)");
    applyFrame->AddFrame(exit, new TGLayoutHints(kLHintsCenterX, 5, 5, 3, 4));
    AddFrame(applyFrame, new TGLayoutHints(kLHintsBottom | kLHintsLeft, 5, 5, 5, 5));
    SetCleanup(kDeepCleanup);
    SetWindowName("CorrFit GUI");
    MapSubwindows();
    Resize(width + 1, nparams * 40 + 240);
    MapWindow();
  }

  void CorrFitGUI::ApplyParams() {
    for (int i = 0; i < fFunc->GetParametersNo(); i++) {
      fFunc->OverwriteParam(i, fSliders[i]->GetValue());
      fFunc->fTempParamsEval[i] = fSliders[i]->GetValue();
    }

    fFunc->ParametersChanged();
    fFunc->SetErrorsNegative();
    auto func = dynamic_cast<CorrFitFunc*>(fFunc);
    if (func) { func->Repaint(); }
  }

  CorrFitGUI::~CorrFitGUI() { Cleanup(); }

  CorrFitParButton::CorrFitParButton(const TGWindow* p, UInt_t w, UInt_t h, UInt_t options, Pixel_t back) :
    TGHorizontalFrame(p, w, h, options, back), fNumberEntry(nullptr), fComboBox(nullptr), fDiscrete(kFALSE) {}

  void CorrFitParButton::Init(CorrFitGUI* gui, TString parName, FitParam parConf) {
    fDiscrete = parConf.IsDiscrete();
    SetLayoutManager(new TGHorizontalLayout(this));

    TGHorizontalFrame* rightLabel = new TGHorizontalFrame(this);

    TGHorizontalFrame* leftLabel = new TGHorizontalFrame(this);

    TGLabel* label = new TGLabel(leftLabel, parName);
    if (parConf.IsFixed()) {
      label->SetTextColor(new TColor(255, 0, 0));
    } else {
      label->SetTextColor(new TColor(0, 0, 0));
    }
    label->SetMinWidth(100);
    leftLabel->AddFrame(label, new TGLayoutHints(kLHintsLeft, 5, 5, 5, 5));

    if (fDiscrete) {
      Int_t par_sec = 0;
      Double_t val =
        Hal::Std::Discretize(parConf.GetNPoints(), parConf.GetMapMin(), parConf.GetMapMax(), parConf.GetFittedValue(), '=');

      Int_t entry = 0;
      for (Double_t a = parConf.GetMapMin(); a <= parConf.GetMapMax(); a += parConf.GetDParam()) {
        fSteps.push_back(a);
        if (a == val) par_sec = entry;
        entry++;
      }

      fComboBox = new TGComboBox(rightLabel);
      for (unsigned int i = 0; i < fSteps.size(); i++) {
        fComboBox->AddEntry(Form("%4.3f", fSteps[i]), i);
      }
      fComboBox->Resize(70, 20);
      fComboBox->Select(par_sec);

      fComboBox->Connect("Selected(Int_t)", gui->ClassName(), gui, "ApplyParams()");
      rightLabel->AddFrame(fComboBox, new TGLayoutHints(kLHintsTop | kLHintsRight, 5, 5, 5, 5));
      this->AddFrame(leftLabel, new TGLayoutHints(kLHintsLeft | kLHintsExpandX | kLHintsExpandY, 5, 5, 5, 5));
      this->AddFrame(rightLabel, new TGLayoutHints(kLHintsRight | kLHintsExpandX | kLHintsExpandY, 5, 5, 5, 5));
      gui->AddFrame(this, new TGLayoutHints(kLHintsTop | kLHintsExpandX, 5, 5, 5, 5));
    } else {
      fNumberEntry = new TGNumberEntry(rightLabel,
                                       parConf.GetFittedValue(),
                                       5,
                                       -1,
                                       TGNumberFormat::EStyle::kNESReal,
                                       TGNumberFormat::EAttribute::kNEAAnyNumber,
                                       TGNumberFormat::kNELLimitMinMax,
                                       parConf.GetMapMin(),
                                       parConf.GetMapMax());
      fNumberEntry->Connect("ValueSet(Long_t)", gui->ClassName(), gui, "ApplyParams()");
      rightLabel->AddFrame(fNumberEntry, new TGLayoutHints(kLHintsTop | kLHintsRight, 5, 5, 5, 5));
      this->AddFrame(leftLabel, new TGLayoutHints(kLHintsLeft | kLHintsExpandX | kLHintsExpandY, 5, 5, 5, 5));
      this->AddFrame(rightLabel, new TGLayoutHints(kLHintsRight | kLHintsExpandX | kLHintsExpandY, 5, 5, 5, 5));
      gui->AddFrame(this, new TGLayoutHints(kLHintsTop | kLHintsExpandX, 5, 5, 5, 5));
    }
  }

  Double_t CorrFitParButton::GetValue() {
    if (fDiscrete) {
      Int_t pos = fComboBox->GetSelected();
      return fSteps[pos];
    } else {
      return fNumberEntry->GetNumber();
    }
  }

  CorrFitChiSelector::CorrFitChiSelector(const TGWindow* p, UInt_t w, UInt_t h, UInt_t options, Pixel_t back) :
    TGHorizontalFrame(p, w, h, options, back), fParName(nullptr), fMin(nullptr), fMax(nullptr), fSteps(nullptr) {}

  void CorrFitChiSelector::Init(CorrFitGUI* gui, CorrFit* f) {
    fParName = new TGComboBox(this);
    for (int i = 0; i < f->GetParametersNo(); i++) {
      fParName->AddEntry(f->GetParameterName(i), i);
    }
    fParName->Selected(0);
    AddFrame(fParName, new TGLayoutHints(kLHintsLeft | kLHintsCenterY | kLHintsExpandX, 5, 5, 5, 5));
    fMin = new TGNumberEntry(this,
                             0,
                             5,
                             -1,
                             TGNumberFormat::EStyle::kNESReal,
                             TGNumberFormat::EAttribute::kNEAAnyNumber,
                             TGNumberFormat::kNELLimitMinMax,
                             -1E+6,
                             1E+6);
    this->AddFrame(fMin, new TGLayoutHints(kLHintsTop | kLHintsCenterY | kLHintsExpandX, 5, 5, 5, 5));
    fMax = new TGNumberEntry(this,
                             0,
                             5,
                             -1,
                             TGNumberFormat::EStyle::kNESReal,
                             TGNumberFormat::EAttribute::kNEAAnyNumber,
                             TGNumberFormat::kNELLimitMinMax,
                             -1E+6,
                             1E+6);
    this->AddFrame(fMax, new TGLayoutHints(kLHintsTop | kLHintsCenterY | kLHintsExpandX, 5, 5, 5, 5));
    fSteps = new TGNumberEntry(this,
                               0,
                               5,
                               -1,
                               TGNumberFormat::EStyle::kNESInteger,
                               TGNumberFormat::EAttribute::kNEAPositive,
                               TGNumberFormat::kNELLimitMinMax,
                               1,
                               1E+6);
    this->AddFrame(fSteps, new TGLayoutHints(kLHintsTop | kLHintsCenterY | kLHintsExpandX, 5, 5, 5, 5));
    fParName->Resize(70, 20);
    gui->AddFrame(this, new TGLayoutHints(kLHintsCenterX, 5, 5, 5, 5));
  }

  void CorrFitChiSelector::GetValue(Int_t& par, Int_t& steps, Double_t& min, Double_t& max) {
    par   = fParName->GetSelected();
    steps = fSteps->GetIntNumber();
    min   = fMin->GetNumber();
    max   = fMax->GetNumber();
  }

  void CorrFitGUI::DrawChi2() {
    SetParams();
    Int_t par1, par2, steps1, steps2;
    Double_t min1, min2, max1, max2;
    fPairChi1->GetValue(par1, steps1, min1, max1);
    fPairChi2->GetValue(par2, steps2, min2, max2);
    if (min1 > max1) return;
    if (min2 > max2) return;
    if (steps1 < 1 || steps2 < 1) return;
    ChiSqMap2D* chi = static_cast<CorrFitFunc*>(fFunc)->GetChiSquareMap(par1, steps1, min1, max1, par2, steps2, min2, max2);
    ApplyParams();
    TVirtualPad* pad = gPad;
    TCanvas* c       = new TCanvas();
    c->cd();
    TString opt = "";
    if (fChiMin->IsOn()) opt = "min";
    if (!fChiFit->IsOn()) {
      if (opt.Length() == 0)
        opt = "nolin";
      else
        opt = opt + "+nolin";
    }
    chi->Draw(opt);
    c->Modified();
    c->Update();

    if (fChiLogz->IsOn()) { gPad->SetLogz(); }
#ifdef __APPLE__
    for (int ispe = 0; ispe < 2; ispe++) {
      if (gSystem->ProcessEvents()) break;
    }
#endif
    gPad = pad;
  }

  void CorrFitGUI::SetParams() {
    for (int i = 0; i < fFunc->GetParametersNo(); i++) {
      fFunc->OverwriteParam(i, fSliders[i]->GetValue());
    }
  }
}  // namespace Hal
