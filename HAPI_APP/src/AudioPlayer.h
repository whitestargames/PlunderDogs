#pragma once

#include <string>
#include <deque>

//bool LoadSound(const std::string &filename)
//bool PlaySound(const std::string &filename)
//bool PlaySound(const std::string &filename, const SoundOptions &options)
//bool PlaySound(const std::string &filename, const SoundOptions &options, int &instanceId)
//bool StopSound(int soundInstanceId, bool togglePause = false)
//bool ChangePlayingSoundOptions(int &soundInstanceId, const SoundOptions &options)
//const EMediaStatus GetSoundStatus(int soundInstanceId, float &playbackPosition) const

class AudioPlayer
{
public:
	static AudioPlayer& getInstance()
	{
		static AudioPlayer instance;
		return instance;
	}
	
	void playSound(const std::string& fileName);


private:
	AudioPlayer();

	//std::deque<
};