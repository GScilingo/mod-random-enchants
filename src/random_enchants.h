/*
* Converted from the original LUA script to a module for Azerothcore (Sunwell)
*/

#include "ScriptMgr.h"
#include "Player.h"
#include "Configuration/Config.h"
#include "Chat.h"

enum ItemQuality
{
    GREY = 0,
    WHITE,
    GREEN,
    BLUE,
    PURPLE,
    ORANGE
};

void rollPossibleEnchant(Player* player, Item* item);
[[nodiscard]] uint32 getRandEnchantment(Item* item);
uint32 GetMaxEnchantmentTier(uint32 itemLevel);
bool IsExcludedItem(uint32 itemId);
bool IsExcludedQuality(uint32 itemQuality);

class RandomEnchantsPlayer : public PlayerScript
{
public:
    RandomEnchantsPlayer() : PlayerScript("RandomEnchantsPlayer") { }

    void OnLogin(Player* player) override;
    void OnLootItem(Player* player, Item* item, uint32 /*count*/, ObjectGuid /*lootguid*/) override;
    void OnCreateItem(Player* player, Item* item, uint32 /*count*/) override;
    void OnQuestRewardItem(Player* player, Item* item, uint32 /*count*/) override;
    void OnGroupRollRewardItem(Player* player, Item* item, uint32 /*count*/, RollVote /*voteType*/, Roll* /*roll*/) override;
};

class RandomEnchantsConfig : public WorldScript
{
    public:
        RandomEnchantsConfig() : WorldScript("RandomEnchantConfig") { }

        void OnBeforeConfigLoad(bool /*reload*/) override;
};

void AddRandomEnchantsScripts()
{
    new RandomEnchantsConfig();
    new RandomEnchantsPlayer();
}
