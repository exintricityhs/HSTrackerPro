/*
 * DeckTrackView.h
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

#import <Cocoa/Cocoa.h>

@interface DeckTrackView : NSView
{
}

// menu items
- (IBAction)loadDeck:(id)sender;
- (IBAction)lockWindows:(id)sender;
- (IBAction)resetWindows:(id)sender;

@property IBOutlet NSImageView* slot1Image;             // card image template
@property IBOutlet NSImageView* slot1Background;        // card background
@property IBOutlet NSImageView* slot1Count;             // card count images (0-9, legendary)
@property IBOutlet NSImageView* slot1CountBackground;   // card count background images
@property IBOutlet NSTextField* slot1Label;             // card name
@property IBOutlet NSTextField* slot1Cost;              // card cost
@property IBOutlet NSImageView* slot1Dim;               // dim

@property IBOutlet NSTextField* mouseTip;               // info about the card slot

@property IBOutlet NSMenuItem* lockWindowItem;

@end
