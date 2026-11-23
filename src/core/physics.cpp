#include "physics.h"
#include "util.h"

bool hasItem(const char inventory[MAX_INVENTORY_CAPACITY], int inventoryCount, char itemSymbol)
{
    for(int i =0;i<inventoryCount;i++)
    {
        if(inventory[i]==itemSymbol)
        {
            return true;
        }
    }
    return false;

}

bool addItem(char inventory[MAX_INVENTORY_CAPACITY], int& inventoryCount, char itemSymbol)
{
    if(inventoryCount>=MAX_INVENTORY_CAPACITY)
    {
        return false;

    }
    else
    {
        inventory[inventoryCount]=itemSymbol;
        inventoryCount++;

    }
    return true;
}

bool removeItem(char inventory[MAX_INVENTORY_CAPACITY], int& inventoryCount, char itemSymbol)
{
    int pos=-1;
    for(int i=0; i<inventoryCount; i++)
    {
        if(inventory[i]==itemSymbol)
        {
           pos=i;
           break;
        }
        
    }
    if(pos==-1)
    {
        return false;
    }
    for(int j=pos; j<inventoryCount-1; j++)
    {
        inventory[j]=inventory[j+1];
         }
    inventoryCount--;
    

    return true;
}

void applyResourceDelta(int& energy, int& time, int deltaEnergy, int deltaTime)
{
    energy=energy+deltaEnergy;
    energy= clampInt(energy,0,MAX_ENERGY);
    
    time =time+deltaTime;
    time =clampInt(time,0,MAX_TIME);
    
}

void applyScoreDelta(int& score, int deltaScore)
{
score=score+deltaScore;
score=clampInt(score,0,520);

}

bool isDirectlyConnected(int fromF, int fromR, int toF, int toR,
                         const char horizontalConn[NUM_FLOORS][ROOMS_PER_FLOOR - 1],
                         char& outConnType)
                         {

                            //Same Floor Neighbour
                            int gap;
                           if( fromF==toF && (fromR==toR-1 || fromR==toR+1))
                           {
                            int smaller;
                            if(fromR<toR)
                            {
                               smaller=fromR;
                              
                            }
                            else
                            {
                             smaller =toR;
                            }
                           gap = smaller-1;
                           outConnType=horizontalConn[fromF-1][gap];
                             return true;
                           }
                         //Stairs
                           if(((fromF==1 && toF==2) || (fromF==2 && toF==1) )&&(fromR==1 && toR==1)) 
                           {
                           outConnType='S';
                           return true;
                           }
                           else if(((fromF==2 && toF==3) || (fromF==3 && toF==2) ) &&(fromR==1 && toR==1)) 
                           {
                           outConnType='S';
                           return true;
                           }
                          else if(((fromF==4 && toF==5)|| (fromF==5 && toF==4)) &&(fromR==1 && toR==1)) 
                          {
                           outConnType='S';
                           return true;
                          }
                        //LIFT 1
                          if(((fromF==1 && toF==2)||(fromF==2 && toF==1)) &&((fromR==2 && toR==3)||(fromR==3 && toR==2)||(fromR==2 && toR==2) ||(fromR==3 && toR==3) ))
                          {
                            outConnType = '1';
                            return true;
                          }
                           
                          //LIFT 2
                          if((fromF==2||fromF==4||fromF==5)&&(toF==2||toF==4||toF==5) &&(fromR==4 &&toR==4))
                          {
                            if(fromF!=toF)
                            {
                            outConnType='2';
                            return true;
                            }
                            else
                            {
                             return false;
                            }

                          }
                           return false;
                         }


bool canTraverse(int targetFloor, int targetRoom, char connType,
                 const char inventory[MAX_INVENTORY_CAPACITY], int inventoryCount,
                 int emergencyTripsLeft, bool lift2Operational)
                 {
                     (void) targetFloor;
                     (void)  targetRoom;
                    if(connType=='D'||connType=='W'||connType=='S'||connType=='1')
                    {
                      return true;
                    }
                    else if(connType=='L')
                    { 
                        bool item = hasItem(inventory,inventoryCount,'<');
                        return item;
                    }
                    else if(connType=='2')
                    {
                        return (lift2Operational ==true || emergencyTripsLeft>0);                      
                    }
                        return false;

                 }
// Movement validation and resource adjustments without structures