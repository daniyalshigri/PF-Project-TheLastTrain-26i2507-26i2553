#include "switches.h"
#include "util.h"
#include "physics.h"

bool canUseLift1(int fromFloor, int toFloor)
{
    return ((fromFloor==1 && toFloor==2 )||(fromFloor==2&&toFloor==1));
}

bool canUseLift2(int fromFloor, int toFloor, bool lift2Operational, int emergencyTripsLeft)
{
     if((fromFloor<1||fromFloor>5)||(toFloor<1||toFloor>5))
     {
        return false;
     }
    else if((fromFloor==toFloor) ||(toFloor==3||fromFloor==3) ||(toFloor==1||fromFloor==1))
    {
        return false;
    }
    else if((lift2Operational==false)&&(emergencyTripsLeft<=0)) 
    {
        return false;
    }

    return true;
}

bool unlockFloor3Door(const char inventory[MAX_INVENTORY_CAPACITY], int inventoryCount)
{
    return hasItem(inventory,inventoryCount,'<');//ENT_MASTER_KEY can be use instead of <

}

bool installFuseControlPanel(char inventory[MAX_INVENTORY_CAPACITY], int& inventoryCount,
                          bool objectives[NUM_OBJECTIVES], int& score)
    {
     if(!hasItem(inventory,inventoryCount,ENT_FUSE))
     {
        return false;
     }
     
        removeItem(inventory,inventoryCount, ENT_FUSE);
        objectives[OBJ_FUSE_INSTALLED]=true;
        objectives[OBJ_LIFT2_ACTIVE]=true;
        applyScoreDelta(score,SCORE_INSTALL_FUSE);
        return true;
     
    }



bool verifyCabinetCode(const char* enteredCode, const bool objectives[NUM_OBJECTIVES])
{
    if(objectives[OBJ_CLUE_2]==false||objectives[OBJ_CLUE_4]==false||objectives[OBJ_CLUE_7]==false)
    {
        return false;
    }
    
    return  stringEquals(enteredCode,"427");
}

// // Door lock mechanics, lifts, and cabinet verification without structures
