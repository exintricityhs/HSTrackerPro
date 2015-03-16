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

#include <iostream>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

#include "hs/Deck.h"
#include "hs/LogParser.h"
#include "util/DBCreator.h"
#include "util/Logger.h"

// log.config goes in ~/Library/Preferences/Blizzard/Hearthstone/log.config

#define ENABLE_TEST (0)

#if ENABLE_TEST
const std::string directory = std::string(getenv("HOME")) + std::string("/workspace/HSParser/decks");
//const std::string fileName = directory + "/testFile.txt";
#else
const std::string directory = std::string(getenv("HOME")) + std::string("/Library/Logs/Unity/");
const std::string fileName = directory + "/Player.log";
#endif

static HS::LogParser logParser;
static HS::DBCreator db;
static HS::Deck playerDeck;

const int MaxSlots = 30;
const float UpdateFps = 30;

// User defaults
NSString* SelfWindowName = @"HSSELFWindowPos";
NSString* OppWindowName = @"HSOPPWindowPos";
NSString* DeckPath = @"HSDeckPath";

OpponentView* opponentView;

// private functions
@interface DeckTrackView()

- (NSImageView*)getCardImage:(int) slot;
- (NSImageView*)getCardCountImage:(int) slot;
- (NSTextField*)getCardNameLabel:(int) slot;
- (NSTextField*)getCardCostLabel:(int) slot;
- (NSImageView*)getDimImage:(int) slot;

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

- (void)windowMoved:(NSNotification *)notification;

- (void)unitTest;
- (void)unitTest2;

@end


@implementation DeckTrackView

NSTimer* updateTimer;
NSDate *lastUpdateTime;
float elapsedTime = 0.0;
Options* optionsWindow;
OpponentWindow* opponentWindow;
int windowAlpha;
int updateCount;
int slotAnimating[MaxSlots];
bool slotDimmed[MaxSlots];
bool currentlyAnimating;
int cardCount[MaxSlots];

- (void)awakeFromNib
{
    opponentWindow = [[OpponentWindow alloc] initWithWindowNibName:@"OpponentWindow"];
    [opponentWindow showWindow:self];
    
    dispatch_async(dispatch_get_main_queue(), ^{
        updateTimer = [NSTimer scheduledTimerWithTimeInterval:1.0/UpdateFps target:self selector:@selector(update) userInfo:nil repeats:YES];
    });
    
    windowAlpha = 100;
    
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
        [self dimSlot:i dim:false];
        [self enableSlot:i enable:false];
        slotAnimating[i] = -1;
        slotDimmed[i] = false;
    }
}

- (void)drawRect:(NSRect)dirtyRect
{
    [self updateAnimation];
}

-(void)update
{
    // get time elapsed since last update
    NSTimeInterval timeElapsed = [lastUpdateTime timeIntervalSinceNow];
    elapsedTime += -timeElapsed;
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
    
    if (currentlyAnimating || drawUpdate)
    {
        [self setNeedsDisplay:true];
    }
}

- (IBAction)loadDeck:(id)sender
{
    NSOpenPanel* openPanel = [NSOpenPanel openPanel];
    
    // setDirectoryURL
    NSString* savedDir = [[NSUserDefaults standardUserDefaults] stringForKey:DeckPath];
    
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
            NSString *directory = [path stringByDeletingLastPathComponent];
            [[NSUserDefaults standardUserDefaults] setObject:directory forKey:DeckPath];
         }
    }];
}

