/*
 * LogParser.cpp
 *
 *  Created on: Jan 30, 2015
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

#include "../hs/LogParser.h"
#include <iostream>
#include <string.h>

namespace HS
{

using namespace std;

LogParser::LogParser()
    : mFile()
    , mSeekPos(0)
    , mCurrentLine()
    , mDB()
    , mGame()
    , mGameReset(false)
    , mMulliganDone(false)
    , mLinkedEntities()
{
}

LogParser::~LogParser()
{
    if (mFile.is_open())
    {
        mFile.close();
    }
}

bool LogParser::open(const std::string& file, bool seekToEnd)
{
    if (mFile.is_open())
    {
        mFile.close();
    }

    mFile.open(file, fstream::in);

    if (!mFile.is_open())
    {
        cout << "Could not open file " << file << endl;
        return false;
    }

    if (seekToEnd)
    {
        mFile.seekg(0, ios::end);
        mSeekPos = mFile.tellg();
    }

    return true;
}
    
void LogParser::setDB(DBCreator* db)
{
    mDB = db;
    mGame.setDB(db);
}

bool LogParser::processLog()
{
    if (!mFile.is_open())
    {
        return false;
    }
    
    mFile.clear();
    mFile.seekg(0, ios::end);
    uint64_t currEOFPos = mFile.tellg();
    mFile.seekg(mSeekPos);
    mSeekPos = currEOFPos;

    while (readLine())
    {
        processPowerGame();
        processZoneGame();
    }
    
    return true;
}
    
std::vector<HS::PlayedCard> LogParser::getPlayedCards()
{
    std::vector<HS::PlayedCard> playedCards;
    
    if (mGame.getSelf())
    {
        Player* player = mGame.getSelf();
        playedCards = player->getNewCardsInHand();
    }

    return playedCards;
}
    
Deck* LogParser::getOpponentDeck()
{
    if (mGame.getOpponent())
    {
        return mGame.getOpponent()->getDeckUpdate();
    }
    
    return nullptr;
}
    
bool LogParser::isGameReset()
{
    if (mGameReset)
    {
        mGameReset = false;
        return true;
    }
    
    return false;
}

bool LogParser::readLine()
{
    // clear previous EOF bit
    mFile.clear();

    getline(mFile, mCurrentLine);

    if (mFile.eof())
    {
        // at end of the file
        return false;
    }

//    cout << "Read line (" << mSeekPos << "): " << mCurrentLine << endl;

    return true;
}

void LogParser::processZoneGame()
{
    if (mCurrentLine.find("[Zone]") == string::npos)
    {
        return;
    }
        
    PlayData playData;

    // figure out self and opponent hero and player id
    if (mCurrentLine.find("TRANSITIONING") != string::npos && mCurrentLine.find("(Hero)") != string::npos)
    {
        size_t pos = mCurrentLine.find("TRANSITIONING");
        pos = mCurrentLine.find_first_of('[', pos);
        
        string data = mCurrentLine.substr(pos);
        findBracketKeyValues(data, &playData);
        
        if (data.find("FRIENDLY PLAY") != string::npos)
        {
            mGame.setSelf(playData["player"]);
            mGame.getSelf()->setHeroId(playData["cardId"]);
            //cout << "====== set SELF to player "<< playData["player"] << " with hero id: " <<playData["cardId"] << endl;
        }
        else
        {
            mGame.setOpponent(playData["player"]);
            mGame.getOpponent()->setHeroId(playData["cardId"]);
            //cout << "====== set OPPONENT to player "<< playData["player"] << " with hero id: " <<playData["cardId"] << endl;
        }
    }
    
    if (!mMulliganDone || !mGame.getSelf() || (mGame.getSelf()->getPlayerType() == Player::Invalid) ||
        !mGame.getOpponent() || (mGame.getOpponent()->getPlayerType() == Player::Invalid))
    {
        return;
    }
    
    if (mCurrentLine.find("TRANSITIONING card") != string::npos)   // process deck to hand for SELF
    {
        size_t pos = mCurrentLine.find("TRANSITIONING card");
        
        // search for OPPONENT plays
        if (mCurrentLine.find("to OPPOSING") != string::npos || mCurrentLine.find("to FRIENDLY HAND"))
        {
            pos = mCurrentLine.find_first_of('[', pos);

            string data = mCurrentLine.substr(pos);
            findBracketKeyValues(data, &playData);
            
            if (playData["cardId"].length())
            {
                if (!removeLinkedId(playData["id"]))
                {
                    mGame.playCard(playData["player"], playData["id"], playData["cardId"]);
                }
            }
        }
    }
    else if (mCurrentLine.find("type=TAG_CHANGE entity=") != string::npos && mCurrentLine.find("value=PLAY") != string::npos && mCurrentLine.find("dstZoneTag=PLAY") != string::npos)
    {
        size_t pos = mCurrentLine.rfind("entity=");
        pos = mCurrentLine.find("[", pos);
        
        if (pos == string::npos)
        {
            return;
        }
        
        string data = mCurrentLine.substr(pos);
        findBracketKeyValues(data, &playData);
        
        if (playData["cardId"].length())
        {
            mGame.playCard(playData["player"], playData["id"], playData["cardId"]);
        }
    }
    else if (mCurrentLine.find("type=TAG_CHANGE entity=") != string::npos && mCurrentLine.find("tag=LINKEDCARD value=") != string::npos)
    {
        size_t pos = mCurrentLine.find("entity=");
        
        string searchStr = mCurrentLine.substr(pos + 7);
        findBracketKeyValues(searchStr, &playData, false);
        
        // find what the entity is being linked to
        pos = mCurrentLine.find("tag=LINKEDCARD");
        size_t endPos = mCurrentLine.find("]", pos);
        searchStr = mCurrentLine.substr(pos, endPos - pos);
        PlayData linkData;
        findKeyValues(searchStr, &linkData);
        
        LinkedEntity linkedEntity;
        linkedEntity.fromCardId = playData["cardId"];
        linkedEntity.fromId = playData["id"];
        linkedEntity.toId = linkData["value"];
        addLinkedEntity(linkedEntity);
    }
}

void LogParser::processPowerGame()
{
    if (mCurrentLine.find("[Power]") == string::npos)
    {
        return;
    }

    // search for start of a new game
    if (mCurrentLine.find("CREATE_GAME") != string::npos)
    {
        mGame.reset();
        mGameReset = true;
        mMulliganDone = false;
        mLinkedEntities.clear();
        //cout << "====== Created new game" << endl;
        return;
    }
    else if (mCurrentLine.find("tag=MULLIGAN_STATE value=DONE") != string::npos)
    {
        mMulliganDone = true;
        return;
    }
    
    size_t pos = mCurrentLine.find_first_of('-');
    
    if (pos == string::npos)
    {
        return;
    }
    
    string data = mCurrentLine.substr(mCurrentLine.find_first_not_of(' ', pos + 1));
    
    if (mCurrentLine.find("SHOW_ENTITY") != string::npos)
    {
        if (mCurrentLine.find("[") != string::npos)
        {
            PlayData entryData;
            PlayData valueData;
            size_t leftBracketPos = data.find_last_of("[");
            size_t rightBracketPos = data.find("]", leftBracketPos);
            string subEntry = data.substr(leftBracketPos + 1, rightBracketPos - leftBracketPos - 1);
            findKeyValues(subEntry, &entryData);
            string cardId = data.substr(data.find("CardID=")+7);
                        
            if (cardId.length() && entryData["player"].length())
            {
                // filter out list of exceptions
                if (mGame.isIgnored(cardId))
                {
                    return;
                }
                
                mGame.moveDeckToHand(entryData["player"], entryData["id"], cardId);
            }
        }
    }
    else if (mCurrentLine.find("m_chosenEntities") != string::npos && mCurrentLine.find("zone=HAND") != string::npos)  // parse mulligans
    {
        PlayData entryData;
        findBracketKeyValues(data, &entryData);
        mGame.moveDeckToHand(entryData["player"], entryData["id"], entryData["cardId"]);
    }
}

void LogParser::findBracketKeyValues(string data, std::map<std::string, std::string>* playData, bool reverse)
{
    size_t leftBracketPos = (reverse ? data.find_last_of("[") : data.find("["));
    
    if (leftBracketPos == string::npos)
    {
        return;
    }
    
    size_t rightBracketPos = data.find("]", leftBracketPos);
    
    if (rightBracketPos == string::npos)
    {
        return;
    }
    
    string subEntry = data.substr(leftBracketPos + 1, rightBracketPos - leftBracketPos - 1);
    findKeyValues(subEntry, playData);
}


void LogParser::findKeyValues(string data, std::map<std::string, std::string>* playData)
{
    char* dataBuf = new char [data.length() + 1];
    strcpy(dataBuf, data.c_str());

    string key;
    string value;
    string tokenStr;

    char* token = strtok(dataBuf, "=");
    
    //std::cout << "Key/value pairs:" << std::endl;

    // TODO : fix nested key/value pairs
    while (token)
    {
        tokenStr = token;

        if (!key.length())
        {
            key = tokenStr;
        }
        else
        {
            // find last space
            size_t spacePos = tokenStr.rfind(" ");

            if (spacePos != string::npos)
            {
                value = tokenStr.substr(0, spacePos);
                playData->insert(pair<string, string>(key, value));
                //std::cout << "    key:" << key << ", value:" << value << std::endl;
                key = tokenStr.substr(spacePos + 1, string::npos);
            }
            else
            {
                value = tokenStr;
                playData->insert(pair<string, string>(key, value));
                //std::cout << "    key:" << key << ", value:" << value << std::endl;
            }
        }

        token = strtok(NULL, "=");
        }

        delete [] dataBuf;
}
                                  
void LogParser::addLinkedEntity(LinkedEntity& entity)
{
    for (LinkedEntities::iterator it=mLinkedEntities.begin(); it!=mLinkedEntities.end(); ++it)
    {
        if (!entity.toId.compare(it->toId))
        {
            return;
        }
    }
    
    mLinkedEntities.push_back(entity);
}
        
bool LogParser::removeLinkedId(const std::string& id)
{
    for (LinkedEntities::iterator it=mLinkedEntities.begin(); it!=mLinkedEntities.end(); ++it)
    {
        if (!it->toId.compare(id))
        {
            //mLinkedEntities.erase(it);
            return true;
        }
    }
    
    return false;
}

}      // namespace HS
