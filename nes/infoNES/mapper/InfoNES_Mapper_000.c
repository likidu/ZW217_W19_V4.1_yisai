/*===================================================================*/
/*                                                                   */
/*                            Mapper 0                               */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 0                                              */
/*-------------------------------------------------------------------*/
void Map0_Init(Nes_Global_T *pGData)
{
  /* Initialize Mapper */
  MapperInit = Map0_Init;

  /* Write to Mapper */
  MapperWrite = Map0_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map0_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  pGData->SRAMBANK = pGData->SRAM;

  /* Set ROM Banks */
  if ( NesHeader.byRomSize > 1 )
  {
    pGData->ROMBANK0 = ROMPAGE( 0 );
    pGData->ROMBANK1 = ROMPAGE( 1 );
    pGData->ROMBANK2 = ROMPAGE( 2 );
    pGData->ROMBANK3 = ROMPAGE( 3 );
  }
  else if ( NesHeader.byRomSize > 0 )
  {
    pGData->ROMBANK0 = ROMPAGE( 0 );
    pGData->ROMBANK1 = ROMPAGE( 1 );
    pGData->ROMBANK2 = ROMPAGE( 0 );
    pGData->ROMBANK3 = ROMPAGE( 1 );
  } else {
    pGData->ROMBANK0 = ROMPAGE( 0 );
    pGData->ROMBANK1 = ROMPAGE( 0 );
    pGData->ROMBANK2 = ROMPAGE( 0 );
    pGData->ROMBANK3 = ROMPAGE( 0 );
  }

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 )
  {
  	int nPage;
    for ( nPage = 0; nPage < 8; ++nPage )
      pGData->PPUBANK[ nPage ] = VROMPAGE( nPage );
    InfoNES_SetupChr(pGData);
  }

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 0 Write Function                                          */
/*-------------------------------------------------------------------*/
void Map0_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData )
{
/*
 *  Dummy Write to Mapper
 *
 */
}

/*-------------------------------------------------------------------*/
/*  Mapper 0 Write to SRAM Function                                  */
/*-------------------------------------------------------------------*/
void Map0_Sram(Nes_Global_T *pGData, WORD wAddr, BYTE byData )
{
/*
 *  Dummy Write to Sram
 *
 */
}

/*-------------------------------------------------------------------*/
/*  Mapper 0 Write to APU Function                                   */
/*-------------------------------------------------------------------*/
void Map0_Apu(Nes_Global_T *pGData, WORD wAddr, BYTE byData )
{
/*
 *  Dummy Write to Apu
 *
 */
}

/*-------------------------------------------------------------------*/
/*  Mapper 0 Read from APU Function                                  */
/*-------------------------------------------------------------------*/
BYTE Map0_ReadApu(Nes_Global_T *pGData, WORD wAddr )
{
/*
 *  Dummy Read from Apu
 *
 */
  return ( wAddr >> 8 );
}

/*-------------------------------------------------------------------*/
/*  Mapper 0 V-Sync Function                                         */
/*-------------------------------------------------------------------*/
void Map0_VSync(Nes_Global_T *pGData)
{
/*
 *  Dummy Callback at VSync
 *
 */
}

/*-------------------------------------------------------------------*/
/*  Mapper 0 H-Sync Function                                         */
/*-------------------------------------------------------------------*/
void Map0_HSync(Nes_Global_T *pGData)
{
/*
 *  Dummy Callback at HSync
 *
 */
#if 0 
  // Frame IRQ
  pGData->FrameStep += STEP_PER_SCANLINE;
  if ( pGData->FrameStep > STEP_PER_FRAME && pGData->FrameIRQ_Enable )
  {
    pGData->FrameStep %= STEP_PER_FRAME;
    IRQ_REQ;
    APU_Reg[ 0x4015 ] |= 0x40;
  }
#endif
}

/*-------------------------------------------------------------------*/
/*  Mapper 0 PPU Function                                            */
/*-------------------------------------------------------------------*/
void Map0_PPU(Nes_Global_T *pGData, WORD wAddr )
{
/*
 *  Dummy Callback at PPU
 *
 */
}

/*-------------------------------------------------------------------*/
/*  Mapper 0 Rendering Screen Function                               */
/*-------------------------------------------------------------------*/
void Map0_RenderScreen(Nes_Global_T *pGData, BYTE byMode )
{
/*
 *  Dummy Callback at Rendering Screen
 *
 */
}
