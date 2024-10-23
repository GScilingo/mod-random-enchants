#include "random_enchants.h"
#include "utils/Utils.h"
#include "Configuration/Config.h"

std::vector<uint32_t> ExcludedItems;
std::vector<uint32_t> ExcludedQualities;

void rollPossibleEnchant(Player* player, Item* item)
{
    uint32 itemQuality = item->GetTemplate()->Quality;
    uint32 itemClass = item->GetTemplate()->Class;
    uint32 itemId = item->GetTemplate()->ItemId;

    /* eliminates enchanting anything that isn't a recognized quality */
    /* eliminates enchanting anything but weapons/armor */
    if ((itemQuality > 5 || itemQuality < 0) || (itemClass != 2 && itemClass != 4))
    {
        return;
    }

    int slotRand[3] = { -1, -1, -1 };
    uint32 slotEnch[3] = { 0, 1, 5 };

    // Fetching the configuration values as float
    // Depends on item quality (config)
    float enchantChance1;
    float enchantChance2;
    float enchantChance3;
    int maxNumberOfEnchantments;
    switch (itemQuality)
    {
        case GREY:
            enchantChance1 = sConfigMgr->GetOption<float>("RandomEnchants.GreyItemRandomEnchantChance1", 5.0f);
            enchantChance2 = sConfigMgr->GetOption<float>("RandomEnchants.GreyItemRandomEnchantChance2", 0.0f);
            enchantChance3 = sConfigMgr->GetOption<float>("RandomEnchants.GreyItemRandomEnchantChance3", 0.0f);
            maxNumberOfEnchantments = sConfigMgr->GetOption<int>("RandomEnchants.GreyItemMaxRandomEnchantNumber", 1);
            break;
        case WHITE:
            enchantChance1 = sConfigMgr->GetOption<float>("RandomEnchants.WhiteItemRandomEnchantChance1", 10.0f);
            enchantChance2 = sConfigMgr->GetOption<float>("RandomEnchants.WhiteItemRandomEnchantChance2", 0.0f);
            enchantChance3 = sConfigMgr->GetOption<float>("RandomEnchants.WhiteItemRandomEnchantChance3", 0.0f);
            maxNumberOfEnchantments = sConfigMgr->GetOption<int>("RandomEnchants.WhiteItemMaxRandomEnchantNumber", 1);
            break;
        case GREEN:
            enchantChance1 = sConfigMgr->GetOption<float>("RandomEnchants.GreenItemRandomEnchantChance1", 10.0f);
            enchantChance2 = sConfigMgr->GetOption<float>("RandomEnchants.GreenItemRandomEnchantChance2", 0.0f);
            enchantChance3 = sConfigMgr->GetOption<float>("RandomEnchants.GreenItemRandomEnchantChance3", 0.0f);
            maxNumberOfEnchantments = sConfigMgr->GetOption<int>("RandomEnchants.GreenItemMaxRandomEnchantNumber", 1);
            break;
        case BLUE:
            enchantChance1 = sConfigMgr->GetOption<float>("RandomEnchants.BlueItemRandomEnchantChance1", 10.0f);
            enchantChance2 = sConfigMgr->GetOption<float>("RandomEnchants.BlueItemRandomEnchantChance2", 5.0f);
            enchantChance3 = sConfigMgr->GetOption<float>("RandomEnchants.BlueItemRandomEnchantChance3", 0.0f);
            maxNumberOfEnchantments = sConfigMgr->GetOption<int>("RandomEnchants.BlueItemMaxRandomEnchantNumber", 2);
            break;
        case PURPLE:
            enchantChance1 = sConfigMgr->GetOption<float>("RandomEnchants.PurpleItemRandomEnchantChance1", 7.0f);
            enchantChance2 = sConfigMgr->GetOption<float>("RandomEnchants.PurpleItemRandomEnchantChance2", 5.0f);
            enchantChance3 = sConfigMgr->GetOption<float>("RandomEnchants.PurpleItemRandomEnchantChance3", 3.0f);
            maxNumberOfEnchantments = sConfigMgr->GetOption<int>("RandomEnchants.PurpleItemMaxRandomEnchantNumber", 3);
            break;
        case ORANGE:
            enchantChance1 = sConfigMgr->GetOption<float>("RandomEnchants.OrangeItemRandomEnchantChance1", 3.0f);
            enchantChance2 = sConfigMgr->GetOption<float>("RandomEnchants.OrangeItemRandomEnchantChance2", 2.0f);
            enchantChance3 = sConfigMgr->GetOption<float>("RandomEnchants.OrangeItemRandomEnchantChance3", 1.0f);
            maxNumberOfEnchantments = sConfigMgr->GetOption<int>("RandomEnchants.OrangeItemMaxRandomEnchantNumber", 3);
            break;
        default:
            break;
    }
    // float enchantChance1 = sConfigMgr->GetOption<float>("RandomEnchants.EnchantChance1", 70.0f);
    // float enchantChance2 = sConfigMgr->GetOption<float>("RandomEnchants.EnchantChance2", 65.0f);
    // float enchantChance3 = sConfigMgr->GetOption<float>("RandomEnchants.EnchantChance3", 60.0f);

    // Depende de la cantidad máxima de tiers que admita la calidad se debe
    // actuar acorde
    if (rand_chance() < enchantChance1 && maxNumberOfEnchantments > 0)
        slotRand[0] = getRandEnchantment(item);
    if (slotRand[0] != -1 && rand_chance() < enchantChance2 && maxNumberOfEnchantments > 1)
        slotRand[1] = getRandEnchantment(item);
    if (slotRand[1] != -1 && rand_chance() < enchantChance3 && maxNumberOfEnchantments > 2)
        slotRand[2] = getRandEnchantment(item);

    for (int i = 0; i < 3; i++)
    {
        if (slotRand[i] != -1)
        {
            if (sSpellItemEnchantmentStore.LookupEntry(slotRand[i]))
            {   //Make sure enchantment id exists
                player->ApplyEnchantment(item, EnchantmentSlot(slotEnch[i]), false);
                item->SetEnchantment(EnchantmentSlot(slotEnch[i]), slotRand[i], 0, 0);
                player->ApplyEnchantment(item, EnchantmentSlot(slotEnch[i]), true);
            }
        }
    }

    if (!sConfigMgr->GetOption<bool>("RandomEnchants.RandomEnchantMessageEnabled", true))
        return;

    ChatHandler chathandle = ChatHandler(player->GetSession());
    uint8 loc_idx = player->GetSession()->GetSessionDbLocaleIndex();
    const ItemTemplate* temp = item->GetTemplate();
    std::string name = temp->Name1;
    if (ItemLocale const* il = sObjectMgr->GetItemLocale(temp->ItemId))
        ObjectMgr::GetLocaleString(il->Name, loc_idx, name);

    if (slotRand[2] != -1)
        chathandle.PSendSysMessage(
            sConfigMgr->GetOption<std::string>(
                    "RandomEnchants.RandomEnchantMessage3",
                    "El objeto obtenido|cffFF0000 {} |rrecibio 3 encantamientos aleatorios!"),
            name);
    else if (slotRand[1] != -1)
        chathandle.PSendSysMessage(
            sConfigMgr->GetOption<std::string>(
                    "RandomEnchants.RandomEnchantMessage2",
                    "El objeto obtenido|cffFF0000 {} |rrecibio 2 encantamientos aleatorios!"),
            name);
    else if (slotRand[0] != -1)
        chathandle.PSendSysMessage(
            sConfigMgr->GetOption<std::string>(
                    "RandomEnchants.RandomEnchantMessage1",
                    "El objeto obtenido|cffFF0000 {} |rrecibio 1 encantamiento aleatorio!"),
            name);
}

