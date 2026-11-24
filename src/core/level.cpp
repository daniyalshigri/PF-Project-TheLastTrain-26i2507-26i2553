#include "level.h"
#include "util.h"
#include <iostream>
#include <fstream>
using namespace std;

static int toNumber(const char* text)
{
    if (text[0]=='\0')
    {
        return -1;
    }
    int number=0;
    for (int i=0 ; text[i] !='\0';i++)
    {
        if(text[i]<'0'||text[i]>'9')
        {
            return -1;
        }
        number =number*10+(text[i]-'0');
    }
    return number;
}
static bool isyes(const char* text)
{
   return stringEquals(text,"YES");
    
}

bool loadInitialState(const char* filename,
                      int& playerFloor, int& playerRoom,
                      int& energy, int& time, int& score,
                      char inventory[MAX_INVENTORY_CAPACITY], int& inventoryCount,
                      bool objectives[NUM_OBJECTIVES], int& emergencyTripsLeft)
{

ifstream file(filename);
if(!file.is_open())
{
    cout<<"Cannot Open the file. "<<endl;
  return false;
}
char line[MAX_LINE_LEN];
energy =INITIAL_ENERGY;
time=INITIAL_TIME;
score=INITIAL_SCORE;
inventoryCount=0;
emergencyTripsLeft=0;
(void)playerFloor;
(void)playerRoom;

for (int i=0; i<NUM_OBJECTIVES ; i++)
{
    objectives[i]=false;
}

for (int i=0; i<MAX_INVENTORY_CAPACITY ; i++)
{
    inventory[i]=ENT_NONE;
}


while (file.getline(line,MAX_LINE_LEN))
{
    trimWhitespace(line);
    if(line[0]=='\0')
    {
        continue;
    }
    
        int i=0;
        while(line[i]!=':' && line[i]!='\0')
        {
            i=i+1;
        }
        if(line[i]=='\0')
        {
            continue;
        }
        char key[MAX_LINE_LEN];
        int k=0;
        while(k<i)
        {
            key[k]=line[k];
            k=k+1;
        }
        key[k]='\0';

        char value[MAX_LINE_LEN];
        int j=0;
        while(line[i+1+j]!='\0')
        {
            value[j]=line[i+1+j];
            j=j+1;
        }
        value[j]='\0';
     trimWhitespace(value);
   if(stringEquals(key,"INITIAL_ENERGY"))
   {
    int res =toNumber(value);
    if(res==-1)
    {
         cout<<"Invalid Initial value for ."<<key<<"."<<endl;
        file.close();
        return false;
    }
    energy=res;
    energy=clampInt(energy,0,MAX_ENERGY);
   }

   else if(stringEquals(key,"INITIAL_TIME"))
   {
    int res =toNumber(value);
    if(res==-1)
    {
         cout<<"Invalid Initial value for ."<<key<<endl;
        file.close();
        return false;
    }
    time=res;
    time=clampInt(time,0,MAX_TIME);
   }
  
   else if(stringEquals(key,"INITIAL_SCORE"))
   {
    int res =toNumber(value);
    if(res==-1)
    {
        cout<<"Invalid Initial value for ."<<key<<endl;
        file.close();
        return false;
    }
    score=res;
    score=clampInt(score,0,520);
   }

   else if(stringEquals(key,"PASSENGER_RESCUED"))
   {
    objectives[OBJ_PASSENGER_RESCUED]=isyes(value);
   }
   else if(stringEquals(key,"CAT_RESCUED"))
   {
    objectives[OBJ_CAT_SECURED]=isyes(value);
   }
    else if(stringEquals(key,"CABINET_UNLOCKED"))
   {
    objectives[OBJ_CABINET_UNLOCKED]=isyes(value);
   }
    else if(stringEquals(key,"TRAIN_KEY_COLLECTED"))
   {
    objectives[OBJ_TRAIN_KEY]=isyes(value);
   }
      else if(stringEquals(key,"FUSE_INSTALLED"))
   {
    objectives[OBJ_FUSE_INSTALLED]=isyes(value);
   }
   else if(stringEquals(key,"LIFT_PASS_COLLECTED"))
   {
    objectives[OBJ_LIFT_PASS]=isyes(value);
   }
   else if(stringEquals(key,"TRAIN_PREPARED"))
   {
    objectives[OBJ_TRAIN_PREPARED]=isyes(value);
   }



}
file.close();
return true;
}

bool validateStartingRoom(int startFloor, int startRoom,
                          const char horizontalConn[NUM_FLOORS][ROOMS_PER_FLOOR - 1])

                          {
                            (void)horizontalConn;
                            if((startFloor>=1 && startFloor<=NUM_FLOORS)&&(startRoom>=1 && startRoom<=ROOMS_PER_FLOOR))
                          {
                            return true;
                          }
                          else
                          {
                            return false;
                          }
                          }


