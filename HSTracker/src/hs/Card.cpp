/*
 * Card.cpp
 *
 *  Created on: Jan 31, 2015
 *      Author: EXintricity
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "Card.h"

#include <iostream>
#include <algorithm>

namespace HS
{

Card::Card()
    : mName()
    , mCardId()
    , mCardType(CardType_Invalid)
    , mAttack(0)
    , mHealth(0)
    , mCost(0)
    , mRarity(CardRarity_Invalid)
{
}

Card::~Card()
{
}

void Card::setName(const std::string& name)
{
    mName = name;
}

const std::string& Card::getName()
{
    return mName;
}

void Card::setCardId(const std::string& cardId)
{
    mCardId = cardId;
}

const std::string& Card::getCardId()
{
    return mCardId;
}

void Card::setCardType(CardType type)
{
    if (mCardType < CardType_Invalid)
    {
        mCardType = type;
    }
}

void Card::setCardType(std::string type)
{
    if (type.compare("Minion") == 0)
    {
        mCardType = CardType_Minion;
    }
    else if (type.compare("Spell") == 0)
    {
        mCardType = CardType_Spell;
    }
    else if (type.compare("Hero") == 0)
    {
        mCardType = CardType_Hero;
    }
    else if (type.compare("Hero Power") == 0)
    {
        mCardType = CardType_HeroPower;
    }
    else if (type.compare("Enchantment") == 0)
    {
        mCardType = CardType_Enchantment;
    }
    else if (type.compare("Weapon") == 0)
    {
        mCardType = CardType_Weapon;
    }
    else
    {
        mCardType = CardType_Invalid;
    }
}

Card::CardType Card::getCardType()
{
    return mCardType;
}

void Card::setAttack(uint32_t attack)
{
    mAttack = attack;
}

uint32_t Card::getAttack()
{
    return mAttack;
}

void Card::setHealth(uint32_t health)
{
    mHealth = health;
}

uint32_t Card::getHealth()
{
    return mHealth;
}

void Card::setCost(uint32_t cost)
{
    mCost = cost;
}

uint32_t Card::getCost()
{
    return mCost;
}


void Card::setRarity(std::string rarity)
{
    if (rarity.compare("Free") == 0)
    {
        mRarity = CardRarity_Free;
    }
    else if (rarity.compare("Common") == 0)
    {
        mRarity = CardRarity_Common;
    }
    else if (rarity.compare("Rare") == 0)
    {
        mRarity = CardRarity_Rare;
    }
    else if (rarity.compare("Epic") == 0)
    {
        mRarity = CardRarity_Epic;
    }
    else if (rarity.compare("Legendary") == 0)
    {
        mRarity = CardRarity_Legendary;
    }
    else
    {
        mRarity = CardRarity_Invalid;
    }
}

void Card::setRarity(CardRarity rarity)
{
    mRarity = rarity;
}

Card::CardRarity Card::getRarity()
{
    return mRarity;
}

std::string Card::getNoSpaceName()
{
    std::string ret = mName;
    
    std::transform(ret.begin(), ret.end(), ret.begin(), ::tolower);
    std::replace(ret.begin(), ret.end(), ' ', '-');
    std::replace(ret.begin(), ret.end(), '\'', '-');
    ret.erase(std::remove(ret.begin(), ret.end(), '.'), ret.end());
    ret.erase(std::remove(ret.begin(), ret.end(), ':'), ret.end());
    return ret;
}
    
}   // namespace HS
