# IECAI-project
PROTECTED UNDER A-GPL LICENSE.
## Introduction
Immersive Environment-Centric Artificial Intelligence (IECAI) is a prototype project for games, written in C++, which aims to make NPC AI more Immersive and Realistic. The project began in August 2020, the repo in September and the first release likely in November. It uses a simple file to interface with a game engine (implements.h), which gives IECAI the base functions to operate with the NPCs in the game engine. These functions are just short placeholders in C++, but all of the IECAI Runtime operates off of them. 

### Introduction: _Target_
> IECAI aims to make AI more intelligent, immersive, realistic and smooth. No more horses swimming mid-air.
### Introduction: _Developers_
- Head Dev: Evan Clegern; evanclegern.work@gmail.com

## Operation
AI is built with multiple factors including: Risk, Self-Preservation, Route Planning, Basic Coordination for Squads, Threat Mitigation and Full Terrain Usage. The AI is built in the following structure:
  An _object class_ is stored in an _NPC Class_, which defines shared characteristics and changeable characteristics, and which the _NPC Object_ - which references directly to the _Engine NPC_ - is derived from.
  
### Operation: _AI Systems_
The IECAI NPCs have multiple systems in the _NPC Class_ or _NPC Object_ to control the NPC in an 'IEC' method:
  - **Event** (a single function grouping that is essentially a direct connection to the game engine)
  - **Routines** (cycled sets of events, like _play "idle animation"_ or _look around_)
  - **Behaviors** (flexible routines for responding to specified "stimuli IDs")
  - **Routes** (set systems of points which serve as a suggested route of travel)
  - **Squads** (flexible groups of other NPCs which have some extra routines and behaviors)
  - **Relations** (flexible modifiers between classess or objects that define extra behaviors)
  - **Emotions** (pseudoroutines which convey extra facial movements and which modify behaviors)
  - **Hints** (point that modifies the behavior and emotion of any NPCs that have it within their look range)

## Contact Us!
You can email me at evanclegern.work@gmail.com, or join the Slack channel at iecai.slack.com
