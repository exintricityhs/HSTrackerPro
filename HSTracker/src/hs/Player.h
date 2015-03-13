/*
 * Player.h
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

#ifndef HS_PLAYER_H_
#define HS_PLAYER_H_

#include <vector>
#include <map>

#include "PlayedCard.h"
#include "Deck.h"
#include "../util/DBCreator.h"

namespace HS
{

class Player
{
public:
    enum PlayerType
    {
        Invalid = -1,
        
        Self = 0,
        Opponent,
    };
    
    Player();
    ~Player();
    
    /**
     * Initializes the player to no player
     */
    void reset();
    
    /**
     * sets the DB
     */
    void setDB(DBCreator* db);
    
    /**
     * Gets/sets the player id (1 or 2)
     */
    uint32_t getPlayerId();
    void setPlayerId(uint32_t playerId);
    
    /**
     * Gets/sets the player type
     */
    PlayerType getPlayerType();
    void setPlayerType(PlayerType type);

    /**
     * Adds a card to the player's hand
     * @return True if added, false if invalid card or already added
     */
    bool addCardToHand(const HS::PlayedCard& card);
    
    /**
     * Removes a card from the player's hand
     */
    void removeCardFromHand(HS::PlayedCard& card);
    
    /**
     * Returns a list of new cards added to the player's hand
     */
    std::vector<HS::PlayedCard> getNewCardsInHand();
    
    /**
     * Returns player deck if updated, nullptr otherwise
     */
    Deck* getDeckUpdate();

    /**
     * Gets/sets the hero id 
     * e.g. HERO_04 = Uther Lightbringer
     */
    const std::string& getHeroId();
    void setHeroId(const std::string& heroId);

private:
    uint32_t mPlayerId;
    PlayerType mType;
    std::string mHeroId;
    std::vector<HS::PlayedCard> mHand;
    Deck mDeck;
    bool mDeckUpdated;
    std::map<std::string, HS::PlayedCard> mCardHistory;
    std::vector<HS::PlayedCard> mNewCardsInPlay;
    DBCreator* mDB;
};

} /* namespace HS */

#endif /* HS_PLAYER_H_ */
