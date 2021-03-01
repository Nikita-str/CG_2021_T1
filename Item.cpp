#include "Item.h"
#include "Player.h"
#include "GameMap.h"

Item::Item(int i_lvl, Point _pos) : item_lvl(i_lvl), pos(_pos)
{
    switch (item_lvl) {
    case -1: item_type = E_ItemTypes::Key; break;
    //case 0: item_type = E_ItemTypes::Potion; break;

    case 0:
        item_type = E_ItemTypes::Other;
        break;
    case 1:
    {
        int t = std::rand() % 3;
        if(true || t == 0)item_type = E_ItemTypes::Potion;
        if(t == 1)item_type = E_ItemTypes::Boots; 
        break;
    }
    case 2:
    {
        int t = std::rand() % 3;
        if (true || t == 0)item_type = E_ItemTypes::Potion;
        if (t == 1)item_type = E_ItemTypes::Boots;
        break;
    }
    case 3:
    {
        int t = std::rand() % 3;
        if (true || t == 0)item_type = E_ItemTypes::Potion;
        if (t == 1)item_type = E_ItemTypes::Boots;
        //if (t == 2)item_type = E_ItemTypes::Other;
        break;
    }
    default:
        item_type = E_ItemTypes::Key;
        item_lvl = -1;
        break;
    } 
    CreateHelper();
}
Item::Item(E_ItemTypes i_type, int i_lvl, Point _pos) : item_type(i_type), item_lvl(i_lvl), pos(_pos)
{
    CreateHelper();
}

bool IsPlaceForItem(){return Player::Get().IsPlaceForItem();}

void Item::CreateHelper()
{
    switch (item_type) {
    case E_ItemTypes::Key:
        inventory = false;
        item_lvl = -1;
        spr = Sprite {"../resources/key.png", 1};
        ict = []() { return Player::Get().CanTakeKey(); };
        ia = [](bool on) {if (on)Player::Get().KeyInc(); else Player::Get().KeyDec(); };
        break;
    case E_ItemTypes::Boots:
    {
        inventory = false;
        spr = Sprite {"../resources/boots_" + std::to_string(item_lvl) + ".png", 1};
        ict = []() { return true; };
        ia = [item_lvl = item_lvl, pos = pos](bool on) {
            if (on) {
                auto &pl = Player::Get();
                int pl_b_lvl = pl.BootsLvl();
                if (pl_b_lvl != 0)GameMap::GetCur()->GetItems().emplace_back(E_ItemTypes::Boots, pl_b_lvl, pos);
                pl.TakeBoots(item_lvl);
            } else Player::Get().KeyDec();
        };
        break;
    }
    case E_ItemTypes::Ring:
        break;
    case E_ItemTypes::Potion:
    {
        inventory = true;
        can_be_used = true;
        spr = Sprite {"../resources/hp_pot_" + std::to_string(item_lvl) + ".png", (item_lvl == 1) ? 7 : 11};
        ict = IsPlaceForItem;
        ia = [item_lvl = item_lvl, pos = pos](bool on) {};
        use = [power = item_lvl]() { Player::Get().HpRestore(4 + power * 8); };
        break;
    }
    case E_ItemTypes::Book:
        break;
    case E_ItemTypes::Other:
    {
        inventory = true;
        spr = Sprite {"../resources/feat.png", 1};
        ict = IsPlaceForItem;
        ia = [pos = pos, this](bool on) {
            auto &pl = Player::Get();
            if (on) {
                pl.AddSpeed(15);
            } else {
                pl.AddSpeed(-15);
            }
        };

        break;
    }
    default:
        break;
    }
}