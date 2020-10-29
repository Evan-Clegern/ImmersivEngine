# Immersive, Environment-Centric AI  (IECAI) Project
PROTECTED UNDER GNU AFFERO PUBLIC LICENSE.
## Introduction
The IECAI system is a prototype project for games, written in C++, which aims to make NPC "AI" more Immersive and Realistic (even though it really isn't _AI_). The project began in August 2020, the repo was first created in September and the first prototype is expected to be ready January 2021. 

### Introduction: _Target_
> IECAI aims to make AI more intelligent, immersive, realistic and smooth. No more horses swimming mid-air.

## Operation
AI is built with multiple factors including: Risk, Self-Preservation, Route Planning, Basic Coordination for Squads, Threat Mitigation and Full Terrain Usage. A majority of these can be enabled or disabled to really model that *dumb* and *smart* difference with NPCs. Operation documentation will be completed once a functional prototype is completed.
  
### Operation: _AI Systems_
IECAI NPCs utilize (or will utilize) a hierarchy-like system for handling behavior, a majority of them building on one another.
  - **Event** (a single function grouping that is essentially a direct connection to the game engine)
  - **Routines** (cycled sets of events, like _play "idle animation"_ while performing an observation of the area)
  - **Behaviors** (flexible routines for responding to specified "stimuli IDs" - in a behavioral [decision tree format](https://en.wikipedia.org/wiki/Decision_tree))
  - **Routes** (set systems of points which serve as a suggested route of travel)
  - **Squads** (flexible groups of other NPCs which have some extra routines and behaviors)
  - **Relations** (flexible modifiers between classess or objects that define extra behaviors)
  - **Emotions** (pseudoroutines which convey extra facial movements and which modify behaviors)
  - **Hints** (point that modifies the behavior and emotion of any NPCs that have it within their look range)

### Operation: _File-side_
A majority of IECAI operation requires persistent data modes, to save memory usage and to enable easy system restoration. Multiple naming standards are to be used when operating with IECAI NPCs, with data stored in [JSON](https://en.wikipedia.org/wiki/JSON) through [jsoncpp](https://github.com/open-source-parsers/jsoncpp). The nomenclature for files is as follows:
  - **ENTITIES FILE**, storing basic information for every entity that is in the world space - iecENTS-???.json
  - **ACTIONS FILE**, storing data for NPC act-events (NPC_action and NPC_vocal) - iecACTS-???.json
  - **ANIMATIONS FILE**, storing the hitbox-related data for the animations related to actions - iecANIM-???.json
  - **CLASS FILE**, storing data for NPC Classes and default configuration - iecCLAS-???.json
  - **BEHAVIOR FILE**, storing structured behaviors and routines - iecBHVR-???.json
  - **CHARACTER FILE**, storing temporary data and positioning for NPCs - iecANPC-???.json
#### Example Nomenclature
An example would be *iecANPC-save00.json* or *iecENTS-level4.json*

## Contact
You can email me at evanclegern.work@gmail.com, or join the [Slack](https://iecai.slack.com) channel.
