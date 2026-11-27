#include "simulator.h"
#include "physics.h"
#include "switches.h"
#include "logger.h"
#include "viewer_iface.h"
#include "util.h"
#include "level.h"
#include <iostream>
using namespace std;


void runSimulation(int& playerFloor, int& playerRoom,
                   int& energy, int& time, int& score,
                   char inventory[MAX_INVENTORY_CAPACITY], int& inventoryCount,
                   bool objectives[NUM_OBJECTIVES], int& emergencyTripsLeft,
                   char horizontalConn[NUM_FLOORS][ROOMS_PER_FLOOR - 1],
                   char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],
                   int entityCounts[NUM_FLOORS][ROOMS_PER_FLOOR])
    {
       
        while(true)
                          {
                            cout<<"Start Floor :";                      
                            cin>>playerFloor;
                            cout<<"Start Room :";
                            cin>>playerRoom;
                            if(!cin)
                            { 
                                return;
                            }
                         if(validateStartingRoom(playerFloor, playerRoom,
                          horizontalConn))
                          {
                            break;
                          }
                          else
                          {
                            cout<<"Invalid room or floor : try again . "<<endl;
                          }
                        
                          }
        

       
    while(true)
    {
        renderDashboard(playerFloor,playerRoom,energy,time,score,
                     inventory,inventoryCount,
                     emergencyTripsLeft);

        renderRoomDetails(playerFloor,playerRoom,
                       roomEntities,entityCounts[playerFloor-1][playerRoom-1]);
        char Command[MAX_NAME_LEN];
        cout<<"Enter the Command :"<<endl;
        cin>>Command;
        if(!cin ||stringEquals(Command,"quit"))
        {
            break;
        }
        else if(stringEquals(Command,"move"))
        {
            int to_f;
            int to_r;
            cout<<"Enter the target floor : ";
            cin>>to_f;
            cout<<"Enter the target room : ";
            cin>>to_r;
            handleMoveCommand(to_f, to_r,
                       playerFloor, playerRoom,
                       energy, time,
                       horizontalConn,
                       inventory, inventoryCount,
                       emergencyTripsLeft,objectives[OBJ_LIFT2_ACTIVE]);

        }
        else if(stringEquals(Command,"inspect"))
        {
            handleInspectCommand(playerFloor, playerRoom,
                          energy, time,
                          roomEntities,
                          entityCounts[playerFloor-1][playerRoom-1],
                          objectives, score);

        }
        else if(stringEquals(Command,"collect"))
        { char symbol;
            cin>>symbol;
            handleCollectCommand(symbol, playerFloor, playerRoom,
                          energy, time, score,
                          roomEntities,
                          entityCounts[playerFloor-1][playerRoom-1],
                          inventory,inventoryCount,
                          objectives);
        }
        else if(stringEquals(Command,"use"))
        { 
            char symbol;
            cin>>symbol;
            handleUseCommand(symbol, playerFloor, playerRoom,
                      energy, time, score,
                      inventory,inventoryCount,
                      objectives);
       }
       else if(stringEquals(Command,"unlock"))
       {
        char code[MAX_NAME_LEN];
        cin>>code;

        handleUnlockCabinetCommand(code,playerFloor,playerRoom,
                                objectives, score,roomEntities,
                                entityCounts[playerFloor-1][playerRoom-1]);
       }
       else
       {
         cout<<"Unknown command "<<endl;
       }
       
    if(checkEndConditions(energy,time,objectives))  
    {
    //  writeGameSummaryReport();
     break;
    }

               
    }           

    }



bool handleMoveCommand(int targetFloor, int targetRoom,
                       int& currentFloor, int& currentRoom,
                       int& energy, int& time,
                       const char horizontalConn[NUM_FLOORS][ROOMS_PER_FLOOR - 1],
                       const char inventory[MAX_INVENTORY_CAPACITY], int inventoryCount,
                       int& emergencyTripsLeft, bool lift2Operational)
                       {

                        if(targetFloor==currentFloor&&targetRoom==currentRoom)
                        {
                            cout<<"You are already here "<<endl;
                            return false;
                        }
                        char connection_type;

                        if(!isDirectlyConnected(currentFloor, currentRoom, targetFloor, targetRoom,
                         horizontalConn,connection_type))
                         { 
                            cout<<"There is no direct path to that room"<<endl;
                            return false;
                         }

                         if(!canTraverse(targetFloor, targetRoom,connection_type,
                 inventory, inventoryCount,
                 emergencyTripsLeft, lift2Operational))
                 {
                if(connection_type=='L')
                {
                    cout<<":The door is locked , Master Key is needed."<<endl;
                }
                else if(connection_type=='#'||connection_type==' ')
                {
                    cout<<"The solid wall blocks your way."<<endl;
                }
                 else if(connection_type=='2')
                {
                    cout<<"Lift 2 is not Working and you have no emergency trips."<<endl;
                }
                 return false;
                 }
                 
                 int cost_energy =0;
                 int cost_time =0;
                 if(connection_type=='D'||connection_type=='L'||connection_type=='S')
                 {
                    cost_energy=COST_MOVE_ORDINARY_ENERGY;
                    cost_time=COST_MOVE_ORDINARY_TIME;
                 }
                 else if(connection_type=='W')
                 {
                    cost_energy=COST_MOVE_WINDOW_ENERGY;
                    cost_time=COST_MOVE_WINDOW_TIME;
                 }
                 else if(connection_type=='1'&& currentFloor==targetFloor)
                 {
                    cost_energy=COST_MOVE_ORDINARY_ENERGY;
                    cost_time=COST_MOVE_ORDINARY_TIME;
                 }
                else if(connection_type=='1'||connection_type=='2')
                  {
                   cost_energy=COST_MOVE_LIFT_ENERGY;
                   cost_time=COST_MOVE_LIFT_TIME;
                  }
                
                  
                if(connection_type=='2' && lift2Operational==false)
                {
                    emergencyTripsLeft=emergencyTripsLeft-1;
                }  

                applyResourceDelta(energy, time,cost_energy, cost_time);
                currentFloor=targetFloor;
                currentRoom=targetRoom;
                cout<<"You moved to Floor "<<currentFloor<<" Room "<<currentRoom<<endl;
                return true;


                       }

