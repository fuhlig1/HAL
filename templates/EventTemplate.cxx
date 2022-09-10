/*
 * EventTemplate.cxx
 *
 *  Created on: 10 wrz 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "EventTemplate.h"

#include <Hal/DataManager.h>

#include "EventInterfaceTemplate.h"
#include "TrackTemplate.h"  // needed for update

namespace MyHal {
  /**
   * do not forget to put name of your class in constructor !
   */
  EventTemplate::EventTemplate() : Event("MyHal::Track", "Hal::V0Track") {}

  Hal::EventInterface* EventTemplate::CreateInterface() const { return new EventInterfaceTemplate(); }

  void EventTemplate::Update(Hal::EventInterface* interface) {
    EventInterfaceTemplate* source = (EventInterfaceTemplate*) interface;
    fTracks->Clear();
    fV0sHiddenInfo->Clear();
    /**
     * fill this event data
     */
  }

  Bool_t EventTemplate::ExistInTree() const {
    Hal::DataManager* manager = Hal::DataManager::Instance();
    if (manager->CheckBranch("MyEvent.")) { return kTRUE; };
    return kFALSE;
  }

  TString EventTemplate::GetFormatName() const { return "MyEvent"; }

  EventTemplate::~EventTemplate() {}

}  // namespace MyHal