- (IBAction)changeAlpha:(id)sender
{
    return; // TODO
    
    /*optionsWindow = [[Options alloc] initWithWindowNibName:@"Options"];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(optionsClosed:) name:NSWindowWillCloseNotification object:nil];
    optionsWindow->sliderValue = windowAlpha;
    [optionsWindow showWindow:self];*/
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

- (void) optionsClosed:(NSNotification*)notification
{
    windowAlpha = optionsWindow->sliderValue;
    [[self window] setAlphaValue:windowAlpha / 100.0];
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

- (NSImageView*)getDimImage:(int) slot
{
    switch(slot)
    {
        case 0:
            return _slot1Dim;
            
        case 1:
            return _slot2Dim;
            
        case 2:
            return _slot3Dim;
            
        case 3:
            return _slot4Dim;
            
        case 4:
            return _slot5Dim;
            
        case 5:
            return _slot6Dim;
            
        case 6:
            return _slot7Dim;
            
        case 7:
            return _slot8Dim;
            
        case 8:
            return _slot9Dim;
            
        case 9:
            return _slot10Dim;
            
        case 10:
            return _slot11Dim;
            
        case 11:
            return _slot12Dim;
            
        case 12:
            return _slot13Dim;
            
        case 13:
            return _slot14Dim;
            
        case 14:
            return _slot15Dim;
            
        case 15:
            return _slot16Dim;
            
        case 16:
            return _slot17Dim;
            
        case 17:
            return _slot18Dim;
            
        case 18:
            return _slot19Dim;
            
        case 19:
            return _slot20Dim;
            
        case 20:
            return _slot21Dim;
            
        case 21:
            return _slot22Dim;
            
        case 22:
            return _slot23Dim;
            
        case 23:
            return _slot24Dim;
            
        case 24:
            return _slot25Dim;
            
        case 25:
            return _slot26Dim;
            
        case 26:
            return _slot27Dim;
            
        case 27:
            return _slot28Dim;
            
        case 28:
            return _slot29Dim;
            
        case 29:
            return _slot30Dim;
            
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

- (void)dimSlot:(int) slot dim:(bool)dim
{
    if (dim)
    {
        slotDimmed[slot] = true;
    }
    else
    {
        [self getDimImage:(int) slot].alphaValue = 0.0;
        slotDimmed[slot] = false;
    }
}

- (void)animateSlot:(int) slot
{
    if (slotAnimating[slot] == -1)
    {
        [self swapAnimateImage:slot isDim:false];
        slotAnimating[slot] = 0;
        currentlyAnimating = true;
    }
}

- (void)updateAnimation
{
    bool isAnimating = false;
    
    for (int i=0; i<MaxSlots; ++i)
    {
        if (slotAnimating[i] != -1)
        {
            slotAnimating[i]++;
            
            if (slotAnimating[i] <= 15)
            {
                [self getDimImage:i].alphaValue = (float) slotAnimating[i] / 15.0;
                isAnimating = true;
            }
            else if (slotAnimating[i] <= 30)
            {
                [self getDimImage:i].alphaValue = 1.0 - (((float) slotAnimating[i] - 15.0) / 15.0);
                isAnimating = true;
            }
            else
            {
                slotAnimating[i] = -1;
                
                if (slotDimmed[i])
                {
                    [self swapAnimateImage:i isDim:true];
                    [self getDimImage:(int) i].alphaValue = 0.7;
                }
            }
        }
    }
    
    currentlyAnimating = isAnimating;
}

- (void)swapAnimateImage:(int) slot isDim:(bool) isDim
{
    NSImage* newImage = nil;
    NSImageView* imageView = [self getDimImage:slot];
    
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
            
            if (card->getRarity() == HS::Card::CardRarity_Legendary)
            {
                [self setSlotCount:i count:-1];
            }
            else
            {
                [self setSlotCount:i count:playerDeck.getCount(i)];
            }
            
            [self dimSlot:i dim:false];
            
            cardCount[i] = playerDeck.getCount(i);
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
            
            if (card->getRarity() == HS::Card::CardRarity_Legendary)
            {
                [self setSlotCount:i count:-1];
            }
            else
            {
                [self setSlotCount:i count:playerDeck.getCount(i)];
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
            
            cardCount[i] = playerDeck.getCount(i);
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
            
            if (card->getRarity() == HS::Card::CardRarity_Legendary)
            {
                [opponentView setSlotCount:i count:-1];
            }
            else
            {
                [opponentView setSlotCount:i count:deck->getCount(i)];
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

static int count = 29;
static int testType = 0;

- (void)unitTest
{
    switch (testType)
    {
        case 0:
        {
            [self setSlotName:count name:@"Voidwalker"];
            //[self setSlotCost:count cost:5];
            //[self setSlotImage:count name:@"voidwalker.png"];
            //[self setSlotCount:count count:1];
            //[self dimSlot:count dim:true];
            
            testType++;
        } break;
            
        case 1:
        {
            //[self setSlotName:count name:@"Ysera"];
            [self setSlotCost:count cost:10];
            //[self setSlotImage:count name:@"ysera.png"];
            //[self setSlotCount:count count:-1];
            //[self dimSlot:count dim:false];
            
            testType++;
        } break;
            
        case 2:
        {
            [self enableSlot:count enable:false];
            testType++;
        } break;
            
        case 3:
        {
            [self enableSlot:count enable:true];
            testType = 0;
            count++;
        } break;
            
        default:
            break;
    }
    
    count++;
    
    /*for (int i=0; i<MaxSlots; ++i)
    {
        [self enableSlot:i enable:true];
        [self dimSlot:i dim:true];
    }*/
    
    if (count >= 30)
    {
        count = 0;
    }
    
    //[self animateSlot:count];
}

- (void)unitTest2
{
    switch (testType)
    {
        case 0:
        {
            [opponentView setSlotName:count name:@"Voidwalker"];
            [opponentView setSlotImage:count name:@"voidwalker.png"];
            testType++;
        } break;
            
        case 1:
        {
            [opponentView setSlotCost:count cost:10];
            testType++;
        } break;
            
        case 2:
        {
            [opponentView setSlotCount:count count:-1];
            testType++;
        } break;
            
        case 3:
        {
            [opponentView enableSlot:count enable:false];
            testType++;
        } break;
            
        case 4:
        {
            [opponentView enableSlot:count enable:true];
            testType = 0;
            count--;
        } break;
            
        default:
            break;
    }
    
    if (count <= 0)
    {
        count = 29;
    }
}

@end
