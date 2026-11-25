#include "viewer_iface.h"
#include "util.h"
#include <iostream>
#include "level.h"
using namespace std;

void renderDashboard(int playerFloor, int playerRoom, int energy, int time, int score,
                     const char inventory[MAX_INVENTORY_CAPACITY], int inventoryCount,
                     int emergencyTripsLeft)
                     {
                        printHeader("THE LAST TRAIN - STATUS");
                        cout<<"Player Location      : "<<"Floor "<<playerFloor<<", Room "<<playerRoom<<endl;
                        cout<<"Energy               : "<<energy<<"/"<<MAX_ENERGY<<endl;
                        cout<<"Time                 : "<<time<<"/"<<MAX_TIME<<endl;
                        cout<<"Score                : "<<score<<endl;
                        cout<<"Emergency trips left : "<<emergencyTripsLeft<<endl;
                        cout<<"Inventory ("<<inventoryCount<<"/"<<MAX_INVENTORY_CAPACITY<<") : ";
                        if(inventoryCount==0)
                        {
                            cout<<"Empty";
                        }
                        else
                        {
                            for(int i=0;i<inventoryCount;i++)
                            {  
                                if(i>0)
                                {
                                   cout<<", ";
                                   
                                }
                                 cout<<getEntityName(inventory[i]);
                            }
                        }cout<<endl;
                     }


void renderRoomDetails(int floor, int room,
                       const char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],
                       int entityCount)
         {
          cout<<"Floor "<<floor<<", Room "<<room<<endl;
      if(entityCount==0)
      {
        cout<<"No Object found here"<<endl;
      }
      else{
        for(int i=0;i<entityCount; i++)
        {
            cout<<i+1<<" "<<getEntityName(roomEntities[floor-1][room-1][i])<<endl;
        }
      }

         }  
         
void renderObjectivesChecklist(const bool objectives[NUM_OBJECTIVES])
{
    printHeader("OBJECTIVES");
    const char* names[NUM_OBJECTIVES]=
    {
       "Master Key collected", "Clue 4 found", "Clue 2 found", "Clue 7 found",
        "Cabinet unlocked", "Train Key collected", "Fuse collected", "Fuse installed",
        "Lift Pass collected", "Lift 2 active", "Passenger rescued", "Cat Carrier collected",
        "Cat secured", "Train prepared", "Mission won", "Mission failed" 
};
for(int i=0; i<NUM_OBJECTIVES ;i++)
{
    if(objectives[i])
    {
        cout<<"[X] "<<names[i]<<endl;
    }
    else
    {
        cout<<"[ ] "<<names[i]<<endl;
    }

}

}         

void renderAsciiBuilding(int playerFloor, int playerRoom)
{
    printHeader("BUILDING MAP");
    
        for(int floor=NUM_FLOORS; floor>=1;floor--)
        {
        cout<<"Floor "<<floor<<" :";
        {
            for(int room=1;room<=ROOMS_PER_FLOOR;room++)
            {
                if(floor==playerFloor&&room==playerRoom)
                {
                    cout<<"[P] ";
                }
                else
                {
                cout<<"[ ] ";
                }
                
            }cout<<endl;

        }
    }
    
 }


// Terminal UI and dashboard rendering using primitive parameters