const char* getEntityName(char entitySymbol)
{
    if(entitySymbol==ENT_MASTER_KEY)
    {
        return "Master Key";
    }
    else if(entitySymbol==ENT_ENERGY_PACK)
    {
        return "Energy Pack";
    }
     else if(entitySymbol==ENT_EMERGENCY_TOOLKIT)
    {
        return "Emergency Toolkit";
    }
     else if(entitySymbol==ENT_CLUE_4)
    {
        return "Clue Digit 4";
    }
     else if(entitySymbol==ENT_FUSE)
    {
        return "Fuse";
    }
    else if(entitySymbol==ENT_CAT)
    {
        return "Stranded Cat";
    }
    else if(entitySymbol==ENT_CONTROL_PANEL)
    {
        return "Control Panel Terminal";
    }
    else if(entitySymbol==ENT_PASSENGER)
    {
        return "Stranded Passenger";
    }
    else if(entitySymbol==ENT_MEDICAL_KIT)
    {
        return "Medical Kit";
    }
    else if(entitySymbol==ENT_EMERGENCY_CARD)
    {
        return "Emergency Access Card";
    }
    else if(entitySymbol==ENT_CLUE_2)
    {
        return "Clue Digit 2";
    }
    else if(entitySymbol==ENT_CABINET_TRAIN_KEY)
    {
        return "Cabinet / Train Key";
    }
    else if(entitySymbol==ENT_CAT_CARRIER)
    {
        return "Cat Carrier";
    }
    else if(entitySymbol==ENT_OBSERVATION_TOKEN)
    {
        return "Observation Token";
    }
     else if(entitySymbol==ENT_LIFT_PASS)
    {
        return "Lift Pass";
    }
     else if(entitySymbol==ENT_CLUE_7)
    {
        return "Clue Digit 7";
    }
    else if(entitySymbol==ENT_NONE)
    {
        return "None";
    }
    else 
    {
        return "Unknown";
    }
}   

bool loadBuildingMap(const char* filename,
                     char horizontalConn[NUM_FLOORS][ROOMS_PER_FLOOR - 1],
                     char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],
                     int entityCounts[NUM_FLOORS][ROOMS_PER_FLOOR])
                     {
                        ifstream file(filename);
                        if(!file.is_open())
                        {
                            cout<<"Cannot Open the file. "<<endl;
                            return false;
                        }

                        char mapLines[17][MAX_LINE_LEN];
                        int total_lines=0;
                         while (total_lines<17 && file.getline(mapLines[total_lines],MAX_LINE_LEN))
                        {
                            total_lines=total_lines+1;             
                           
                       }
                        if(total_lines<17)
                                {
                                    cout<<"Map Could not Load";
                                    return false;
                                }

                                for(int flor_n=1; flor_n<=NUM_FLOORS; flor_n++)
                                {
                                    int row=(3+3*(NUM_FLOORS-flor_n))-1;
                                    for(int gap_n=1;gap_n<=ROOMS_PER_FLOOR-1; gap_n++)
                                    {  
                                        int column=12*(gap_n-1)+20;
                                       
                                      horizontalConn[flor_n-1][gap_n-1]=mapLines[row][column];
                                    }
                                }
                                 
                                //slot empty
                                for(int flor_n=1; flor_n<=NUM_FLOORS; flor_n++)
                                {
                        
                
                                    for(int room=1;room<=ROOMS_PER_FLOOR; room++)
                                      {  
                                    
                                        entityCounts[flor_n-1][room-1]=0;
                                        for(int slot = 0; slot<MAX_ENTITIES_PER_ROOM;slot++)
                                        {
                                        
                                          roomEntities[flor_n-1][room-1][slot]= ENT_NONE;
                                        }

                                      }
                                       
                                }
                                    //Slot fill
                                  for(int flor_n=1; flor_n<=NUM_FLOORS; flor_n++)
                                {
                                    int itemrow =(3+3*(NUM_FLOORS-flor_n))-1+1;
                
                                    for(int room=1;room<=ROOMS_PER_FLOOR; room++)
                                    {  
                                        int startCol=9 + 12*(room - 1);
                                        
                                        for(int col = startCol; col<=startCol+10;col++)
                                        {
                                        char c=mapLines[itemrow][col];
                                        if(c!=' ')
                                        {
                                          if(entityCounts[flor_n-1][room-1]<MAX_ENTITIES_PER_ROOM)
                                          {
                                             int slot =entityCounts[flor_n-1][room-1];
                                          roomEntities[flor_n-1][room-1][slot]= c;
                                          entityCounts[flor_n-1][room-1]=slot+1;
                                          }
                                          
                                         
                                        }

                                        }
                                       
                                    }
                                    
                                }
                                
                                

                              file.close();
                                return true;
                    }