uint32 getRandEnchantment(Item* item)
{
    uint32 itemClass = item->GetTemplate()->Class;
    uint32 itemQuality = item->GetTemplate()->Quality;
    uint32 itemSubClass = item->GetTemplate()->SubClass;
    uint32 itemLevel = item->GetTemplate()->ItemLevel;
    std::string classQueryString = "";
    int rarityRoll = -1;
    uint32 tier = 0;

    switch (itemClass)
    {
        case 2:
            classQueryString = "WEAPON";
            break;
        case 4:
            classQueryString = "ARMOR";
            break;
    }

    if (classQueryString == "")
        return -1;

    double enchantProbabilities[5];

    switch (itemQuality)
    {
        case GREY:
            enchantProbabilities[0] = sConfigMgr->GetOption<float>("RandomEnchants.GreyItemTier1RandomEnchantChance", 100.0f);
            enchantProbabilities[1] = sConfigMgr->GetOption<float>("RandomEnchants.GreyItemTier2RandomEnchantChance", 0.0f);
            enchantProbabilities[2] = sConfigMgr->GetOption<float>("RandomEnchants.GreyItemTier3RandomEnchantChance", 0.0f);
            enchantProbabilities[3] = sConfigMgr->GetOption<float>("RandomEnchants.GreyItemTier4RandomEnchantChance", 0.0f);
            enchantProbabilities[4] = sConfigMgr->GetOption<float>("RandomEnchants.GreyItemTier5RandomEnchantChance", 0.0f);

            tier = urandweighted(5, enchantProbabilities) + 1;
            break;
        case WHITE:
            enchantProbabilities[0] = sConfigMgr->GetOption<float>("RandomEnchants.WhiteItemTier1RandomEnchantChance", 100.0f);
            enchantProbabilities[1] = sConfigMgr->GetOption<float>("RandomEnchants.WhiteItemTier2RandomEnchantChance", 0.0f);
            enchantProbabilities[2] = sConfigMgr->GetOption<float>("RandomEnchants.WhiteItemTier3RandomEnchantChance", 0.0f);
            enchantProbabilities[3] = sConfigMgr->GetOption<float>("RandomEnchants.WhiteItemTier4RandomEnchantChance", 0.0f);
            enchantProbabilities[4] = sConfigMgr->GetOption<float>("RandomEnchants.WhiteItemTier5RandomEnchantChance", 0.0f);

            tier = urandweighted(5, enchantProbabilities) + 1;
            break;
        case GREEN:
            enchantProbabilities[0] = sConfigMgr->GetOption<float>("RandomEnchants.GreenItemTier1RandomEnchantChance", 90.0f);
            enchantProbabilities[1] = sConfigMgr->GetOption<float>("RandomEnchants.GreenItemTier2RandomEnchantChance", 10.0f);
            enchantProbabilities[2] = sConfigMgr->GetOption<float>("RandomEnchants.GreenItemTier3RandomEnchantChance", 0.0f);
            enchantProbabilities[3] = sConfigMgr->GetOption<float>("RandomEnchants.GreenItemTier4RandomEnchantChance", 0.0f);
            enchantProbabilities[4] = sConfigMgr->GetOption<float>("RandomEnchants.GreenItemTier5RandomEnchantChance", 0.0f);

            tier = urandweighted(5, enchantProbabilities) + 1;
            break;
        case BLUE:
            enchantProbabilities[0] = sConfigMgr->GetOption<float>("RandomEnchants.BlueItemTier1RandomEnchantChance", 70.0f);
            enchantProbabilities[1] = sConfigMgr->GetOption<float>("RandomEnchants.BlueItemTier2RandomEnchantChance", 20.0f);
            enchantProbabilities[2] = sConfigMgr->GetOption<float>("RandomEnchants.BlueItemTier3RandomEnchantChance", 10.0f);
            enchantProbabilities[3] = sConfigMgr->GetOption<float>("RandomEnchants.BlueItemTier4RandomEnchantChance", 0.0f);
            enchantProbabilities[4] = sConfigMgr->GetOption<float>("RandomEnchants.BlueItemTier5RandomEnchantChance", 0.0f);

            tier = urandweighted(5, enchantProbabilities) + 1;
            break;
        case PURPLE:
            enchantProbabilities[0] = sConfigMgr->GetOption<float>("RandomEnchants.PurpleItemTier1RandomEnchantChance", 50.0f);
            enchantProbabilities[1] = sConfigMgr->GetOption<float>("RandomEnchants.PurpleItemTier2RandomEnchantChance", 30.0f);
            enchantProbabilities[2] = sConfigMgr->GetOption<float>("RandomEnchants.PurpleItemTier3RandomEnchantChance", 15.0f);
            enchantProbabilities[3] = sConfigMgr->GetOption<float>("RandomEnchants.PurpleItemTier4RandomEnchantChance", 5.0f);
            enchantProbabilities[4] = sConfigMgr->GetOption<float>("RandomEnchants.PurpleItemTier5RandomEnchantChance", 0.0f);

            tier = urandweighted(5, enchantProbabilities) + 1;
            break;
        case ORANGE:
            enchantProbabilities[0] = sConfigMgr->GetOption<float>("RandomEnchants.OrangeItemTier1RandomEnchantChance", 50.0f);
            enchantProbabilities[1] = sConfigMgr->GetOption<float>("RandomEnchants.OrangeItemTier2RandomEnchantChance", 30.0f);
            enchantProbabilities[2] = sConfigMgr->GetOption<float>("RandomEnchants.OrangeItemTier3RandomEnchantChance", 10.0f);
            enchantProbabilities[3] = sConfigMgr->GetOption<float>("RandomEnchants.OrangeItemTier4RandomEnchantChance", 7.0f);
            enchantProbabilities[4] = sConfigMgr->GetOption<float>("RandomEnchants.OrangeItemTier5RandomEnchantChance", 3.0f);

            tier = urandweighted(5, enchantProbabilities) + 1;
            break;
    }

    // if (rarityRoll < 0)
    //     return -1;

    // if (rarityRoll <= 44)
    //     tier = 1;
    // else if (rarityRoll <= 64)
    //     tier = 2;
    // else if (rarityRoll <= 79)
    //     tier = 3;
    // else if (rarityRoll <= 92)
    //     tier = 4;
    // else
    //     tier = 5;
    int maxItemEnchantmentTier = GetMaxEnchantmentTier(itemLevel);

    if (tier > maxItemEnchantmentTier) {
        if (sConfigMgr->GetOption<bool>("RandomEnchants.DiscardOutOfRangeTier", false)) {
            return 0; // Descarta el encantamiento porque se paso de rango
        }

        // Si se paso de rango entonces setealo al maximo posible
        tier = maxItemEnchantmentTier;
    }

    if (tier == 0)
        return 0;

    QueryResult result = nullptr;

    if (itemClass == 2)
    { 
        result = WorldDatabase.Query("SELECT `enchantID` FROM `item_enchantment_random_tiers` WHERE `tier`{}{} AND ((`exclusiveSubClass` IS NULL AND (`class`='ANY' OR `class`='WEAPON')) OR (`exclusiveSubClass`={} AND `class`='WEAPON')) ORDER BY RAND() LIMIT 1",
        sConfigMgr->GetOption<bool>("RandomEnchants.TierAcumulative", false) ? "<=" : "=",
        tier,
        itemSubClass);
    } else if (itemClass == 4) { 
        result = WorldDatabase.Query("SELECT `enchantID` FROM `item_enchantment_random_tiers` WHERE `tier`{}{} AND ((`class`='ANY' OR `class`='ARMOR')) ORDER BY RAND() LIMIT 1",
        sConfigMgr->GetOption<bool>("RandomEnchants.TierAcumulative", false) ? "<=" : "=",
        tier);
    }

    if (!result)
        return 0;

    return result->Fetch()[0].Get<uint32>();
}

