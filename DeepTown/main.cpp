#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <math.h>
using namespace std;
class itemInfo;
class buildingInfo;
class ingredientInfo;
class productInfo;
class TimeProfileInfo;
class procedureInfo;

class itemInfo
{
    public:
    itemInfo(string newName, double newPrice)
    {
        name = newName;
        price = newPrice;
        isRaw = false;
        ToMake.clear();
        isConsiderIngredientTime = false;
        
        reservation_level = 0;
        reservation_quest = 0;
        isAvailable = true;
        currentQuantity = 0;
    }
    
    static int findItem(string itemName, vector<itemInfo>& itemList)
    {
        for(unsigned int i = 0; i<itemList.size(); i++)
        {
            if(itemName == itemList[i].name) return i;
        }
        
        cout<<"Error: cannot find item: "<<itemName<<endl;
        return -1;
    }
    
    static void setConsiderIngredientTime(string ProductName, vector<itemInfo>& itemList)
    {
        int itemIndex = itemInfo::findItem(ProductName,itemList);
        itemList[itemIndex].isConsiderIngredientTime = true;
    }
    
    void setTimePerPiece(vector<itemInfo>& itemList, vector<procedureInfo>& procedureList, vector<buildingInfo>& buildingList);
    
    static void set_reservation_level(vector<itemInfo>& itemList, vector<ingredientInfo>& level);
    static void set_reservation_quest(vector<itemInfo>& itemList, vector<ingredientInfo>& quest);
    
    static void set_currentQuantity(string itemName, double quantity, vector<itemInfo>& itemList)
    {
        int itemIndex = itemInfo::findItem(itemName,itemList);
        
        if(quantity == -1) itemList[itemIndex].isAvailable = false;
        else itemList[itemIndex].currentQuantity = quantity;
    }
    
    string name;
    double price;
    bool isRaw;
    double timePerPiece;
    double pricePerTime;
    vector<int> procedureIndex;
    vector<int> ToMake;
    bool isConsiderIngredientTime;
    vector<double> remainingIngredients;
    vector<double>  timeProfile;
    
    double reservation_level;
    double reservation_quest;
    double reservation_total;
    double total;
    
    bool isAvailable;
    double currentQuantity;
};

class buildingInfo
{
    public:
    buildingInfo(string newName, int newSlot)
    {
        name = newName;
        slot = newSlot;
    }
    
    static int findBuilding(string buildingName, vector<buildingInfo>& buildingList)
    {
        for(unsigned int i = 0; i<buildingList.size(); i++)
        {
            if(buildingName == buildingList[i].name) return i;
        }
        
        cout<<"Error: cannot find building: "<<buildingName<<endl;
        return -1;
    }
    
    string name;
    int slot;
};

class ingredientInfo
{
    public:
    ingredientInfo(string itemName, double newQuantity, vector<itemInfo>& itemList)
    {
        itemIndex = itemInfo::findItem(itemName,itemList);
        quantity = newQuantity;
    }
    int itemIndex;
    double quantity;
};

class productInfo
{
    public:
    productInfo(string itemName, double newQuantity, vector<itemInfo>& itemList)
    {
        itemIndex = itemInfo::findItem(itemName,itemList);
        quantity = newQuantity;
    }
    int itemIndex;
    double quantity;
};

class procedureInfo
{
    public:
    
    //0->1
    procedureInfo(string buildingName,
                  double newTime,
                  string product1, double pQuantity1,
                  vector<buildingInfo>& buildingList, vector<itemInfo>& itemList)
    {
        buildingIndex = buildingInfo::findBuilding(buildingName,buildingList);
        time = newTime;
        ingredients.clear();
        products.clear();
        products.push_back(productInfo(product1,pQuantity1,itemList));
    }
    
    //1->1
    procedureInfo(string buildingName,
                  string ingredient1, double iQuantity1,
                  double newTime,
                  string product1, double pQuantity1,
                  vector<buildingInfo>& buildingList, vector<itemInfo>& itemList)
    {
        buildingIndex = buildingInfo::findBuilding(buildingName,buildingList);
        time = newTime;
        ingredients.clear();
        products.clear();
        ingredients.push_back(ingredientInfo(ingredient1,iQuantity1,itemList));
        products.push_back(productInfo(product1,pQuantity1,itemList));
    }
    
    //2->1
    procedureInfo(string buildingName,
                  string ingredient1, double iQuantity1,
                  string ingredient2, double iQuantity2,
                  double newTime,
                  string product1, double pQuantity1,
                  vector<buildingInfo>& buildingList, vector<itemInfo>& itemList)
    {
        buildingIndex = buildingInfo::findBuilding(buildingName,buildingList);
        time = newTime;
        ingredients.clear();
        products.clear();
        ingredients.push_back(ingredientInfo(ingredient1,iQuantity1,itemList));
        ingredients.push_back(ingredientInfo(ingredient2,iQuantity2,itemList));
        products.push_back(productInfo(product1,pQuantity1,itemList));
    }
    
    //3->1
    procedureInfo(string buildingName,
                  string ingredient1, double iQuantity1,
                  string ingredient2, double iQuantity2,
                  string ingredient3, double iQuantity3,
                  double newTime,
                  string product1, double pQuantity1,
                  vector<buildingInfo>& buildingList, vector<itemInfo>& itemList)
    {
        buildingIndex = buildingInfo::findBuilding(buildingName,buildingList);
        time = newTime;
        ingredients.clear();
        products.clear();
        ingredients.push_back(ingredientInfo(ingredient1,iQuantity1,itemList));
        ingredients.push_back(ingredientInfo(ingredient2,iQuantity2,itemList));
        ingredients.push_back(ingredientInfo(ingredient3,iQuantity3,itemList));
        products.push_back(productInfo(product1,pQuantity1,itemList));
    }
    
    //1->2
    procedureInfo(string buildingName,
                  string ingredient1, double iQuantity1,
                  double newTime,
                  string product1, double pQuantity1,
                  string product2, double pQuantity2,
                  vector<buildingInfo>& buildingList, vector<itemInfo>& itemList)
    {
        buildingIndex = buildingInfo::findBuilding(buildingName,buildingList);
        time = newTime;
        ingredients.clear();
        products.clear();
        ingredients.push_back(ingredientInfo(ingredient1,iQuantity1,itemList));
        products.push_back(productInfo(product1,pQuantity1,itemList));
        products.push_back(productInfo(product2,pQuantity2,itemList));
    }
    
    void setTimePerProcedure(vector<buildingInfo>& buildingList);
    
    double time; //in seconds
    int buildingIndex;
    vector<ingredientInfo> ingredients;
    vector<productInfo> products;
    
    double timePerProcedure;
};

void itemInfo::setTimePerPiece(vector<itemInfo>& itemList, vector<procedureInfo>& procedureList, vector<buildingInfo>& buildingList)
{
    //initialize timeProfile
    vector<double> timeProfile2;
    for(unsigned int i = 0; i<buildingList.size(); i++)
    {
        timeProfile.push_back(0);
        timeProfile2.push_back(0);
    }
    
    //remainingIngredients
    for(unsigned int i = 0; i<itemList.size(); i++)
    {
        remainingIngredients.push_back(0);
    }
    
    //Fill timeProfile and remainingIngredients
    if(procedureIndex.size() == 1 && !isRaw)
    {
        int ProcedureIndex = procedureIndex[0];
        
        //Get productQuantity
        double productQuantity = 0;
        for(unsigned int i = 0; i<procedureList[ProcedureIndex].products.size(); i++)
        {
            int productIndex = procedureList[ProcedureIndex].products[i].itemIndex;
            if(itemList[productIndex].name == name)
            {
                productQuantity = procedureList[ProcedureIndex].products[i].quantity;
            }
        }
        
        //Fill the time per piece for the procedure
        timeProfile[procedureList[ProcedureIndex].buildingIndex] += procedureList[ProcedureIndex].timePerProcedure / productQuantity;
        
        //Fill the time needed for all ingredients
        for(unsigned int i = 0; i<procedureList[ProcedureIndex].ingredients.size(); i++)
        {
            int ingredientIndex = procedureList[ProcedureIndex].ingredients[i].itemIndex;
            for(unsigned int j = 0; j<buildingList.size(); j++)
            {
                timeProfile[j] += itemList[ingredientIndex].timeProfile[j] * procedureList[ProcedureIndex].ingredients[i].quantity / productQuantity;
            }
            
            for(unsigned int j = 0; j<itemList.size(); j++)
            {
                remainingIngredients[j] += itemList[ingredientIndex].remainingIngredients[j] * procedureList[ProcedureIndex].ingredients[i].quantity / productQuantity;
            }
        }
    }
    else
    {
        int itemIndex = itemInfo::findItem(name,itemList);
        remainingIngredients[itemIndex] += 1;
    }
    
    //copy timeProfile to timeProfile2
    for(unsigned int i = 0; i<buildingList.size(); i++)
    {
        timeProfile2[i] = timeProfile[i];
    }
    
    for(unsigned int i = 0; i<itemList.size(); i++)
    {
        if(remainingIngredients[i] == 0) continue;
        if(itemList[i].procedureIndex.size() >= 2) continue;
        if(isConsiderIngredientTime || isRaw)
        {
            //Fill timeProfile
            int ProcedureIndex = itemList[i].procedureIndex[0];
            
            //Get productQuantity
            double productQuantity = 0;
            for(unsigned int j = 0; j<procedureList[ProcedureIndex].products.size(); j++)
            {
                int productIndex = procedureList[ProcedureIndex].products[j].itemIndex;
                if(i == productIndex)
                {
                    productQuantity = procedureList[ProcedureIndex].products[j].quantity;
                }
            }
            
            timeProfile2[procedureList[ProcedureIndex].buildingIndex] += procedureList[ProcedureIndex].timePerProcedure *remainingIngredients[i] / productQuantity;
        }
    }
    
    for(unsigned int i = 0; i<itemList.size(); i++)
    {
        if(remainingIngredients[i] == 0) continue;
        if(itemList[i].procedureIndex.size() <= 1) continue;
        if(isConsiderIngredientTime || isRaw)
        {
            //coal
            double PiecePerTime = 0;
            PiecePerTime += (1.0 + 0.7 + 0.59 + 0.54 + 0.48 + 0.43 + 0.38 + 0.33 + 0.27 + 0.22)*15 /60;
            PiecePerTime += 50.0/90;
            
            timePerPiece = 1/PiecePerTime;
            pricePerTime = price / timePerPiece;
            return;
        }
    }
    
    double max = 0;
    for(unsigned int i = 0; i<buildingList.size(); i++)
    {
        if(timeProfile2[i] > max) max = timeProfile2[i];
    }
    
    timePerPiece = max;
    pricePerTime = price / timePerPiece;
}

