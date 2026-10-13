#pragma once

#include "../globals.h"

typedef struct Camera
{
  float y;
  float prevY;

  // Impact shake, held as "trauma" in 0..1 rather than as a pixel count.
  // What gets drawn is trauma *squared*, so a graze barely registers while a
  // detonation throws the frame around - the response an arcade player expects
  // from a hit, and the one a linear magnitude does not give.
  float trauma;
  float shakeTime; // advances the noise the shake is sampled from
  float punch;     // 0..1 hit zoom, decays on its own
} Camera;

extern Camera camera;

bool isCameraMoving(void);

// Adds impact shake. `amount` is trauma in 0..1: about 0.2 for a small hit,
// 0.5 for an explosion, 0.8 for losing a life. It accumulates and is capped.
void addTrauma(float amount);

// Adds a short zoom-in punch, 0..1, for impacts that want weight without
// the frame rattling.
void addPunch(float amount);

// trauma squared - the shake strength everything visual is scaled by
float shakeAmount(void);

// Current shake displacement in logical pixels and roll in degrees. Sampled
// from smooth noise rather than drawn fresh each frame: a random number per
// frame is a buzz, and noise is a camera being knocked about.
void cameraShake(float *outX, float *outY, float *outAngle);

float cameraPunch(void);

// World-Y of the bottom of the playfield: where the safety net sits, where a
// ball is lost and where falling things stop. It is the bottom edge of the
// screen unless the paddle has been raised on a short level.
float cameraFieldBottom(void);

// PHONE ZOOM (BREAKUP_PHONE_ZOOM). The playing field is drawn `cameraZoom()`
// times bigger around the middle of the screen, so the part of the world that
// shows is x in [cameraFieldLeft(), cameraFieldRight()] and a height of
// cameraFieldHeight() from camera.y. Without the zoom these are 0, 800 and
// SCREEN_HEIGHT, and everything below does nothing.

// Picks the zoom for a level. Boss levels are never zoomed.
void cameraSetLevel(int levelIndex);

float cameraZoom(void);
float cameraFieldLeft(void);
float cameraFieldRight(void);
float cameraFieldHeight(void);

// Whether a pattern column is inside the picture. Columns outside it have no
// bricks and no spawners, because nothing could ever hit them.
bool cameraColumnInField(int col);

// A pointer position in the 800 x SCREEN_HEIGHT window, as a world x.
float cameraScreenToFieldX(float screenX);

// Zooms everything drawn until cameraResetZoom(). Call after beginScene() and
// reset before the scene is composited or anything unzoomed is drawn.
void cameraApplyZoom(void);
void cameraResetZoom(void);

void initializeCamera(void);

void updateCamera(void);
