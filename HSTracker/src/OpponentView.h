/*
 * OpponentView.h
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

@interface OpponentView : NSView

- (void)setSlotName:(int) slot name:(NSString*)name;
- (void)setSlotCost:(int) slot cost:(int)cost;
- (void)setSlotImage:(int) slot name:(NSString*)name;
- (void)setSlotCount:(int) slot count:(int)count;
- (void)enableSlot:(int) slot enable:(bool)enable;

// TODO: Switch this to an IBOutletContainer or NSArray

// card images
@property IBOutlet NSImageView* slot1View;
@property IBOutlet NSImageView* slot2View;
@property IBOutlet NSImageView* slot3View;
@property IBOutlet NSImageView* slot4View;
@property IBOutlet NSImageView* slot5View;
@property IBOutlet NSImageView* slot6View;
@property IBOutlet NSImageView* slot7View;
@property IBOutlet NSImageView* slot8View;
@property IBOutlet NSImageView* slot9View;
@property IBOutlet NSImageView* slot10View;
@property IBOutlet NSImageView* slot11View;
@property IBOutlet NSImageView* slot12View;
@property IBOutlet NSImageView* slot13View;
@property IBOutlet NSImageView* slot14View;
@property IBOutlet NSImageView* slot15View;
@property IBOutlet NSImageView* slot16View;
@property IBOutlet NSImageView* slot17View;
@property IBOutlet NSImageView* slot18View;
@property IBOutlet NSImageView* slot19View;
@property IBOutlet NSImageView* slot20View;
@property IBOutlet NSImageView* slot21View;
@property IBOutlet NSImageView* slot22View;
@property IBOutlet NSImageView* slot23View;
@property IBOutlet NSImageView* slot24View;
@property IBOutlet NSImageView* slot25View;
@property IBOutlet NSImageView* slot26View;
@property IBOutlet NSImageView* slot27View;
@property IBOutlet NSImageView* slot28View;
@property IBOutlet NSImageView* slot29View;
@property IBOutlet NSImageView* slot30View;

// card overlay images (single, double, legendary)
@property IBOutlet NSImageView* slot1Count;
@property IBOutlet NSImageView* slot2Count;
@property IBOutlet NSImageView* slot3Count;
@property IBOutlet NSImageView* slot4Count;
@property IBOutlet NSImageView* slot5Count;
@property IBOutlet NSImageView* slot6Count;
@property IBOutlet NSImageView* slot7Count;
@property IBOutlet NSImageView* slot8Count;
@property IBOutlet NSImageView* slot9Count;
@property IBOutlet NSImageView* slot10Count;
@property IBOutlet NSImageView* slot11Count;
@property IBOutlet NSImageView* slot12Count;
@property IBOutlet NSImageView* slot13Count;
@property IBOutlet NSImageView* slot14Count;
@property IBOutlet NSImageView* slot15Count;
@property IBOutlet NSImageView* slot16Count;
@property IBOutlet NSImageView* slot17Count;
@property IBOutlet NSImageView* slot18Count;
@property IBOutlet NSImageView* slot19Count;
@property IBOutlet NSImageView* slot20Count;
@property IBOutlet NSImageView* slot21Count;
@property IBOutlet NSImageView* slot22Count;
@property IBOutlet NSImageView* slot23Count;
@property IBOutlet NSImageView* slot24Count;
@property IBOutlet NSImageView* slot25Count;
@property IBOutlet NSImageView* slot26Count;
@property IBOutlet NSImageView* slot27Count;
@property IBOutlet NSImageView* slot28Count;
@property IBOutlet NSImageView* slot29Count;
@property IBOutlet NSImageView* slot30Count;

// card name labels
@property IBOutlet NSTextField* slot1Label;
@property IBOutlet NSTextField* slot2Label;
@property IBOutlet NSTextField* slot3Label;
@property IBOutlet NSTextField* slot4Label;
@property IBOutlet NSTextField* slot5Label;
@property IBOutlet NSTextField* slot6Label;
@property IBOutlet NSTextField* slot7Label;
@property IBOutlet NSTextField* slot8Label;
@property IBOutlet NSTextField* slot9Label;
@property IBOutlet NSTextField* slot10Label;
@property IBOutlet NSTextField* slot11Label;
@property IBOutlet NSTextField* slot12Label;
@property IBOutlet NSTextField* slot13Label;
@property IBOutlet NSTextField* slot14Label;
@property IBOutlet NSTextField* slot15Label;
@property IBOutlet NSTextField* slot16Label;
@property IBOutlet NSTextField* slot17Label;
@property IBOutlet NSTextField* slot18Label;
@property IBOutlet NSTextField* slot19Label;
@property IBOutlet NSTextField* slot20Label;
@property IBOutlet NSTextField* slot21Label;
@property IBOutlet NSTextField* slot22Label;
@property IBOutlet NSTextField* slot23Label;
@property IBOutlet NSTextField* slot24Label;
@property IBOutlet NSTextField* slot25Label;
@property IBOutlet NSTextField* slot26Label;
@property IBOutlet NSTextField* slot27Label;
@property IBOutlet NSTextField* slot28Label;
@property IBOutlet NSTextField* slot29Label;
@property IBOutlet NSTextField* slot30Label;

// card cost labels
@property IBOutlet NSTextField* slot1Cost;
@property IBOutlet NSTextField* slot2Cost;
@property IBOutlet NSTextField* slot3Cost;
@property IBOutlet NSTextField* slot4Cost;
@property IBOutlet NSTextField* slot5Cost;
@property IBOutlet NSTextField* slot6Cost;
@property IBOutlet NSTextField* slot7Cost;
@property IBOutlet NSTextField* slot8Cost;
@property IBOutlet NSTextField* slot9Cost;
@property IBOutlet NSTextField* slot10Cost;
@property IBOutlet NSTextField* slot11Cost;
@property IBOutlet NSTextField* slot12Cost;
@property IBOutlet NSTextField* slot13Cost;
@property IBOutlet NSTextField* slot14Cost;
@property IBOutlet NSTextField* slot15Cost;
@property IBOutlet NSTextField* slot16Cost;
@property IBOutlet NSTextField* slot17Cost;
@property IBOutlet NSTextField* slot18Cost;
@property IBOutlet NSTextField* slot19Cost;
@property IBOutlet NSTextField* slot20Cost;
@property IBOutlet NSTextField* slot21Cost;
@property IBOutlet NSTextField* slot22Cost;
@property IBOutlet NSTextField* slot23Cost;
@property IBOutlet NSTextField* slot24Cost;
@property IBOutlet NSTextField* slot25Cost;
@property IBOutlet NSTextField* slot26Cost;
@property IBOutlet NSTextField* slot27Cost;
@property IBOutlet NSTextField* slot28Cost;
@property IBOutlet NSTextField* slot29Cost;
@property IBOutlet NSTextField* slot30Cost;

@end
