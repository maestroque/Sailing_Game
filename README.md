# Sailing Game
A turn-based console sailing game.

![εικόνα](https://github.com/maestroque/sailing-game/assets/74024609/be3623ad-382e-4a37-bdc4-1609c7de6e74)

## Build Intructions
Windows-only for the time being.
```bat
cd sailing-game
gcc SailingGame.c
./SailingGame.exe
```
## How to play
The goal is to make a full lap of the lake, around the island, before the other players.

Each player is represented as an ASCII character on the map, and taking turns each declares their desired move using the WASD keys (e.g. 2 moves up, 3 moves right). After all players have declared their intended moves, the moves are realised at the same time. However, there are some effects taken into account during the execution of these moves.

### Wind
Wind is a constantly changing factor in this game (... and it can be wild).

It varies in direction and intensity, and changes its state in each round. The wind affects **in a greater way**, players that have inserted more moves in a given round, than players with a more constrained intended trail. This is implemented both to make larger moves more risky and have the effect that players with greater speed (more moves in the same time interval) are affected more by the wind

### Shallow Waters
Shalow Waters are represented as light blue blocks on the map. A player cannot move to these blocks by a standard declared move. However, the wind can **push** the players near shallow waters to a shallow water block. When this occurs, the affected player loses a playing round. When the player's turn arrives once again, they can move only **out of the shallow waters** towards the direction of the nearest normal water block.

### Crash
It is possible for players to crash into each other. This can happen either due to a miscalculated declared move that leads two or more players to the same position or due to the wind pushing the players to the same block. When this happens all players affected return to the intitial starting position and start over.




