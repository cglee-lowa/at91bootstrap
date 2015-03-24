/* ----------------------------------------------------------------------------
 *         SAM Software Package License
 * ----------------------------------------------------------------------------
 * Copyright (c) 2011, Atmel Corporation
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the disclaimer below.
 *
 * Atmel's name may not be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * DISCLAIMER: THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ----------------------------------------------------------------------------
 */

/** \addtogroup dmad_module
 *
 * \section DmaConfig Dma Configuration Usage
 *
 * To configure a DMA channel, the user has to follow these few steps :
 * <ul>
 * <li> Initialize a DMA driver instance by DMAD_Initialize().</li>
 * <li> choose an available (disabled) channel using DMAD_AllocateChannel().</li>
 * <li> After the DMAC selected channel has been programmed, DMAD_PrepareChannel() is to enable
 * clock and dma peripheral of the DMA, and set Configuration register to set up the transfer type
 * (memory or non-memory peripheral for source and destination) and flow control device.</li>
 * <li> Configure DMA multi-buffer transfers using DMAD_PrepareMultiTransfer() to set up the chain of Linked List Items,
 * single-buffer transfers using DMAD_PrepareSingleTransfer().</li>
 * <li> Invoke DMAD_StartTransfer() to start DMA transfer, or DMAD_StopTransfer() to force stop DMA transfer.</li>
 * <li> If picture-in-picture mode is enabled, DMAD_ConfigurePIP() helps to configure PIP mode.</li>
 * <li> Once the buffer of data is transferred, DMAD_IsTransferDone() checks if DMA transfer is finished.</li>
 * <li> DMAD_Handler() handles DMA interrupt, and invoking DMAD_SetCallback() if provided.</li>
 * </ul>
 *
 * Related files:\n
 * \ref dmad.h\n
 * \ref dmad.c.\n
 */

/** \file */

/** \addtogroup dmad_functions
  @{*/

/*----------------------------------------------------------------------------
 *        Includes
 *----------------------------------------------------------------------------*/
#include "hardware.h"
#include "board.h"
#include "dmad.h"
#include "debug.h"
#include "dma_hardware_interface.h"
#include "arch/at91_pmc.h"

/*----------------------------------------------------------------------------
 *        Local functions
 *----------------------------------------------------------------------------*/
/**
 * \brief Get Periph Status for the given peripheral ID.
 *
 * \param id  Peripheral ID (ID_xxx).
 * @note : ported from SoftPack : pmc.c
 */
static unsigned int PMC_IsPeriphEnabled( unsigned int dwId )
{
    if (dwId < 32) {
        return ( readl (AT91C_BASE_PMC + PMC_PCSR) & (1 << dwId) ) ;
    } else {
        return ( readl (AT91C_BASE_PMC + PMC_PCSR1) & (1 << (dwId - 32)) ) ;
    }
}
/**
 * \brief Enables the clock of a peripheral. The peripheral ID is used
 * to identify which peripheral is targetted.
 *
 * \note The ID must NOT be shifted (i.e. 1 << ID_xxx).
 *
 * \param id  Peripheral ID (ID_xxx).
 * @note : ported from SoftPack : pmc.c
 */
extern void PMC_EnablePeripheral( unsigned int dwId )
{
    if (dwId < 32)
    {
        if ( (readl (AT91C_BASE_PMC + PMC_PCSR) & ((unsigned int)1 << dwId)) == ((unsigned int)1 << dwId) )
        {
         //   TRACE_DEBUG( "PMC_EnablePeripheral: clock of peripheral"  " %u is already enabled\n", dwId ) ;
        }
        else
        {
            writel((1 << dwId),AT91C_BASE_PMC + PMC_PCER);
        }
    } else {
        if ( (readl (AT91C_BASE_PMC + PMC_PCSR1) & ((unsigned int)1 << ( dwId - 32))) == ((unsigned int)1 << (dwId - 32)) )
        {
           // TRACE_DEBUG( "PMC_EnablePeripheral: clock of peripheral"  " %u is already enabled\n", dwId ) ;
        }
        else
        {
        	writel((1 << (dwId - 32)),AT91C_BASE_PMC + PMC_PCER1);
        }
    }
}

