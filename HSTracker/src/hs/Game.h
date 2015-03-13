/*
 * Game.h
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

#ifndef HS_GAME_H_
#define HS_GAME_H_


#include <string>
#include <list>

#include "Player.h"
#include "PlayedCard.h"
#include "../util/DBCreator.h"

namespace HS
{

class Game
{
public:
    Game();
    ~Game();
    
    /**
     * Resets the game state.
     */
    void reset();

    /**
     * Gets/sets self based on player id.
     */
    Player* getSelf();
    void setSelf(const std::string& playerId);
    
    /**
     * Gets/sets opponent.
     */
    Player* getOpponent();
    void setOpponent(const std::string& playerId);
    
    /**
     * Sets the DB
     */
    void setDB(DBCreator* db);
    
    /**
     * Moves a card from the player's deck into his hand.
     */
    void moveDeckToHand(const std::string& playerId, const std::string& id, const std::string& cardId);
    
    /**
     * Tracks played cards
     */
    void playCard(const std::string& playerId, const std::string& id, const std::string& cardId);
    
    /**
     * Checks whether a card should be ignored for tracking.
     */
    bool isIgnored(const std::string& cardId);

private:
    typedef std::list<PlayedCard> PlayHistory;
    
    std::vector<Player> mPlayers;
    PlayHistory mHistory;
    DBCreator* mDB;
};

} /* namespace HS */

#endif /* HS_GAME_H_ */