void procedureInfo::setTimePerProcedure(vector<buildingInfo>& buildingList)
{
    if(buildingList[buildingIndex].name == "mining station")
    {
        timePerProcedure = 60;
    }
    else
    {
        timePerProcedure = time / buildingList[buildingIndex].slot;
    }
}

void itemInfo::set_reservation_level(vector<itemInfo>& itemList, vector<ingredientInfo>& level)
{
    for(unsigned int i = 0; i<level.size(); i++)
    {
        itemList[ level[i].itemIndex ].reservation_level += level[i].quantity;
    }
}

void itemInfo::set_reservation_quest(vector<itemInfo>& itemList, vector<ingredientInfo>& quest)
{
    for(unsigned int i = 0; i<quest.size(); i++)
    {
        itemList[ quest[i].itemIndex ].reservation_quest += quest[i].quantity;
    }
}

void GainResources(string itemName, double quantity, vector<ingredientInfo>& quest, vector<itemInfo>& itemList, vector<procedureInfo>& procedureList)
{
    int itemIndex = itemInfo::findItem(itemName,itemList);
    int procedureIndex = itemList[itemIndex].procedureIndex[0];
    for(unsigned int i = 0; i<procedureList[procedureIndex].ingredients.size(); i++)
    {
        int ingredientIndex = procedureList[procedureIndex].ingredients[i].itemIndex;
        double totalQuantity = procedureList[procedureIndex].ingredients[i].quantity * (double(quantity) / procedureList[procedureIndex].products[0].quantity) ;
        quest.push_back(ingredientInfo(itemList[ingredientIndex].name, totalQuantity, itemList));
    }
    
    quest.push_back(ingredientInfo(itemList[itemIndex].name, - quantity, itemList));
}

void printHierarchy(vector< vector<int> >& hierarchy, vector<itemInfo>& itemList, vector<procedureInfo>& procedureList)
{
    for(unsigned int i = 0; i<hierarchy.size(); i++)
    {
        cout<<"Tier "<<i<<":"<<endl;
        for(unsigned int j = 0; j<hierarchy[i].size(); j++)
        {
            int itemIndex = hierarchy[i][j];
            double quantity_diff = itemList[itemIndex].total - itemList[itemIndex].currentQuantity;
            if(quantity_diff <= 0) continue;
            if(!itemList[itemIndex].isAvailable) continue;
            
            cout<<std::setw(20)<<itemList[itemIndex].name;
            if(itemList[itemIndex].ToMake.size() == 0) cout<<" (*)";
            else cout<<"    ";
            
            if(quantity_diff <= 0) cout<<"  OK";
            else if(!itemList[itemIndex].isAvailable) cout<<"  NA";
            else if(itemList[itemIndex].reservation_total == itemList[itemIndex].total) cout<<" END"; //is end
            else if(quantity_diff <= itemList[itemIndex].reservation_total) cout<<" SLF"; //to self-fill
            else cout<<" TRF"; //transfer
            
            cout<<":";
            
            cout<<std::setw(5)<<itemList[itemIndex].reservation_level<<" ";
            cout<<std::setw(6)<<itemList[itemIndex].reservation_quest<<" ";
            cout<<std::setw(7)<<ceil(itemList[itemIndex].reservation_total)<<" ";
            cout<<std::setw(7)<<int(ceil(itemList[itemIndex].total));
            if(quantity_diff <= 0) cout<<std::setw(8)<<0;
            else cout<<std::setw(8)<<int(ceil(quantity_diff));
            cout<<":";
            
            int procedureIndex = itemList[itemIndex].procedureIndex[0];
            for(unsigned int k = 0; k<procedureList[procedureIndex].ingredients.size(); k++)
            {
                int ingredientIndex = procedureList[procedureIndex].ingredients[k].itemIndex;
                double ingredientQuantity = procedureList[procedureIndex].ingredients[k].quantity;
                cout<<std::setw(20)<<itemList[ingredientIndex].name<<" "<<std::setw(4)<<ingredientQuantity<<" ";
            }
            cout<<endl;
        }
    }
}

