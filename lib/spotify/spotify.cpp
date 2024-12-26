#include "spotify.h"

const char* CLIENT_ID = "client_id";
const char* CLIENT_SECRET = "client_secret";

SpotifyHandle::SpotifyHandle()
    : spotify(CLIENT_ID, CLIENT_SECRET), 
    client_id(CLIENT_ID),
    client_secret(CLIENT_SECRET) {}

void SpotifyHandle::setupSpotify() {
    spotify.begin();
    Serial.println(F("Spotify initialized!"));
}