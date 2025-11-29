#pragma once

/**
 * Includes for all effects, also general effects to be used anywhere.
 */

#include "Typedefs.h"
#include "Effect.h"
#include "NullEffect.h"
#include "Clear.h"
#include "ClearParticles.h"
#include "EmitParticles.h"
#include "Expand.h"
#include "ExpandX.h"
#include "ExpandY.h"
#include "Glitch.h"
#include "Invert.h"
#include "Mirror.h"
#include "ParticleColumn.h"
#include "ParticleEdge.h"
#include "Rotate90.h"
#include "Rotate180.h"
#include "SetColumn.h"
#include "SetPixel.h"
#include "SetRow.h"
#include "TVStatic.h"
#include "VerticalMirror.h"
#include "DrawBitmap.h"
#include "Rectangle.h"
#include "Blink.h"
#include "MouthEffect.h"
#include "Translate.h"
#include "Tween.h"
#include "LambdaEffect.h"
#include "RandomizeFace.h"
#include "LosePower.h"
#include "Earthquake.h"
#include "SetRowFromBitmap.h"

// general effects
extern Mirror* mirrorLeft;
extern Mirror* mirrorRright;

extern Rotate180* flipMouth;

extern Effect* flipLeftEye;
extern Effect* flipRightEye;

extern Rotate90* rotateEyes90 ;
extern Rotate180* rotateEyes180;
extern Rotate90* rotateEyes270;

extern Clear* clearAll;
extern ClearParticles* clearAllParticles;
extern Invert* invertAll;