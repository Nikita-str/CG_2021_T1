#include "Item.h"
#include "Player.h"
#include "GameMap.h"

constexpr int case_special(const char* s, int sz)
{
    int ret = 0;
    for (int i = 0; i < sz; i++) ret = ret * 40 + s[i] - 'a' + 10;
    return ret;
}

constexpr int case_book = case_special("book", 4);
constexpr int case_ring = case_special("ring", 4);
constexpr int case_boots = case_special("boots", 5);
constexpr int case_potion = case_special("pot", 3);

Item::Item(int i_lvl, Point _pos) : item_lvl(i_lvl), pos(_pos)
{
    switch (item_lvl) {
    case -1: item_type = E_ItemTypes::Key; break;

    case 0: item_type = E_ItemTypes::Other; break;
    case 1:
    {
        int t = std::rand() % 4;
        if (t == 0)item_type = E_ItemTypes::Potion;
        if (t == 1)item_type = E_ItemTypes::Boots;
        if (t == 2)item_type = E_ItemTypes::Book;
        if (t == 3)item_type = E_ItemTypes::Ring;
        break;
    }
    case 2:
    {
        int t = std::rand() % 5;
        if (t < 2)item_type = E_ItemTypes::Potion;
        if (t == 2)item_type = E_ItemTypes::Book;
        if (t == 3)item_type = E_ItemTypes::Ring;
        if (t == 4)item_type = E_ItemTypes::Boots;
        break;
    }
    case 3:
    {
        int t = std::rand() % 5;
        if (t < 3)item_type = E_ItemTypes::Potion;
        if (t == 3)item_type = E_ItemTypes::Book;
        if (t == 4)item_type = E_ItemTypes::Boots;
        break;
    }
    case case_boots:
    case case_book:
    {
        int t = std::rand() % 18;
        item_type = (item_lvl == case_book) ? E_ItemTypes::Book : E_ItemTypes::Boots;
        if (t < 11)item_lvl = 1;
        else if (t < 16)item_lvl = 2;
        else item_lvl = 3;
        break;
    }
    case case_potion:
    {
        int t = std::rand() % 18;
        item_type = E_ItemTypes::Potion;
        if (t < 8)item_lvl = 1;
        else if (t < 14)item_lvl = 2;
        else item_lvl = 3;
        break;
    }
    case case_ring:
    {
        int t = std::rand() % 18;
        item_type = E_ItemTypes::Ring;
        if (t < 13)item_lvl = 1;
        else item_lvl = 2;
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
    {
        inventory = true;
        Image img {0, 0, 0};
        if (item_lvl == 1) {
            if (std::rand() % 2) {
                img = Image {"../resources/ring_hp_1.png"};
                ia = [](bool on) {
                    auto &pl = Player::Get();
                    if (on) pl.MaxHpChange(10);
                    else pl.MaxHpChange(-10);
                };
            } else {
                img = Image {"../resources/ring_dmg_1.png"};
                ia = [](bool on) {
                    auto &pl = Player::Get();
                    if (on) pl.DamageChange(1);
                    else pl.DamageChange(-1);
                };
            }
        } else {
            img = Image {"../resources/ring_dmg_2.png"};
            ia = [](bool on) {
                auto &pl = Player::Get();
                if (on) pl.DamageChange(2);
                else pl.DamageChange(-2);
            };
        }
        img.PixelsChange([](auto x) { return (x.a == 0 || x.r == x.b && x.r == x.g && x.r == 255) ? TRANSP_COLOR : x; }, false);
        spr = Sprite {img, SpritePixSz(15)};
        ict = IsPlaceForItem;
        break;
    }
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
    {
        inventory = true;
        can_be_used = true;
        Image img {0, 0, 0};
        if (item_lvl == 3 || item_lvl == 1) {
            img = Image {"../resources/book_dmg_" + std::to_string(item_lvl) + ".png"};
            use = [power = item_lvl]() { Player::Get().DamageChange(power); };
        } else {
            int t = std::rand() % 3;
            switch (t) {
            case 1:
            {
                img = Image {"../resources/book_hp.png"};
                use = []() { Player::Get().MaxHpChange(10); };
                break;
            }
            case 2:
            {
                img = Image {"../resources/book_inv.png"};
                use = []() { Player::Get().InventorySizeInc(); };
                break;
            }
            default:
            {
                img = Image {"../resources/book_dmg_2.png"};
                use = []() { Player::Get().DamageChange(2); };
                break;
            }
            }
        }

        img.PixelsChange([](auto x) { return (x.a == 0 || x.r == x.b && x.r == x.g && x.r == 255) ? TRANSP_COLOR : x; }, false);
        spr = Sprite{img, SpritePixSz(14)};
        ict = IsPlaceForItem;
        ia = [item_lvl = item_lvl, pos = pos](bool on) {};
        break;
    }
    case E_ItemTypes::Other:
    {
        inventory = true;
        spr = Sprite {"../resources/feat.png", 1};
        ict = IsPlaceForItem;
        ia = [pos = pos](bool on) {
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