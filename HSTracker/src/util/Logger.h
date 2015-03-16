/*
 * Logger.h
 *
 *  Created on: Mar 14, 2015
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

#ifndef Logger_h
#define Logger_h

#include <iostream>
#include <sstream>

namespace HS
{
    
enum LogLevels
{
    Level_Trace = 0,
    Level_Debug,
    Level_Info,
    Level_Warn,
    Level_Fatal,
};
    
#define DEFAULT_LOG_LEVEL   (HS::Level_Warn)
    
#if DEBUG
    
#define LOG_TRACE(logline)   if (DEFAULT_LOG_LEVEL <= HS::Level_Trace) { std::stringstream ss; ss << logline; std::cout << ss.str(); }
#define LOG_DEBUG(logline)   if (DEFAULT_LOG_LEVEL <= HS::Level_Debug) { std::stringstream ss; ss << logline; std::cout << ss.str(); }
#define LOG_INFO(logline)    if (DEFAULT_LOG_LEVEL <= HS::Level_Info)  { std::stringstream ss; ss << logline; std::cout << ss.str(); }
#define LOG_WARN(logline)    if (DEFAULT_LOG_LEVEL <= HS::Level_Warn)  { std::stringstream ss; ss << logline; std::cout << ss.str(); }
#define LOG_FATAL(logline)   if (DEFAULT_LOG_LEVEL <= HS::Level_Fatal) { std::stringstream ss; ss << logline; std::cout << ss.str(); }
    
#else
    
#define LOG_TRACE(logline)
#define LOG_DEBUG(logline)
#define LOG_INFO(logline)
#define LOG_WARN(logline)
#define LOG_FATAL(logline)
    
#endif
}

#endif
