/*
 * Game.cpp
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

#include "Game.h"
#include "../util/Logger.h"

namespace HS
{
    
const uint32_t MaxPlayers = 2;

Game::Game()
    : mPlayers()
    , mHistory()
    , mDB(nullptr)
{
    for (uint32_t i=0; i<MaxPlayers; ++i)
    {
        mPlayers.push_back(Player());
        mPlayers[i].setPlayerId(i + 1);
    }
}

Game::~Game()
{
}
    
void Game::reset()
{
    for (uint32_t i=0; i<MaxPlayers; ++i)
    {
        mPlayers[i].reset();
        mPlayers[i].setPlayerId(i + 1);
    }
    
    mHistory.clear();
}

Player* Game::getSelf()
{
    for (uint32_t i=0; i<MaxPlayers; ++i)
    {
        if (mPlayers[i].getPlayerType() == Player::Self)
        {
            return &mPlayers[i];
        }
    }
    
    return nullptr;
}

void Game::setSelf(const std::string& playerId)
{
    int32_t playerNumber = std::stoi(playerId);
    
    if (playerNumber > MaxPlayers || playerNumber <= 0)
    {
        LOG_WARN("Invalid playerId passed in" << std::endl);
        return;
    }
    
    mPlayers[playerNumber - 1].setPlayerType(Player::Self);
}

Player* Game::getOpponent()
{
    for (uint32_t i=0; i<MaxPlayers; ++i)
    {
        if (mPlayers[i].getPlayerType() == Player::Opponent)
        {
            return &mPlayers[i];
        }
    }
    
    return nullptr;
}

void Game::setOpponent(const std::string& playerId)
{
    int32_t playerNumber = std::stoi(playerId);
    
    if (playerNumber > MaxPlayers || playerNumber <= 0)
    {
        LOG_WARN("Invalid playerId passed in" << std::endl);
        return;
    }
    
    mPlayers[playerNumber - 1].setPlayerType(Player::Opponent);
}
    
void Game::setDB(DBCreator* db)
{
    mDB = db;
    
    for (uint32_t i=0; i<MaxPlayers; ++i)
    {
        mPlayers[i].setDB(db);
    }
}

void Game::moveDeckToHand(const std::string& playerId, const std::string& id, const std::string& cardId)
{
    PlayedCard card;
    card.setCardId(cardId);
    card.setId(id);
    
    int32_t playerNumber = std::stoi(playerId);
    
    if (playerNumber > MaxPlayers || playerNumber <= 0 || mPlayers[playerNumber - 1].getPlayerType() != Player::Self)
    {
        return;
    }
    
    if (mPlayers[playerNumber - 1].addCardToHand(card))
    {
        if (mDB)
        {
            Card* card = mDB->findCardById(cardId);
            
            if (card)
            {
                LOG_INFO("Added card "<< card->getName() << " with id: " << id << " to player" << playerId << std::endl);
            }
        }
    }
}
    
void Game::playCard(const std::string& playerId, const std::string& id, const std::string& cardId)
{
    for (PlayHistory::reverse_iterator it=mHistory.rbegin(); it!=mHistory.rend(); ++it)
    {
        if (it->getId() == id)
        {
            return;
        }
    }
    
    // check exceptions (e.g. minion from hero power)
    if (isIgnored(cardId))
    {
        return;
    }
    
    Card* justPlayedCard = nullptr;

    if (mDB)
    {
        justPlayedCard = mDB->findCardById(cardId);
        
        if (justPlayedCard)
        {
            LOG_INFO("Player " << playerId << " played card:" << justPlayedCard->getName() << " id:" << id << "cardId: " << cardId << std::endl);
        }
    }
    
    PlayedCard card;
    card.setId(id);
    card.setCardId(cardId);
    
    // add card to game history
    mHistory.push_back(card);
    
    // remove card from player's hand
    int32_t playerNumber = std::stoi(playerId);
    
    if (playerNumber > MaxPlayers || playerNumber <= 0)
    {
        LOG_WARN("Invalid playerId passed in" << std::endl);
        return;
    }
    
    mPlayers[playerNumber - 1].removeCardFromHand(card);
}
    
bool Game::isIgnored(const std::string& cardId)
{
    if (mDB)
    {
        Card* card = mDB->findCardById(cardId);
        
        if (!card)
        {
            return false;
        }
        
        if (card->getCardType() == Card::CardType_Hero || card->getCardType() == Card::CardType_HeroPower || !card->getCollectible())
        {
            return true;
        }
    }
    
    return false;
}

}   // HS







