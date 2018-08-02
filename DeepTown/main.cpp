#include <iostream>
#include <string>
#include <vector>
using namespace std;
class itemInfo;
class buildingInfo;
class ingredientInfo;
class productInfo;
class procedureInfo;

class itemInfo
{
    public:
    itemInfo(string newName, double newPrice)
    {
        name = newName;
        price = newPrice;
        reservation_level = 0;
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
    
    double setTimePerPiece(vector<itemInfo>& itemList, vector<procedureInfo>& procedureList, vector<buildingInfo>& buildingList);
    static void set_reservation_level(vector<itemInfo>& itemList, vector<ingredientInfo>& level);
    static void set_reservation_quest(vector<itemInfo>& itemList, vector<ingredientInfo>& quest);
    
    string name;
    double price;
    double timePerPiece;
    double pricePerTime;
    
    int reservation_level;
    int reservation_quest;
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
        isConsiderIngredientTime = false;
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
        isConsiderIngredientTime = false;
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
        isConsiderIngredientTime = false;
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
        isConsiderIngredientTime = false;
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
        isConsiderIngredientTime = false;
    }
    
    static void setConsiderIngredientTime(string ProductName, vector<itemInfo>& itemList, vector<procedureInfo>& procedureList)
    {
        int itemIndex1 = itemInfo::findItem(ProductName,itemList);
        for(unsigned int i = 0; i<procedureList.size(); i++)
        {
            int itemIndex2 = procedureList[i].products[0].itemIndex;
            if(itemIndex1 == itemIndex2)
            {
                if(procedureList[i].ingredients.size() == 0) return;
                procedureList[i].isConsiderIngredientTime = true;
            }
        }
    }
    
    double setTimePerProcedure(vector<itemInfo>& itemList, vector<procedureInfo>& procedureList, vector<buildingInfo>& buildingList);
    
    double time; //in seconds
    int buildingIndex;
    vector<ingredientInfo> ingredients;
    vector<productInfo> products;
    
    double timePerProcedure;
    bool isConsiderIngredientTime;
};

double itemInfo::setTimePerPiece(vector<itemInfo>& itemList, vector<procedureInfo>& procedureList, vector<buildingInfo>& buildingList)
{
    double PiecePerTime = 0;
    for(unsigned int j = 0; j<procedureList.size(); j++)
    {
        for(unsigned int k = 0; k<procedureList[j].products.size(); k++)
        {
            int productIndex = procedureList[j].products[k].itemIndex;
            if(itemList[productIndex].name == name)
            {
                double timePerProcedure = procedureList[j].setTimePerProcedure(itemList,procedureList,buildingList);
                PiecePerTime += procedureList[j].products[k].quantity / timePerProcedure;
            }
        }
    }
    
    timePerPiece = 1/PiecePerTime;
    pricePerTime = price / timePerPiece;
    
    return timePerPiece;
}