/**
 * \brief Try to allocate a DMA channel for on given controller.
 * \param pDmad  Pointer to DMA driver instance.
 * \param bDmac  DMA controller ID (0 ~ 1).
 * \param bSrcID Source peripheral ID, 0xFF for memory.
 * \param bDstID Destination peripheral ID, 0xFF for memory.
 * \return Channel number if allocation sucessful, return
 * DMAD_ALLOC_FAILED if allocation failed.
 */
static unsigned int DMAD_AllocateDmacChannel( sDmad *pDmad,
                                          unsigned char bDmac,
                                          unsigned char bSrcID,
                                          unsigned char bDstID)
{
    unsigned int i;

    /* Can't support peripheral to peripheral */
    if ((( bSrcID != DMAD_TRANSFER_MEMORY ) && ( bDstID != DMAD_TRANSFER_MEMORY )))
    {
        return DMAD_ALLOC_FAILED;
    }
    /* dma transfer from peripheral to memory */
    if ( bDstID == DMAD_TRANSFER_MEMORY)
    {
        if( (!DMAIF_IsValidatedPeripherOnDma(bDmac, bSrcID)) )
        {
            return DMAD_ALLOC_FAILED;
        }
    }
    /* dma transfer from memory to peripheral */
    if ( bSrcID == DMAD_TRANSFER_MEMORY )
    {
        if( (!DMAIF_IsValidatedPeripherOnDma(bDmac, bDstID)) )
        {
            return DMAD_ALLOC_FAILED;
        }
    }
    for (i = 0; i < pDmad->numChannels; i ++)
    {
        if ( pDmad->dmaChannels[bDmac][i].state == DMAD_FREE )
        {
            /* Allocate the channel */
            pDmad->dmaChannels[bDmac][i].state = DMAD_IN_USE;
            /* Get general informations */
            pDmad->dmaChannels[bDmac][i].bSrcPeriphID = bSrcID;
            pDmad->dmaChannels[bDmac][i].bDstPeriphID = bDstID;
            pDmad->dmaChannels[bDmac][i].bSrcTxIfID =
                DMAIF_Get_ChannelNumber(bDmac, bSrcID, 0);
            pDmad->dmaChannels[bDmac][i].bSrcRxIfID =
                DMAIF_Get_ChannelNumber(bDmac, bSrcID, 1);
            pDmad->dmaChannels[bDmac][i].bDstTxIfID =
                DMAIF_Get_ChannelNumber(bDmac, bDstID, 0);
            pDmad->dmaChannels[bDmac][i].bDstTxIfID =
                DMAIF_Get_ChannelNumber(bDmac, bDstID, 1);

            return ((bDmac << 8)) | ((i) & 0xFF);
        }
    }
    return DMAD_ALLOC_FAILED;
}

/*----------------------------------------------------------------------------
 *        Exported functions
 *----------------------------------------------------------------------------*/

/**
 * \brief Initialize DMA driver instance.
 * \param pDmad Pointer to DMA driver instance.
 * \param bPollingMode Polling DMA transfer:
 *                     1. Via DMAD_IsTransferDone(); or
 *                     2. Via DMAD_Handler().
 */
void DMAD_Initialize( sDmad *pDmad,
                      unsigned char bPollingMode )
{
    unsigned int i, j;


    pDmad->pDmacs[0] = (Dmac*) AT91C_BASE_DMAC0;
    pDmad->pDmacs[1] = (Dmac*) AT91C_BASE_DMAC1;
    pDmad->pollingMode = bPollingMode;
    pDmad->numControllers = 2;
    pDmad->numChannels    = 8;

    for (i = 0; i < pDmad->numControllers; i ++)
    {
        for (j = 0; j < pDmad->numChannels; j ++)
        {
            pDmad->dmaChannels[i][j].fCallback = 0;
            pDmad->dmaChannels[i][j].pArg      = 0;

            pDmad->dmaChannels[i][j].bIrqOwner    = 0;
            pDmad->dmaChannels[i][j].bSrcPeriphID = 0;
            pDmad->dmaChannels[i][j].bDstPeriphID = 0;
            pDmad->dmaChannels[i][j].bSrcTxIfID   = 0;
            pDmad->dmaChannels[i][j].bSrcRxIfID   = 0;
            pDmad->dmaChannels[i][j].bDstTxIfID   = 0;
            pDmad->dmaChannels[i][j].bDstRxIfID   = 0;

            pDmad->dmaChannels[i][j].state = DMAD_FREE;
        }
    }
}

