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
#import "Slot.h"

// private functions
@interface OpponentView()

- (void)setupCollections;

@end

@implementation OpponentView

NSMutableArray* opponentSlots;

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
    
    [self setupCollections];
    
    for (int i=0; i<MaxSlots; ++i)
    {
        Slot* s = opponentSlots[i];
        
        [self enableSlot:i enable:false];
        s.animationFrame = -1;
        s.dimmed = false;
        s.animating = false;
    }
}

- (void)drawRect:(NSRect)dirtyRect
{
    [super drawRect:dirtyRect];
}

- (void)setupCollections
{
    opponentSlots = [[NSMutableArray alloc] initWithCapacity:MaxSlots];
    
    // setup template
    Slot* templateSlot = [[Slot alloc] init];
    templateSlot.slotImage = _slot1Image;
    templateSlot.slotBackground = _slot1Background;
    templateSlot.slotCount = _slot1Count;
    templateSlot.slotCountBackground = _slot1CountBackground;
    templateSlot.slotLabel = _slot1Label;
    templateSlot.slotCost = _slot1Cost;
    
    opponentSlots[0] = templateSlot;
    
    const int yOffset = 26;
    
    for (int i=1; i<MaxSlots; ++i)
    {
        Slot* slot = [[Slot alloc] init];
        
        // add card image
        slot.slotImage = [[NSImageView alloc] initWithFrame:templateSlot.slotImage.frame];
        [slot.slotImage setImage:[NSImage imageNamed:@"abomination.png"]];
        [self addSubview:slot.slotImage];
        [slot.slotImage setBounds:templateSlot.slotImage.bounds];
        [slot.slotImage setFrameOrigin:NSMakePoint(templateSlot.slotImage.frame.origin.x, templateSlot.slotImage.frame.origin.y - (yOffset*i))];
        slot.slotImage.imageAlignment = templateSlot.slotImage.imageAlignment;
        [slot.slotImage setImageScaling:templateSlot.slotImage.imageScaling];
        [slot.slotImage display];
        
        // add slot background
        slot.slotBackground = [[NSImageView alloc] initWithFrame:templateSlot.slotBackground.frame];
        [slot.slotBackground setImage:[NSImage imageNamed:@"cardBack.png"]];
        [self addSubview:slot.slotBackground];
        [slot.slotBackground setBounds:templateSlot.slotBackground.bounds];
        [slot.slotBackground setFrameOrigin:NSMakePoint(templateSlot.slotBackground.frame.origin.x, templateSlot.slotBackground.frame.origin.y - (yOffset*i))];
        slot.slotBackground.imageAlignment = templateSlot.slotBackground.imageAlignment;
        [slot.slotBackground setImageScaling:templateSlot.slotBackground.imageScaling];
        [slot.slotBackground display];
        
        // add count background
        slot.slotCountBackground = [[NSImageView alloc] initWithFrame:templateSlot.slotCountBackground.frame];
        [slot.slotCountBackground setImage:[NSImage imageNamed:@"frame_countbox.png"]];
        [self addSubview:slot.slotCountBackground];
        [slot.slotCountBackground setBounds:templateSlot.slotCountBackground.bounds];
        [slot.slotCountBackground setFrameOrigin:NSMakePoint(templateSlot.slotCountBackground.frame.origin.x, templateSlot.slotCountBackground.frame.origin.y - (yOffset*i))];
        slot.slotCountBackground.imageAlignment = templateSlot.slotCountBackground.imageAlignment;
        [slot.slotCountBackground setImageScaling:templateSlot.slotCountBackground.imageScaling];
        [slot.slotCountBackground display];
        
        // add count
        slot.slotCount = [[NSImageView alloc] initWithFrame:templateSlot.slotCount.frame];
        [slot.slotCount setImage:[NSImage imageNamed:@"frame_2.png"]];
        [self addSubview:slot.slotCount];
        [slot.slotCount setBounds:templateSlot.slotCount.bounds];
        [slot.slotCount setFrameOrigin:NSMakePoint(templateSlot.slotCount.frame.origin.x, templateSlot.slotCount.frame.origin.y - (yOffset*i))];
        slot.slotCount.imageAlignment = templateSlot.slotCount.imageAlignment;
        [slot.slotCount setImageScaling:templateSlot.slotCount.imageScaling];
        [slot.slotCount display];
        
        // add card name label
        slot.slotLabel = [[NSTextField alloc] initWithFrame:templateSlot.slotLabel.frame];
        [self addSubview:slot.slotLabel];
        [slot.slotLabel setBounds:templateSlot.slotLabel.bounds];
        [slot.slotLabel setFrameOrigin:NSMakePoint(templateSlot.slotLabel.frame.origin.x, templateSlot.slotLabel.frame.origin.y - (yOffset*i))];
        [slot.slotLabel setAlignment:templateSlot.slotLabel.alignment];
        [slot.slotLabel setStringValue:[NSString stringWithFormat:@"slot_%d", i]];
        [slot.slotLabel setEditable:templateSlot.slotLabel.editable];
        [slot.slotLabel setTextColor:templateSlot.slotLabel.textColor];
        [slot.slotLabel setBordered:templateSlot.slotLabel.bordered];
        [slot.slotLabel setBackgroundColor:templateSlot.slotLabel.backgroundColor];
        [slot.slotLabel setDrawsBackground:templateSlot.slotLabel.drawsBackground];
        [slot.slotLabel setFont:templateSlot.slotLabel.font];
        [slot.slotLabel display];
        
        // add cost label
        slot.slotCost = [[NSTextField alloc] initWithFrame:templateSlot.slotCost.frame];
        [self addSubview:slot.slotCost];
        [slot.slotCost setBounds:templateSlot.slotCost.bounds];
        [slot.slotCost setFrameOrigin:NSMakePoint(templateSlot.slotCost.frame.origin.x, templateSlot.slotCost.frame.origin.y - (yOffset*i))];
        [slot.slotCost setAlignment:templateSlot.slotCost.alignment];
        [slot.slotCost setStringValue:[NSString stringWithFormat:@"1"]];
        [slot.slotCost setEditable:templateSlot.slotCost.editable];
        [slot.slotCost setTextColor:templateSlot.slotCost.textColor];
        [slot.slotCost setBordered:templateSlot.slotCost.bordered];
        [slot.slotCost setBackgroundColor:templateSlot.slotCost.backgroundColor];
        [slot.slotCost setDrawsBackground:templateSlot.slotCost.drawsBackground];
        [slot.slotCost setFont:templateSlot.slotCost.font];
        [slot.slotCost display];
        
        opponentSlots[i] = slot;
    }
}

