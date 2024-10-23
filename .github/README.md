# Random Enchants Module

This is a fork of the Random Enchants Module for Azerothcore which adds more configuration options to the original one.

## Original configuration options

- Login message for the module (on, off and what to display)
- Select if you want to enchant loot, quest reward, profession or all
- Modify the chances of subsequent enchantments

## Added configuration options

- Setting minimum item level for random enchantments (per tier)
- Accumulative enchantment tiers (e.g. roll a tier 2 enchantment then you can get a tier 1 or 2)
- Exclude list for items id
- Exclude item qualities
- Disable in PvP Arena and/or Battlegrounds
- Max random enchantment tier per item quality
- Maximum number of random enchants per item quality
- Random enchantment chance for any of the 3 slots configurable per item quality (Altough they keep being accumulative so if you dont get the first you can't get the second and so on...)
- Random enchantment tier chance per item quality
- Enable/disable random enchant messages and customize them (Defaults are in spanish so you probably want to change them)

## Default configuration

This is a default configuration that the module comes with:

| **Item<br>Quality** | **Max<br>Enchant Tier** | **Max N° of<br>Random Enchants** | **Enchant<br>Chance (1)** | **Enchant <br>Chance (2)** | **Enchant<br>Chance (3)** | **Tier 1<br>Chance** | **Tier 2<br>Chance** | **Tier 3<br>Chance** | **Tier 4<br>Chance** | **Tier 5<br>Chance** |
|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|
| <span style="color:grey">Grey</span> | 1 | 1 | 10% | - | - | 100% | 0% | 0% | 0% | 0% |
| White | 1 | 1 | 10% | - | - | 100% | 0% | 0% | 0% | 0% |
| <span style="color:green">Green</span> | 2 | 1 | 10% | - | - | 90% | 10% | 0% | 0% | 0% |
| <span style="color:blue">Blue</span> | 3 | 2 | 10% | 5% | - | 70% | 20% | 10% | 0% | 0% |
| <span style="color:purple">Purple</span> | 4 | 3 | 7% | 5% | 3% | 50% | 30% | 15% | 5% | 0% |
| <span style="color:orange">Orange</span> | 5 | 3 | 3% | 2% | 1% | 50% | 30% | 10% | 7% | 3% |

_This numbers asumes that the item meets the minimum item level requirement for every tier, if they don't then (depending on the config) they will recieve the maximum tier they can if rolled or be discarded_

- **Enchant Chance (N)** is the chance that the item will get an enchantment in that (N) slot. They're accumulative (for now at least) so if you don't get the first enchant then you can't get the next ones.
- **Tier (N) Chance** is the chance of, once getting the _Enchant Chance (N) roll_, getting an enchantment of (N) tier in that slot.

## Adding possible random enchantments

Not all the possible enchantments are added in the module but you can add them if you like (or remove some of them if you don't). 
The module creates a table in `acore_world` named `item_enchantment_random_tier` where you will find entries like this

| **Enchant ID** | **Tier** | **Class** | **exclusiveSubClass** |
|:---:|:---:|:---:|:---:|

Adding an enchantment is as simple as adding a new entry to the table where:
- **Enchant ID** is the ID of the enchant to be added, it can be obtained from [this site](https://wowwiki-archive.fandom.com/wiki/EnchantId/Enchant_IDs) or by extracting the SpellItemEnchantments.dbc file. I've extracted the file and uploaded it to this [Google Spreadsheet](https://docs.google.com/spreadsheets/d/18Rwvshr7yiFzUwzVudMOrgBaoGdyQdK8L3u_xEVmqLE/edit?usp=sharing) in case you're interested.
- **Tier** is an arbitrary number in the range 1-5 (inclusive), choose whatever suits the enchantment for you.
- **Class** is a text that can be `ANY`, `WEAPON` or `ARMOR` depending if the enchantment is exclusive to a category or not.
- **exclusiveSubClass** is a number that refers to an item subclass, it's used to be able to have random enchantments coherent with the weapon. e.g: `+3 Sword Skill` should be exclusive to swords. You can find the subclasses numbers [here](https://www.azerothcore.org/wiki/item_template#subclass).

## TODO

This fork it's still WIP and i have some other changes i want to make, but i consider it to be customizable enough for most of the players, if you encounter any problem or you want to improve the module then don't hesitate opening an issue/enhancement.

- [ ] Allow adding more tiers so you can categorize your enchantment in as many tiers as you like
- [x] Move the in-game item enchant message to config ~~(Right now is in spanish and it have a typo, mb)~~
- [ ] Add option for random enchantment rolls to not be accumulative

## Credits

Credits to whoever make the original lua script, the people behind the original module and AzerothCore for the amazing core.

# Original README.md
# ![logo](https://raw.githubusercontent.com/azerothcore/azerothcore.github.io/master/images/logo-github.png) AzerothCore

## RandomEnchantsModule

- Latest build status with azerothcore:

[![Build Status](https://github.com/azerothcore/mod-random-enchants/workflows/core-build/badge.svg?branch=master&event=push)](https://github.com/azerothcore/mod-random-enchants)

- Make sure to support the main project:

https://github.com/azerothcore/azerothcore-wotlk/

- This is a "fun" script/module converted from an LUA script I found, credits to whoever did this originally

## What this does

- Will sometimes add random enchantments on any item Looted, Quest Rewarded or Profession created

## Configurable

- Login message for the module (on, off and what to display)
- Select if you want to enchant loot, quest reward, profession or all
- Modify the source to mess with the chances of enchantment
