#include <stdio.h>
#include <tchar.h>
#include <conio.h>
#include <math.h>

#define MONO 128.0

struct WAVHEADER
{
    char chunkId[4];
    unsigned long chunkSize;
    char format[4];
    char subchunk1Id[4];
    unsigned long subchunk1Size;
    unsigned short audioFormat;
    unsigned short numChannels;
    unsigned long sampleRate;
    unsigned long byteRate;
    unsigned short blockAlign;
    unsigned short bitsPerSample;
    char subchunk2Id[4];
    unsigned long subchunk2Size;
};

int _tmain(int argc, _TCHAR* argv[])
{
    FILE* file;
    FILE* file2;
    errno_t err;
    err = fopen_s(&file, "Test.wav", "rb");
    if (err)
    {
        printf_s("Failed open file, error %d", err);
        return 0;
    }
    err = fopen_s(&file2, "Test2.wav", "wb");
    if (err)
    {
        printf_s("Failed open file2, error %d", err);
        return 0;
    }
    WAVHEADER header;
    char data;
    fread_s(&header, sizeof(WAVHEADER), sizeof(WAVHEADER), 1, file);

    // Выводим полученные данные
    printf_s("Sample rate: %d\n", header.sampleRate);
    printf_s("Channels: %d\n", header.numChannels);
    printf_s("Bits per sample: %d\n", header.bitsPerSample);

    // Посчитаем длительность воспроизведения в секундах
    float fDurationSeconds = 1.f * header.subchunk2Size / (header.bitsPerSample / 8) / header.numChannels / header.sampleRate;
    int iDurationMinutes = (int)floor(fDurationSeconds) / 60;
    fDurationSeconds = fDurationSeconds - (iDurationMinutes * 60);
    printf_s("Duration: %02d:%02.f\n", iDurationMinutes, fDurationSeconds);

    fwrite(&header, sizeof(WAVHEADER), 1, file2);
	//Нормализация аудио
    int max = 0;
    while (!feof(file))
    {
        fread_s(&data, sizeof(data), sizeof(data), 1, file);
        abs(data) > max ? max = abs(data) : NULL;
    }
    fseek(file, sizeof(WAVHEADER), SEEK_SET);
    double l = MONO / max;
    printf("Koof = %f", l);
    while (!feof(file))
    {
        fread_s(&data, sizeof(data), sizeof(data), 1, file);
        data *= l;
        fwrite(&data, sizeof(data), 1, file2);
    }
    fclose(file);
    fclose(file2);
    _getch();
    return 0;
}