- (void)setSlotName:(int) slot name:(NSString*)name
{
    if (slot >= MaxSlots)
    {
        return;
    }
    
    [((Slot*)opponentSlots[slot]).slotLabel setStringValue:name];
}

- (void)setSlotCost:(int) slot cost:(int)cost
{
    if (slot >= MaxSlots)
    {
        return;
    }
    
    [((Slot*)opponentSlots[slot]).slotCost setStringValue:[NSString stringWithFormat:@"%d", cost]];
}

- (void)setSlotImage:(int) slot name:(NSString*)name
{
    if (slot >= MaxSlots)
    {
        return;
    }
    
    NSImage* newImage = [NSImage imageNamed:name];
    
    if (newImage)
    {
        NSImageView* imageView = ((Slot*)opponentSlots[slot]).slotImage;
        [newImage setSize:imageView.image.size];
        [imageView setImage:newImage];
    }
}

- (void)setSlotCount:(int) slot count:(int)count
{
    if (slot >= MaxSlots)
    {
        return;
    }
    
    NSImage* newImage = nil;
    
    if (count == -1)
    {
        // special case for legendary
        newImage = [NSImage imageNamed:@"frame_legendary"];
    }
    else if (count >= 2 && count <= 8)
    {
        NSString* imageName = [NSString stringWithFormat:@"frame_%d", count];
        newImage = [NSImage imageNamed:imageName];
    }
    else if (count >= 9)
    {
        newImage = [NSImage imageNamed:@"frame_9.png"];
    }
    
    NSImageView* imageView = ((Slot*)opponentSlots[slot]).slotCount;
    NSImageView* countBackgroud = ((Slot*)opponentSlots[slot]).slotCountBackground;
    
    if (newImage)
    {
        [newImage setSize:imageView.image.size];
        [imageView setImage:newImage];
        [imageView setHidden:false];
        [countBackgroud setHidden:false];
    }
    else
    {
        // hide both count background and count
        [imageView setHidden:true];
        [countBackgroud setHidden:true];
    }
}

- (void)enableSlot:(int) slot enable:(bool)enable
{
    if (slot >= MaxSlots)
    {
        return;
    }
    
    Slot* s = opponentSlots[slot];
    
    [s.slotImage setHidden:!enable];
    [s.slotBackground setHidden:!enable];
    [s.slotCount setHidden:!enable];
    [s.slotCountBackground setHidden:!enable];
    [s.slotLabel setHidden:!enable];
    [s.slotCost setHidden:!enable];
    [s.slotDim setHidden:!enable];
    s.animationFrame = -1;
    s.animating = false;
    s.dimmed = false;
}

- (void)animateSlot:(int) slot
{
    if (slot >= MaxSlots)
    {
        return;
    }
    
    Slot* s = opponentSlots[slot];
    
    if (s.animationFrame < 0)
    {
        s.animationFrame = 0;
    }
}

- (void)updateAnimation
{
    bool isAnimating = false;
    
    for (int i=0; i<MaxSlots; ++i)
    {
        Slot* s = opponentSlots[i];
        
        if (s.animationFrame != -1)
        {
            s.animationFrame++;
            isAnimating = true;
            
            if (s.animationFrame <= 30)
            {
                float alphaValue = (float) s.animationFrame / 30.0;
                
                s.slotImage.alphaValue = alphaValue;
                s.slotCount.alphaValue = alphaValue;
                s.slotCountBackground.alphaValue = alphaValue;
                s.slotLabel.alphaValue = alphaValue;
                s.slotCost.alphaValue = alphaValue;
                isAnimating = true;
            }
            else
            {
                s.animationFrame = -1;
            }
        }
    }
    
    if (isAnimating)
    {
        [self setNeedsDisplay:true];
    }
}

@end
