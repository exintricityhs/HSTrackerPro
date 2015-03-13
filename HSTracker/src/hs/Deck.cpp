/*
 * Deck.cpp
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

#include "Deck.h"

#include <iostream>
#include <string.h>

namespace HS {

using namespace std;

Deck::Deck()
    : mFile()
    , mFileName()
    , mDeck()
{
}

Deck::Deck(const std::string& filename)
    : mFile()
    , mFileName(filename)
    , mDeck()
{
    mDeck.clear();
    open(filename);
}
    
Deck::Deck(const Deck& rhs)
    : mFile()
    , mFileName(rhs.mFileName)
    , mDeck()
{
    size_t size = mDeck.size();
    for (int i = 0; i < size; i++)
    {
        addCardToDeck(mDeck[i]->card, mDeck[i]->count);
    }
}

Deck::~Deck()
{
    reset();
}

bool Deck::open(const std::string& filename)
{
    mFileName = filename;
    if (mFile.is_open())
    {
        mFile.close();
    }
    
    reset();

    mFile.open(filename, fstream::in);

    if (!mFile.is_open())
    {
        cout << "Could not open file " << filename << endl;
        return false;
    }

    return true;
}

bool Deck::createDeck(DBCreator& db)
{
    string currentLine;
    while(!mFile.eof())
    {
        size_t start = 0;
        size_t end = 0;
        getline(mFile, currentLine);
        size_t length = currentLine.length();
        if (length < 1)
        {
            // something went wrong
            continue;
        }

        // first grab number
        end = currentLine.find(" ");
        if (end != string::npos)
        {
            string count = currentLine.substr(start, (end - start));
            // then the rest of the string is the name of the card
            string name = currentLine.substr(end+1, length - end+1);
            uint32_t value = atoi(count.c_str());
            Card* card = db.findCardByName(name);
            if (card == NULL)
            {
                cout << "could not find card " << name << " in database" << endl;
                continue;
            }
            if (addCardToDeck(card, value) == false)
            {
                cout << "failed to add " << card->getCardId() << " to deck" << endl;
                continue;
            }
        }
    }
    
    return true;
}
    
void Deck::reset()
{
    if (mFile.is_open())
    {
        mFile.close();
    }
    
    //cleanup memory
    size_t size = mDeck.size();
    for (int i = 0; i < size; i++)
    {
        delete mDeck[i];
    }
    // delete map
    mDeck.clear();
}
    
void Deck::reopen()
{
    reset();
    open(mFileName);
    
}

uint32_t Deck::getSize()
{
    return mDeck.size();
}

bool Deck::decrementCount(const string& id, uint32_t decAmount)
{
    for(int i = 0; i < mDeck.size(); i++)
    {
        Card* card = mDeck[i]->card;
        if (id.compare(card->getCardId()) == 0)
        {
            mDeck[i]->count -= decAmount;
            return true;
        }
    }

    return false;
}
    
bool Deck::incrementCount(const std::string& id, uint32_t decAmount)
{
    for(int i = 0; i < mDeck.size(); i++)
    {
        Card* card = mDeck[i]->card;
        if (id.compare(card->getCardId()) == 0)
        {
            mDeck[i]->count += decAmount;
            return true;
        }
    }
    
    return false;
}

bool Deck::decrementCount(uint32_t pos, uint32_t decAmount)
{
    if (pos > mDeck.size())
    {
        return false;
    }

    if (mDeck[pos]->count > 0)
    {
        mDeck[pos]->count -= decAmount;
    }

    return true;
}

Card* Deck::getCard(uint32_t pos)
{
    if (pos > mDeck.size())
    {
        return NULL;
    }

    return mDeck[pos]->card;
}

uint32_t Deck::getCount(uint32_t pos)
{
    if (pos > mDeck.size())
    {
        return false;
    }
    return mDeck[pos]->count;
}

Card* Deck::getCardById(const string& id)
{
    for(int i = 0;i< mDeck.size(); i++)
    {
        if (id.compare(mDeck[i]->card->getCardId()) == 0)
        {
            return mDeck[i]->card;
        }
    }
    //cout << "could not find card " << id << endl;
    return NULL;
}

Card* Deck::getCardByName(const string& name)
{
    for(int i = 0;i< mDeck.size(); i++)
    {
        if (name.compare(mDeck[i]->card->getName()) == 0)
        {
            return mDeck[i]->card;
        }
    }
    cout << "could not find card " << name << endl;
    return NULL;
}

bool Deck::addCardToDeck(Card* card, uint32_t count)
{
    if (card == NULL)
    {
        return false;
    }

    if (mDeck.size() < 1)
    {
        Slot *newSlot = new Slot();
        newSlot->card = card;
        newSlot->count = count;
        mDeck.push_back(newSlot);
        return true;
    }

    // else iterate and find place;
    std::vector<Slot*>::iterator it;
    for(it = mDeck.begin(); it != mDeck.end(); ++it)
    {
        Card* myCard = (*it)->card;
        if (myCard == NULL)
        {
            cout << "card is null" << std::endl;
            return false;
        }
        if (card->getCost() < myCard->getCost())
        {
            Slot* newSlot = new Slot();
            newSlot->card = card;
            newSlot->count = count;
            mDeck.insert(it, newSlot);
            return true;
        }
        else if (card->getCost() == myCard->getCost())
        {
            if (card->getName().compare(myCard->getName()) < 0)
            {
                Slot* newSlot = new Slot();
                newSlot->card = card;
                newSlot->count = count;
                mDeck.insert(it, newSlot);
                return true;
            }
            else if (card->getName().compare(myCard->getName()) == 0)
            {
                (*it)->count += count;
                return true;
            }
            else
            {
                continue;
            }
        }
        else
        {
            continue;
        }
    }

    if (mDeck.size() >= kMaxSlots)
    {
        cout << "max deck size of " << kMaxSlots << " reached! cannot add any more cards" << std::endl;
        return false;
    }

    Slot* newSlot = new Slot();
    newSlot->card = card;
    newSlot->count = count;
    mDeck.push_back(newSlot);
    return true;
}

} /* namespace HS */
