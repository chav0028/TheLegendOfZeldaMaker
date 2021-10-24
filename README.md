# The Legend Of Zelda Maker
Final Project for GAM1540-Game Programming III, made using a custom C++ game engine, GameDev2D, developed by Bradley Flood, a course instructor at Algonquin College.

Starting with a non-functional framework to recreate The Legend of Zelda (NES) and a level editor, we were tasked to create a working version of the Zelda game with a level editor. The emphasis of the project was in implementing the AI, with different states and A* pathfinding; creating a level editor that could read/write save files, and supported multiple tile types; and implementing randomized enemy drops; all of these, besides the basic game functionality.

Some of the features implemented for this project were:
- Multi-screens (sections) world map, with each section connected in the overall world map.
- Multiple types of tiles that were walkable or not, and tile to tile collision
- Level editor that allowed placing different types of tiles.
- Loading and writing levels to a binary file (edit them in level editor, play them in game).
- Enemy AI, including different states (walking, idle, searching, etc.), and A* pathfinding across their subsection tile map.
- Randomized drops for enemies. These drops (health, rupees, or nothing) had an specified chance of appearing, that varied according to conditions like the player's health.
- UI to display the player's current health and their number of rupees.
- Loading screen, a loading bar showed the percentage of assets loaded before the scene was changed.
- Playing of sound effects for different actions in the game, and the background music.
