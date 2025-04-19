#pragma once
#include <unordered_map>
#include <filesystem>
#include "gameEngineUtils.h"
#include <string>
#include "AudioHelper.h"
#include "SDL2_mixer/SDL_mixer.h"
#include "LuaHandler.h"
class AudioDB
{

public:
	static inline std::unordered_map<std::string, Mix_Chunk*> audioTable;
	
	void LoadAudioManager(LuaHandler* luaHandler) 
	{
		AudioHelper::Mix_AllocateChannels(50);
		AudioHelper::Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
		luabridge::getGlobalNamespace(luaHandler->GetLuaState())
			.beginNamespace("Audio")
			.addFunction("Play", &PlayAudioOnChannel)
			.addFunction("Halt", &StopAudioOnChannel)
			.addFunction("SetVolume", &SetVolumeOnChannel)
			.endNamespace();

	}

	static inline void PlayAudioOnChannel(int channel, std::string audio,  bool does_loop)
	{
		int loop = 0;
		if (does_loop) 
		{
			loop = -1;
		}
		
		if (!(AudioDB::audioTable.count(audio) > 0))
		{
			std::string filepath = "";
			if (std::filesystem::exists("resources/audio/" + audio + ".wav"))
			{
				filepath = "resources/audio/" + audio + ".wav";
			}
			else if (std::filesystem::exists("resources/audio/" + audio + ".ogg"))
			{
				filepath = "error: failed to play audio clip " + audio;
			}
			else 
			{
				gameEngineUtils::HandlePrint_PV("error: failed to play audio clip " + audio, true);
				exit(0);
			}
			AudioDB::audioTable[audio] = AudioHelper::Mix_LoadWAV(filepath.c_str());
		
		}
		AudioHelper::Mix_PlayChannel(channel, audioTable[audio], loop);
	}


	static inline void StopAudioOnChannel(int channel)
	{
		AudioHelper::Mix_HaltChannel(channel);
	}

	static inline void SetVolumeOnChannel(int channel, int volume)
	{
		AudioHelper::Mix_Volume(channel, volume);
	}



};

