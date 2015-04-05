/*
 * DeckTrackView.mm
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

#import "DeckTrackView.h"
#import "Options.h"
#import "OpponentView.h"
#import "OpponentWindow.h"
#import "Slot.h"

#include <iostream>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

#include "hs/Deck.h"
#include "hs/LogParser.h"
#include "util/DBCreator.h"
//#include "util/ImageRecognition.h"
#include "util/Logger.h"

// log.config goes in ~/Library/Preferences/Blizzard/Hearthstone/log.config

#define ENABLE_TEST (0)

#if ENABLE_TEST
const std::string directory = std::string(getenv("HOME")) + std::string("/Documents/decks/");
const std::string fileName = directory + "/testFile.txt";
#else
const std::string directory = std::string(getenv("HOME")) + std::string("/Library/Logs/Unity/");
const std::string fileName = directory + "/Player.log";
#endif

static HS::LogParser logParser;
static HS::DBCreator db;
static HS::Deck playerDeck;
//HS::ImageRecognition imageRec;

const int MaxSlots = 30;
const float UpdateFps = 30;
const int yOffset = 26;

// User defaults
NSString* SelfWindowName = @"HSSELFWindowPos";
NSString* OppWindowName = @"HSOPPWindowPos";
NSString* DeckPath = @"HSDeckPath";

OpponentView* opponentView;

// private functions
@interface DeckTrackView()

- (void)setupCollections;

- (void)setSlotName:(int) slot name:(NSString*)name;
- (void)setSlotCost:(int) slot cost:(int)cost;
- (void)setSlotImage:(int) slot name:(NSString*)name;
- (void)setSlotCount:(int) slot count:(int)count;

- (void)enableSlot:(int) slot enable:(bool)enable;
- (void)dimSlot:(int) slot dim:(bool)dim;
- (void)animateSlot:(int) slot;
- (void)updateAnimation;
- (void)swapAnimateImage:(int) slot isDim:(bool)isDim;

- (void)resetDeck;
- (void)updateDeck;
- (void)updateOpponentDeck:(HS::Deck*)deck;
- (void)checkMouseOver;

- (void)windowMoved:(NSNotification *)notification;

- (void)unitTest;
- (void)unitTest2;

@end


@implementation DeckTrackView
{
    NSMutableArray* slots;
    Options* options;
}

NSTimer* updateTimer;
NSDate *lastUpdateTime;
float elapsedTime = 0.0;
float checkCursorElapsedTime = 0.0;
Options* optionsWindow;
OpponentWindow* opponentWindow;
int updateCount;
bool currentlyAnimating;
int cardCount[MaxSlots];

- (void)awakeFromNib
{
    // create options helper
    options = [[Options alloc] init];
    
    // setup UI
    [self setupCollections];
    
    // create opponent window
    opponentWindow = [[OpponentWindow alloc] initWithWindowNibName:@"OpponentWindow"];
    [opponentWindow showWindow:self];
    
    [self lockWindows:nil];
    
    // set update loop
    dispatch_async(dispatch_get_main_queue(), ^{
        updateTimer = [NSTimer scheduledTimerWithTimeInterval:1.0/UpdateFps target:self selector:@selector(update) userInfo:nil repeats:YES];
    });
    
    // save last position of windows
    [[self window] setFrameUsingName:SelfWindowName];
    [[opponentView window] setFrameUsingName:OppWindowName];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(windowMoved:) name:NSWindowDidMoveNotification object:nil];
    
    // open card DB
    NSString* dbPath = [[NSBundle mainBundle] pathForResource:@"cardsDB.enUS" ofType:@"json"];
    if (dbPath && db.open([dbPath UTF8String]))
    {
        db.createDatabase();
        //db.printDatabase();
    }
    
#if ENABLE_TEST
    logParser.open(fileName, false);
#else
    logParser.open(fileName);
#endif
    
    logParser.setDB(&db);
        
    for (int i=0; i<MaxSlots; ++i)
    {
        Slot* s = slots[i];
        
        [self dimSlot:i dim:false];
        [self enableSlot:i enable:false];
        s.animationFrame = -1;
        s.dimmed = false;
        s.animating = false;
    }
}

- (void)setupCollections
{
    slots = [[NSMutableArray alloc] initWithCapacity:MaxSlots];
    
    // setup template
    Slot* templateSlot = [[Slot alloc] init];
    templateSlot.slotImage = _slot1Image;
    templateSlot.slotBackground = _slot1Background;
    templateSlot.slotCount = _slot1Count;
    templateSlot.slotCountBackground = _slot1CountBackground;
    templateSlot.slotLabel = _slot1Label;
    templateSlot.slotCost = _slot1Cost;
    templateSlot.slotDim = _slot1Dim;
        
    slots[0] = templateSlot;
    
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
        
        // add dim
        slot.slotDim = [[NSImageView alloc] initWithFrame:templateSlot.slotDim.frame];
        [slot.slotDim setImage:[NSImage imageNamed:@"frameBlack.png"]];
        [self addSubview:slot.slotDim];
        [slot.slotDim setBounds:templateSlot.slotDim.bounds];
        [slot.slotDim setFrameOrigin:NSMakePoint(templateSlot.slotDim.frame.origin.x, templateSlot.slotDim.frame.origin.y - (yOffset*i))];
        slot.slotDim.imageAlignment = templateSlot.slotDim.imageAlignment;
        [slot.slotDim setImageScaling:templateSlot.slotDim.imageScaling];
        [slot.slotDim display];
        
        slots[i] = slot;
    }

    // position hover tip in front of all views
    //[self addSubview:_mouseTip];
}

- (void)drawRect:(NSRect)dirtyRect
{
    [super drawRect:dirtyRect];
}

-(void)update
{
    // get time elapsed since last update
    NSTimeInterval timeElapsed = [lastUpdateTime timeIntervalSinceNow];
    elapsedTime += -timeElapsed;
    checkCursorElapsedTime += -timeElapsed;
    lastUpdateTime = [NSDate date];
        
    bool drawUpdate = false;
    
    if (elapsedTime >= 1.0)
    {
        elapsedTime -= 1.0;
        
        logParser.processLog();
        
        if (logParser.isGameReset())
        {
            playerDeck.reopen();
            playerDeck.createDeck((db));
            [self resetDeck];
        }
        
        std::vector<HS::PlayedCard> playedCards = logParser.getPlayedCards();
        
        if (playedCards.size())
        {
            drawUpdate = true;
            
            for (std::vector<HS::PlayedCard>::iterator it=playedCards.begin(); it!=playedCards.end(); ++it)
            {
                for (int i=0; i<playerDeck.getSize(); ++i)
                {
                    if (!playerDeck.getCard(i)->getCardId().compare(it->getCardId()))
                    {
                        playerDeck.decrementCount(i, 1);
                        break;
                    }
                }
            }
            
            [self updateDeck];
        }
        
        // update opponent's played cards
        HS::Deck* opponentDeck = logParser.getOpponentDeck();
        
        if (opponentDeck)
        {
            [self updateOpponentDeck:opponentDeck];
        }
        
        //[self unitTest];
        //[self unitTest2];
    }
    
    [self updateAnimation];
    //[opponentView updateAnimation];
    
    if (checkCursorElapsedTime >= 0.25)
    {
        //[self checkMouseOver];
    }
    
    if (currentlyAnimating || drawUpdate)
    {
        [self setNeedsDisplay:true];
    }
}

- (IBAction)loadDeck:(id)sender
{
    NSOpenPanel* openPanel = [NSOpenPanel openPanel];
    
    // setDirectoryURL
    NSString* savedDir = [options getLastDeckPath];
    
    if (savedDir)
    {
        NSURL* url = [NSURL URLWithString:savedDir];
        [openPanel setDirectoryURL:url];
    }
    
    openPanel.title = @"Choose a deck file";
    openPanel.showsResizeIndicator = YES;
    openPanel.showsHiddenFiles = NO;
    openPanel.canChooseDirectories = NO;
    openPanel.canCreateDirectories = NO;
    openPanel.allowsMultipleSelection = NO;
    openPanel.allowedFileTypes = @[@"txt"];
    
    
    [openPanel beginSheetModalForWindow:nil completionHandler:^(NSInteger result)
    {
        if (result==NSOKButton)
        {
            NSURL* selection = openPanel.URLs[0];
            NSString* path = [selection.path stringByResolvingSymlinksInPath];
            
            playerDeck.open([path UTF8String]);
            playerDeck.createDeck((db));
            
            // update deck
            [self resetDeck];
            
            // save directory for net time
            [options setLastDeckPath:[path stringByDeletingLastPathComponent]];
         }
    }];
}

- (IBAction)lockWindows:(id)sender
{
    bool isLocked = [options getLocked];
    
    // if initializing, set it as saved
    if (!sender)
    {
        isLocked = !isLocked;
    }
    
    if (isLocked)
    {
        [[self window] setStyleMask:[self window].styleMask | NSTitledWindowMask];
        [[self window] setTitle:@"SELF"];
        [[opponentView window] setStyleMask:[self window].styleMask | NSTitledWindowMask];
        [_lockWindowItem setTitle:@"Lock Windows"];
        [[opponentView window] setTitle:@"OPPONENT"];
    }
    else
    {
        [[self window] setStyleMask:[self window].styleMask & ~NSTitledWindowMask];
        [[opponentView window] setStyleMask:[self window].styleMask & ~NSTitledWindowMask];
        [_lockWindowItem setTitle:@"Unlock Windows"];
    }
    
    [options setLocked:!isLocked];
}

- (IBAction)resetWindows:(id)sender
{
    NSPoint point;
    point.x = 20;
    point.y = 150;
    
    [[self window] setFrameOrigin:point];
    
    point.x += 200;
    [[opponentView window] setFrameOrigin:point];
}

- (void)setSlotName:(int) slot name:(NSString*)name
{
    if (slot >= MaxSlots)
    {
        return;
    }
    
    [((Slot*)slots[slot]).slotLabel setStringValue:name];
}

- (void)setSlotCost:(int) slot cost:(int)cost
{
    if (slot >= MaxSlots)
    {
        return;
    }
    
    [((Slot*)slots[slot]).slotCost setStringValue:[NSString stringWithFormat:@"%d", cost]];
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
        NSImageView* imageView = ((Slot*)slots[slot]).slotImage;
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
    
    NSImageView* imageView = ((Slot*)slots[slot]).slotCount;
    NSImageView* countBackgroud = ((Slot*)slots[slot]).slotCountBackground;
    
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
    
    Slot* s = slots[slot];
    
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

- (void)dimSlot:(int) slot dim:(bool)dim
{
    if (slot >= MaxSlots)
    {
        return;
    }
    
    Slot* s = slots[slot];
    s.dimmed = dim;

    if (!dim)
    {
        s.slotDim.alphaValue = 0.0;
        [s.slotDim setHidden:true];
    }
    else
    {
        [s.slotDim setHidden:false];
    }
}

- (void)animateSlot:(int) slot
{
    if (slot >= MaxSlots)
    {
        return;
    }
    
    Slot* s = slots[slot];
    
    if (s.animationFrame < 0)
    {
        [s.slotDim setHidden:false];
        [self swapAnimateImage:slot isDim:false];
        s.animationFrame = 0;
        currentlyAnimating = true;
    }
}

- (void)updateAnimation
{
    bool isAnimating = false;
    
    for (int i=0; i<MaxSlots; ++i)
    {
        Slot* s = slots[i];
        
        if (s.animationFrame != -1)
        {
            s.animationFrame++;
            isAnimating = true;
            
            if (s.animationFrame <= 15)
            {
                s.slotDim.alphaValue = (float) s.animationFrame / 15.0;
            }
            else if (s.animationFrame <= 30)
            {
                s.slotDim.alphaValue = 1.0 - (((float) s.animationFrame - 15.0) / 15.0);
            }
            else
            {
                s.animationFrame = -1;
                
                if (s.dimmed)
                {
                    [self swapAnimateImage:i isDim:true];
                    s.slotDim.alphaValue = 0.7;
                }
            }
        }
    }
    
    currentlyAnimating = isAnimating;
}

- (void)swapAnimateImage:(int) slot isDim:(bool) isDim
{
    if (slot >= MaxSlots)
    {
        return;
    }
    
    Slot* s = slots[slot];
    
    NSImage* newImage = nil;
    NSImageView* imageView = s.slotDim;
    
    if (isDim)
    {
        newImage = [NSImage imageNamed:@"frameBlack.png"];
    }
    else
    {
        newImage = [NSImage imageNamed:@"frameHighlight"];
    }
    
    [newImage setSize:imageView.image.size];
    [imageView setImage:newImage];
    
}

- (void)resetDeck
{
    // update deck
    int numUniqueCards = playerDeck.getSize();
    
    for (int i=0; i<MaxSlots; ++i)
    {
        if (i < numUniqueCards)
        {
            [self enableSlot:i enable:true];
            
            // fill out info
            HS::Card* card = playerDeck.getCard(i);
            
            [self setSlotName:i name:[NSString stringWithUTF8String:card->getName().c_str()]];
            [self setSlotCost:i cost:card->getCost()];
            [self setSlotImage:i name:[NSString stringWithUTF8String:card->getNoSpaceName().c_str()]];
            
            int count = playerDeck.getCount(i);
            
            if (count >= 2)
            {
                [self setSlotCount:i count:count];
            }
            else if (card->getRarity() == HS::Card::CardRarity_Legendary)
            {
                [self setSlotCount:i count:-1];
            }
            else
            {
                [self setSlotCount:i count:count];
            }
            
            [self dimSlot:i dim:false];
            
            cardCount[i] = count;
        }
        else
        {
            [self enableSlot:i enable:false];
        }
        
        [opponentView enableSlot:i enable:false];
    }
}

- (void)updateDeck
{
    // update deck
    int numUniqueCards = playerDeck.getSize();
    
    for (int i=0; i<MaxSlots; ++i)
    {
        if (i < numUniqueCards)
        {
            [self enableSlot:i enable:true];
            
            // fill out info
            HS::Card* card = playerDeck.getCard(i);
            
            [self setSlotCost:i cost:card->getCost()];
            
            int count = playerDeck.getCount(i);
            
            if (count >= 2)
            {
                [self setSlotCount:i count:count];
            }
            else if (card->getRarity() == HS::Card::CardRarity_Legendary)
            {
                [self setSlotCount:i count:-1];
            }
            else
            {
                [self setSlotCount:i count:count];
            }
            
            if (playerDeck.getCount(i) <= 0)
            {
                [self dimSlot:i dim:true];
            }
            else
            {
                [self dimSlot:i dim:false];
            }
            
            if (cardCount[i] != playerDeck.getCount(i))
            {
                [self animateSlot:i];
            }
            
            cardCount[i] = count;
        }
        else
        {
            [self enableSlot:i enable:false];
        }
    }
}

- (void)updateOpponentDeck:(HS::Deck*)deck
{
    // update deck
    int numUniqueCards = deck->getSize();
    
    for (int i=0; i<MaxSlots; ++i)
    {
        if (i < numUniqueCards)
        {
            [opponentView enableSlot:i enable:true];
            
            // fill out info
            HS::Card* card = deck->getCard(i);
            
            [opponentView setSlotCost:i cost:card->getCost()];
            
            int count = deck->getCount(i);
            
            if (count >= 2)
            {
                [opponentView setSlotCount:i count:count];
            }
            else if (card->getRarity() == HS::Card::CardRarity_Legendary)
            {
                [opponentView setSlotCount:i count:-1];
            }
            else
            {
                [opponentView setSlotCount:i count:count];
            }
            
            [opponentView setSlotName:i name:[NSString stringWithUTF8String:card->getName().c_str()]];
            [opponentView setSlotImage:i name:[NSString stringWithUTF8String:card->getNoSpaceName().c_str()]];
        }
        else
        {
            [opponentView enableSlot:i enable:false];
        }
    }
    
    [opponentView setNeedsDisplay:true];
}

- (void)checkMouseOver
{
    NSPoint event_location = [[self window] mouseLocationOutsideOfEventStream];
    NSPoint local_point = [self convertPoint:event_location fromView:nil];
    
    Slot* s = slots[0];
    
    // check if it's within the SELF column
    if (local_point.x >= s.slotDim.frame.origin.x && local_point.x <= s.slotDim.frame.origin.x + s.slotDim.frame.size.width)
    {
        for (int i=0; i<playerDeck.getSize(); ++i)
        {
            Slot* s = slots[i];
            
            if ([self mouse:local_point inRect:s.slotDim.frame])
            {
                //LOG_INFO("mouse in slot: " << i << std::endl);
                [_mouseTip setFrameOrigin:NSMakePoint(_mouseTip.frame.origin.x, s.slotDim.frame.origin.y - yOffset)];
                
                int32_t chance = (float) playerDeck.getCount(i) / (float) playerDeck.getRemaining() * 100.0;
                [_mouseTip setStringValue:[NSString stringWithFormat:@"%d\%", chance]];
                break;
            }
        }
    }
}

- (void)windowMoved:(NSNotification *)notification
{
    if (notification.object == [self window])
    {
        [[self window] saveFrameUsingName:SelfWindowName];
    }
    else if (notification.object == [opponentView window])
    {
        [[opponentView window] saveFrameUsingName:OppWindowName];
    }
}

- (void)unitTest
{
    static int count = -1;
    static int testType = -1;
    
    if (count == -1)
    {
        // init
        for (int i=0; i<MaxSlots; ++i)
        {
            [self dimSlot:i dim:false];
            [self enableSlot:i enable:true];
        }

        count++;
    }
    
    switch (testType)
    {
        case -1:
        {
            [self setSlotName:count name:@"Ysera"];
            [self setSlotImage:count name:@"ysera.png"];
            [self setSlotCount:count count:testType];
            [self setSlotCost:count cost:10];
            testType++;
        } break;
            
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        {
            [self setSlotCount:count count:testType];
            testType++;
        } break;
        
        case 10:
        {
            [self dimSlot:count dim:true];
            [self animateSlot:count];
            testType++;
        } break;
            
        case 11:
        {
            testType++;
        } break;
    
        case 12:
        {
            [self enableSlot:count enable:false];
            testType++;
        } break;
            
        case 13:
        {
            [self enableSlot:count enable:true];
            testType = -1;
            count++;
        } break;
            
        default:
            break;
    }
    
    if (count >= 30)
    {
        count = 0;
    }
}

- (void)unitTest2
{
    static int count2 = -1;
    static int testType2 = -1;
    
    if (count2 == -1)
    {
        // init
        for (int i=0; i<MaxSlots; ++i)
        {
            [opponentView enableSlot:i enable:true];
        }
        
        count2++;
    }
    
    switch (testType2)
    {
        case -1:
        {
            [opponentView setSlotName:count2 name:@"Ysera"];
            [opponentView setSlotImage:count2 name:@"ysera.png"];
            [opponentView setSlotCount:count2 count:testType2];
            [opponentView setSlotCost:count2 cost:10];
            testType2++;
        } break;
            
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        {
            [opponentView setSlotCount:count2 count:testType2];
            testType2++;
        } break;
            
        case 10:
        {
            [opponentView animateSlot:count2];
            testType2++;
        } break;
            
        case 11:
        {
            testType2++;
        } break;
            
        case 12:
        {
            //[opponentView enableSlot:count2 enable:false];
            testType2++;
        } break;
            
        case 13:
        {
            //[opponentView enableSlot:count2 enable:true];
            testType2 = -1;
            count2++;
        } break;
            
        default:
            break;
    }
    
    if (count2 >= 30)
    {
        count2 = 0;
    }
}

@end