/**
 * \brief DMA interrupt handler
 * \param pDmad Pointer to DMA driver instance.
 */
void DMAD_Handler( sDmad *pDmad )
{
    Dmac *pDmac;
    sDmadChannel *pCh;
    unsigned int _iController, iChannel;
    unsigned int dmaSr, chSr;
    unsigned int dmaRc = DMAD_OK;


    for (_iController = 0; _iController < pDmad->numControllers; _iController ++)
    {
        pDmac = pDmad->pDmacs[_iController];

        /* Check raw status but not masked one for polling mode support */
        dmaSr = DMAC_GetStatus( pDmac );
        if ((dmaSr & 0x00FFFFFF) == 0) continue;

        chSr  = DMAC_GetChannelStatus( pDmac );

        for (iChannel = 0; iChannel < pDmad->numChannels; iChannel ++)
        {
            unsigned char bExec = 1;

            pCh = &pDmad->dmaChannels[_iController][iChannel];
            /* Error */
            if (dmaSr & (DMAC_EBCIDR_ERR0 << iChannel))
            {
                DMAC_DisableChannel( pDmac, iChannel );
                if (pCh->state > DMAD_IN_USE)   pCh->state = DMAD_STALL;
                dmaRc = DMAD_ERROR;
            }
            /* Chained buffer complete */
            else if (dmaSr & (DMAC_EBCIDR_CBTC0 << iChannel))
            {
            	//------------------------
                DMAC_DisableChannel( pDmac, iChannel );
                if (pCh->state > DMAD_IN_USE)   pCh->state = DMAD_IN_USE;
                dmaRc = DMAD_OK;

            }
            /* Buffer complete */
            else if (dmaSr & (DMAC_EBCIDR_BTC0 << iChannel))
            {
                dmaRc = DMAD_PARTIAL_DONE;
                /* Re-enable */
                if ((chSr & (DMAC_CHSR_ENA0 << iChannel)) == 0)
                {
                    DMAC_EnableChannel( pDmac, iChannel );
                }
            }
            else
            {
                bExec = 0;
            }
            /* Execute callback */
            if (bExec && pCh->fCallback)
            {
                pCh->fCallback(dmaRc, pCh->pArg);
            }
        }
    }
}

/**
 * \brief Allocate a DMA channel for upper layer.
 * \param pDmad  Pointer to DMA driver instance.
 * \param bSrcID Source peripheral ID, 0xFF for memory.
 * \param bDstID Destination peripheral ID, 0xFF for memory.
 * \return Channel number if allocation sucessful, return
 * DMAD_ALLOC_FAILED if allocation failed.
 */
unsigned int DMAD_AllocateChannel( sDmad *pDmad,
                               unsigned char bSrcID,
                               unsigned char bDstID)
{
    unsigned int _iController;
    unsigned int _dwChannel = DMAD_ALLOC_FAILED;

    for ( _iController = 0; _iController < pDmad->numControllers; _iController ++)
    {
        _dwChannel = DMAD_AllocateDmacChannel( pDmad, _iController,
                                              bSrcID, bDstID );
        if (_dwChannel != DMAD_ALLOC_FAILED)
            break;
    }
    return _dwChannel;
}

/**
 * \brief Free the specified DMA channel.
 * \param pDmad     Pointer to DMA driver instance.
 * \param _dwChannel ControllerNumber << 8 | ChannelNumber.
 */
eDmadRC DMAD_FreeChannel( sDmad *pDmad, unsigned int _dwChannel )
{
    unsigned char _iController = (_dwChannel >> 8);
    unsigned char iChannel    = (_dwChannel) & 0xFF;

    switch ( pDmad->dmaChannels[_iController][iChannel].state )
    {

    case DMAD_IN_XFR:
        return DMAD_BUSY;

    case DMAD_IN_USE:
        pDmad->dmaChannels[_iController][iChannel].state = DMAD_FREE;
        break;
    }
	return DMAD_OK;
}

/**
 * \brief Set the callback function for DMA channel transfer.
 * \param pDmad     Pointer to DMA driver instance.
 * \param _dwChannel ControllerNumber << 8 | ChannelNumber.
 * \param fCallback Pointer to callback function.
 * \param pArg Pointer to optional argument for callback.
 */
