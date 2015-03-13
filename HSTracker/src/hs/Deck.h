/*
 * Deck.h
 *
 *  Created on: Feb 19, 2015
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

#ifndef HS_DECK_H_
#define HS_DECK_H_

#include <fstream>
#include <vector>

#include "../util/DBCreator.h"

namespace HS
{

/**
 * Deck represents a player's deck (arena or constructed), with a max of 30 cards
 */
class Deck
{
public:
    Deck();
    Deck(const std::string& filename);
    Deck(const Deck& rhs);
    ~Deck();

    /**
     * Opens deck file.
     * @param file Full path of the file.
     * @return True if the file was opened, false otherwise.
     */
    bool open(const std::string& filename);

    /**
     * Creates an array of Cards based on deck File.
     * @param db Database of full card list
     * @return True if deck was successfully created, false otherwise.
     */
    bool createDeck(DBCreator& db);
    
    /**
     * Resets the deck to an empty deck.
     */
    void reset();
    
    /**
     * Resets the deck to the originally loaded deck.
     */
    void reopen();
    
    /**
     * returns size of unique Cards array.
     * @return size of unique cards in deck
     */
    uint32_t getSize();

    /**
     * returns decrements the number of particular cards in deck
     * @param id NameId of Card
     * @param decAmount amount to decrement by
     * @return true of successful, false otherwise
     */
    bool decrementCount(const std::string& id, uint32_t decAmount);
    
    /**
     * increments the number of particular cards in deck
     * @param id NameId of Card
     * @param decAmount amount to decrement by
     * @return true of successful, false otherwise
     */
    bool incrementCount(const std::string& id, uint32_t decAmount);

    /**
     * returns decrements the number of particular cards in deck
     * @param id NameId of Card
     * @param decAmount amount to decrement by
     * @return true of successful, false otherwise
     */
    bool decrementCount(uint32_t pos, uint32_t decAmount);

    /**
     * returns card at a certain position of deck
     * @param pos position in dec
     * @return pointer to card at the specified position in deck, null otherwise
     */
    Card* getCard(uint32_t pos);

    /**
     * returns number of cards at position of deck
     * @param pos position in dec
     * @return how many of the particular card is in the deck at the specified position
     */
    uint32_t getCount(uint32_t pos);

    /**
     * Returns a card by id (e.g. CS1_042)
     */
    Card* getCardById(const std::string& id);
    
    /**
     * Returns a card by name (e.g. Goldshire Footman)
     */
    Card* getCardByName(const std::string& name);

    /**
     * Adds a card to the player's deck
     */
    bool addCardToDeck(Card* card, uint32_t count);

private:

    typedef struct pair
    {
        Card* card;
        uint32_t count;
    } Slot;

    std::fstream mFile;
    std::string mFileName;
    std::vector<Slot*> mDeck;
    const uint32_t kMaxSlots = 30;
};

} /* namespace HS */

#endif /* HS_DECK_H_ */