bool IsExcludedItem(uint32 itemId) {
    if (std::find(ExcludedItems.begin(), ExcludedItems.end(), itemId) != ExcludedItems.end()) {
        return true;
    }

    return false;
}

bool IsExcludedQuality(uint32 itemQuality) {
    if (std::find(ExcludedQualities.begin(), ExcludedQualities.end(), itemQuality) != ExcludedQualities.end()) {
        return true;
    }

    return false;
}

uint32 GetMaxEnchantmentTier(uint32 itemLevel) {
    int tierOneMinItemLevel = sConfigMgr->GetOption<uint32>("RandomEnchants.TierOneMinItemLevel", 0);
    int tierTwoMinItemLevel = sConfigMgr->GetOption<uint32>("RandomEnchants.TierTwoMinItemLevel", 30);
    int tierThreeMinItemLevel = sConfigMgr->GetOption<uint32>("RandomEnchants.TierThreeMinItemLevel", 40);
    int tierFourMinItemLevel = sConfigMgr->GetOption<uint32>("RandomEnchants.TierFourMinItemLevel", 60);
    int tierFiveMinItemLevel = sConfigMgr->GetOption<uint32>("RandomEnchants.TierFiveMinItemLevel", 100);

    if (itemLevel < tierOneMinItemLevel) {
        return 0;
    } else if (itemLevel < tierTwoMinItemLevel) {
        return 1;
    } else if (itemLevel < tierThreeMinItemLevel) {
        return 2;
    } else if (itemLevel < tierFourMinItemLevel) {
        return 3;
    } else if (itemLevel < tierFiveMinItemLevel) {
        return 4;
    } else {
        return 5;

    }
}

