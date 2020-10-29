# IECAI-project
PROTECTED UNDER GNU AFFERO PUBLIC LICENSE.
## Introduction
Immersive Environment-Centric Artificial Intelligence (IECAI) is a prototype project for games, written in C++, which aims to make NPC "AI" more Immersive and Realistic (even though it really isn't _AI_). The project began in August 2020, the repo in September and the first release likely in January 2021. 

### Introduction: _Target_
> IECAI aims to make AI more intelligent, immersive, realistic and smooth. No more horses swimming mid-air.

## Operation
AI is built with multiple factors including: Risk, Self-Preservation, Route Planning, Basic Coordination for Squads, Threat Mitigation and Full Terrain Usage. Operation documentation will be completed in the near future.
  
### Operation: _AI Systems_
The IECAI NPCs have multiple systems in the _NPC Class_ or _NPC Object_ to control the NPC in an 'IEC' method:
  - **Event** (a single function grouping that is essentially a direct connection to the game engine)
  - **Routines** (cycled sets of events, like _play "idle animation"_ while performing an observation of the area)
  - **Behaviors** (flexible routines for responding to specified "stimuli IDs" - in a behavioral [decision tree format](https://en.wikipedia.org/wiki/Decision_tree))
  - **Routes** (set systems of points which serve as a suggested route of travel)
  - **Squads** (flexible groups of other NPCs which have some extra routines and behaviors)
  - **Relations** (flexible modifiers between classess or objects that define extra behaviors)
  - **Emotions** (pseudoroutines which convey extra facial movements and which modify behaviors)
  - **Hints** (point that modifies the behavior and emotion of any NPCs that have it within their look range)

### Operation: _File-side_
IECAI definitions are stored on the filesystem of the computer, while runtime is split between that and memory - all expected, anyway. Multiple file extensions are to be used when operating with IECAI NPCs, data stored in [JSON](https://en.wikipedia.org/wiki/JSON) through [jsoncpp](https://github.com/open-source-parsers/jsoncpp), the reason being that there is a lot of data to be stored and a lot of that connects cross-module, even if one module is a child of another. IECAI Project file extensions are:
  - **ENTITIES FILE**, storing basic information for every entity that is in the world space - iecENTS-???.json
  - **ACTIONS FILE**, storing data for NPC act-events (NPC_action and NPC_vocal) - iecACTS-???.json
  - **ANIMATIONS FILE**, storing the hitbox-related data for the animations related to actions - iecANIM-???.json
  - **CLASS FILE**, storing data for NPC Classes and default configuration - iecCLAS-???.json
  - **BEHAVIOR FILE**, storing structured behaviors and routines - iecBHVR-???.json
  - **CHARACTER FILE**, storing temporary data and positioning for NPCs - iecANPC-???.json
An example would be *iecANPC-save00.json* or *iecENTS-level4.json*

## Contact Us!
You can email me at evanclegern.work@gmail.com, or join the [Slack](https://iecai.slack.com) channel.

## Using IECAI in a game project
As described by the GNU Affero Public License, we need: License and Copyright Notice, a statement of any changes, source disclosure and the license being present. Other than that, use it freely with your game project. Basically, just give us credit, say what you did and don't mark it as your own.
