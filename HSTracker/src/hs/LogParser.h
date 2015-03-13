/*
 * LogParser.h
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

#ifndef LOGPARSER_H_
#define LOGPARSER_H_

#include <fstream>
#include <map>
#include <vector>

#include "PlayedCard.h"
#include "Game.h"
#include "../util/DBCreator.h"

namespace HS
{

class LogParser
{
public:
    LogParser();
    ~LogParser();

    /**
     * Opens a log file.
     * @param file Full path of the file.
     * @param seekToEnd Seeks to end of the file to ignore prior activity.
     * @return True if the file was opened, false otherwise.
     */
    bool open(const std::string& file, bool seekToEnd=true);
    
    /**
     * Inits the DB.
     */
    void setDB(DBCreator* db);

    /**
     * Processes the log file from the current position until the end of the file is reached.
     * @return True if successful, false if an error occurred.
     */
    bool processLog();
    
    /**
     * Returns a list of played cards processed in processLog.
     */
    std::vector<HS::PlayedCard> getPlayedCards();
    
    /**
     * Returns a list of the opponent's played cards.
     * @return Deck list if updates have been made, nullptr otherwise.
     */
    Deck* getOpponentDeck();
    
    /**
     * Returns whether the game was reset since the last call
     */
    bool isGameReset();

private:
    struct LinkedEntity
    {
        std::string fromCardId;
        std::string fromId;
        std::string toId;
    };
    
    typedef std::map<std::string, std::string> PlayData;
    typedef std::vector<LinkedEntity> LinkedEntities;

    /**
     * Reads the next line in the log file.
     * @return True if a line was read, false if the end of the file was reached.
     */
    bool readLine();

    void processPowerGame();
    void processZoneGame();
    void findBracketKeyValues(std::string data, PlayData* playData, bool reverse=true);
    void findKeyValues(std::string data, PlayData* playData);
    
    void addLinkedEntity(LinkedEntity& entity);
    bool removeLinkedId(const std::string& id);

    std::fstream    mFile;
    uint64_t        mSeekPos;
    std::string     mCurrentLine;
    DBCreator*      mDB;
    Game            mGame;
    bool            mGameReset;
    bool            mMulliganDone;
    LinkedEntities  mLinkedEntities;
};

}   // namespace HS

#endif /* LOGPARSER_H_ */
