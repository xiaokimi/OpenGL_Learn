#include <iostream>
#include "AudioManager.h"

FMOD_RESULT F_CALLBACK callbackChannel(FMOD_CHANNELCONTROL *channelcontrol, FMOD_CHANNELCONTROL_TYPE controltype, FMOD_CHANNELCONTROL_CALLBACK_TYPE callbacktype, void *commanddata1, void *commanddata2);


AudioManager *_audioManager = nullptr;

AudioManager *AudioManager::getInstance()
{
	if (!_audioManager)
	{
		_audioManager = new AudioManager();
	}

	return _audioManager;
}

AudioManager::AudioManager()
{
	FMOD::System_Create(&_system);
	_system->init(32, FMOD_INIT_NORMAL, 0);
}

AudioManager::~AudioManager()
{
	if (_audioManager)
	{
		delete _audioManager;
		_audioManager = nullptr;

		for (auto sound : _sounds)
		{
			sound.second._sound->release();
		}

		_system->close();
		_system->release();
	}
}

void AudioManager::update()
{
	if (_system)
	{
		_system->update();
	}
}

void AudioManager::preloadBackgroundMusic(const char *filename)
{
	FMOD::Sound *sound;
	FMOD::Channel *channel = 0;

	_system->createStream(filename, FMOD_LOOP_NORMAL | FMOD_2D, 0, &sound);

	_sounds.insert(std::pair<std::string, SoundChannel>("background", SoundChannel(sound, channel)));
}

void AudioManager::playBackgroundMusic()
{
	auto iter = _sounds.find("background");
	if (iter != _sounds.end())
	{
		FMOD::Sound *sound = _sounds["background"]._sound;
		FMOD::Channel *channel = _sounds["background"]._channel;

		_system->playSound(sound, 0, false, &channel);
		_sounds["background"]._channel = channel;
	}
}

void AudioManager::pauseBackgroundMusic()
{
	auto iter = _sounds.find("background");
	if (iter != _sounds.end())
	{
		FMOD::Channel *channel = _sounds["background"]._channel;

		channel->setPaused(true);
	}
}

void AudioManager::resumeBackgroundMusic()
{
	auto iter = _sounds.find("background");
	if (iter != _sounds.end())
	{
		FMOD::Channel *channel = _sounds["background"]._channel;

		channel->setPaused(false);
	}
}

void AudioManager::setMusicVolume(float volume)
{
	auto iter = _sounds.find("background");
	if (iter != _sounds.end())
	{
		FMOD::Channel *channel = _sounds["background"]._channel;

		channel->setVolume(volume);
	}
}

void AudioManager::loadFromMemory(const char *filename, void **buff, int *length)
{
	FILE *file = NULL;
	fopen_s(&file, filename, "rb");

	fseek(file, 0, SEEK_END);
	long len = ftell(file);
	fseek(file, 0, SEEK_SET);

	void *mem = malloc(len);
	fread(mem, 1, len, file);

	fclose(file);

	*buff = mem;
	*length = len;

	//free(mem);
}

void AudioManager::createSound(const char *filename, const char *soundname)
{
	FMOD::Sound *sound;
	FMOD::Channel *channel = 0;

	_system->createSound(filename, FMOD_DEFAULT, 0, &sound);

	_sounds.insert(std::pair<std::string, SoundChannel>(soundname, SoundChannel(sound, channel)));
}

void AudioManager::playSound(const char *filename, bool autoRelease /* = true */)
{
	auto iter = _sounds.find(filename);
	if (iter != _sounds.end())
	{
		FMOD::Sound *sound = _sounds[filename]._sound;
		FMOD::Channel *channel = _sounds[filename]._channel;

		_system->playSound(sound, 0, false, &channel);
		_sounds[filename]._channel = channel;

		if (autoRelease)
		{
			channel->setCallback(callbackChannel);
		}
	}
}

FMOD_RESULT F_CALLBACK callbackChannel(FMOD_CHANNELCONTROL *channelcontrol, FMOD_CHANNELCONTROL_TYPE controltype, FMOD_CHANNELCONTROL_CALLBACK_TYPE callbacktype, void *commanddata1, void *commanddata2)
{
	FMOD::Channel *channel = (FMOD::Channel *)channelcontrol;
	if (callbacktype == FMOD_CHANNELCONTROL_CALLBACK_END)
	{
		std::cout << "play sound callback" << std::endl;

	}

	return FMOD_OK;
}