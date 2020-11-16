
using namespace std;
using namespace entbase; //from 'base_entity.h'
using namespace npc_actions; //from 'implements.h'
using namespace eng_actions; //from 'implements.h'
using namespace statSys; //from 'base_stats.h'
using namespace weaponry; //from 'base_weapon.h'
class NPC {
	vector<NPC_action> actionQueue;
	int panicLevel, timesHit, timesAttacked, timesFled, timesDefended, timesHelped;
	vector<entity&> squad, nearbyGood, nearbyEnemy, nearbyOther;
	vector<weapon&> weaponInventory;
protected:
	entity& sister;
	NPC_class& parent;
	advancedstats stats;
	entBase& stepparent;
	NPC_config config;
	terrain_slice terrainPos;
	point truePos;
	vector<NPC_relationship> relations;
public:
	bool queueAction(NPC_action new) {
		if (this.actionQueue.size() > this.config.maxQueueSize) {
			return false;
		}
		try {
			this.actionQueue.push_back(new);
		} catch {
			return false;
		}
		return true;
	}
	bool moveTo(point to_true, terrain_slice to_terrain, bool run) {
		
	}
};
