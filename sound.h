#ifndef SONIDOS_H_INCLUDED
#define SONIDOS_H_INCLUDED
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#define AMPLITUD_TONO 8192
#define FREC_MUESTREO 44100

//estructura de formato de audio soportado y error.
typedef enum {
    SOUND_ERR = -1,
    SOUND_MP3 = 0x01
} tFormatsSnd;

//estructura opaca
typedef struct sSound tSound;

//inicia audios y devuelve el formato o error.
tFormatsSnd sound_start(void);

//carga el archivo de audio, y retorna su puntero o NULL.
tSound* sound_load(const char *path);

//reproduce el sonido indicando la cantidad de veces, puede ser infinita con -1.
void sound_play(const tSound *sound, int32_t cantVeces,int channel);

//libera la memoria del sonido.
void sound_destroy(tSound *sound);

//cierra y finaliza el audio.
void sound_finish(void);

#endif // SONIDOS_H_INCLUDED
