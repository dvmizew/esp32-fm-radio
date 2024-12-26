#ifndef SPOTIFY_H
#define SPOTIFY_H

#include <SpotifyEsp32.h>

class SpotifyHandle {
public:
    SpotifyHandle();
    void setupSpotify();

private:
    Spotify spotify;
    const char* client_id;
    const char* client_secret;
};

#endif