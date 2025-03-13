/*!*********************************************************************************************************************
@file user_app1.c                                                                
@brief User's tasks / applications are written here.  This description
should be replaced by something specific to the task.

----------------------------------------------------------------------------------------------------------------------
To start a new task using this user_app1 as a template:
 1. Copy both user_app1.c and user_app1.h to the Application directory
 2. Rename the files yournewtaskname.c and yournewtaskname.h
 3. Add yournewtaskname.c and yournewtaskname.h to the Application Include and Source groups in the IAR project
 4. Use ctrl-h (make sure "Match Case" is checked) to find and replace all instances of "user_app1" with "yournewtaskname"
 5. Use ctrl-h to find and replace all instances of "UserApp1" with "YourNewTaskName"
 6. Use ctrl-h to find and replace all instances of "USER_APP1" with "YOUR_NEW_TASK_NAME"
 7. Add a call to YourNewTaskNameInitialize() in the init section of main
 8. Add a call to YourNewTaskNameRunActiveState() in the Super Loop section of main
 9. Update yournewtaskname.h per the instructions at the top of yournewtaskname.h
10. Delete this text (between the dashed lines) and update the Description below to describe your task
----------------------------------------------------------------------------------------------------------------------

------------------------------------------------------------------------------------------------------------------------
GLOBALS
- NONE

CONSTANTS
- NONE

TYPES
- NONE

PUBLIC FUNCTIONS
- NONE

PROTECTED FUNCTIONS
- void UserApp1Initialize(void)
- void UserApp1RunActiveState(void)


**********************************************************************************************************************/

#include "configuration.h"
#include "lcd_NHD-C12864LZ.h"

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_<type>UserApp1"
***********************************************************************************************************************/
/* New variables */
volatile u32 G_u32UserApp1Flags;                          /*!< @brief Global state flags */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern volatile u32 G_u32SystemTime1ms;                   /*!< @brief From main.c */
extern volatile u32 G_u32SystemTime1s;                    /*!< @brief From main.c */
extern volatile u32 G_u32SystemFlags;                     /*!< @brief From main.c */
extern volatile u32 G_u32ApplicationFlags;                /*!< @brief From main.c */

extern const u8 aau8StitchK; // from lcd_bitmaps
extern const u8 aau8StitchP; // from lcd_bitmaps
extern const u8 aau8StitchYO; // from lcd_bitmaps
extern const u8 aau8StitchSSK; // from lcd_bitmaps
extern const u8 aau8StitchK2TOG; // from lcd_bitmaps
extern const u8 aau8StitchCheck; // from lcd_bitmaps
extern const u8 aau8RowMark; // from lcd_bitmaps



/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp1_<type>" and be declared as static.
***********************************************************************************************************************/
static fnCode_type UserApp1_pfStateMachine;               /*!< @brief The state machine function pointer */
//static u32 UserApp1_u32Timeout;                           /*!< @brief Timeout counter used across states */


