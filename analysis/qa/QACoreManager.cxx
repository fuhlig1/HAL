/*
 * QACoreManager.cxx
 *
 *  Created on: 4 maj 2021
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "QACoreManager.h"

#include "AnalysisManager.h"
#include "Const.h"
#include "Source.h"
#include "TrackAna.h"
#include "TrackOnlyPrimariesCut.h"
#include "TrackPdgCut.h"

namespace Hal {
  void QACoreManager::SetMcTrackCut(TrackAna* ana, ePidCut cut, eParticleType primary, TString flag) {
    Int_t pid = 0;
    switch (cut) {
      case ePidCut::kPionPlus: {
        pid = Const::PionPlusPID();
      } break;
      case ePidCut::kPionMinus: {
        pid = -Const::PionPlusPID();
      } break;
      case ePidCut::kKaonPlus: {
        pid = Const::KaonPlusPID();
      } break;
      case ePidCut::kKaonMinus: {
        pid = -Const::KaonPlusPID();
      } break;
      case ePidCut::kProton: {
        pid = Const::ProtonPID();
      } break;
      case ePidCut::kAntiProton: {
        pid = -Const::ProtonPID();
      } break;
    }
    TrackPdgCut pdgCut;
    pdgCut.SetMinAndMax(pid);
    ana->AddCut(pdgCut, flag);
    switch (primary) {
      case eParticleType::kPrimaryOnly: {
        TrackOnlyPrimariesCut prim;
        ana->AddCut(prim, flag);
      } break;
      case eParticleType::kSecondaryOnly: {
        TrackOnlyPrimariesCut prim;
        prim.AcceptOnlyNonPrimaries();
        ana->AddCut(prim, flag);
      } break;
    }
  }
  Event* QACoreManager::GetFormat(eFormatType /*type*/, eAnaType /*ana*/) { return nullptr; }

  void QACoreManager::SetRecoTrackCut(TrackAna* /*ana*/, ePidCut /*cut*/, eParticleType /*primary*/, TString /*flag*/) {}

  void QACoreManager::SetEventCut(TrackAna* /*ana*/, Int_t /*col*/, TString /*flag*/) {};

  AnalysisManager* QACoreManager::GetAna(TString outFile, TString simFile, TString recoFile) {
    AnalysisManager* run = new AnalysisManager();
    run->SetOutput(outFile);
    RootSource* source = new RootSource(simFile);
    run->SetSource(source);
    if (!recoFile.EqualTo(simFile)) { source->AddFriend(simFile); }
    return run;
  }

  void QACoreManager::SetPairCut(TwoTrackAna* /*ana*/, ePidCut /*pid1*/, ePidCut /*pid2*/) {};

  QACoreManager::ePidCut QACoreManager::ToPid(Int_t i) {
    switch (i) {
      case 0: return QACoreManager::ePidCut::kPionPlus; break;
      case 1: return QACoreManager::ePidCut::kPionMinus; break;
      case 2: return QACoreManager::ePidCut::kKaonPlus; break;
      case 3: return QACoreManager::ePidCut::kKaonMinus; break;
      case 4: return QACoreManager::ePidCut::kProton; break;
      case 5: return QACoreManager::ePidCut::kAntiProton; break;
      default: return QACoreManager::ePidCut::kUnkown;
    }
  }
}  // namespace Hal
