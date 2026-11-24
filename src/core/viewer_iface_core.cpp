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

// Terminal UI and dashboard rendering using primitive parameters