int main()
{
    //item list
    vector<itemInfo> itemList;
    
    //Raw
    itemList.push_back(itemInfo("coal",1));
    itemList.push_back(itemInfo("copper",2));
    itemList.push_back(itemInfo("iron",3));
    itemList.push_back(itemInfo("amber",4));
    itemList.push_back(itemInfo("aluminium",5));
    itemList.push_back(itemInfo("silver",7));
    itemList.push_back(itemInfo("gold",10));
    itemList.push_back(itemInfo("emerald",12));
    itemList.push_back(itemInfo("platinum",13));
    itemList.push_back(itemInfo("topaz",14));
    itemList.push_back(itemInfo("ruby",15));
    itemList.push_back(itemInfo("sapphire",16));
    itemList.push_back(itemInfo("amethyst",18));
    itemList.push_back(itemInfo("diamond",18));
    itemList.push_back(itemInfo("titanium ore",19));
    itemList.push_back(itemInfo("alexandrite",19));
    itemList.push_back(itemInfo("uranium",22));
    
    itemList.push_back(itemInfo("obsidian",20));
    itemList.push_back(itemInfo("helium 3",400));
    itemList.push_back(itemInfo("sodium chloride",100));
    itemList.push_back(itemInfo("lutetium ore",500));
    
    //smelting
    itemList.push_back(itemInfo("copper bar",25));
    itemList.push_back(itemInfo("iron bar",40));
    itemList.push_back(itemInfo("aluminium bar",50));
    itemList.push_back(itemInfo("steel bar",180));
    itemList.push_back(itemInfo("silver bar",200));
    itemList.push_back(itemInfo("gold bar",250));
    itemList.push_back(itemInfo("lab flask",800));
    itemList.push_back(itemInfo("steel plate",1800));
    itemList.push_back(itemInfo("titanium bar",3000));
    itemList.push_back(itemInfo("diamond cutter",5000));
    itemList.push_back(itemInfo("motherboard",17000));
    
    itemList.push_back(itemInfo("magnetite bar",137000));
    itemList.push_back(itemInfo("lutetium bar",68000));
    
    //craft
    itemList.push_back(itemInfo("copper nail",7));
    itemList.push_back(itemInfo("amber insulation",125));
    itemList.push_back(itemInfo("solar panel",69000));
    itemList.push_back(itemInfo("graphite",15));
    itemList.push_back(itemInfo("green laser",400));
    itemList.push_back(itemInfo("wire",15));
    itemList.push_back(itemInfo("insulated wire",750));
    itemList.push_back(itemInfo("amber charger",4));
    itemList.push_back(itemInfo("aluminium bottle",55));
    itemList.push_back(itemInfo("bomb",55500));
    itemList.push_back(itemInfo("gear",18500));
    itemList.push_back(itemInfo("battery",200));
    itemList.push_back(itemInfo("lamp",760));
    itemList.push_back(itemInfo("accumulator",9000));
    itemList.push_back(itemInfo("solid propellant",27000));
    itemList.push_back(itemInfo("circuits",2070));
    
    itemList.push_back(itemInfo("compressor",44000));
    itemList.push_back(itemInfo("optic fiber",10500));
    itemList.push_back(itemInfo("dry ice",140000));
    itemList.push_back(itemInfo("magnet",300000));
    itemList.push_back(itemInfo("electrical engine",745000));
    itemList.push_back(itemInfo("chipset",40000));
    
    //chemistry
    itemList.push_back(itemInfo("water",5));
    itemList.push_back(itemInfo("oil",21));
    itemList.push_back(itemInfo("sodium",100));
    itemList.push_back(itemInfo("sulfur",100));
    itemList.push_back(itemInfo("silicon",100));
    itemList.push_back(itemInfo("titanium",260));
    itemList.push_back(itemInfo("nitrogen",300));
    itemList.push_back(itemInfo("hydrogen",400));
    itemList.push_back(itemInfo("oxygen",900)); //hydrogen and oxygen need to be continuous, for calculating the number of clean water
    itemList.push_back(itemInfo("glass",450));
    itemList.push_back(itemInfo("clean water",1200));
    itemList.push_back(itemInfo("gunpowder",2500));
    itemList.push_back(itemInfo("sulfuric acid",3500));
    itemList.push_back(itemInfo("rubber",4000));
    itemList.push_back(itemInfo("ethanol",4200));
    itemList.push_back(itemInfo("refined oil",16500));
    itemList.push_back(itemInfo("diethyl ether",17000));
    itemList.push_back(itemInfo("uranium rod",17000));
    itemList.push_back(itemInfo("plastic plate",40000));
    
    itemList.push_back(itemInfo("liquid nitrogen",12500));
    itemList.push_back(itemInfo("magnetite ore",12500));
    itemList.push_back(itemInfo("enhanced helium 3",190000));
    itemList.push_back(itemInfo("toxic bomb",77500));
    itemList.push_back(itemInfo("hydrochloric acid",12000));
    itemList.push_back(itemInfo("lutetium",13500));
    
    //jewel
    int tradingLevel = 100;
    double SF = (1 + 0.02 * (tradingLevel-1)) * 1.35;
    itemList.push_back(itemInfo("polished amber",70));
    itemList.push_back(itemInfo("polished emerald",160));
    itemList.push_back(itemInfo("polished topaz",200));
    itemList.push_back(itemInfo("polished sapphire",230));
    itemList.push_back(itemInfo("polished ruby",250));
    itemList.push_back(itemInfo("polished amethyst",250));
    itemList.push_back(itemInfo("polished alexandrite",270));
    itemList.push_back(itemInfo("amber bracelet",280*SF));
    itemList.push_back(itemInfo("polished diamond",300));
    itemList.push_back(itemInfo("emerald ring",450*SF));
    itemList.push_back(itemInfo("maya calendar",6000*SF));
    itemList.push_back(itemInfo("haircomb",14000*SF));
    
    itemList.push_back(itemInfo("polished obsidian",280));
    itemList.push_back(itemInfo("obsidian knife",32000*SF));
    
    //tree
    itemList.push_back(itemInfo("tree seed",20));
    itemList.push_back(itemInfo("tree",193));
    itemList.push_back(itemInfo("liana seed",1000));
    itemList.push_back(itemInfo("grape seed",1200));
    itemList.push_back(itemInfo("grape",1500));
    itemList.push_back(itemInfo("liana",1700));
    
    //mining station
    vector<buildingInfo> buildingList;
    buildingList.push_back(buildingInfo("mining station",0));
    
    const double BasicMiningRate = 15;
    double miningRate;
    vector<procedureInfo> procedureList;
    miningRate = (1 + 0.7 + 0.59 + 0.54 + 0.48 + 0.43 + 0.38 + 0.33 + 0.27 + 0.22) * BasicMiningRate;
    procedureList.push_back(procedureInfo("mining station",60,"coal",miningRate,buildingList,itemList));
    miningRate = BasicMiningRate;
    procedureList.push_back(procedureInfo("mining station",60,"copper",miningRate,buildingList,itemList));
    miningRate = (0.11 + 0.12 + 0.13 + 0.14 + 0.15 + 0.16 + 0.30) * BasicMiningRate;
    procedureList.push_back(procedureInfo("mining station",60,"iron",miningRate,buildingList,itemList));
    miningRate = BasicMiningRate;
    procedureList.push_back(procedureInfo("mining station",60,"amber",miningRate,buildingList,itemList));
    miningRate = BasicMiningRate;
    procedureList.push_back(procedureInfo("mining station",60,"aluminium",miningRate,buildingList,itemList));
    miningRate = (0.11 + 0.11 + 0.12 + 0.12 + 0.13 + 0.14 + 0.14 + 0.15 + 0.15 + 0.16 + 0.14 + 0.12 + 0.11) * BasicMiningRate;
    procedureList.push_back(procedureInfo("mining station",60,"silver",miningRate,buildingList,itemList));
    miningRate = (0.15 + 0.20 + 0.25 + 0.30 + 0.35 + 0.40 + 0.45 + 0.50 + 0.55 + 1 + 0.70 + 0.50 + 0.45 + 0.40 + 0.35 + 0.30 + 0.25 + 0.19 + 0.15) * BasicMiningRate;
    procedureList.push_back(procedureInfo("mining station",60,"gold",miningRate,buildingList,itemList));
    miningRate = BasicMiningRate;
    procedureList.push_back(procedureInfo("mining station",60,"emerald",miningRate,buildingList,itemList));
    miningRate = BasicMiningRate;
    procedureList.push_back(procedureInfo("mining station",60,"platinum",miningRate,buildingList,itemList));
    miningRate = BasicMiningRate;
    procedureList.push_back(procedureInfo("mining station",60,"topaz",miningRate,buildingList,itemList));
    miningRate = BasicMiningRate;
    procedureList.push_back(procedureInfo("mining station",60,"ruby",miningRate,buildingList,itemList));
    miningRate = BasicMiningRate;
    procedureList.push_back(procedureInfo("mining station",60,"sapphire",miningRate,buildingList,itemList));
    miningRate = BasicMiningRate;
    procedureList.push_back(procedureInfo("mining station",60,"amethyst",miningRate,buildingList,itemList));
    miningRate = (0.11 + 0.13 + 0.15 + 0.16 + 0.18 + 0.16 + 0.15 + 0.13) * BasicMiningRate;
    procedureList.push_back(procedureInfo("mining station",60,"diamond",miningRate,buildingList,itemList));
    miningRate = (0.13 + 0.15 + 0.16 + 0.18) * BasicMiningRate;
    procedureList.push_back(procedureInfo("mining station",60,"titanium ore",miningRate,buildingList,itemList));
    miningRate = BasicMiningRate;
    procedureList.push_back(procedureInfo("mining station",60,"alexandrite",miningRate,buildingList,itemList));
    miningRate = BasicMiningRate;
    procedureList.push_back(procedureInfo("mining station",60,"uranium",miningRate,buildingList,itemList));
    miningRate = BasicMiningRate;
    procedureList.push_back(procedureInfo("mining station",60,"obsidian",miningRate,buildingList,itemList));
    miningRate = BasicMiningRate;
    procedureList.push_back(procedureInfo("mining station",60,"helium 3",miningRate,buildingList,itemList));
    
    //chemical mining
    buildingList.push_back(buildingInfo("chemical mining",8));
    procedureList.push_back(procedureInfo("chemical mining",10*60,"silicon",20,buildingList,itemList));
    procedureList.push_back(procedureInfo("chemical mining",10*60,"sulfur",20,buildingList,itemList));
    procedureList.push_back(procedureInfo("chemical mining",10*60,"sodium",20,buildingList,itemList));
    procedureList.push_back(procedureInfo("chemical mining",10*60,"nitrogen",20,buildingList,itemList));
    
    //oil mining
    buildingList.push_back(buildingInfo("oil mining",1));
    procedureList.push_back(procedureInfo("oil mining",60*60,"oil",7,buildingList,itemList));
    
    //Asteroid Mining
    miningRate = BasicMiningRate;
    procedureList.push_back(procedureInfo("mining station",60,"sodium chloride",miningRate,buildingList,itemList));
    miningRate = BasicMiningRate;
    procedureList.push_back(procedureInfo("mining station",60,"lutetium ore",miningRate,buildingList,itemList));
    
    //water collector
    buildingList.push_back(buildingInfo("water collector",1));
    procedureList.push_back(procedureInfo("water collector",60,"water",7,buildingList,itemList));
    
    //seed store
    buildingList.push_back(buildingInfo("seed store",1));
    procedureList.push_back(procedureInfo("seed store",0,"tree seed",1,buildingList,itemList));
    procedureList.push_back(procedureInfo("seed store",0,"liana seed",1,buildingList,itemList));
    procedureList.push_back(procedureInfo("seed store",0,"grape seed",1,buildingList,itemList));
    
    //greenhouse
    buildingList.push_back(buildingInfo("greenhouse",2));
    procedureList.push_back(procedureInfo("greenhouse","tree seed",1,"water",10,30*60,"tree",10,buildingList,itemList));
    procedureList.push_back(procedureInfo("greenhouse","liana seed",1,"water",20,30*60,"liana",1,buildingList,itemList));
    procedureList.push_back(procedureInfo("greenhouse","grape seed",1,"water",15,30*60,"grape",2,buildingList,itemList));
    
    //smelting
    buildingList.push_back(buildingInfo("smelting",8));
    procedureList.push_back(procedureInfo("smelting","copper",5,10,"copper bar",1,buildingList,itemList));
    procedureList.push_back(procedureInfo("smelting","iron",5,15,"iron bar",1,buildingList,itemList));
    procedureList.push_back(procedureInfo("smelting","silicon",2,60,"glass",1,buildingList,itemList));
    procedureList.push_back(procedureInfo("smelting","aluminium",5,15,"aluminium bar",1,buildingList,itemList));
    procedureList.push_back(procedureInfo("smelting","graphite",1,"iron bar",1,45,"steel bar",1,buildingList,itemList));
    procedureList.push_back(procedureInfo("smelting","silver",5,60,"silver bar",1,buildingList,itemList));
    procedureList.push_back(procedureInfo("smelting","tree",1,60,"coal",50,buildingList,itemList));
    procedureList.push_back(procedureInfo("smelting","gold",5,60,"gold bar",1,buildingList,itemList));
    procedureList.push_back(procedureInfo("smelting","steel bar",5,2*60,"steel plate",1,buildingList,itemList));
    procedureList.push_back(procedureInfo("smelting","titanium",5,60,"titanium bar",1,buildingList,itemList));
    procedureList.push_back(procedureInfo("smelting","magnetite ore",5,60,"magnetite bar",1,buildingList,itemList));
    procedureList.push_back(procedureInfo("smelting","lutetium",5,60,"lutetium bar",1,buildingList,itemList));
    
    //crafting
    buildingList.push_back(buildingInfo("crafting",8));
    procedureList.push_back(procedureInfo("crafting","coal",5,5,"graphite",1,buildingList,itemList));
    procedureList.push_back(procedureInfo("crafting","copper bar",1,20,"copper nail",10,buildingList,itemList));
    procedureList.push_back(procedureInfo("crafting","copper bar",1,30,"wire",5,buildingList,itemList));
    procedureList.push_back(procedureInfo("crafting","amber",1,"iron bar",1,"copper bar",5,2*60,"battery",1,buildingList,itemList));
    procedureList.push_back(procedureInfo("crafting","iron bar",10,"graphite",50,"copper bar",20,3*60,"circuits",1,buildingList,itemList));
    procedureList.push_back(procedureInfo("crafting","copper bar",5,"wire",10,"graphite",20,80,"lamp",1,buildingList,itemList));
    procedureList.push_back(procedureInfo("crafting","glass",1,60,"lab flask",1,buildingList,itemList));
    procedureList.push_back(procedureInfo("crafting","amber",1,5,"amber charger",1,buildingList,itemList));
    procedureList.push_back(procedureInfo("crafting","aluminium bar",1,30,"aluminium bottle",1,buildingList,itemList));
    procedureList.push_back(procedureInfo("crafting","amber",10,"aluminium bottle",1,20,"amber insulation",1,buildingList,itemList));
    procedureList.push_back(procedureInfo("crafting","wire",1,"amber insulation",1,3*60+20,"insulated wire",1,buildingList,itemList));
    procedureList.push_back(procedureInfo("crafting","polished emerald",1,"insulated wire",1,"lamp",1,20,"green laser",5,buildingList,itemList));
    procedureList.push_back(procedureInfo("crafting","steel plate",1,"polished diamond",5,30,"diamond cutter",1,buildingList,itemList));
    procedureList.push_back(procedureInfo("crafting","gold bar",1,"circuits",3,"silicon",3,30*60,"motherboard",1,buildingList,itemList));
    procedureList.push_back(procedureInfo("crafting","aluminium bar",1,"rubber",3,20*60,"solid propellant",1,buildingList,itemList));
    procedureList.push_back(procedureInfo("crafting","sulfur",20,"sodium",20,3*60,"accumulator",1,buildingList,itemList));
    procedureList.push_back(procedureInfo("crafting","rubber",3,"silicon",10,"glass",50,60,"solar panel",1,buildingList,itemList));
    procedureList.push_back(procedureInfo("crafting","diamond cutter",1,"titanium bar",1,80,"gear",1,buildingList,itemList));
    procedureList.push_back(procedureInfo("crafting","steel bar",5,"gunpowder",10,3*60,"bomb",1,buildingList,itemList));
    procedureList.push_back(procedureInfo("crafting","refined oil",2,"rubber",1,"iron bar",5,3*60,"compressor",1,buildingList,itemList));
    procedureList.push_back(procedureInfo("crafting","plastic plate",1,"oxygen",10,"silicon",10,2*60,"optic fiber",10,buildingList,itemList));
    procedureList.push_back(procedureInfo("crafting","graphite",1000,"compressor",1,"green laser",10,2*60,"dry ice",1,buildingList,itemList));
    procedureList.push_back(procedureInfo("crafting","magnetite bar",1,2*60,"magnet",1,buildingList,itemList));
    procedureList.push_back(procedureInfo("crafting","insulated wire",50,"magnet",1,"aluminium bar",20,5*60,"electrical engine",1,buildingList,itemList));
    procedureList.push_back(procedureInfo("crafting","silver bar",1,"hydrochloric acid",1,"circuits",3,60,"chipset",1,buildingList,itemList));
    
    //chemistry
    buildingList.push_back(buildingInfo("chemistry",5));
    procedureList.push_back(procedureInfo("chemistry","lab flask",1,"water",1,10*60,"clean water",1,buildingList,itemList));
    procedureList.push_back(procedureInfo("chemistry","clean water",1,15*60,"hydrogen",2,"oxygen",1,buildingList,itemList));
    procedureList.push_back(procedureInfo("chemistry","liana",1,30*60,"rubber",2,buildingList,itemList));
    procedureList.push_back(procedureInfo("chemistry","sulfur",2,"clean water",1,30*60,"sulfuric acid",1,buildingList,itemList));
    procedureList.push_back(procedureInfo("chemistry","aluminium bottle",1,"grape",2,30*60,"ethanol",1,buildingList,itemList));
    procedureList.push_back(procedureInfo("chemistry","oil",10,"hydrogen",10,"lab flask",1,30*60,"refined oil",1,buildingList,itemList));
    procedureList.push_back(procedureInfo("chemistry","green laser",1,"coal",50,"refined oil",1,10*60,"plastic plate",1,buildingList,itemList));
    procedureList.push_back(procedureInfo("chemistry","sulfuric acid",1,"titanium ore",100,20,"titanium",50,buildingList,itemList));
    procedureList.push_back(procedureInfo("chemistry","sulfuric acid",1,"ethanol",1,60,"diethyl ether",1,buildingList,itemList));
    procedureList.push_back(procedureInfo("chemistry","diethyl ether",1,"sulfuric acid",2,"tree",2,2*60,"gunpowder",20,buildingList,itemList));
    procedureList.push_back(procedureInfo("chemistry","aluminium bottle",1,"nitrogen",10,"compressor",1,2*60,"liquid nitrogen",4,buildingList,itemList));
    procedureList.push_back(procedureInfo("chemistry","oxygen",5,"iron bar",10,"green laser",5,6*60,"magnetite ore",1,buildingList,itemList));
    procedureList.push_back(procedureInfo("chemistry","aluminium bottle",1,"helium 3",100,"compressor",1,30*60,"enhanced helium 3",1,buildingList,itemList));
    procedureList.push_back(procedureInfo("chemistry","sulfuric acid",10,2*60,"toxic bomb",10,buildingList,itemList));
    procedureList.push_back(procedureInfo("chemistry","sulfuric acid",1,"sodium chloride",20,2*60,"hydrochloric acid",4,buildingList,itemList));
    procedureList.push_back(procedureInfo("chemistry","sulfuric acid",1,"lutetium ore",20,10*60,"lutetium",1,buildingList,itemList));
    
    //jewel crafting
    buildingList.push_back(buildingInfo("jewel crafting",4));
    procedureList.push_back(procedureInfo("jewel crafting","amber",5,30,"polished amber",1,buildingList,itemList));
    procedureList.push_back(procedureInfo("jewel crafting","gold bar",1,"polished emerald",1,5*60,"emerald ring",1,buildingList,itemList));
    procedureList.push_back(procedureInfo("jewel crafting","silver bar",1,"polished amber",1,2*60,"amber bracelet",1,buildingList,itemList));
    procedureList.push_back(procedureInfo("jewel crafting","silver bar",2,"gold bar",10,2*60,"maya calendar",1,buildingList,itemList));
    procedureList.push_back(procedureInfo("jewel crafting","silver bar",1,"polished amethyst",15,"polished alexandrite",10,2*60,"haircomb",1,buildingList,itemList));
    procedureList.push_back(procedureInfo("jewel crafting","silver bar",1,"tree",2,"polished obsidian",50,2*60,"obsidian knife",1,buildingList,itemList));
    procedureList.push_back(procedureInfo("jewel crafting","emerald",5,30,"polished emerald",1,buildingList,itemList));
    procedureList.push_back(procedureInfo("jewel crafting","topaz",5,60,"polished topaz",1,buildingList,itemList));
    procedureList.push_back(procedureInfo("jewel crafting","ruby",5,60,"polished ruby",1,buildingList,itemList));
    procedureList.push_back(procedureInfo("jewel crafting","diamond",5,60,"polished diamond",1,buildingList,itemList));
    procedureList.push_back(procedureInfo("jewel crafting","sapphire",5,60,"polished sapphire",1,buildingList,itemList));
    procedureList.push_back(procedureInfo("jewel crafting","amethyst",5,60,"polished amethyst",1,buildingList,itemList));
    procedureList.push_back(procedureInfo("jewel crafting","alexandrite",5,60,"polished alexandrite",1,buildingList,itemList));
    procedureList.push_back(procedureInfo("jewel crafting","obsidian",5,60,"polished obsidian",1,buildingList,itemList));
    
    //uranium enrichment
    buildingList.push_back(buildingInfo("uranium enrichment",2));
    procedureList.push_back(procedureInfo("uranium enrichment","uranium",100,"sodium",50,10*60,"uranium rod",1,buildingList,itemList));
    
    //reservation for level upgrade
    vector<ingredientInfo> level;
    //level 300
    level.push_back(ingredientInfo("gear",30,itemList));
    level.push_back(ingredientInfo("copper nail",3000,itemList));
    level.push_back(ingredientInfo("plastic plate",15,itemList));
    level.push_back(ingredientInfo("polished topaz",200,itemList));
    level.push_back(ingredientInfo("motherboard",35,itemList));
    level.push_back(ingredientInfo("hydrogen",100,itemList));
    level.push_back(ingredientInfo("rubber",100,itemList));
    level.push_back(ingredientInfo("gold bar",1000,itemList));
    level.push_back(ingredientInfo("insulated wire",550,itemList));
    level.push_back(ingredientInfo("accumulator",40,itemList));
    
    //level 330
    level.push_back(ingredientInfo("refined oil",40,itemList));
    level.push_back(ingredientInfo("silver bar",500,itemList));
    level.push_back(ingredientInfo("steel plate",100,itemList));
    level.push_back(ingredientInfo("polished sapphire",1000,itemList));
    level.push_back(ingredientInfo("tree",100,itemList));
    level.push_back(ingredientInfo("solar panel",12,itemList));
    level.push_back(ingredientInfo("grape",150,itemList));
    level.push_back(ingredientInfo("polished amethyst",1000,itemList));
    
    //level 340
    level.push_back(ingredientInfo("optic fiber",50,itemList));
    level.push_back(ingredientInfo("polished obsidian",1500,itemList));
    level.push_back(ingredientInfo("copper bar",8700,itemList));
    level.push_back(ingredientInfo("polished amber",3000,itemList));
    level.push_back(ingredientInfo("diethyl ether",50,itemList));
    level.push_back(ingredientInfo("polished alexandrite",500,itemList));
    level.push_back(ingredientInfo("gold bar",1000,itemList));
    level.push_back(ingredientInfo("rubber",50,itemList));
    
    //level 350
    level.push_back(ingredientInfo("gear",55,itemList));
    level.push_back(ingredientInfo("oxygen",60,itemList));
    level.push_back(ingredientInfo("aluminium bar",5000,itemList));
    level.push_back(ingredientInfo("polished emerald",3600,itemList));
    level.push_back(ingredientInfo("solar panel",15,itemList));
    level.push_back(ingredientInfo("clean water",200,itemList));
    level.push_back(ingredientInfo("tree",1350,itemList));
    level.push_back(ingredientInfo("polished diamond",900,itemList));
    
    //level 360
    level.push_back(ingredientInfo("diamond cutter",200,itemList));
    level.push_back(ingredientInfo("motherboard",20,itemList));
    level.push_back(ingredientInfo("polished topaz",1500,itemList));
    level.push_back(ingredientInfo("graphite",19000,itemList));
    level.push_back(ingredientInfo("accumulator",60,itemList));
    level.push_back(ingredientInfo("lamp",1000,itemList));
    level.push_back(ingredientInfo("silver bar",1500,itemList));
    level.push_back(ingredientInfo("clean water",250,itemList));
    
    //level 370
    level.push_back(ingredientInfo("rubber",120,itemList));
    level.push_back(ingredientInfo("titanium bar",50,itemList));
    level.push_back(ingredientInfo("liana",195,itemList));
    level.push_back(ingredientInfo("sulfuric acid",95,itemList));
    level.push_back(ingredientInfo("sulfuric acid",50,itemList));
    level.push_back(ingredientInfo("steel plate",800,itemList));
    level.push_back(ingredientInfo("accumulator",40,itemList));
    level.push_back(ingredientInfo("titanium bar",120,itemList));
    
    //level 380
    level.push_back(ingredientInfo("plastic plate",60,itemList));
    level.push_back(ingredientInfo("polished topaz",500,itemList));
    level.push_back(ingredientInfo("grape",250,itemList));
    level.push_back(ingredientInfo("graphite",25600,itemList));
    level.push_back(ingredientInfo("iron bar",2500,itemList));
    level.push_back(ingredientInfo("polished ruby",400,itemList));
    level.push_back(ingredientInfo("hydrogen",1010,itemList));
    level.push_back(ingredientInfo("tree",2000,itemList));
    
    //level 390
    level.push_back(ingredientInfo("copper nail",15200,itemList));
    level.push_back(ingredientInfo("magnetite bar",1,itemList));
    level.push_back(ingredientInfo("polished sapphire",1900,itemList));
    level.push_back(ingredientInfo("ethanol",105,itemList));
    level.push_back(ingredientInfo("liana",67,itemList));
    level.push_back(ingredientInfo("wire",7580,itemList));
    level.push_back(ingredientInfo("silver bar",2350,itemList));
    level.push_back(ingredientInfo("polished amethyst",1900,itemList));
    
    //level 400
    level.push_back(ingredientInfo("titanium",470,itemList));
    level.push_back(ingredientInfo("grape",80,itemList));
    level.push_back(ingredientInfo("battery",2500,itemList));
    level.push_back(ingredientInfo("steel plate",280,itemList));
    level.push_back(ingredientInfo("polished obsidian",470,itemList));
    level.push_back(ingredientInfo("gunpowder",52,itemList));
    level.push_back(ingredientInfo("tree",2750,itemList));
    level.push_back(ingredientInfo("circuits",270,itemList));
    
    //level 410
    level.push_back(ingredientInfo("liana",82,itemList));
    level.push_back(ingredientInfo("liquid nitrogen",10,itemList));
    level.push_back(ingredientInfo("amber insulation",10500,itemList));
    level.push_back(ingredientInfo("copper bar",23300,itemList));
    level.push_back(ingredientInfo("polished amber",2150,itemList));
    level.push_back(ingredientInfo("magnetite ore",12,itemList));
    level.push_back(ingredientInfo("grape",410,itemList));
    level.push_back(ingredientInfo("lab flask",780,itemList));
    
    //level 420
    level.push_back(ingredientInfo("polished alexandrite",590,itemList));
    level.push_back(ingredientInfo("gold bar",650,itemList));
    level.push_back(ingredientInfo("enhanced helium 3",4,itemList));
    level.push_back(ingredientInfo("tree",3500,itemList));
    level.push_back(ingredientInfo("aluminium bar",3500,itemList));
    level.push_back(ingredientInfo("gear",10,itemList));
    level.push_back(ingredientInfo("liana",410,itemList));
    level.push_back(ingredientInfo("insulated wire",1000,itemList));
    
    //level 430
    level.push_back(ingredientInfo("silver bar",1000,itemList));
    level.push_back(ingredientInfo("polished emerald",2000,itemList));
    level.push_back(ingredientInfo("refined oil",50,itemList));
    level.push_back(ingredientInfo("grape",500,itemList));
    level.push_back(ingredientInfo("compressor",5,itemList));
    level.push_back(ingredientInfo("titanium bar",70,itemList));
    level.push_back(ingredientInfo("polished diamond",5000,itemList));
    level.push_back(ingredientInfo("diethyl ether",50,itemList));
    
    //level 440
    level.push_back(ingredientInfo("steel plate",150,itemList));
    level.push_back(ingredientInfo("electrical engine",1,itemList));
    
    //setTimePerProcedure
    for(unsigned int i = 0; i<procedureList.size(); i++)
    {
        procedureList[i].setTimePerProcedure(buildingList);
    }
    
    //set procedureIndex and ToMake
    //set isRaw
    for(unsigned int i = 0; i<itemList.size(); i++)
    {
        if(itemList[i].name == "coal") itemList[i].isRaw = true;
        
        int procedureIndex = 0;
        for(unsigned int j = 0; j<procedureList.size(); j++)
        {
            //set procedureIndex
            for(unsigned int k = 0; k<procedureList[j].products.size(); k++)
            {
                int productIndex = procedureList[j].products[k].itemIndex;
                if(i == productIndex)
                {
                    procedureIndex = j;
                    itemList[i].procedureIndex.push_back(j);
                    
                    //set isRaw
                    if(buildingList[ procedureList[j].buildingIndex ].name == "mining station" ||
                       buildingList[ procedureList[j].buildingIndex ].name == "chemical mining" ||
                       buildingList[ procedureList[j].buildingIndex ].name == "oil mining" ||
                       buildingList[ procedureList[j].buildingIndex ].name == "water collector" ||
                       buildingList[ procedureList[j].buildingIndex ].name == "seed store" )
                    {
                        itemList[i].isRaw = true;
                    }
                }
            }
        }
        
        //set ToMake
        for(unsigned int j = 0; j<procedureList[procedureIndex].ingredients.size(); j++)
        {
            int ingredientIndex = procedureList[procedureIndex].ingredients[j].itemIndex;
            for(unsigned int k = 0; k<procedureList[procedureIndex].products.size(); k++)
            {
                int productIndex = procedureList[procedureIndex].products[k].itemIndex;
                itemList[ingredientIndex].ToMake.push_back(productIndex);
            }
        }
    }
    
    //reservation for quest
    vector<ingredientInfo> quest;
    
    //New Gas: Nitrogen
    quest.push_back(ingredientInfo("motherboard",50,itemList));
    quest.push_back(ingredientInfo("insulated wire",1000,itemList));
    quest.push_back(ingredientInfo("steel plate",500,itemList));
    GainResources("nitrogen",10,quest,itemList,procedureList);
    GainResources("compressor",1,quest,itemList,procedureList);
    GainResources("liquid nitrogen",1,quest,itemList,procedureList);
    
    //Here I Go Crafting Again!
    GainResources("optic fiber",10+50,quest,itemList,procedureList);
    GainResources("compressor",10+50,quest,itemList,procedureList);
    
    //Quite The Goldsmith
    GainResources("polished obsidian",10+100,quest,itemList,procedureList);
    GainResources("polished sapphire",100+300,quest,itemList,procedureList);
    GainResources("polished diamond",100+300,quest,itemList,procedureList);
    
    //Providing The Weaponry
    quest.push_back(ingredientInfo("obsidian knife",10+50+100,itemList));
    
    //Stage I
    quest.push_back(ingredientInfo("aluminium bar",1250,itemList));
    quest.push_back(ingredientInfo("diamond cutter",225,itemList));
    quest.push_back(ingredientInfo("rubber",45,itemList));
    
    //fuel tech
    quest.push_back(ingredientInfo("compressor",30,itemList));
    quest.push_back(ingredientInfo("gunpowder",2000,itemList));
    quest.push_back(ingredientInfo("titanium bar",210,itemList));
    
    //fuel tank
    quest.push_back(ingredientInfo("solid propellant",20,itemList));
    quest.push_back(ingredientInfo("uranium rod",95,itemList));
    quest.push_back(ingredientInfo("enhanced helium 3",380,itemList));
    
    //rocket engine
    //Engine IR
    quest.push_back(ingredientInfo("aluminium bar",3120,itemList));
    quest.push_back(ingredientInfo("diamond cutter",310,itemList));
    quest.push_back(ingredientInfo("rubber",25,itemList));
    
    //Nozzle IR
    quest.push_back(ingredientInfo("graphite",15100,itemList));
    quest.push_back(ingredientInfo("aluminium bar",6250,itemList));
    quest.push_back(ingredientInfo("titanium bar",25,itemList));
    
    //Engine IL
    quest.push_back(ingredientInfo("aluminium bar",3120,itemList));
    quest.push_back(ingredientInfo("diamond cutter",310,itemList));
    quest.push_back(ingredientInfo("rubber",25,itemList));
    
    //Nozzle IL
    quest.push_back(ingredientInfo("graphite",15100,itemList));
    quest.push_back(ingredientInfo("aluminium bar",6250,itemList));
    quest.push_back(ingredientInfo("titanium bar",25,itemList));
    
    //Engine IC
    quest.push_back(ingredientInfo("aluminium bar",3120,itemList));
    quest.push_back(ingredientInfo("diamond cutter",310,itemList));
    quest.push_back(ingredientInfo("rubber",25,itemList));
    
    //Nozzle IC
    quest.push_back(ingredientInfo("graphite",15100,itemList));
    quest.push_back(ingredientInfo("aluminium bar",6250,itemList));
    quest.push_back(ingredientInfo("titanium bar",25,itemList));
    
    //Stage I Winglet
    quest.push_back(ingredientInfo("aluminium bar",8888,itemList));
    quest.push_back(ingredientInfo("steel plate",1111,itemList));
    quest.push_back(ingredientInfo("gear",100,itemList));
    
    //Decoupler I
    quest.push_back(ingredientInfo("copper nail",33350,itemList));
    quest.push_back(ingredientInfo("steel plate",777,itemList));
    quest.push_back(ingredientInfo("rubber",25,itemList));
    
    //stage II
    quest.push_back(ingredientInfo("aluminium bar",1250,itemList));
    quest.push_back(ingredientInfo("diamond cutter",225,itemList));
    quest.push_back(ingredientInfo("rubber",45,itemList));
    
    //fuel tech
    quest.push_back(ingredientInfo("compressor",30,itemList));
    quest.push_back(ingredientInfo("gunpowder",2000,itemList));
    quest.push_back(ingredientInfo("titanium bar",210,itemList));
    
    //fuel tank
    quest.push_back(ingredientInfo("solid propellant",20,itemList));
    quest.push_back(ingredientInfo("uranium rod",95,itemList));
    quest.push_back(ingredientInfo("enhanced helium 3",380,itemList));
    
    //Nozzle II
    quest.push_back(ingredientInfo("graphite",15250,itemList));
    quest.push_back(ingredientInfo("aluminium bar",6450,itemList));
    quest.push_back(ingredientInfo("titanium bar",20,itemList));
    
    //Stage II Winglet
    quest.push_back(ingredientInfo("aluminium bar",6250,itemList));
    quest.push_back(ingredientInfo("steel plate",777,itemList));
    quest.push_back(ingredientInfo("gear",85,itemList));
    
    //Decoupler II
    quest.push_back(ingredientInfo("copper nail",33350,itemList));
    quest.push_back(ingredientInfo("steel plate",777,itemList));
    quest.push_back(ingredientInfo("rubber",25,itemList));
    
    //Stage III
    quest.push_back(ingredientInfo("aluminium bar",1250,itemList));
    quest.push_back(ingredientInfo("diamond cutter",225,itemList));
    quest.push_back(ingredientInfo("rubber",45,itemList));
    
    //Electronic Components
    quest.push_back(ingredientInfo("solar panel",65,itemList));
    quest.push_back(ingredientInfo("accumulator",2555,itemList));
    
    //Rocket Engine
    quest.push_back(ingredientInfo("iron bar",22250,itemList));
    quest.push_back(ingredientInfo("wire",10000,itemList));
    quest.push_back(ingredientInfo("rubber",90,itemList));
    
    //Decoupler III
    quest.push_back(ingredientInfo("copper nail",33350,itemList));
    quest.push_back(ingredientInfo("steel plate",777,itemList));
    quest.push_back(ingredientInfo("rubber",25,itemList));
    
    //Node
    quest.push_back(ingredientInfo("lamp",1900,itemList));
    quest.push_back(ingredientInfo("glass",1600,itemList));
    quest.push_back(ingredientInfo("copper bar",9555,itemList));
    
    //Node
    quest.push_back(ingredientInfo("insulated wire",390,itemList));
    quest.push_back(ingredientInfo("battery",645,itemList));
    quest.push_back(ingredientInfo("circuits",425,itemList));
    quest.push_back(ingredientInfo("motherboard",45,itemList));
    quest.push_back(ingredientInfo("optic fiber",645,itemList));
    
    //S.A.S Module
    quest.push_back(ingredientInfo("silver bar",1600,itemList));
    quest.push_back(ingredientInfo("glass",1600,itemList));
    quest.push_back(ingredientInfo("green laser",7550,itemList));
    
    //Main Capsule
    quest.push_back(ingredientInfo("aluminium bar",1250,itemList));
    quest.push_back(ingredientInfo("diamond cutter",225,itemList));
    quest.push_back(ingredientInfo("rubber",45,itemList));
    
    //Rocket LES
    quest.push_back(ingredientInfo("aluminium bar",3200,itemList));
    quest.push_back(ingredientInfo("steel bar",1060,itemList));
    
    //Large Obsidian Spear
    GainResources("polished obsidian",2000,quest,itemList,procedureList);
    GainResources("tree",4000,quest,itemList,procedureList);
    
    //Radiation Protection Unit
    GainResources("silicon",3600,quest,itemList,procedureList);
    GainResources("copper bar",10000,quest,itemList,procedureList);
    
    //Mementos
    GainResources("gold bar",15000,quest,itemList,procedureList);
    GainResources("silver bar",5000,quest,itemList,procedureList);
    
    //Craft The PC
    GainResources("accumulator",240,quest,itemList,procedureList);
    GainResources("motherboard",20,quest,itemList,procedureList);
    GainResources("circuits",300,quest,itemList,procedureList);
    
    //Mini Tesla Coils
    GainResources("polished amber",4000,quest,itemList,procedureList);
    GainResources("insulated wire",2000,quest,itemList,procedureList);
    GainResources("graphite",50000,quest,itemList,procedureList);
    
    //Shiny Arms For The MC!
    GainResources("aluminium bar",10000,quest,itemList,procedureList);
    GainResources("wire",50000,quest,itemList,procedureList);
    GainResources("silver bar",10000,quest,itemList,procedureList);
    GainResources("circuits",200,quest,itemList,procedureList);
    
    //The Answer To Life
    quest.push_back(ingredientInfo("circuits",42,itemList));
    
    //Rocket Type A
    quest.push_back(ingredientInfo("steel plate",50,itemList));
    quest.push_back(ingredientInfo("optic fiber",10,itemList));
    quest.push_back(ingredientInfo("solar panel",1,itemList));
    
    //Stone of Wisdom
    //quest.push_back(ingredientInfo("shards of wisdom",100,itemList));
    quest.push_back(ingredientInfo("rubber",3,itemList));
    quest.push_back(ingredientInfo("sulfur",100,itemList));
    
    //Stone of Strength
    //quest.push_back(ingredientInfo("shards of strength",100,itemList));
    quest.push_back(ingredientInfo("rubber",3,itemList));
    quest.push_back(ingredientInfo("sulfur",100,itemList));
    
    //Stone of Kindness
    //quest.push_back(ingredientInfo("shards of kindness",100,itemList));
    quest.push_back(ingredientInfo("rubber",3,itemList));
    quest.push_back(ingredientInfo("sulfur",100,itemList));
    
    //Stone of Mistakes
    //quest.push_back(ingredientInfo("shards of mistakes",100,itemList));
    quest.push_back(ingredientInfo("rubber",3,itemList));
    quest.push_back(ingredientInfo("sulfur",100,itemList));
    
    //Stone of Shine
    //quest.push_back(ingredientInfo("shards of shine",100,itemList));
    quest.push_back(ingredientInfo("rubber",3,itemList));
    quest.push_back(ingredientInfo("sulfur",100,itemList));
    
    //Stone of Empathy
    //quest.push_back(ingredientInfo("shards of empathy",100,itemList));
    quest.push_back(ingredientInfo("rubber",3,itemList));
    quest.push_back(ingredientInfo("sulfur",100,itemList));
    
    //Stone of Entropy
    //quest.push_back(ingredientInfo("shards of entropy",100,itemList));
    quest.push_back(ingredientInfo("rubber",3,itemList));
    quest.push_back(ingredientInfo("sulfur",100,itemList));
    
    //Stone of Radiation
    //quest.push_back(ingredientInfo("shards of radiation",100,itemList));
    quest.push_back(ingredientInfo("rubber",3,itemList));
    quest.push_back(ingredientInfo("sulfur",100,itemList));
    
    //Stone of Eternity
    //quest.push_back(ingredientInfo("shards of eternity",100,itemList));
    quest.push_back(ingredientInfo("rubber",3,itemList));
    quest.push_back(ingredientInfo("sulfur",100,itemList));
    
    //Scanning The Elder
    //Research Beam
    quest.push_back(ingredientInfo("accumulator",100,itemList));
    quest.push_back(ingredientInfo("chipset",50,itemList));
    quest.push_back(ingredientInfo("lutetium bar",40,itemList));
    
    //Asteroid Mining
    GainResources("sodium chloride",100,quest,itemList,procedureList);
    GainResources("hydrochloric acid",5,quest,itemList,procedureList);
    GainResources("chipset",5,quest,itemList,procedureList);
    GainResources("lutetium ore",100,quest,itemList,procedureList);
    GainResources("lutetium",5,quest,itemList,procedureList);
    GainResources("lutetium bar",1,quest,itemList,procedureList);

    /////////
    //set isConsiderIngredientTime to true
    itemInfo::setConsiderIngredientTime("accumulator",itemList); //sulfur, sodium
    
    //Raw
    itemInfo::set_currentQuantity("coal",240000,itemList);
    itemInfo::set_currentQuantity("copper",580000,itemList);
    itemInfo::set_currentQuantity("iron",300000,itemList);
    itemInfo::set_currentQuantity("amber",70000,itemList);
    itemInfo::set_currentQuantity("aluminium",700000,itemList);
    itemInfo::set_currentQuantity("silver",220000,itemList);
    itemInfo::set_currentQuantity("gold",540000,itemList);
    itemInfo::set_currentQuantity("emerald",200000,itemList);
    itemInfo::set_currentQuantity("platinum",29000,itemList);
    itemInfo::set_currentQuantity("topaz",200000,itemList);
    itemInfo::set_currentQuantity("ruby",300000,itemList);
    itemInfo::set_currentQuantity("sapphire",100000,itemList);
    itemInfo::set_currentQuantity("amethyst",200000,itemList);
    itemInfo::set_currentQuantity("diamond",40000,itemList);
    itemInfo::set_currentQuantity("titanium ore",50000,itemList);
    itemInfo::set_currentQuantity("alexandrite",150000,itemList);
    itemInfo::set_currentQuantity("uranium",20000,itemList);
    
    itemInfo::set_currentQuantity("obsidian",-1,itemList);
    itemInfo::set_currentQuantity("helium 3",-1,itemList);
    itemInfo::set_currentQuantity("sodium chloride",-1,itemList);
    itemInfo::set_currentQuantity("lutetium ore",-1,itemList);
    
    //smelting
    itemInfo::set_currentQuantity("copper bar",80000,itemList);
    itemInfo::set_currentQuantity("iron bar",50000,itemList);
    itemInfo::set_currentQuantity("aluminium bar",56457,itemList);
    itemInfo::set_currentQuantity("steel bar",3000,itemList);
    itemInfo::set_currentQuantity("silver bar",11000,itemList);
    itemInfo::set_currentQuantity("gold bar",14000,itemList);
    itemInfo::set_currentQuantity("lab flask",10000,itemList);
    itemInfo::set_currentQuantity("steel plate",6210,itemList);
    itemInfo::set_currentQuantity("titanium bar",3800,itemList);
    itemInfo::set_currentQuantity("diamond cutter",2200,itemList);
    itemInfo::set_currentQuantity("motherboard",130,itemList);
    
    itemInfo::set_currentQuantity("magnetite bar",-1,itemList);
    itemInfo::set_currentQuantity("lutetium bar",-1,itemList);
    
    //craft
    itemInfo::set_currentQuantity("copper nail",131000,itemList);
    itemInfo::set_currentQuantity("amber insulation",12506,itemList);
    itemInfo::set_currentQuantity("solar panel",678,itemList);
    itemInfo::set_currentQuantity("graphite",81000,itemList);
    itemInfo::set_currentQuantity("green laser",7847,itemList);
    itemInfo::set_currentQuantity("wire",110000,itemList);
    itemInfo::set_currentQuantity("insulated wire",24000,itemList);
    itemInfo::set_currentQuantity("amber charger",8000,itemList);
    itemInfo::set_currentQuantity("aluminium bottle",28000,itemList);
    itemInfo::set_currentQuantity("bomb",84,itemList);
    itemInfo::set_currentQuantity("gear",441,itemList);
    itemInfo::set_currentQuantity("battery",3148,itemList);
    itemInfo::set_currentQuantity("lamp",2952,itemList);
    itemInfo::set_currentQuantity("accumulator",2567,itemList);
    itemInfo::set_currentQuantity("solid propellant",40,itemList);
    itemInfo::set_currentQuantity("circuits",1302,itemList);
    
    itemInfo::set_currentQuantity("compressor",-1,itemList);
    itemInfo::set_currentQuantity("optic fiber",-1,itemList);
    itemInfo::set_currentQuantity("dry ice",-1,itemList);
    itemInfo::set_currentQuantity("magnet",-1,itemList);
    itemInfo::set_currentQuantity("electrical engine",-1,itemList);
    itemInfo::set_currentQuantity("chipset",-1,itemList);
    
    //chemistry
    itemInfo::set_currentQuantity("water",40000,itemList);
    itemInfo::set_currentQuantity("oil",12000,itemList);
    itemInfo::set_currentQuantity("sodium",10000,itemList);
    itemInfo::set_currentQuantity("sulfur",10000,itemList);
    itemInfo::set_currentQuantity("silicon",90000,itemList);
    itemInfo::set_currentQuantity("titanium",13000,itemList);
    itemInfo::set_currentQuantity("nitrogen",-1,itemList);
    itemInfo::set_currentQuantity("hydrogen",6000,itemList);
    itemInfo::set_currentQuantity("glass",3600,itemList);
    itemInfo::set_currentQuantity("oxygen",4000,itemList);
    itemInfo::set_currentQuantity("clean water",1400,itemList);
    itemInfo::set_currentQuantity("gunpowder",4070,itemList);
    itemInfo::set_currentQuantity("sulfuric acid",486,itemList);
    itemInfo::set_currentQuantity("rubber",1558,itemList);
    itemInfo::set_currentQuantity("ethanol",105,itemList);
    itemInfo::set_currentQuantity("refined oil",96,itemList);
    itemInfo::set_currentQuantity("diethyl ether",161,itemList);
    itemInfo::set_currentQuantity("uranium rod",499,itemList);
    itemInfo::set_currentQuantity("plastic plate",146,itemList);
    
    itemInfo::set_currentQuantity("liquid nitrogen",-1,itemList);
    itemInfo::set_currentQuantity("magnetite ore",-1,itemList);
    itemInfo::set_currentQuantity("enhanced helium 3",-1,itemList);
    itemInfo::set_currentQuantity("toxic bomb",-1,itemList);
    itemInfo::set_currentQuantity("hydrochloric acid",-1,itemList);
    itemInfo::set_currentQuantity("lutetium",-1,itemList);
    
    //jewel
    itemInfo::set_currentQuantity("polished amber",3000,itemList);
    itemInfo::set_currentQuantity("polished emerald",100000,itemList);
    itemInfo::set_currentQuantity("polished topaz",13000,itemList);
    itemInfo::set_currentQuantity("polished sapphire",3000,itemList);
    itemInfo::set_currentQuantity("polished ruby",60000,itemList);
    itemInfo::set_currentQuantity("polished amethyst",3000,itemList);
    itemInfo::set_currentQuantity("polished alexandrite",1277,itemList);
    itemInfo::set_currentQuantity("amber bracelet",0,itemList);
    itemInfo::set_currentQuantity("polished diamond",20000,itemList);
    itemInfo::set_currentQuantity("emerald ring",0,itemList);
    itemInfo::set_currentQuantity("maya calendar",0,itemList);
    itemInfo::set_currentQuantity("haircomb",0,itemList);
    
    itemInfo::set_currentQuantity("polished obsidian",-1,itemList);
    itemInfo::set_currentQuantity("obsidian knife",-1,itemList);
    
    //tree
    itemInfo::set_currentQuantity("tree seed",3284,itemList);
    itemInfo::set_currentQuantity("tree",488,itemList);
    itemInfo::set_currentQuantity("liana seed",170,itemList);
    itemInfo::set_currentQuantity("grape seed",170,itemList);
    itemInfo::set_currentQuantity("grape",1050,itemList);
    itemInfo::set_currentQuantity("liana",584,itemList);
    
    //Find hierarchy
    vector< vector<int> > hierarchy;
    while(true)
    {
        bool isEnd = true;
        vector<int> newHierarchy;
        for(unsigned int i = 0; i<itemList.size(); i++)
        {
            //decide whether the item is inside the hierarchy
            bool isFound = false;
            for(unsigned int j = 0; j<hierarchy.size(); j++)
            {
                for(unsigned int k = 0; k<hierarchy[j].size(); k++)
                {
                    if(i == hierarchy[j][k]) isFound = true;
                }
            }
            
            if(isFound) continue;
            //add item in the hierarchy
            
            //decide whether all ingredients are inside the hierarchy
            bool isFoundAllIngredients = true;
            for(unsigned int j = 0; j<itemList[i].procedureIndex.size(); j++)
            {
                int procedureIndex = itemList[i].procedureIndex[j];
                for(unsigned int k = 0; k<procedureList[procedureIndex].ingredients.size(); k++)
                {
                    bool isFoundIngredient = false;
                    for(unsigned int m = 0; m<hierarchy.size(); m++)
                    {
                        for(unsigned int n = 0; n<hierarchy[m].size(); n++)
                        {
                            if(procedureList[procedureIndex].ingredients[k].itemIndex == hierarchy[m][n]) isFoundIngredient = true;
                        }
                    }
                    if(!isFoundIngredient) isFoundAllIngredients = false;
                }
            }
            
            if(!isFoundAllIngredients)
            {
                isEnd = false;
                continue;
            }
            
            //add into the new hierarchy
            newHierarchy.push_back(i);
        }
        
        hierarchy.push_back(newHierarchy);
        if(isEnd) break;
    }
    
    //calculate limiting time per piece
    for(unsigned int i = 0; i<hierarchy.size(); i++)
    {
        for(unsigned int j = 0; j<hierarchy[i].size(); j++)
        {
            int itemIndex = hierarchy[i][j];
            itemList[itemIndex].setTimePerPiece(itemList,procedureList,buildingList);
        }
    }

    itemInfo::set_reservation_level(itemList,level);
    itemInfo::set_reservation_quest(itemList,quest);
    
    //set total and
    for(unsigned int i = 0; i<itemList.size(); i++)
    {
        itemList[i].reservation_total = itemList[i].reservation_level + itemList[i].reservation_quest;
        itemList[i].total = itemList[i].reservation_total;
    }
    
    for(unsigned long i = hierarchy.size()-1; i>=1; i--)
    {
        for(unsigned int j = 0; j<hierarchy[i].size(); j++)
        {
            int itemIndex = hierarchy[i][j];
            int procedureIndex = itemList[itemIndex].procedureIndex[0];
            
            if(procedureList[procedureIndex].products.size() == 1)
            {
                double quantity_diff = itemList[itemIndex].total - itemList[itemIndex].currentQuantity;
                if(quantity_diff <= 0) continue;
                
                for(unsigned int k = 0; k<procedureList[procedureIndex].ingredients.size(); k++)
                {
                    int ingredientIndex = procedureList[procedureIndex].ingredients[k].itemIndex;
                    double TotalQuantity = procedureList[procedureIndex].ingredients[k].quantity * (quantity_diff / procedureList[procedureIndex].products[0].quantity);
                    itemList[ingredientIndex].total += TotalQuantity;
                    if(!itemList[itemIndex].isAvailable) itemList[ingredientIndex].reservation_total += TotalQuantity;
                }
            }
            else
            {
                //hydrogen and oxygen
                //skip for oxygen
                j++;
                
                cout<<"considering special case: "<<itemList[itemIndex].name<<" "<<itemList[itemIndex+1].name<<endl;
                
                double quantity_diff_H = itemList[itemIndex].total - itemList[itemIndex].currentQuantity;
                double quantity_diff_O = itemList[itemIndex+1].total - itemList[itemIndex+1].currentQuantity;
                if(quantity_diff_H <= 0 && quantity_diff_O <= 0 ) continue;
                
                quantity_diff_H = quantity_diff_H / procedureList[procedureIndex].products[0].quantity;
                quantity_diff_O = quantity_diff_O / procedureList[procedureIndex].products[1].quantity;
                double quantity_diff;
                if(quantity_diff_H > quantity_diff_O) quantity_diff = quantity_diff_H;
                else quantity_diff = quantity_diff_O;
                
                for(unsigned int k = 0; k<procedureList[procedureIndex].ingredients.size(); k++)
                {
                    int ingredientIndex = procedureList[procedureIndex].ingredients[k].itemIndex;
                    itemList[ingredientIndex].total += procedureList[procedureIndex].ingredients[k].quantity * quantity_diff;
                }
            }
        }
        //printHierarchy(hierarchy,itemList,procedureList);
    }
    
    
    //print hierarchy
    printHierarchy(hierarchy,itemList,procedureList);
    
    //print
    for(unsigned int i = 0; i<hierarchy.size(); i++)
    {
        //cout<<"Tier "<<i<<":"<<endl;
        for(unsigned int j = 0; j<hierarchy[i].size(); j++)
        {
            int itemIndex = hierarchy[i][j];
            if(itemList[itemIndex].pricePerTime < 268) continue;
            cout<<std::setw(20)<<itemList[itemIndex].name<<": "<<std::setw(8)<<itemList[itemIndex].timePerPiece<<" "<<std::setw(9)<<itemList[itemIndex].pricePerTime<<endl;
        }
        //cout<<endl;
    }
    
    return 0;
}
