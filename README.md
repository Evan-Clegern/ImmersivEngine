# iecai-project
PROTECTED UNDER A-GPL LICENSE.
## Introduction
Immersive Environment-Centric Artificial Intelligence (IECAI) is a prototype project for games, written in C++, which aims to make NPC AI more Immersive and Realistic. The project began in August 2020, the repo in September and the first release likely in November. It uses a simple file to interface with a game engine (implements.h), which gives IECAI the base functions to operate with the NPCs in the game engine.

## Target
IECAI is, simply, to make AI more intelligent and smooth. Currently, it is still heavily in development, but a basic working version should be ready around November of 2020.

## Operation
AI is built with multiple factors including: Risk, Self-Preservation, Route Planning, Basic Coordination for Squads, Threat Mitigation and Full Terrain Usage. The AI is built in the following structure:
  Engine NPC -> IECAI Object -> IECAI NPC Class -> IECAI NPC Object
The IECAI NPCs have multiple systems:
  Event (a single function grouping that is essentially a direct connection to the game engine)
  Routines (cycled sets of events, like 'play idle animation' and 'look around')
  Behaviors (flexible routines for responding to specified "stimuli IDs")
  Routes (set systems of points which serve as a suggested route of travel)
  Squads (flexible groups of other NPCs which have some extra routines and behaviors)
  Relations (flexible modifiers between classess or objects that define extra behaviors)
  Emotions (pseudoroutines which convey extra facial movements and which modify behaviors)
  Hints (point that modifies the behavior and emotion of any NPCs that have it within their look range)