bool handleInspectCommand(int currentFloor, int currentRoom,
                          int& energy, int& time,
                          const char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],
                          int entityCount,
                          bool objectives[NUM_OBJECTIVES], int& score)
                        
                       
      {
        char symbol;
        applyResourceDelta(energy,time,COST_INSPECT_ENERGY,COST_INSPECT_TIME);
        for(int i=0; i<entityCount;i++)
        {
            symbol=roomEntities[currentFloor-1][currentRoom-1][i];
        
        if(symbol==ENT_CLUE_4 && !objectives[OBJ_CLUE_4])
        {
            objectives[OBJ_CLUE_4]=true;
           applyScoreDelta(score, SCORE_CLUE_FOUND);
           cout<<"You found clue digit 4."<<endl;
        }
        if(symbol==ENT_CLUE_2 && !objectives[OBJ_CLUE_2])
        {
            objectives[OBJ_CLUE_2]=true;
           applyScoreDelta(score, SCORE_CLUE_FOUND);
           cout<<"You found clue digit 2."<<endl;
        }
          if(symbol==ENT_CLUE_7 && !objectives[OBJ_CLUE_7])
        {
            objectives[OBJ_CLUE_7]=true;
           applyScoreDelta(score, SCORE_CLUE_FOUND);
           cout<<"You found clue digit 7."<<endl;
        }

      }  
       return true;
    }            


bool handleCollectCommand(char itemSymbol, int currentFloor, int currentRoom,
                          int& energy, int& time, int& score,
                          char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],
                          int& entityCount,
                          char inventory[MAX_INVENTORY_CAPACITY], int& inventoryCount,
                          bool objectives[NUM_OBJECTIVES])
                       
                          {
                            int pos=-1;
                        for(int i=0; i<entityCount; i++)
                        {
                       if(roomEntities[currentFloor-1][currentRoom-1][i]==itemSymbol)
                              {
                                pos=i;
                                 break;
                         }
                        }
                          if(pos==-1)
                           {
                            cout<<"The item is not here"<<endl;
                           return false;
                           }

                        
                       if(itemSymbol==ENT_PASSENGER ||itemSymbol==ENT_CAT ||itemSymbol==ENT_CONTROL_PANEL||
                         itemSymbol==ENT_CABINET_TRAIN_KEY||
                         itemSymbol==ENT_CLUE_2||
                        itemSymbol==ENT_CLUE_4||
                        itemSymbol==ENT_CLUE_7)
                              {
                               cout<<"You can not pick it up ."<<endl;
                               return false;
                        }
                           
                        

                        if(!addItem(inventory,inventoryCount, itemSymbol))
                        {
                            cout<<"The inventory has no space "<<endl;
                            return false;
                        }
                        
                      


    for(int j=pos; j<entityCount-1; j++)
    {
       roomEntities[currentFloor-1][currentRoom-1][j]=roomEntities[currentFloor-1][currentRoom-1][j+1];

         }
    roomEntities[currentFloor-1][currentRoom-1][entityCount-1]=ENT_NONE;
    entityCount--;

    applyResourceDelta(energy,  time, COST_ITEM_PICKUP_ENERGY, COST_ITEM_PICKUP_TIME);
    
    return true;
        }     
                       

bool handleUseCommand(char itemSymbol, int currentFloor, int currentRoom,
                      int& energy, int& time, int& score,
                      char inventory[MAX_INVENTORY_CAPACITY], int& inventoryCount,
                      bool objectives[NUM_OBJECTIVES]){
                        return false;
                       }

bool handleUnlockCabinetCommand(const char* code, int currentFloor, int currentRoom,
                                bool objectives[NUM_OBJECTIVES], int& score,
                                char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],
                                int& entityCount){
                        return false;
                       }

bool checkEndConditions(int energy, int time, const bool objectives[NUM_OBJECTIVES]){
                        return false;
                       }

// Modular simulation loop using primitive variables