eDmadRC DMAD_SetCallback( sDmad *pDmad, unsigned int _dwChannel,
                          DmadTransferCallback fCallback, void* pArg )
{
    unsigned char _iController = (_dwChannel >> 8);
    unsigned char iChannel    = (_dwChannel) & 0xFF;

    if ( pDmad->dmaChannels[_iController][iChannel].state == DMAD_FREE )
        return DMAD_ERROR;
    else if ( pDmad->dmaChannels[_iController][iChannel].state == DMAD_IN_XFR )
        return DMAD_BUSY;

    pDmad->dmaChannels[_iController][iChannel].fCallback = fCallback;
    pDmad->dmaChannels[_iController][iChannel].pArg = pArg;

    return DMAD_OK;
}

/**
 * \brief Configure Picture-in-Picture mode for DMA transfer.
 * \param pDmad     Pointer to DMA driver instance.
 * \param _dwChannel ControllerNumber << 8 | ChannelNumber.
 * \param srcPIP    Source PIP setting.
 * \param dstPIP    Destination PIP setting.
 */
eDmadRC DMAD_ConfigurePIP( sDmad *pDmad,
                           unsigned int _dwChannel,
                           unsigned int dwSrcPIP,
                           unsigned int dwDstPIP )
{
    unsigned char _iController = (_dwChannel >> 8);
    unsigned char iChannel    = (_dwChannel) & 0xFF;

    Dmac *pDmac = pDmad->pDmacs[_iController];
    if ( pDmad->dmaChannels[_iController][iChannel].state == DMAD_FREE )
        return DMAD_ERROR;
    else if ( pDmad->dmaChannels[_iController][iChannel].state == DMAD_IN_XFR )
        return DMAD_BUSY;

    DMAC_SetPipMode(pDmac, iChannel, dwSrcPIP, dwDstPIP);
    return DMAD_OK;
}

/**
 * \brief Enable clock of the DMA peripheral, Enable the dma peripheral,
 * configure configuration register for DMA transfer.
 * \param pDmad     Pointer to DMA driver instance.
 * \param _dwChannel ControllerNumber << 8 | ChannelNumber.
 * \param dwCfg     Configuration value.
 */
eDmadRC DMAD_PrepareChannel( sDmad *pDmad,
                             unsigned int _dwChannel,
                             unsigned int dwCfg )
{
    unsigned char _iController = (_dwChannel >> 8);
    unsigned char iChannel    = (_dwChannel) & 0xFF;
    unsigned int _dwdmaId;

    Dmac *pDmac = pDmad->pDmacs[_iController];

    if ( pDmad->dmaChannels[_iController][iChannel].state == DMAD_FREE )
        return DMAD_ERROR;
    else if ( pDmad->dmaChannels[_iController][iChannel].state == DMAD_IN_XFR )
        return DMAD_BUSY;
    DMAC_SetCFG( pDmac, iChannel, dwCfg );
    _dwdmaId = (_iController == 0) ?  AT91C_ID_DMAC0 : AT91C_ID_DMAC1 ;
    /* Enable clock of the DMA peripheral */
    while (!PMC_IsPeriphEnabled( _dwdmaId ))
    {
        PMC_EnablePeripheral( _dwdmaId );
    }

    /* Enables the DMAC peripheral. */
    DMAC_Enable( pDmac );

    /* Disables DMAC interrupt for the given channel. */
    DMAC_DisableIt (pDmac,
                    (DMAC_EBCIDR_BTC0 << iChannel)
                   |(DMAC_EBCIDR_CBTC0 << iChannel)
                   |(DMAC_EBCIDR_ERR0 << iChannel) );

    /* Disable the given dma channel. */
    DMAC_DisableChannel( pDmac, iChannel );
    /* Clear dummy status */
    DMAC_GetChannelStatus( pDmac );
    DMAC_GetStatus (pDmac);
    return DMAD_OK;
}

/**
 * \brief Check if DMA transfer is finished.
 *        In polling mode DMAD_Handler() is polled.
 * \param pDmad     Pointer to DMA driver instance.
 * \param _dwChannel ControllerNumber << 8 | ChannelNumber.
 */
