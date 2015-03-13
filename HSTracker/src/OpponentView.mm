/*
 * OpponentView.mm
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

#import "OpponentView.h"

// private functions
@interface OpponentView()

- (NSImageView*)getCardImage:(int) slot;
- (NSImageView*)getCardCountImage:(int) slot;
- (NSTextField*)getCardNameLabel:(int) slot;
- (NSTextField*)getCardCostLabel:(int) slot;

@end

@implementation OpponentView

const int MaxSlots = 30;

- (void)awakeFromNib
{
    [[self window] setLevel:NSFloatingWindowLevel];
    [self.window setHasShadow:NO];
    [[self window] setOpaque:NO];
    
    NSColor *clearColor = [NSColor colorWithDeviceRed:0.0 green:0.0 blue:0.0 alpha:0.0];
    [[self window] setBackgroundColor:clearColor];
    
    // TODO: fix
    extern OpponentView* opponentView;
    opponentView = self;
    
    for (int i=0; i<MaxSlots; ++i)
    {
        [self enableSlot:i enable:false];
    }
}

- (void)drawRect:(NSRect)dirtyRect
{
    [super drawRect:dirtyRect];
}

- (NSImageView*)getCardImage:(int) slot
{
    switch(slot)
    {
        case 0:
            return _slot1View;
            
        case 1:
            return _slot2View;
            
        case 2:
            return _slot3View;
            
        case 3:
            return _slot4View;
            
        case 4:
            return _slot5View;
            
        case 5:
            return _slot6View;
            
        case 6:
            return _slot7View;
            
        case 7:
            return _slot8View;
            
        case 8:
            return _slot9View;
            
        case 9:
            return _slot10View;
            
        case 10:
            return _slot11View;
            
        case 11:
            return _slot12View;
            
        case 12:
            return _slot13View;
            
        case 13:
            return _slot14View;
            
        case 14:
            return _slot15View;
            
        case 15:
            return _slot16View;
            
        case 16:
            return _slot17View;
            
        case 17:
            return _slot18View;
            
        case 18:
            return _slot19View;
            
        case 19:
            return _slot20View;
            
        case 20:
            return _slot21View;
            
        case 21:
            return _slot22View;
            
        case 22:
            return _slot23View;
            
        case 23:
            return _slot24View;
            
        case 24:
            return _slot25View;
            
        case 25:
            return _slot26View;
            
        case 26:
            return _slot27View;
            
        case 27:
            return _slot28View;
            
        case 28:
            return _slot29View;
            
        case 29:
            return _slot30View;
            
        default:
            return nil;
    }
}

- (NSImageView*)getCardCountImage:(int) slot
{
    switch(slot)
    {
        case 0:
            return _slot1Count;
            
        case 1:
            return _slot2Count;
            
        case 2:
            return _slot3Count;
            
        case 3:
            return _slot4Count;
            
        case 4:
            return _slot5Count;
            
        case 5:
            return _slot6Count;
            
        case 6:
            return _slot7Count;
            
        case 7:
            return _slot8Count;
            
        case 8:
            return _slot9Count;
            
        case 9:
            return _slot10Count;
            
        case 10:
            return _slot11Count;
            
        case 11:
            return _slot12Count;
            
        case 12:
            return _slot13Count;
            
        case 13:
            return _slot14Count;
            
        case 14:
            return _slot15Count;
            
        case 15:
            return _slot16Count;
            
        case 16:
            return _slot17Count;
            
        case 17:
            return _slot18Count;
            
        case 18:
            return _slot19Count;
            
        case 19:
            return _slot20Count;
            
        case 20:
            return _slot21Count;
            
        case 21:
            return _slot22Count;
            
        case 22:
            return _slot23Count;
            
        case 23:
            return _slot24Count;
            
        case 24:
            return _slot25Count;
            
        case 25:
            return _slot26Count;
            
        case 26:
            return _slot27Count;
            
        case 27:
            return _slot28Count;
            
        case 28:
            return _slot29Count;
            
        case 29:
            return _slot30Count;
            
        default:
            return nil;
    }
}

- (NSTextField*)getCardNameLabel:(int) slot
{
    switch(slot)
    {
        case 0:
            return _slot1Label;
            
        case 1:
            return _slot2Label;
            
        case 2:
            return _slot3Label;
            
        case 3:
            return _slot4Label;
            
        case 4:
            return _slot5Label;
            
        case 5:
            return _slot6Label;
            
        case 6:
            return _slot7Label;
            
        case 7:
            return _slot8Label;
            
        case 8:
            return _slot9Label;
            
        case 9:
            return _slot10Label;
            
        case 10:
            return _slot11Label;
            
        case 11:
            return _slot12Label;
            
        case 12:
            return _slot13Label;
            
        case 13:
            return _slot14Label;
            
        case 14:
            return _slot15Label;
            
        case 15:
            return _slot16Label;
            
        case 16:
            return _slot17Label;
            
        case 17:
            return _slot18Label;
            
        case 18:
            return _slot19Label;
            
        case 19:
            return _slot20Label;
            
        case 20:
            return _slot21Label;
            
        case 21:
            return _slot22Label;
            
        case 22:
            return _slot23Label;
            
        case 23:
            return _slot24Label;
            
        case 24:
            return _slot25Label;
            
        case 25:
            return _slot26Label;
            
        case 26:
            return _slot27Label;
            
        case 27:
            return _slot28Label;
            
        case 28:
            return _slot29Label;
            
        case 29:
            return _slot30Label;
            
        default:
            return nil;
    }
}

- (NSTextField*)getCardCostLabel:(int) slot
{
    switch(slot)
    {
        case 0:
            return _slot1Cost;
            
        case 1:
            return _slot2Cost;
            
        case 2:
            return _slot3Cost;
            
        case 3:
            return _slot4Cost;
            
        case 4:
            return _slot5Cost;
            
        case 5:
            return _slot6Cost;
            
        case 6:
            return _slot7Cost;
            
        case 7:
            return _slot8Cost;
            
        case 8:
            return _slot9Cost;
            
        case 9:
            return _slot10Cost;
            
        case 10:
            return _slot11Cost;
            
        case 11:
            return _slot12Cost;
            
        case 12:
            return _slot13Cost;
            
        case 13:
            return _slot14Cost;
            
        case 14:
            return _slot15Cost;
            
        case 15:
            return _slot16Cost;
            
        case 16:
            return _slot17Cost;
            
        case 17:
            return _slot18Cost;
            
        case 18:
            return _slot19Cost;
            
        case 19:
            return _slot20Cost;
            
        case 20:
            return _slot21Cost;
            
        case 21:
            return _slot22Cost;
            
        case 22:
            return _slot23Cost;
            
        case 23:
            return _slot24Cost;
            
        case 24:
            return _slot25Cost;
            
        case 25:
            return _slot26Cost;
            
        case 26:
            return _slot27Cost;
            
        case 27:
            return _slot28Cost;
            
        case 28:
            return _slot29Cost;
            
        case 29:
            return _slot30Cost;
            
        default:
            return nil;
    }
}

- (void)setSlotName:(int) slot name:(NSString*)name
{
    [[self getCardNameLabel:slot] setStringValue:name];
}

- (void)setSlotCost:(int) slot cost:(int)cost
{
    NSTextField* label = [self getCardCostLabel:slot];
    
    switch (cost)
    {
        case 0:
            [label setStringValue:@"0"];
            break;
            
        case 1:
            [label setStringValue:@"1"];
            break;
            
        case 2:
            [label setStringValue:@"2"];
            break;
            
        case 3:
            [label setStringValue:@"3"];
            break;
            
        case 4:
            [label setStringValue:@"4"];
            break;
            
        case 5:
            [label setStringValue:@"5"];
            break;
            
        case 6:
            [label setStringValue:@"6"];
            break;
            
        case 7:
            [label setStringValue:@"7"];
            break;
            
        case 8:
            [label setStringValue:@"8"];
            break;
            
        case 9:
            [label setStringValue:@"9"];
            break;
            
        case 10:
            [label setStringValue:@"10"];
            break;
            
        default:
            break;
    }
}

- (void)setSlotImage:(int) slot name:(NSString*)name
{
    NSImage* newImage = [NSImage imageNamed:name];
    NSImageView* imageView = [self getCardImage:slot];
    
    if (newImage)
    {
        [newImage setSize:imageView.image.size];
        [imageView setImage:newImage];
    }
}

- (void)setSlotCount:(int) slot count:(int)count
{
    NSImage* newImage = nil;
    NSImageView* imageView = [self getCardCountImage:slot];
    
    if (count == -1)
    {
        // special case for legendary
        newImage = [NSImage imageNamed:@"cardBackL.png"];
    }
    else if (count >= 0 && count <= 1)
    {
        newImage = [NSImage imageNamed:@"cardBack.png"];
    }
    else
    {
        // TODO: handle arena case for 3+
        newImage = [NSImage imageNamed:@"cardBack2.png"];
    }
    
    [newImage setSize:imageView.image.size];
    [imageView setImage:newImage];
}

- (void)enableSlot:(int) slot enable:(bool)enable
{
    CGFloat alpha = 1.0;
    
    if (!enable)
    {
        alpha = 0.0;
    }
    
    [self getCardImage:slot].alphaValue = alpha;
    [self getCardCountImage:slot].alphaValue = alpha;
    [self getCardNameLabel:slot].alphaValue = alpha;
    [self getCardCostLabel:slot].alphaValue = alpha;
}

@end
