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
    
    string name;
    double price;
    double timePerPiece;
    double pricePerTime;
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
    //itemList.push_back(itemInfo("obsidian",));
    //itemList.push_back(itemInfo("helium 3",));
    
    itemList.push_back(itemInfo("silicon",100));
    itemList.push_back(itemInfo("sulfur",100));
    itemList.push_back(itemInfo("sodium",100));
    //itemList.push_back(itemInfo("nitrogen",100));
    
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
    //itemList.push_back(itemInfo("magnetite bar",));
    
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
    //itemList.push_back(itemInfo("compressor",));
    //itemList.push_back(itemInfo("optic fiber",));
    //itemList.push_back(itemInfo("dry ice",));
    //itemList.push_back(itemInfo("magnet",));
    //itemList.push_back(itemInfo("electrical engine",));
    
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
    //itemList.push_back(itemInfo("liquid nitrogen",));
    //itemList.push_back(itemInfo("magnetite ore",));
    //itemList.push_back(itemInfo("enhanced helium 3",));
    //itemList.push_back(itemInfo("toxic bomb",));
    
    int tradingLevel = 50;
    double SF = (1 + 0.02 * (tradingLevel-1)) * 1.35;
    itemList.push_back(itemInfo("polished amber",70));
    itemList.push_back(itemInfo("emerald ring",450*SF));
    itemList.push_back(itemInfo("amber bracelet",280*SF));
    itemList.push_back(itemInfo("maya calendar",6000*SF));
    itemList.push_back(itemInfo("haircomb",14000*SF));
    //itemList.push_back(itemInfo("obsidian knife",32000*SF));
    itemList.push_back(itemInfo("polished emerald",160));
    itemList.push_back(itemInfo("polished topaz",200));
    itemList.push_back(itemInfo("polished ruby",250));
    itemList.push_back(itemInfo("polished diamond",300));
    itemList.push_back(itemInfo("polished sapphire",230));
    itemList.push_back(itemInfo("polished amethyst",250));
    itemList.push_back(itemInfo("polished alexandrite",270));
    //itemList.push_back(itemInfo("polished obsidian",));
    
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
    
    //chemical mining
    buildingList.push_back(buildingInfo("chemical mining",8));
    procedureList.push_back(procedureInfo("chemical mining",10*60,"silicon",20,buildingList,itemList));
    procedureList.push_back(procedureInfo("chemical mining",10*60,"sulfur",20,buildingList,itemList));
    procedureList.push_back(procedureInfo("chemical mining",10*60,"sodium",20,buildingList,itemList));
    
    //oil mining
    buildingList.push_back(buildingInfo("oil mining",1));
    procedureList.push_back(procedureInfo("oil mining",60*60,"oil",7,buildingList,itemList));
    
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
    
    //jewel crafting
    buildingList.push_back(buildingInfo("jewel crafting",4));
    procedureList.push_back(procedureInfo("jewel crafting","amber",5,30,"polished amber",1,buildingList,itemList));
    procedureList.push_back(procedureInfo("jewel crafting","gold bar",1,"polished emerald",1,5*60,"emerald ring",1,buildingList,itemList));
    procedureList.push_back(procedureInfo("jewel crafting","silver bar",1,"polished amber",1,2*60,"amber bracelet",1,buildingList,itemList));
    procedureList.push_back(procedureInfo("jewel crafting","silver bar",2,"gold bar",10,2*60,"maya calendar",1,buildingList,itemList));
    procedureList.push_back(procedureInfo("jewel crafting","silver bar",1,"polished amethyst",15,"polished alexandrite",10,2*60,"haircomb",1,buildingList,itemList));
    //procedureList.push_back(procedureInfo("jewel crafting","silver bar",1,"tree",2,"polished obsidian",50,2*60,"obsidian knife",1,buildingList,itemList));
    procedureList.push_back(procedureInfo("jewel crafting","emerald",5,30,"polished emerald",1,buildingList,itemList));
    procedureList.push_back(procedureInfo("jewel crafting","topaz",5,60,"polished topaz",1,buildingList,itemList));
    procedureList.push_back(procedureInfo("jewel crafting","ruby",5,60,"polished ruby",1,buildingList,itemList));
    procedureList.push_back(procedureInfo("jewel crafting","diamond",5,60,"polished diamond",1,buildingList,itemList));
    procedureList.push_back(procedureInfo("jewel crafting","sapphire",5,60,"polished sapphire",1,buildingList,itemList));
    procedureList.push_back(procedureInfo("jewel crafting","amethyst",5,60,"polished amethyst",1,buildingList,itemList));
    procedureList.push_back(procedureInfo("jewel crafting","alexandrite",5,60,"polished alexandrite",1,buildingList,itemList));
    //procedureList.push_back(procedureInfo("jewel crafting","obsidian",5,60,"polished obsidian",1,buildingList,itemList));
    
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
    
    //print
    for(unsigned int i = 0; i<itemList.size(); i++)
    {
        cout<<itemList[i].name<<" "<<itemList[i].timePerPiece<<" "<<itemList[i].pricePerTime<<endl;
    }
    
    return 0;
}
