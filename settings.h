#ifndef SETTINGS_H_INCLUDED
#define SETTINGS_H_INCLUDED

#include "library.h"

#define SETTINGS_FILE "settings.bin"

typedef struct {
  int volume;            // de 0 a 128 (SDL_mixer range)
  int allowDefaultNames; // 1 = si, 0 = no
} tSettings;

void settingsDefault(tSettings *s);
int settingsLoad(tSettings *s);
int settingsSave(const tSettings *s);
void settingsApply(const tSettings *s);

#endif // SETTINGS_H_INCLUDED
