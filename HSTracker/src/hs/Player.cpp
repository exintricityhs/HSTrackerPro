/*
 * Player.cpp
 *
 *  Created on: Feb 18, 2015
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

#include "Player.h"

#include <iostream>

namespace HS
{

Player::Player()
    : mPlayerId(0)
    , mType(Invalid)
    , mHeroId()
    , mHand()
    , mDeck()
    , mDeckUpdated(false)
    , mCardHistory()
    , mNewCardsInPlay()
    , mDB(nullptr)
{
}


Player::~Player()
{
}
    
void Player::reset()
{
    mPlayerId = 0;
    mType = Invalid;
    mHeroId.clear();
    mHand.clear();
    mDeck.reset();
    mDeckUpdated = false;
    mCardHistory.clear();
    mNewCardsInPlay.clear();
}
    
void Player::setDB(DBCreator* db)
{
    mDB = db;
}

uint32_t Player::getPlayerId()
{
    return mPlayerId;
}

void Player::setPlayerId(uint32_t playerId)
{
    mPlayerId = playerId;
}
    
Player::PlayerType Player::getPlayerType()
{
    return mType;
}

void Player::setPlayerType(PlayerType type)
{
    mType = type;
}

bool Player::addCardToHand(const HS::PlayedCard& playedCard)
{
    if (!playedCard.getId().length())
    {
        std::cout << "addCardToHand - ignoring due to no id" << std::endl;
        return false;
    }
    
    // ensure it's not already accounted for
    for (std::map<std::string, HS::PlayedCard>::iterator it=mCardHistory.begin(); it!=mCardHistory.end(); ++it)
    {
        if (!it->first.compare(playedCard.getId()))
        {
            return false;
        }
    }
    
    mHand.push_back(playedCard);
    mCardHistory.insert(std::pair<std::string, HS::PlayedCard>(playedCard.getId(), playedCard));
    mNewCardsInPlay.push_back(playedCard);
    return true;
}
    
void Player::removeCardFromHand(HS::PlayedCard& card)
{
    for (std::vector<HS::PlayedCard>::iterator it=mHand.begin(); it!=mHand.end();)
    {
        if (!it->getId().compare(card.getId()))
        {
            it = mHand.erase(it);
        }
        else
        {
            ++it;
        }
    }
    
    if (mType == Player::Self)
    {
        /*std::cout << "SELF hand: " << std::endl;
        
        for (std::vector<HS::PlayedCard>::iterator it=mHand.begin(); it!=mHand.end(); ++it)
        {
            std::cout << "    cardId:" << it->getCardId() << std::endl;
        }*/
    }
    else if (mType == Player::Opponent)
    {
        if (mDeck.getCardById(card.getCardId()))
        {
            mDeck.incrementCount(card.getCardId(), 1);
        }
        else
        {
            if (mDB)
            {
                Card* dbCard = mDB->findCardById(card.getCardId());
                
                if (dbCard)
                {
                    mDeck.addCardToDeck(dbCard, 1);
                }
            }
            
        }
        
        mDeckUpdated = true;
    }
}
    
std::vector<HS::PlayedCard> Player::getNewCardsInHand()
{
    std::vector<HS::PlayedCard> newCards = mNewCardsInPlay;
    mNewCardsInPlay.clear();
    return newCards;
}
    
Deck* Player::getDeckUpdate()
{
    if (mDeckUpdated)
    {
        mDeckUpdated = false;
        return &mDeck;
    }
    else
    {
        return nullptr;
    }
}

const std::string& Player::getHeroId()
{
    return mHeroId;
}

void Player::setHeroId(const std::string& heroId)
{
    mHeroId = heroId;
}


} /* namespace HS */