/**********************************************************************************************************************
Function Definitions
**********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/*! @publicsection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/

void LoadLittleDove(u8 u8RowState) {
    // load row arrow at 1
    static PixelAddressType sPointWSLoc = {U8_LCD_SMALL_FONT_LINE6, U16_LCD_LEFT_MOST_COLUMN};
    u8 au8PointWSStr[] = "W>";
    LcdLoadString(au8PointWSStr, LCD_FONT_SMALL, &sPointWSLoc);

    // load row arrow at 2
    u8 au8TestString[] = "<R";
    static PixelAddressType sTestLoc = {U8_LCD_SMALL_FONT_LINE6 - 12, (U16_LCD_RIGHT_MOST_COLUMN) - 13};
    LcdLoadString(au8TestString, LCD_FONT_SMALL, &sTestLoc);
  
    // load check boxes
  
    static PixelBlockType sCheckBox;
    sCheckBox.u16RowStart = 0;
    sCheckBox.u16ColumnStart = 12;
    sCheckBox.u16RowSize = 8; // pixels tall 
    sCheckBox.u16ColumnSize = 10; // pixels wide
  
    for(u8 i = 0; i < 5; i++){
      LcdLoadBitmap(&aau8StitchK, &sCheckBox);
      sCheckBox.u16RowStart +=12;
    } 
  
    // load grid
    // grid row 1, 3, 5 (purl all stitches)
  
    static PixelBlockType sPRow;
    sPRow.u16RowStart = U8_LCD_SMALL_FONT_LINE6;
    sPRow.u16ColumnStart = 23;
    sPRow.u16RowSize = 8;
    sPRow.u16ColumnSize = 10;
  
    for(u8 h = 0; h < 3; h++){
      for(u8 i=0; i< 9; i++)
      {
        LcdLoadBitmap(&aau8StitchP, &sPRow);
        sPRow.u16ColumnStart +=10;
      }
      sPRow.u16RowStart -= 24;
      sPRow.u16ColumnStart = 23;
    }

  
    // grid row 2
    static PixelBlockType sRow2;
    sRow2.u16RowStart = (U8_LCD_SMALL_FONT_LINE6)-12;
    sRow2.u16ColumnStart = 23;
    sRow2.u16RowSize = 8;
    sRow2.u16ColumnSize = 10;

    LcdLoadBitmap(&aau8StitchK, &sRow2);
    sRow2.u16ColumnStart+=10;
    LcdLoadBitmap(&aau8StitchYO, &sRow2);
    sRow2.u16ColumnStart+=10;
    LcdLoadBitmap(&aau8StitchK, &sRow2);
    sRow2.u16ColumnStart+=10;
    LcdLoadBitmap(&aau8StitchK, &sRow2);
    sRow2.u16ColumnStart+=10;
    LcdLoadBitmap(&aau8StitchSSK, &sRow2);
    sRow2.u16ColumnStart+=10;
    LcdLoadBitmap(&aau8StitchK2TOG, &sRow2);
    sRow2.u16ColumnStart+=10;
    LcdLoadBitmap(&aau8StitchK, &sRow2);
    sRow2.u16ColumnStart+=10;
    LcdLoadBitmap(&aau8StitchK, &sRow2);
    sRow2.u16ColumnStart+=10;
    LcdLoadBitmap(&aau8StitchYO, &sRow2);

  
    // grid row 4
    static PixelBlockType sRow4;
    sRow4.u16RowStart = (U8_LCD_SMALL_FONT_LINE6)-36;
    sRow4.u16ColumnStart = 23;
    sRow4.u16RowSize = 8;
    sRow4.u16ColumnSize = 10;

    LcdLoadBitmap(&aau8StitchYO, &sRow4);
    sRow4.u16ColumnStart+=10;
    LcdLoadBitmap(&aau8StitchK, &sRow4);
    sRow4.u16ColumnStart+=10;
    LcdLoadBitmap(&aau8StitchK, &sRow4);
    sRow4.u16ColumnStart+=10;
    LcdLoadBitmap(&aau8StitchSSK, &sRow4);
    sRow4.u16ColumnStart+=10;
    LcdLoadBitmap(&aau8StitchK2TOG, &sRow4);
    sRow4.u16ColumnStart+=10;
    LcdLoadBitmap(&aau8StitchK, &sRow4);
    sRow4.u16ColumnStart+=10;
    LcdLoadBitmap(&aau8StitchK, &sRow4);
    sRow4.u16ColumnStart+=10;
    LcdLoadBitmap(&aau8StitchYO, &sRow4);
    sRow4.u16ColumnStart+=10;
    LcdLoadBitmap(&aau8StitchK, &sRow4);
  
    // load lines marking place at row 1
  
    //static PixelBlockType sUpperRowMark = {(u8)60, (u8)23, (u8)36, (u8)1};
    //LcdLoadBitmap(aau8RowMark, &sUpperRowMark);
  
  
    //static PixelAddressType sRowMarkUpper = {U8_LCD_SMALL_FONT_LINE6, U16_LCD_LEFT_MOST_COLUMN};
    //u8 au8RowMark[] = "_____________";
    //LcdLoadString(au8RowMark, LCD_FONT_SMALL, &sRowMarkUpper);
    //static PixelAddressType sRowMarkLower = {U8_LCD_SMALL_FONT_LINE7, U16_LCD_LEFT_MOST_COLUMN};
    //LcdLoadString(au8RowMark, LCD_FONT_SMALL, &sRowMarkLower);
    
    // load rectangle around grid

}

void LoadLegend(void)
{

  // Load title strings
  static PixelAddressType sTitleStr1 = {U8_LCD_SMALL_FONT_LINE0, U16_LCD_LEFT_MOST_COLUMN};
  u8 au8TitleStr1[] = "Pattern Repeat Chart";
  static PixelAddressType sTitleStr2 = {U8_LCD_SMALL_FONT_LINE1, U16_LCD_LEFT_MOST_COLUMN};
  u8 au8TitleStr2[] = "Little Dove Blanket";
  static PixelAddressType sTitleStr3 = {U8_LCD_SMALL_FONT_LINE3, U16_LCD_LEFT_MOST_COLUMN};
  u8 au8TitleStr3[] = "Legend";

  LcdLoadString(au8TitleStr1, LCD_FONT_SMALL, &sTitleStr1);
  LcdLoadString(au8TitleStr2, LCD_FONT_SMALL, &sTitleStr2);
  LcdLoadString(au8TitleStr3, LCD_FONT_SMALL, &sTitleStr3);


  // Load legend strings
  static PixelAddressType sLabelAddress = {U8_LCD_SMALL_FONT_LINE6, U16_LCD_LEFT_MOST_COLUMN};
  u8 au8LegendLabels[] = "K   P  YO  SSK  K2TOG";
  LcdLoadString(au8LegendLabels, LCD_FONT_SMALL, &sLabelAddress);
  

  // Load stitch bitmaps above the text
  static PixelBlockType sStsAddress;
  sStsAddress.u16RowStart = 38;
  sStsAddress.u16ColumnStart = 0;
  sStsAddress.u16RowSize = 8; // pixels tall 
  sStsAddress.u16ColumnSize = 10; // pixels wide

  LcdLoadBitmap(&aau8StitchK, &sStsAddress);
  sStsAddress.u16ColumnStart +=20;

  LcdLoadBitmap(&aau8StitchP, &sStsAddress);
  sStsAddress.u16ColumnStart +=20;

  LcdLoadBitmap(&aau8StitchYO, &sStsAddress);
  sStsAddress.u16ColumnStart +=30;

  LcdLoadBitmap(&aau8StitchSSK, &sStsAddress);
  sStsAddress.u16ColumnStart +=33;
  
  LcdLoadBitmap(&aau8StitchK2TOG, &sStsAddress);

}

/*--------------------------------------------------------------------------------------------------------------------*/
/*! @protectedsection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/

/*!--------------------------------------------------------------------------------------------------------------------
@fn void UserApp1Initialize(void)

@brief
Initializes the State Machine and its variables.

Should only be called once in main init section.

Requires:
- NONE

Promises:
- NONE

*/
void UserApp1Initialize(void)
{
  
  LcdClearScreen();
  
  // ~Load static pattern. 
  LoadLittleDove(0b00000);

  
  /* If good initialization, set state to Idle */
  if( 1 )
  {
    UserApp1_pfStateMachine = UserApp1SM_Idle;
  }
  else
  {
    /* The task isn't properly initialized, so shut it down and don't run */
    UserApp1_pfStateMachine = UserApp1SM_Error;
  }

} /* end UserApp1Initialize() */

  
/*!----------------------------------------------------------------------------------------------------------------------
@fn void UserApp1RunActiveState(void)

@brief Selects and runs one iteration of the current state in the state machine.

All state machines have a TOTAL of 1ms to execute, so on average n state machines
may take 1ms / n to execute.

Requires:
- State machine function pointer points at current state

Promises:
- Calls the function to pointed by the state machine function pointer

*/
void UserApp1RunActiveState(void)
{
  UserApp1_pfStateMachine();

} /* end UserApp1RunActiveState */


