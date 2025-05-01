#include <iostream>
#include <unordered_map>
#include <map>
#include <memory>
#include <stdexcept>

using namespace std;

//DuplicateItemException Class
class DuplicateItemException : public runtime_error 
{

public:
    DuplicateItemException(const string& msg) : runtime_error(msg) {}
};

//ItemNotFoundException Class
class ItemNotFoundException : public runtime_error 
{

public:
    ItemNotFoundException(const string& msg) : runtime_error(msg) {}
};

//StoredItem Class
class StoredItem 
{

private:
    string id;
    string description;
    string location;

public:
    StoredItem(string id, string desc, string loc)
        : id(id), description(desc), location(loc) {
    }

    string getId() const { return id; }
    string getDescription() const { return description; }
    string getLocation() const { return location; }
};

//StorageManger Class
class StorageManager 
{

private:
    unordered_map<string, shared_ptr<StoredItem>> itemById;
    map<string, shared_ptr<StoredItem>> itemByDescription;

public:
    
    void addItem(const shared_ptr<StoredItem>& item) {
        
        string id;
        string description;
        id = item->getId();
        description = item->getDescription();

        if (itemById.find(id) != itemById.end()) 
        {
            throw DuplicateItemException("Item with ID " + id + " already exists");
        }

        itemById[id] = item;
        itemByDescription[description] = item;
    }

    shared_ptr<StoredItem> findById(const string& id) const {
        
        auto items = itemById.find(id);
        
        if (items == itemById.end())
        {
            throw ItemNotFoundException("Item with ID " + id + " not found!");
        }

        return items->second;
    }

    void removeItem(const string& id) {
        
        auto items = itemById.find(id);
       
        if (items == itemById.end()) 
        {
            throw ItemNotFoundException("Item with ID " + id + " not found!");
        }
       
        string description = items->second->getDescription();
        itemById.erase(items);
        itemByDescription.erase(description);
    }


    void listItemsByDescription() const {
        
        cout << "Items in Description Order:" << endl;
        
        for (const auto& pair : itemByDescription) 
        {
            cout << "- " << pair.second->getDescription() << ": " << pair.second->getLocation() << endl;
        }
    }
};

//Adds duplicate items 
void testDuplicateAddition(StorageManager& pos)
{
    
    cout << "Attempting to add ITEM001 again..." << endl;
    try {
        auto duplicateItem = make_shared<StoredItem>("ITEM001", "LED Light", "Aisle 9, Shelf 9");
        pos.addItem(duplicateItem);
    }
    catch (const DuplicateItemException& e) {
        cout << "Error: " << e.what() << endl;
    }
}
//Throws error if item does not exist
void testItemNotFound(StorageManager& pos)
{
    cout << "removing ITEM003..." << endl;
    try {
        pos.removeItem("ITEM003");
    }
    catch (const ItemNotFoundException& e) {
        cout << "Error: " << e.what() << endl;
    }
}

int main() {

    StorageManager pos;

    //Creates item and adds
    cout << "Addint item: ITEM001 - LED Light" << endl;
    auto item1 = make_shared<StoredItem>("ITEM001", "LED Light", "Aisle 3, Shelf 1");
    pos.addItem(item1);

    cout << "Adding item: ITEM002 - Fan Motor" << endl;
    auto item2 = make_shared<StoredItem>("ITEM002", "Fan Motor", "Aisle 2, Shelf 5");
    pos.addItem(item2);

    //Duplicate catcher
    testDuplicateAddition(pos);

    //Prints items already inputted
    cout << "Retrieving ITEM002..." << endl;
    
    //try catch loop to make sure item exists
    try 
    {
        auto found = pos.findById("ITEM002");
        cout << "found: " << found->getDescription() << " at " << found->getLocation() << endl;
    }
    catch (const ItemNotFoundException& e) 
    {
        cout << "Error: " << e.what() << endl;
    }

    //Ensures item can be found, used to catch and remove non existent item
    testItemNotFound(pos);

    pos.listItemsByDescription();

    
    return 0;
}