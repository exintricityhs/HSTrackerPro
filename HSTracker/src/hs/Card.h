/*
 * Card.h
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

#ifndef HS_CARD_H_
#define HS_CARD_H_

#include <string>

namespace HS
{

class Card
{
public:
    static const int32_t CardIdInvalid = -1;

    // card types
    typedef enum type
    {
        CardType_Minion = 0,
        CardType_Spell = 1,
        CardType_Hero = 2,
        CardType_HeroPower = 3,
        CardType_Enchantment = 4,
        CardType_Weapon = 5,
        CardType_Invalid = 6
    } CardType;

    typedef enum rarity
    {
        CardRarity_Free = 0,
        CardRarity_Common = 1,
        CardRarity_Rare = 2,
        CardRarity_Epic = 3,
        CardRarity_Legendary = 4,
        CardRarity_Invalid = 5
    } CardRarity;

    Card();
    virtual ~Card();

    /**
     * Sets the name of the card (e.g. Goldshire Footman)
     */
    void setName(const std::string& name);
    const std::string& getName();
    
    /**
     * Sets the id  of the card (e.g. CS1_042)
     * This is the unique id per card in the DB, not the unique id per card in hand
     */
    void setCardId(const std::string& cardId);
    const std::string& getCardId();

    /**
     * Gets/sets the card type
     */
    void setCardType(std::string type);
    void setCardType(CardType type);
    CardType getCardType();

    /**
     * Gets/sets the attack value
     */
    void setAttack(uint32_t attack);
    uint32_t getAttack();

    /**
     * Gets/sets the health value
     */
    void setHealth(uint32_t health);
    uint32_t getHealth();

    /**
     * Gets/sets the cost
     */
    void setCost(uint32_t cost);
    uint32_t getCost();

    /**
     * Gets/sets the rarity
     */
    void setRarity(std::string rarity);
    void setRarity(CardRarity rarity);
    CardRarity getRarity();
    
    /**
     * Gets/sets collectible
     */
    void setCollectible(bool collectible);
    bool getCollectible();

    /**
     * Helper function that returns name in all lower case with '-' instead of spaces for texture lookup
     */
    std::string getNoSpaceName();

private:
    std::string mName;
    std::string mCardId;
    CardType    mCardType;
    uint32_t    mAttack;
    uint32_t    mHealth;
    uint32_t    mCost;
    CardRarity  mRarity;
    bool        mCollectible;

};

}   // namespace HS

#endif /* HS_CARD_H_ */
