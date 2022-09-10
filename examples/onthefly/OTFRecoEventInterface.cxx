/*
 * OTFRecoEventInterface.cxx
 *
 *  Created on: 28 maj 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "OTFRecoEventInterface.h"

#include "DataManager.h"

#include <RtypesCore.h>

namespace HalOTF {
  RecoEventInterface::RecoEventInterface() : fEvent(nullptr), fCanDelete(kFALSE) {}

  void RecoEventInterface::ConnectToTreeInternal(eMode mode) {
    Hal::DataManager* manager = Hal::DataManager::Instance();
    switch (mode) {
      case Hal::EventInterface::eMode::kRead: {
        fEvent = (OTF::RecoEvent*) manager->GetObject("OTF::RecoEvent.");
      } break;
      case Hal::EventInterface::eMode::kWrite: {
        fEvent = new OTF::RecoEvent();
        manager->Register("OTF::RecoEvent.", "OTF", fEvent, kTRUE);
      } break;
      case Hal::EventInterface::eMode::kWriteVirtual: {
        fEvent = new OTF::RecoEvent();
        manager->Register("OTF::RecoEvent.", "OTF", fEvent, kFALSE);
        fCanDelete = kTRUE;
      } break;
    }
  }

  RecoEventInterface::~RecoEventInterface() {
    if (fCanDelete && fEvent) delete fEvent;
  }
}  // namespace HalOTF
