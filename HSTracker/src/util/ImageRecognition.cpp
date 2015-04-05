/*
 * ImageRecognition.h
 *
 *  Created on: Mar 26, 2015
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
#if 0

#include "ImageRecognition.h"

#include <leptonica/allheaders.h>

#include "Logger.h"

namespace HS
{

ImageRecognition::ImageRecognition()
    : mAPI(nullptr)
{
}

ImageRecognition::~ImageRecognition()
{
    delete mAPI;
    mAPI = nullptr;
}
    
bool ImageRecognition::init()
{
    if (mAPI)
    {
        delete mAPI;
        mAPI = nullptr;
    }
    
    mAPI = new tesseract::TessBaseAPI();
    
    if (mAPI)
    {
        // TODO: fix up path
        char** pConfig = new char* [1];
        pConfig[0] = new char [3];
        strcpy(pConfig[0], "hs");
        pConfig[2] = 0;
        
        mAPI->SetPageSegMode(tesseract::PSM_SINGLE_COLUMN);
        
        if (mAPI->Init("/usr/local/share/tessdata/", "eng", tesseract::OEM_TESSERACT_CUBE_COMBINED, pConfig, 1, NULL, NULL, false))
        //if (mAPI->Init("/usr/local/share/tessdata/", "eng", tesseract::OEM_TESSERACT_CUBE_COMBINED))
        {
            return false;
        }
                
        return true;
    }
    
    return false;
}
    
void ImageRecognition::scan()
{
    if (!mAPI)
    {
        return;
    }
    
    Pix* image = pixRead("test.png");
    Pix* scaledImage = pixScale(image, 5.0, 5.0);
    Pix* outImage = new Pix;
    pixSauvolaBinarize(scaledImage, 29, 0.35, 0, nullptr, nullptr, nullptr, &outImage);
    
    if (image)
    {
        mAPI->SetImage(scaledImage);
        
        char* outText = mAPI->GetUTF8Text();
        LOG_INFO("got text: " << outText << std::endl);
        
        mAPI->End();
        delete [] outText;
        
        pixDestroy(&image);
    }
}
    
}

#endif