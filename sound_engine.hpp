#ifndef SOUND_ENGINE_H_
#define SOUND_ENGINE_H_

#include <SFML/Audio.hpp>
#include <string>
#include <iostream>

using namespace std;


class Sound{
    private:
    sf::SoundBuffer buffer;
    sf::SoundBuffer left_channel_buffer, right_channel_buffer;
    sf::Sound left_sound, right_sound;
    public:

    float left_vol;
    float right_vol;


    Sound(string file_name, float volume){
        buffer.loadFromFile(file_name);
        const sf::Int16* samples = buffer.getSamples();
        size_t sampleCount = buffer.getSampleCount();
        
        sf::Int16* leftChannelSamples = new sf::Int16[sampleCount];
        sf::Int16* rightChannelSamples = new sf::Int16[sampleCount];

        for (std::size_t i = 0; i < sampleCount; i += 2) {
            leftChannelSamples[i] = samples[i];
            leftChannelSamples[i + 1] = 0;

            rightChannelSamples[i] = 0;
            rightChannelSamples[i + 1] = samples[i + 1];
        }

        left_channel_buffer.loadFromSamples(leftChannelSamples, 
                                            sampleCount, buffer.getChannelCount(), 
                                            buffer.getSampleRate());

        right_channel_buffer.loadFromSamples(rightChannelSamples, 
                                            sampleCount, buffer.getChannelCount(), 
                                            buffer.getSampleRate());
        
        delete[] leftChannelSamples;
        delete[] rightChannelSamples;

        
        left_sound.setBuffer(left_channel_buffer);
        right_sound.setBuffer(right_channel_buffer);

        left_sound.setVolume(volume);
        right_sound.setVolume(volume);
        left_vol, right_vol = volume;

        left_sound.play();
        right_sound.play();
        // cout << "Now Playing..." << file_name << endl;
    }

    void set_volume(float r, float l){
        if (l < 0) {l = 0;}
        if (r < 0) {r = 0;}
        if (l > 100) {l = 100;}
        if (r > 100) {r = 100;}
        
        left_sound.setVolume(l);
        right_sound.setVolume(r);
    }

    bool is_playing(){
        if (left_sound.getStatus() == sf::Sound::Playing || right_sound.getStatus() == sf::Sound::Playing){
            return true;
        } else {
            return false;
        }
    }

};




#endif