eDmadRC DMAD_IsTransferDone( sDmad *pDmad, unsigned int _dwChannel )
{
    unsigned char _iController = (_dwChannel >> 8);
    unsigned char iChannel    = (_dwChannel) & 0xFF;

#ifndef SPI_FLASH_WITH_DMA_IRQ
    if ( pDmad->pollingMode ) DMAD_Handler( pDmad ); // should be done here BEFORE any state check
#endif
    if ( pDmad->dmaChannels[_iController][iChannel].state == DMAD_FREE )
        return DMAD_ERROR;
    else if ( pDmad->dmaChannels[_iController][iChannel].state == DMAD_IN_XFR )
    {
        return DMAD_BUSY;
    }
    return DMAD_OK;
}

/**
 * \brief Clear the automatic mode that services the next-to-last
    buffer transfer.
 * \param pDmad     Pointer to DMA driver instance.
 * \param _dwChannel ControllerNumber << 8 | ChannelNumber.
 */
void DMAD_ClearAuto( sDmad *pDmad, unsigned int _dwChannel )
{
    unsigned char _iController = (_dwChannel >> 8);
    unsigned char iChannel    = (_dwChannel) & 0xFF;
    Dmac *pDmac;

    pDmac = pDmad->pDmacs[_iController];
    DMAC_DisableAutoMode( pDmac, iChannel );
}

/**
 * \brief Start DMA transfer.
 * \param pDmad     Pointer to DMA driver instance.
 * \param _dwChannel ControllerNumber << 8 | ChannelNumber.
 */
eDmadRC DMAD_StartTransfer( sDmad *pDmad, unsigned int _dwChannel )
{
    unsigned char _iController = (_dwChannel >> 8);
    unsigned char iChannel    = (_dwChannel) & 0xFF;

    Dmac *pDmac = pDmad->pDmacs[_iController];

    if ( pDmad->dmaChannels[_iController][iChannel].state == DMAD_FREE )
        return DMAD_ERROR;
    else if ( pDmad->dmaChannels[_iController][iChannel].state == DMAD_IN_XFR )
        return DMAD_BUSY;
    /* Change state to transferring */
    pDmad->dmaChannels[_iController][iChannel].state = DMAD_IN_XFR;
    DMAC_EnableChannel(pDmac, iChannel);
    if ( pDmad->pollingMode == 0 )
    {
        /* Monitor status in interrupt handler */
        DMAC_EnableIt(pDmac, (DMAC_EBCIDR_BTC0 << iChannel)
                            |(DMAC_EBCIDR_CBTC0 << iChannel)
                            |(DMAC_EBCIDR_ERR0 << iChannel) );
    }

    return DMAD_OK;
}

/**
 * \brief Start DMA transfers on the same controller.
 * \param pDmad      Pointer to DMA driver instance.
 * \param bDmac      DMA Controller number.
 * \param bmChannels Channels bitmap.
 */
eDmadRC DMAD_StartTransfers( sDmad *pDmad, unsigned char bDmac, unsigned int bmChannels )
{
    unsigned int iChannel;
    unsigned int bmChs = 0, bmIts = 0;

    Dmac *pDmac = pDmad->pDmacs[bDmac];

    for (iChannel = 0; iChannel < pDmad->numChannels; iChannel ++)
    {
        unsigned int bmChBit = 1 << iChannel;

        /* Skipped channels */
        if ( pDmad->dmaChannels[bDmac][iChannel].state == DMAD_FREE )
            continue;
        else if ( pDmad->dmaChannels[bDmac][iChannel].state == DMAD_IN_XFR )
            continue;
        /* Log to start bit map */
        if (bmChannels & bmChBit)
        {
            bmChs |= bmChBit;
            bmIts |= (  (DMAC_EBCIDR_BTC0 << iChannel)
                       |(DMAC_EBCIDR_CBTC0 << iChannel)
                       |(DMAC_EBCIDR_ERR0 << iChannel) );
            /* Change state */
            pDmad->dmaChannels[bDmac][iChannel].state = DMAD_IN_XFR;
        }
    }

    DMAC_EnableChannels(pDmac, bmChs);
    if ( pDmad->pollingMode == 0 )
    {
        /* Monitor status in interrupt handler */
        DMAC_EnableIt( pDmac, bmIts );
    }

    return DMAD_OK;
}

/**
 * \brief Stop DMA transfer.
 * \param pDmad     Pointer to DMA driver instance.
 * \param _dwChannel ControllerNumber << 8 | ChannelNumber.
 */
