#include "settings.h"

void settingsDefault(tSettings *s) {
  s->volume = 64;           // Default mitad volumen
  s->allowDefaultNames = 1; // Default SI
}

int settingsLoad(tSettings *s) {
  FILE *f = fopen(SETTINGS_FILE, "rb");
  if (!f) {
    settingsDefault(s);
    return ERR;
  }
  fread(s, sizeof(tSettings), 1, f);
  fclose(f);
  return OK;
}

int settingsSave(const tSettings *s) {
  FILE *f = fopen(SETTINGS_FILE, "wb");
  if (!f) {
    return ERR;
  }
  fwrite(s, sizeof(tSettings), 1, f);
  fclose(f);
  return OK;
}

void settingsApply(const tSettings *s) {
  Mix_VolumeMusic(s->volume);
  Mix_Volume(-1, s->volume);
}