/*------------------------------------------------------------------------------------------------------------------*/
/*! @privatesection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/


/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/
/*-------------------------------------------------------------------------------------------------------------------*/
/* What does this state do? */
static void UserApp1SM_Idle(void)
{ 
  static u8 u8Legend_On = 0;
  if(WasButtonPressed(BUTTON0)) 
  {
    LcdClearScreen();
    if(u8Legend_On == 0)
    {
      LoadLegend();
      u8Legend_On = 1; 
    } // if(u8Legend_On ==0)
    else
    {
      LoadLittleDove(0b0000);
      u8Legend_On = 0;
    } //else
    ButtonAcknowledge(BUTTON0);
  } // if(WasButtonPressed)

  static u8 u8current_row = 1; // updated based on button1

  static u8 u8row_state = 0b00000; // updated based on button1

  // load check box at row level, then update the row state
  static u8 u8AlreadyHeld = 0;
  
  if(IsButtonHeld(BUTTON1, 2000)) 
  {
    if(!(u8Legend_On)) 
    {
      if(u8AlreadyHeld == 0)
      {
        u8AlreadyHeld = 1;
        static PixelBlockType sCheckPosition;
        sCheckPosition.u16ColumnStart = 12;
        sCheckPosition.u16RowSize = 8; // pixels tall 
        sCheckPosition.u16ColumnSize = 10; // pixels wide
        switch(u8current_row)
        {
          case 1:
            sCheckPosition.u16RowStart = 48; 
            if(u8row_state &= 0b00001) // check if the last bit is already completed. Value will be non-zero if the bit is a one, and zero if the bit is 0
            {
              u8row_state &= 0b11110; // set first bit to 0, other bits remain the same.
              LcdLoadBitmap(&aau8StitchK, &sCheckPosition);
            }
            else
            {
              u8row_state |= 0b00001; // set first bit equal to 1, other bits remain the same
              LcdLoadBitmap(&aau8StitchCheck, &sCheckPosition);
            }
            break;
          case 2:
            sCheckPosition.u16RowStart = 36;
            if(u8row_state &= 0b00010) // check if the bit is already completed. Value will be non-zero if the bit is a one, and zero if the bit is 0
            {
              u8row_state &= 0b11101; // set first bit to 0, other bits remain the same.
              LcdLoadBitmap(&aau8StitchK, &sCheckPosition);
            }
            else
            {
              u8row_state |= 0b00010; // set second bit equal to 1, other bits remain the same
              LcdLoadBitmap(&aau8StitchCheck, &sCheckPosition);
            }
            break;
          case 3:
            sCheckPosition.u16RowStart = 24;
            if(u8row_state &= 0b00100) // check if the last bit is already completed. Value will be non-zero if the bit is a one, and zero if the bit is 0
            {
              u8row_state &= 0b11011; // set third bit to 0, other bits remain the same.
              LcdLoadBitmap(&aau8StitchK, &sCheckPosition);
            }
            else
            {
              u8row_state |= 0b00100; // set third bit equal to 1, other bits remain the same
              LcdLoadBitmap(&aau8StitchCheck, &sCheckPosition);
            }
            break;
          case 4:
            sCheckPosition.u16RowStart = 12;
            if(u8row_state &= 0b01000) // check if the bit is already completed. Value will be non-zero if the bit is a one, and zero if the bit is 0
            {
              u8row_state &= 0b10111; // set fourth bit to 0, other bits remain the same.
              LcdLoadBitmap(&aau8StitchK, &sCheckPosition);
            }
            else
            {
              u8row_state |= 0b01000; // set fourth bit equal to 1, other bits remain the same
              LcdLoadBitmap(&aau8StitchCheck, &sCheckPosition);
            }
            break;
          case 5:
            sCheckPosition.u16RowStart = 0;
            if(u8row_state &= 0b10000) // check if the bit is already completed. Value will be non-zero if the bit is a one, and zero if the bit is 0
            {
              u8row_state &= 0b01111; // set fifth bit to 0, other bits remain the same.
              LcdLoadBitmap(&aau8StitchK, &sCheckPosition);
            }
            else
            {
              u8row_state |= 0b10000; // set fifth bit equal to 1, other bits remain the same
              LcdLoadBitmap(&aau8StitchCheck, &sCheckPosition);
            }
            break;
        } //switch(u8current_row)
      } //if(u8AlreadyHeld)
    } //if(!(u8LegendOn)
  } else u8AlreadyHeld = 0; // if(WasButtonPressed(BUTTON1)


  if(WasButtonPressed(BUTTON1))
  {
    ButtonAcknowledge(BUTTON1);
    if(u8Legend_On == 0)
    {
      // change to next row
      (u8current_row < 5) ? (u8current_row++) : (u8current_row = 1);
      
      static PixelBlockType aau8LeftColumn = {0, 0, 60, 11};
      static PixelBlockType aau8RightColumn = {0, 114, 60, 11};
      LcdClearPixels(&aau8LeftColumn);
      LcdClearPixels(&aau8RightColumn);
      // load row arrow at current row
      if(((int)u8current_row)%2 == 0){


        // load row arrow at RS
        u8 au8TestString[] = "<R";
        static PixelAddressType sTestLoc = {U8_LCD_SMALL_FONT_LINE6 - 12, (U16_LCD_RIGHT_MOST_COLUMN) - 13};
        sTestLoc.u16PixelRowAddress = 48 - (12*(u8current_row-1));
        LcdLoadString(au8TestString, LCD_FONT_SMALL, &sTestLoc);


      }
      else
      {
        static PixelAddressType sPointWSLoc;
        sPointWSLoc.u16PixelRowAddress = 48 - (12*(u8current_row-1));
        sPointWSLoc.u16PixelColumnAddress = 0;
        u8 au8PointWSStr[] = "W>";
        LcdLoadString(au8PointWSStr, LCD_FONT_SMALL, &sPointWSLoc);   
        
        //aau8RowArrow.u16RowStart = 36-12*(u8current_row);
        //aau8RowArrow.u16ColumnStart = 144;
        //LcdClearPixels(&aau8RowArrow);
      }
   }


  } // if(WasButtonPressed(BUTTON1))


} /* end UserApp1SM_Idle() */



/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserApp1SM_Error(void)          
{
  
} /* end UserApp1SM_Error() */




/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