eDmadRC DMAD_StopTransfer( sDmad *pDmad, unsigned int _dwChannel )
{
    unsigned char _iController = (_dwChannel >> 8);
    unsigned char iChannel    = (_dwChannel) & 0xFF;

    Dmac *pDmac = pDmad->pDmacs[_iController];
    sDmadChannel *pCh = &pDmad->dmaChannels[_iController][iChannel];

    unsigned int to = 0x1000;

    if ( pDmad->dmaChannels[_iController][iChannel].state == DMAD_FREE )
        return DMAD_ERROR;

    if ( pDmad->dmaChannels[_iController][iChannel].state != DMAD_IN_XFR )
        return DMAD_OK;

    /* Suspend */
    DMAC_SuspendChannel(pDmac, iChannel);

    /* Poll empty */
    for (;to; to --)
    {
        if (DMAC_GetChannelStatus(pDmac) & (DMAC_CHSR_EMPT0 << iChannel))
        {
            break;
        }
    }

    /* Disable channel */
    DMAC_DisableChannel(pDmac, iChannel);
    /* Disable interrupts */
    DMAC_DisableIt(pDmac, (DMAC_EBCIDR_BTC0 << iChannel)
                         |(DMAC_EBCIDR_CBTC0 << iChannel)
                         |(DMAC_EBCIDR_ERR0 << iChannel) );
    /* Clear pending status */
    DMAC_GetChannelStatus(pDmac);
    DMAC_GetStatus(pDmac);
    /* Resume */
    DMAC_RestoreChannel(pDmac, iChannel);
    /* Change state */
    pDmad->dmaChannels[_iController][iChannel].state = DMAD_IN_USE;
    /* Invoke callback */
    if (pCh->fCallback) pCh->fCallback(DMAD_CANCELED, pCh->pArg);
    return DMAD_OK;
}

/**
 * \brief Configure DMA for a single transfer.
 * \param pDmad     Pointer to DMA driver instance.
 * \param _dwChannel ControllerNumber << 8 | ChannelNumber.
 */
eDmadRC DMAD_PrepareSingleTransfer( sDmad *pDmad,
                                    unsigned int _dwChannel,
                                    sDmaTransferDescriptor *pXfrDesc )
{
    unsigned char _iController = (_dwChannel >> 8);
    unsigned char iChannel    = (_dwChannel) & 0xFF;
    Dmac *pDmac = pDmad->pDmacs[_iController];

    if ( pDmad->dmaChannels[_iController][iChannel].state == DMAD_FREE )
        return DMAD_ERROR;
    if ( pDmad->dmaChannels[_iController][iChannel].state == DMAD_IN_XFR )
        return DMAD_BUSY;

    DMAC_SetSourceAddr(pDmac, iChannel, pXfrDesc->dwSrcAddr);
    DMAC_SetDestinationAddr(pDmac, iChannel, pXfrDesc->dwDstAddr);
    DMAC_SetDescriptorAddr(pDmac, iChannel, 0, 0);
    DMAC_SetControlA(pDmac, iChannel, pXfrDesc->dwCtrlA);
    DMAC_SetControlB(pDmac, iChannel, pXfrDesc->dwCtrlB);

    return DMAD_OK;
}

/**
 * \brief Configure DMA multi-buffer transfers using linked lists
 * \param pDmad     Pointer to DMA driver instance.
 * \param _dwChannel ControllerNumber << 8 | ChannelNumber.
 * \param pXfrDesc  Pointer to DMA Linked List.
 */
eDmadRC DMAD_PrepareMultiTransfer( sDmad *pDmad,
                                   unsigned int _dwChannel,
                                   sDmaTransferDescriptor *pXfrDesc )
{
    unsigned char _iController = (_dwChannel >> 8);
    unsigned char iChannel    = (_dwChannel) & 0xFF;

    Dmac *pDmac = pDmad->pDmacs[_iController];

    if ( pDmad->dmaChannels[_iController][iChannel].state == DMAD_FREE )
        return DMAD_ERROR;
    if ( pDmad->dmaChannels[_iController][iChannel].state == DMAD_IN_XFR )
        return DMAD_BUSY;

    DMAC_SetDescriptorAddr( pDmac, iChannel, (unsigned int)pXfrDesc, 0 );
    DMAC_SetControlB( pDmac, iChannel, 0);

    return DMAD_OK;
}

/**@}*/

