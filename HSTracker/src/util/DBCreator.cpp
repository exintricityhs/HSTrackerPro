/*
 * DBCreator.cpp
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

#include "DBCreator.h"
#include "cjson/cJSON.h"

#include <iostream>
#include <fstream>

namespace HS
{

DBCreator::DBCreator()
    : mDbJson(NULL)
{
}

DBCreator::DBCreator(const std::string& file)
    : mDbJson(NULL)
{
    open(file);
}

DBCreator::~DBCreator()
{
    if (mDbJson != NULL)
    {
        cJSON_Delete(mDbJson);
    }

    // delete map
    for (std::map<std::string,Card*>::iterator it=mCardDB.begin(); it!=mCardDB.end(); ++it)
    {
        delete it->second;
        it->second = 0; // I don't think this is strictly necessary...
    }
    mCardDB.clear();
}

bool DBCreator::open(const std::string& filename)
{
    std::ifstream file(filename.c_str());

    if (!file.is_open())
    {
        std::cout << "Could not open file " << filename << std::endl;
        return false;
    }

    file.seekg(0, std::ios::end);
    int fileLength = (int) file.tellg() + 1;
    file.seekg(0, std::ios::beg);

    char* fileData = (char*) malloc(sizeof(char) * fileLength);

    if (fileData == NULL)
    {
        std::cout << "Could not malloc file data" << filename << std::endl;
        file.close();
        return false;
    }

    file.get(fileData, fileLength * sizeof(char), (char) 0);
    file.close();

    mDbJson = cJSON_Parse(fileData);

    free(fileData);
    fileData = NULL;

    if (mDbJson == NULL)
    {
        std::cout << "cJSON_Parse failed " << filename << std::endl;
        return false;
    }
    else
    {
        //std::cout << "successfully parsed json file" << std::endl;
    }

    return true;
}

void DBCreator::getDeck(std::string deckType)
{
    // just parse basic cards for now
    cJSON*    basic = cJSON_GetObjectItem(mDbJson, deckType.c_str());

    // basic json is an array (or should be)
    if (basic == NULL)
    {
        return;
    }

    int size = cJSON_GetArraySize(basic);
    //std::cout << "size of array is" << size << std::endl;
    for (int i = 0; i < size; i++)
    {
        cJSON* newEntry = cJSON_GetArrayItem(basic, i);
        if (newEntry)
        {
            // check id, if id doesn't exist, can't insert into map
            cJSON* id = cJSON_GetObjectItem(newEntry, "id");
            if (id == NULL)
            {
                continue;
            }

            Card* card = new Card();

            if (id && id->type == cJSON_String)
            {
                std::string value = std::string(id->valuestring);
                card->setCardId(value);
            }

            cJSON* name = cJSON_GetObjectItem(newEntry, "name");
            if (name && name->type == cJSON_String)
            {
                std::string value = std::string(name->valuestring);
                card->setName(value);
            }

            cJSON* type = cJSON_GetObjectItem(newEntry, "type");
            if (type && type->type == cJSON_String)
            {
                std::string value = std::string(type->valuestring);
                card->setCardType(value);
            }

            cJSON* cost = cJSON_GetObjectItem(newEntry, "cost");
            if (cost && cost->type == cJSON_Number)
            {
                card->setCost(cost->valueint);
            }

            cJSON* attack = cJSON_GetObjectItem(newEntry, "attack");
            if (attack && attack->type == cJSON_Number)
            {
                card->setAttack(attack->valueint);
            }

            cJSON* health = cJSON_GetObjectItem(newEntry, "health");
            if (health && health->type == cJSON_Number)
            {
                card->setHealth(health->valueint);
            }

            cJSON* rarity = cJSON_GetObjectItem(newEntry, "rarity");
            if (rarity && rarity->type == cJSON_String)
            {
                std::string value = std::string(rarity->valuestring);
                card->setRarity(value);
            }

            mCardDB[card->getCardId()] = card;

        }
    }
}

bool DBCreator::createDatabase()
{
    if (mDbJson == NULL)
    {
        std::cout << "no database json has been set" << std::endl;
        return false;
    }
    getDeck("Basic");
    getDeck("Expert");
    getDeck("Curse of Naxxramas");
    getDeck("Goblins vs Gnomes");

    return true;
}


Card* DBCreator::findCardByName(const std::string& name)
{
    for (std::map<std::string,Card*>::iterator it=mCardDB.begin(); it!=mCardDB.end(); ++it)
    {
        if (it->second != NULL)
        {
            Card* card = it->second;
            if (name.compare(card->getName()) == 0)
            {
                //std::cout << "found card!!" << std::endl;
                return card;
            }
        }
    }
    return NULL;
}

Card* DBCreator::findCardById(const std::string& id)
{
    std::map<std::string, Card*>::iterator it;
    it = mCardDB.find(id);
    
    Card* ret = nullptr;
    
    if (it != mCardDB.end())
    {
        ret = it->second;
    }
    
    return ret;
}

std::vector<Card*> DBCreator::findCardWithIdSubstring(const std::string& substr, bool searchId)
{
    std::vector<Card*> ret;
    
    for (std::map<std::string,Card*>::iterator it=mCardDB.begin(); it!=mCardDB.end(); ++it)
    {
        if (it->second != NULL)
        {
            Card* card = it->second;
            
            std::string searchStr = searchId ? card->getCardId() : card->getName();
            if (searchStr.find(substr) != std::string::npos)
            {
                ret.push_back(card);
            }
        }
    }
    
    return ret;
}
    
// debug function
void DBCreator::printDatabase()
{
    for (std::map<std::string,Card*>::iterator it=mCardDB.begin(); it!=mCardDB.end(); ++it)
    {
        if (it != mCardDB.end())
        {
            std::cout << "card with id: " << it->first;
            Card* card = it->second;
            if (card)
            {
                std::cout << " name: " << card->getName();
                std::cout << " type: " << card->getCardType() << std::endl;
            }
        }
    }
}

} /* namespace HS */
