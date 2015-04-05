/*
 * DBCreator.h
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

#ifndef UTIL_DBCREATOR_H_
#define UTIL_DBCREATOR_H_

#include <string>
#include <map>
#include <vector>

#include "cjson/cJson.h"
#include "../hs/Card.h"

namespace HS
{

class DBCreator
{
public:
    DBCreator();
    DBCreator(const std::string& file);
    ~DBCreator();

    /**
     * Opens a card db file.
     * @param file Full path of the file.
     * @return True if the file was opened, false otherwise.
     */
    bool open(const std::string& file);

    /**
     * Parses db json and creates an hash map of cards
     * @param cjson that contains db of cards
     * @return True if parsing succeeds, false otherwise
     */
    bool parseDbJson(cJSON);

    /**
     * creates an hash map of cards from db json
     * @return True if database is successfully created, false otherwise
     */
    bool createDatabase();

    Card* findCardByName(const std::string& name);

    Card* findCardById(const std::string& id);
    
    /**
     * Helper function for debugging
     * Searches for cards with matching the substr in their id or name
     * @param searchId If true, searches the id for the substr, otherwise searches the name
     */
    std::vector<Card*> findCardWithIdSubstring(const std::string& substr, bool searchId);

    void printDatabase();
    
    /**
     * Helper function for new card releases
     */
    void convertJson(const std::string& filename);


private:
    void getDeck(std::string deckType);

    cJSON* mDbJson;
    std::map<std::string, Card*> mCardDB;
};

} /* namespace HS */

#endif /* UTIL_DBCREATOR_H_ */
