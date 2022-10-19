# just_another_modloader

> Just another modloader for Touhou 15.5 without injector.

## Table of Contents

* [General Info](#general-information)
* [Technologies Used](#technologies-used)
* [Features](#features)
* [Setup](#setup)
* [Usage](#usage)
* [Project Status](#project-status)
* [Room for Improvement](#room-for-improvement)
* [Acknowledgements](#acknowledgements)
* [License](#license)

## General Information

* Loading addons into the game process and using callbacks upon event triggering.

## Technologies Used

* mingw-std-threads - version latest
* Detours - version 4.0.1
* deser-hjson - version 1.0.2
* lazy_static - version 1.4.0
* serde - version 1.0.145
* winapi - version 0.3.9

## Features

* hjson.
* Easy to add a new event to trigger.

## Setup

> Upgrade depencies by running  
`git init`  
`git submodule foreach git pull origin master`  
install i686 mingw and use **build_*.sh** to build binaries.

## Usage

Drop both **Netplay.dll** and **states.dll** into game root directory.  
Create **addons** folder in game root directory.  
Drop addons into **addonds** directory.

## Project Status

Project is: _in progress_.

## Room for Improvement

Room for improvement:

* Improve code quality.

To do:

* Refresh addons in-game.
* Addons updater.

## Acknowledgements

* Many thanks to [_SOG_](https://github.com/SonofGod1998) and [_JustAPenguin9_](https://github.com/JustAPenguin9).

## License

This project is open source and available under the [GNU General Public License v3.0](https://github.com/lurkydismal/just_another_modloader/blob/main/LICENSE).
