#ifndef __AUDIO_MANEGER_H__
#define __AUDIO_MANEGER_H__

#include <map>
#include <fmod/fmod.hpp>

struct SoundChannel
{
	SoundChannel()
	: _sound(nullptr)
	, _channel(nullptr)
	{

	}

	SoundChannel(FMOD::Sound* sound, FMOD::Channel* channel)
	: _sound(sound)
	, _channel(channel)
	{

	}

	FMOD::Sound* _sound;
	FMOD::Channel* _channel;
};

class AudioManager
{
public:
	static AudioManager *getInstance();

	void update();

	void preloadBackgroundMusic(const char *filename);

	void playBackgroundMusic();

	void pauseBackgroundMusic();

	void resumeBackgroundMusic();

	void setMusicVolume(float volume);

	void loadFromMemory(const char *filename, void **buff, int *length);

	void createSound(const char *filename, const char *soundname);

	void playSound(const char *filename, bool autoRelease = true);
protected:
	AudioManager();

	~AudioManager();

private:
	FMOD::System *_system;
	std::map<std::string, SoundChannel> _sounds;
};

#endif