double procedureInfo::setTimePerProcedure(vector<itemInfo>& itemList, vector<procedureInfo>& procedureList, vector<buildingInfo>& buildingList)
{
    if(buildingList[buildingIndex].name == "mining station")
    {
        timePerProcedure = 60;
    }
    else
    {
        double max = time / buildingList[buildingIndex].slot;
        if(isConsiderIngredientTime)
        {
            for(unsigned int j = 0; j<ingredients.size(); j++)
            {
                int ingredientIndex = ingredients[j].itemIndex;
                double TimePerPiece = itemList[ingredientIndex].setTimePerPiece(itemList,procedureList,buildingList);
                double time = TimePerPiece * ingredients[j].quantity;
                if(time > max) max = time;
            }
        }
        
        timePerProcedure = max;
    }

    return timePerProcedure;
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

void FillIngredients(string itemName, int quantity, vector<ingredientInfo>& quest, vector<itemInfo>& itemList, vector<procedureInfo>& procedureList)
{
    int itemIndex = itemInfo::findItem(itemName,itemList);
    for(unsigned int i = 0; i<procedureList.size(); i++)
    {
        for(unsigned int j = 0; j<procedureList[i].products.size(); j++)
        {
            int productIndex = procedureList[i].products[j].itemIndex;
            if(itemIndex == productIndex)
            {
                for(unsigned int k = 0; k<procedureList[i].ingredients.size(); k++)
                {
                    int ingredientIndex = procedureList[i].ingredients[k].itemIndex;
                    quest.push_back(ingredientInfo(itemList[ingredientIndex].name, procedureList[i].ingredients[k].quantity *quantity, itemList));
                }
            }
        }
    }
}

int main()
{
    //item list
    vector<itemInfo> itemList;
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
    
    itemList.push_back(itemInfo("silicon",100));
    itemList.push_back(itemInfo("sulfur",100));
    itemList.push_back(itemInfo("sodium",100));
    itemList.push_back(itemInfo("nitrogen",300));
    
    itemList.push_back(itemInfo("oil",21));
    itemList.push_back(itemInfo("water",5));
    
    itemList.push_back(itemInfo("tree seed",20));
    itemList.push_back(itemInfo("liana seed",1000));
    itemList.push_back(itemInfo("grape seed",1200));
    itemList.push_back(itemInfo("tree",193));
    itemList.push_back(itemInfo("liana",1700));
    itemList.push_back(itemInfo("grape",1500));
    
    itemList.push_back(itemInfo("copper bar",25));
    itemList.push_back(itemInfo("iron bar",40));
    itemList.push_back(itemInfo("glass",450));
    itemList.push_back(itemInfo("aluminium bar",50));
    itemList.push_back(itemInfo("steel bar",180));
    itemList.push_back(itemInfo("silver bar",200));
    itemList.push_back(itemInfo("gold bar",250));
    itemList.push_back(itemInfo("steel plate",1800));
    itemList.push_back(itemInfo("titanium bar",3000));
    itemList.push_back(itemInfo("magnetite bar",137000));
    itemList.push_back(itemInfo("lutetium bar",68000));
    
    itemList.push_back(itemInfo("graphite",15));
    itemList.push_back(itemInfo("copper nail",7));
    itemList.push_back(itemInfo("wire",15));
    itemList.push_back(itemInfo("battery",200));
    itemList.push_back(itemInfo("circuits",2070));
    itemList.push_back(itemInfo("lamp",760));
    itemList.push_back(itemInfo("lab flask",800));
    itemList.push_back(itemInfo("amber charger",4));
    itemList.push_back(itemInfo("aluminium bottle",55));
    itemList.push_back(itemInfo("amber insulation",125));
    itemList.push_back(itemInfo("insulated wire",750));
    itemList.push_back(itemInfo("green laser",400));
    itemList.push_back(itemInfo("diamond cutter",5000));
    itemList.push_back(itemInfo("motherboard",17000));
    itemList.push_back(itemInfo("solid propellant",27000));
    itemList.push_back(itemInfo("accumulator",9000));
    itemList.push_back(itemInfo("solar panel",69000));
    itemList.push_back(itemInfo("gear",18500));
    itemList.push_back(itemInfo("bomb",55500));
    itemList.push_back(itemInfo("compressor",44000));
    itemList.push_back(itemInfo("optic fiber",10500));
    itemList.push_back(itemInfo("dry ice",140000));
    itemList.push_back(itemInfo("magnet",300000));
    itemList.push_back(itemInfo("electrical engine",745000));
    itemList.push_back(itemInfo("chipset",40000));
    
    itemList.push_back(itemInfo("clean water",1200));
    itemList.push_back(itemInfo("hydrogen",400));
    itemList.push_back(itemInfo("oxygen",900));
    itemList.push_back(itemInfo("rubber",4000));
    itemList.push_back(itemInfo("sulfuric acid",3500));
    itemList.push_back(itemInfo("ethanol",4200));
    itemList.push_back(itemInfo("refined oil",16500));
    itemList.push_back(itemInfo("plastic plate",40000));
    itemList.push_back(itemInfo("titanium",260));
    itemList.push_back(itemInfo("diethyl ether",17000));
    itemList.push_back(itemInfo("gunpowder",2500));
    itemList.push_back(itemInfo("liquid nitrogen",12500));
    itemList.push_back(itemInfo("magnetite ore",12500));
    itemList.push_back(itemInfo("enhanced helium 3",190000));
    itemList.push_back(itemInfo("toxic bomb",77500));
    itemList.push_back(itemInfo("hydrochloric acid",12000));
    itemList.push_back(itemInfo("lutetium",13500));
    
    int tradingLevel = 50;
    double SF = (1 + 0.02 * (tradingLevel-1)) * 1.35;
    itemList.push_back(itemInfo("polished amber",70));
    itemList.push_back(itemInfo("emerald ring",450*SF));
    itemList.push_back(itemInfo("amber bracelet",280*SF));
    itemList.push_back(itemInfo("maya calendar",6000*SF));
    itemList.push_back(itemInfo("haircomb",14000*SF));
    itemList.push_back(itemInfo("obsidian knife",32000*SF));
    itemList.push_back(itemInfo("polished emerald",160));
    itemList.push_back(itemInfo("polished topaz",200));
    itemList.push_back(itemInfo("polished ruby",250));
    itemList.push_back(itemInfo("polished diamond",300));
    itemList.push_back(itemInfo("polished sapphire",230));
    itemList.push_back(itemInfo("polished amethyst",250));
    itemList.push_back(itemInfo("polished alexandrite",270));
    itemList.push_back(itemInfo("polished obsidian",280));
    
    itemList.push_back(itemInfo("uranium rod",17000));
    
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
    procedureList.push_back(procedureInfo("crafting","wire",1,"amber insulation",1,3*60,"insulated wire",1,buildingList,itemList));
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
    
    //set isConsiderIngredientTime to true
    procedureInfo::setConsiderIngredientTime("iron bar",itemList,procedureList);
    procedureInfo::setConsiderIngredientTime("glass",itemList,procedureList);
    procedureInfo::setConsiderIngredientTime("steel bar",itemList,procedureList);
    procedureInfo::setConsiderIngredientTime("silver bar",itemList,procedureList);
    procedureInfo::setConsiderIngredientTime("coal",itemList,procedureList);
    procedureInfo::setConsiderIngredientTime("gold bar",itemList,procedureList);
    procedureInfo::setConsiderIngredientTime("steel plate",itemList,procedureList);
    procedureInfo::setConsiderIngredientTime("titanium bar",itemList,procedureList);
    
    procedureInfo::setConsiderIngredientTime("graphite",itemList,procedureList);
    procedureInfo::setConsiderIngredientTime("circuits",itemList,procedureList);
    procedureInfo::setConsiderIngredientTime("lamp",itemList,procedureList);
    procedureInfo::setConsiderIngredientTime("lab flask",itemList,procedureList);
    procedureInfo::setConsiderIngredientTime("green laser",itemList,procedureList);
    procedureInfo::setConsiderIngredientTime("diamond cutter",itemList,procedureList);
    procedureInfo::setConsiderIngredientTime("solid propellant",itemList,procedureList);
    procedureInfo::setConsiderIngredientTime("accumulator",itemList,procedureList);
    procedureInfo::setConsiderIngredientTime("solar panel",itemList,procedureList);
    procedureInfo::setConsiderIngredientTime("gear",itemList,procedureList);
    procedureInfo::setConsiderIngredientTime("bomb",itemList,procedureList);
    
    procedureInfo::setConsiderIngredientTime("rubber",itemList,procedureList);
    procedureInfo::setConsiderIngredientTime("refined oil",itemList,procedureList);
    procedureInfo::setConsiderIngredientTime("plastic plate",itemList,procedureList);
    procedureInfo::setConsiderIngredientTime("titanium",itemList,procedureList);
    procedureInfo::setConsiderIngredientTime("diethyl ether",itemList,procedureList);
    procedureInfo::setConsiderIngredientTime("gunpowder",itemList,procedureList);
    
    procedureInfo::setConsiderIngredientTime("maya calendar",itemList,procedureList);
    procedureInfo::setConsiderIngredientTime("haircomb",itemList,procedureList);
    //procedureInfo::setConsiderIngredientTime("obsidian knife",itemList,procedureList);
    procedureInfo::setConsiderIngredientTime("polished diamond",itemList,procedureList);
    //procedureInfo::setConsiderIngredientTime("",itemList,procedureList);
    
    //calculate limiting time per piece
    for(unsigned int i = 0; i<itemList.size(); i++)
    {
        itemList[i].setTimePerPiece(itemList,procedureList,buildingList);
    }
    
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
    
    itemInfo::set_reservation_level(itemList,level);
    
    //reservation for quest
    vector<ingredientInfo> quest;
    
    //New Gas: Nitrogen
    quest.push_back(ingredientInfo("motherboard",50,itemList));
    quest.push_back(ingredientInfo("insulated wire",1000,itemList));
    quest.push_back(ingredientInfo("steel plate",500,itemList));
    FillIngredients("compressor",1,quest,itemList,procedureList);
    FillIngredients("liquid nitrogen",1,quest,itemList,procedureList);
    
    //Here I Go Crafting Again!
    FillIngredients("optic fiber",10+50,quest,itemList,procedureList);
    FillIngredients("compressor",10+50,quest,itemList,procedureList);
    
    //Quite The Goldsmith
    FillIngredients("polished obsidian",10+100,quest,itemList,procedureList);
    FillIngredients("polished sapphire",100+300,quest,itemList,procedureList);
    FillIngredients("polished diamond",100+300,quest,itemList,procedureList);
    
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
    FillIngredients("polished obsidian",2000,quest,itemList,procedureList);
    
    //Radiation Protection Unit
    FillIngredients("copper bar",10000,quest,itemList,procedureList);
    
    //Mementos
    FillIngredients("gold bar",15000,quest,itemList,procedureList);
    FillIngredients("silver bar",5000,quest,itemList,procedureList);
    
    //Craft The PC
    FillIngredients("accumulator",240,quest,itemList,procedureList);
    FillIngredients("motherboard",20,quest,itemList,procedureList);
    FillIngredients("circuits",300,quest,itemList,procedureList);
    
    //Mini Tesla Coils
    FillIngredients("polished amber",4000,quest,itemList,procedureList);
    FillIngredients("insulated wire",2000,quest,itemList,procedureList);
    FillIngredients("graphite",50000,quest,itemList,procedureList);
    
    //Shiny Arms For The MC!
    FillIngredients("aluminium bar",10000,quest,itemList,procedureList);
    FillIngredients("wire",50000,quest,itemList,procedureList);
    FillIngredients("silver bar",10000,quest,itemList,procedureList);
    FillIngredients("circuits",200,quest,itemList,procedureList);
    
    //The Answer To Life
    FillIngredients("circuits",42,quest,itemList,procedureList);
    
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
    FillIngredients("hydrochloric acid",5,quest,itemList,procedureList);
    FillIngredients("chipset",5,quest,itemList,procedureList);
    FillIngredients("lutetium",5,quest,itemList,procedureList);
    FillIngredients("lutetium bar",1,quest,itemList,procedureList);
    
    itemInfo::set_reservation_quest(itemList,quest);
    
    //print
    for(unsigned int i = 0; i<itemList.size(); i++)
    {
        cout<<itemList[i].name<<": "<<itemList[i].timePerPiece<<" "<<itemList[i].pricePerTime<<endl;
        //cout<<itemList[i].name<<": "<<itemList[i].reservation_level<<endl;
        //cout<<itemList[i].name<<": "<<itemList[i].reservation_quest<<endl;
        //cout<<itemList[i].name<<": "<<itemList[i].reservation_level<<" "<<itemList[i].reservation_quest<<endl;
    }
    
    return 0;
}
