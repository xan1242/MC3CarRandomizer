# Midnight Club 3 DUB Remix Vehicle Randomizer

This is a very simple vehicle randomizer for Midnight Club 3 that changes your active vehicle at each race finish.

Your vehicle modifications will be retained as the car is changed.

The seed is generated from your profile name and stored inside the save file.

## Installation

1. Get [the latest build of PCSX2 Fork with Plugins here](https://github.com/ASI-Factory/PCSX2-Fork-With-Plugins/releases/tag/latest) (MSVC AVX2 build should be good for most people)

2. Place `MC3CarRandomizer.elf` in the `PLUGINS` directory of PCSX2 Fork

## Game compatibility

Currently it only works with the MC3 Remix US build (CRC: `0x60A42FF5`, ID: `SLUS-21355`)

## Is it possible to run this on a PS2?

As of now, not yet. [There is a tool made by ermaccer](https://github.com/ermaccer/ps2plugininjector) that could inject it into the executable, however, it's not fully stable because it doesn't relocate the code (it relies on a fixed section and prays that it doesn't get overwritten by the game).

## How does this work?

### Short explanation

- Randomizer seed is generated from your profile name and stored in your save file (overwrites one of the strings used in online mode chat). 

- Car changes when you finish a race (applies with each loading screen).

- Car parts are retained no matter which car you have selected in the garage.

### Long explanation

The randomizer hooks a few key events in the game:

1. Car configuration setting/application - this is where the car override gets applied

2. Career data loading - this is where the seed data and the active vehicle is read from the save data

3. Career data saving - this is where the seed data and the active vehicle is stored into the save data

4. Save creation - this is where the seed is generated

5. Race finish - this is where the car randomization is triggered

6. Race over screen - this is where verification that you've randomized the car happens

The car list is obtained from a vehicle list pointer (at `0x006E0170` in the Remix US build) that the game creates from `tune/vehicle/vehicle.lst`.

The vehicle count is found right next to the vehicle list pointer as a 16-bit integer.

The randomizer simply selects a number within the available count range, and the needed data is passed from the list into the override.

The RNG function itself is pulled from NFS Most Wanted (raw bytes in `bRandom.c`).
