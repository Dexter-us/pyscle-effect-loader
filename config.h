#pragma once

#define PLUG_NAME "Dexter U.S. Psycle adapter"
#define PLUG_MFR "Dexter_US"
#define PLUG_VERSION_HEX 0x00010000 // 1.0.0
#define PLUG_VERSION_STR "1.0.0"
#define PLUG_UNIQUE_ID 'P001'
#define PLUG_MFR_ID 'Pamp'

#define PLUG_NUM_AUDIO_IOS 2
#define PLUG_AUDIO_STR_IN "2"
#define PLUG_AUDIO_STR_OUT "2"

#define PLUG_CHANNEL_IO_STR "2-2"

#define PLUG_LATENCY 0
#define PLUG_IS_INST 0
#define PLUG_DOES_MIDI_IN 0
#define PLUG_DOES_MIDI_OUT 0

// UI Windows Dimensions configuration matching your yellow UI specs
#define PLUG_WIDTH 450
#define PLUG_HEIGHT 220
#define PLUG_FPS 60

// Target Format Flags
#define VST3_SUITE 1
#define AAX_TYPE 0
#define AU_TYPE 0
