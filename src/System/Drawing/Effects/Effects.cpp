#include "Effects.h"

Mirror* mirrorLeft = new Mirror(Config::Displays::LEFT_SIDE);
Mirror* mirrorRright = new Mirror(Config::Displays::RIGHT_SIDE);
Rotate180* flipMouth = new Rotate180(Config::Displays::MOUTH);
Effect* flipLeftEye = new VerticalMirror(Config::Displays::EYE_L);
Effect* flipRightEye = new VerticalMirror(Config::Displays::EYE_R);
Rotate90* rotateEyes90 = new Rotate90(Config::Displays::EYES);
Rotate180* rotateEyes180 = new Rotate180(Config::Displays::EYES);
Rotate90* rotateEyes270 = new Rotate90(Config::Displays::EYES, true);
Clear* clearAll = new Clear();
ClearParticles* clearAllParticles = new ClearParticles();
Invert* invertAll = new Invert();