void RandomEnchantsPlayer::OnLogin(Player* player)
{
    if (sConfigMgr->GetOption<bool>("RandomEnchants.AnnounceOnLogin", true) && (sConfigMgr->GetOption<bool>("RandomEnchants.Enable", true)))
        ChatHandler(player->GetSession()).SendSysMessage(sConfigMgr->GetOption<std::string>("RandomEnchants.OnLoginMessage", "This server is running a RandomEnchants Module.").c_str());
}

void RandomEnchantsPlayer::OnLootItem(Player* player, Item* item, uint32 /*count*/, ObjectGuid /*lootguid*/)
{
    if (
        sConfigMgr->GetOption<bool>("RandomEnchants.Enable", true) 
        && sConfigMgr->GetOption<bool>("RandomEnchants.OnLoot", true)  
        && !IsExcludedItem(item->GetTemplate()->ItemId)
        && !IsExcludedQuality(item->GetTemplate()->Quality)
        && (sConfigMgr->GetOption<bool>("RandomEnchants.DisableInArena", true) && !player->InArena())
        && (sConfigMgr->GetOption<bool>("RandomEnchants.DisableInBattleGrounds", true) && !player->InBattleground())
    )
        rollPossibleEnchant(player, item);
}

void RandomEnchantsPlayer::OnCreateItem(Player* player, Item* item, uint32 /*count*/)
{
    if (
        sConfigMgr->GetOption<bool>("RandomEnchants.Enable", true)
        && sConfigMgr->GetOption<bool>("RandomEnchants.OnCreate", true) 
        && !IsExcludedItem(item->GetTemplate()->ItemId)
        && !IsExcludedQuality(item->GetTemplate()->Quality)
        && (sConfigMgr->GetOption<bool>("RandomEnchants.DisableInArena", true) && !player->InArena())
        && (sConfigMgr->GetOption<bool>("RandomEnchants.DisableInBattleGrounds", true) && !player->InBattleground())
    )
        rollPossibleEnchant(player, item);
}

