export module EnemiesManager;

import Enemy;
import EnemyCommands;

import linAlg;
import GameMap;

import Player;

export class EnemiesManager {
private:
	GameMap* gameMap_handle;
	Player* player_handle;

private:

	bool checkIfEnemySeesPlayer(Enemy& e);

public:

	EnemiesManager();
	~EnemiesManager();

	void init(GameMap* gameMap_handle, Player* p);

	// For starters:
	// 1. Check which command is being performed
	// 2. If it is the wait command -> queue new commands
	//		2.1. Choose random point on map
	//		2.2. Calculate the velocity versor to this point 
	//		2.3. Queue the appropriate commands
	// Else do nothing
	void updateEnemy(Enemy& e);

};