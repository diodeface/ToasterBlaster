#pragma once

#include "Utilities/Typedefs.h"
#include "Effect.h"

class NullEffect : public Effect {
  public:
    /**
     * Empty effect. Used for terminating overlay effect sequences.
     */
    NullEffect()
        : Effect({}, false) {
    }
};