void RandomEnchantsPlayer::OnQuestRewardItem(Player* player, Item* item, uint32 /*count*/)
{
    if (
        sConfigMgr->GetOption<bool>("RandomEnchants.Enable", true)
        && sConfigMgr->GetOption<bool>("RandomEnchants.OnQuestReward", true) 
        && !IsExcludedItem(item->GetTemplate()->ItemId)
        && !IsExcludedQuality(item->GetTemplate()->Quality)
        && (sConfigMgr->GetOption<bool>("RandomEnchants.DisableInArena", true) && !player->InArena())
        && (sConfigMgr->GetOption<bool>("RandomEnchants.DisableInBattleGrounds", true) && !player->InBattleground())
    )
        rollPossibleEnchant(player, item);
}

void RandomEnchantsPlayer::OnGroupRollRewardItem(Player* player, Item* item, uint32 /*count*/, RollVote /*voteType*/, Roll* /*roll*/)
{
    if (
        sConfigMgr->GetOption<bool>("RandomEnchants.Enable", true)
        && sConfigMgr->GetOption<bool>("RandomEnchants.OnGroupRoll", true) 
        && !IsExcludedItem(item->GetTemplate()->ItemId)
        && !IsExcludedQuality(item->GetTemplate()->Quality)
        && (sConfigMgr->GetOption<bool>("RandomEnchants.DisableInArena", true) && !player->InArena())
        && (sConfigMgr->GetOption<bool>("RandomEnchants.DisableInBattleGrounds", true) && !player->InBattleground())
    )
        rollPossibleEnchant(player, item);
}

void RandomEnchantsConfig::OnBeforeConfigLoad(bool /*reload*/)
{
    // Load excluded items
    LoadList(sConfigMgr->GetOption<std::string>("RandomEnchants.ExcludedItems", ""), ExcludedItems);
    LoadList(sConfigMgr->GetOption<std::string>("RandomEnchants.ExcludedQualities", ""), ExcludedQualities);
}