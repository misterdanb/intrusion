#include "sound.h"
#include <SDL.h>
#include "4klang.inh"

static const int channels = 2;
static const Uint16 play_buffer_size = 4096;

static SAMPLE_TYPE buffer[MAX_SAMPLES * channels];
static int position = 0;

static void callback(void* userdata, Uint8* stream, int length)
{
    if (position >= MAX_SAMPLES)
    {
        return;
    }

    static float* sample = buffer;
    int16_t* p = (int16_t*)stream;

    for (int i = length / (sizeof(short) * channels); i > 0 ; i--)
    {
        *p++ = (int16_t)(INT16_MAX * *sample++);
        *p++ = (int16_t)(INT16_MAX * *sample++);

        position += 1;
    }
}

void initialize_sound()
{
    SDL_AudioSpec spec;
    spec.freq = SAMPLE_RATE;
    spec.format = AUDIO_S16SYS;
    spec.channels = channels;
    spec.silence = 0;
    spec.samples = play_buffer_size;
    spec.size = 0;
    spec.callback = callback;
    spec.userdata = buffer;

    SDL_OpenAudio(&spec, NULL);

    __4klang_render(buffer);
}

void play_sound()
{
    SDL_PauseAudio(0);
}

