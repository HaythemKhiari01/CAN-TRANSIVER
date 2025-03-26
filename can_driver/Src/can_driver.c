#include "can_driver.h"


#define GPIOBEN (1U<<1)
#define CAN_AF 0x09

void can_gpio_init(void) {
	/*1 Enable Clock Access to GPIOB*/
	RCC->AHB1ENR |= GPIOBEN;
	/*2 Set PB8 and PB9 to alternate function mode*/
	GPIOB->MODER &=~(1U<<16);
	GPIOB->MODER |=(1U<<17);

	GPIOB->MODER &=~(1U<<18);
	GPIOB->MODER |=(1U<<19);
	/*3 SET PB8 and PB9 alternate function  to CAN1 RX and TX*/
	GPIOB->AFR[1] = (CAN_AF<<0);
	GPIOB->AFR[1]=  (CAN_AF<<4);
	/*4 Enable CAN RX0 Interrupt for message reception*/
	NVIC_EnableIRQ(CAN1_RX0_IRQn);
}

void can_parms_init(uint8_t mode){
	/*1 Enable Clock Access to CAN1*/
	RCC->APB1ENR |= RCC_APB1ENR_CAN1EN;
	/*2 Enter Initialization mode*/
	CAN1->MCR |= CAN_MCR_INRQ;
	/*3 Wait until CAN1 is in Initialization mode*/
	while (!(CAN1->MSR & CAN_MSR_INAK)) {}
	/*4 Exit Sleep Mode*/
	CAN1->MCR &=~ CAN_MCR_SLEEP;
	/*5 Wait until CAN1 is OUT of Sleep Mode*/
	while ((CAN1->MSR & CAN_MSR_SLAK)) {}
	/*5 Configure timing parameters including baudrate by configuring time segment 1 and 2 and prescaler*/
	CAN1->BTR = (1<<16) | (0<< CAN_BTR_TS1_Pos) | (9 << CAN_BTR_BRP_Pos);
	/*6 Select Mode*/
	if (mode) {
		/*Normal Mode*/
		CAN1->BTR &=~(1<<30);
	}
	else {
		//loopBack mode
		CAN1->BTR |=(1<<30);
	}
}

void can_start(void) {
	/*1 EXIT Initialization mode*/
		CAN1->MCR &=~CAN_MCR_INRQ;
	/*2 Wait until CAN1 is OUT Initialization mode*/
		while ((CAN1->MSR & CAN_MSR_INAK)) {}
	/*3 Enable Interrupt for FIFO message pending*/
		CAN1->IER |=(1<<1);
}

/*Function to Get an CAN frame from the Rx FIFO zone into the message RAM.*/

uint8_t can_add_tx_message(can_tx_header_typedef *pHeader, uint8_t aData[], uint32_t *pTxMailbox){
    // 1 Variable to hold the selected transmit mailbox
	 uint32_t transmitmailbox;
	// 2 Read the Transmit Status Register
	 uint32_t tsr = READ_REG(CAN1->TSR);
	// 3 Check that at least one Tx mailbox is empty
	 if (((tsr & CAN_TSR_TME0)!=0)||
		((tsr & CAN_TSR_TME1)!=0)||
		((tsr & CAN_TSR_TME2)!=0)) {
	// 1 Select an empty transmit mailbox
		 transmitmailbox = (tsr & CAN_TSR_CODE) >> CAN_TSR_CODE_Pos;
	// 2 Check transmitmailbox validity */
		 if (transmitmailbox > 2U)
		   {
		      return 1; // Invalid mailbox selected
		       }
    // 3 Store TX mailbox
		 *pTxMailbox = (uint32_t)1 << transmitmailbox;
	// 4 Set up the ID
		 if (pHeader->ide == CAN_ID_STD) {
		CAN1->sTxMailBox[transmitmailbox].TIR =((pHeader->std_id << CAN_TI0R_STID_Pos)|
                                                 pHeader->rtr);
		 }
		 else {
		CAN1->sTxMailBox[transmitmailbox].TIR = ((pHeader->ext_id << CAN_TI0R_EXID_Pos) |
			                                                            pHeader->ide    |
			                                                            pHeader->rtr);
		 }
	// 5 Setup the DLC
		CAN1->sTxMailBox[transmitmailbox].TDTR = (pHeader->dlc);
	// 6 Setup the Transmit Global Time Mode
		if (pHeader->transmit_global_time == 1)
		      {
		        SET_BIT(CAN1->sTxMailBox[transmitmailbox].TDTR, CAN_TDT0R_TGT);
		      }
	//7 Set up the DATA Filed
		WRITE_REG(CAN1->sTxMailBox[transmitmailbox].TDHR,
		                ((uint32_t)aData[7] << CAN_TDH0R_DATA7_Pos) |
		                ((uint32_t)aData[6] << CAN_TDH0R_DATA6_Pos) |
		                ((uint32_t)aData[5] << CAN_TDH0R_DATA5_Pos) |
		                ((uint32_t)aData[4] << CAN_TDH0R_DATA4_Pos));

	    WRITE_REG(CAN1->sTxMailBox[transmitmailbox].TDLR,
		                ((uint32_t)aData[3] << CAN_TDL0R_DATA3_Pos) |
		                ((uint32_t)aData[2] << CAN_TDL0R_DATA2_Pos) |
		                ((uint32_t)aData[1] << CAN_TDL0R_DATA1_Pos) |
		                ((uint32_t)aData[0] << CAN_TDL0R_DATA0_Pos));

	 // 8 Request Transmission
	    SET_BIT(CAN1->sTxMailBox[transmitmailbox].TIR, CAN_TI0R_TXRQ);

	 //9 Return Function Status
	    return 0;
	 }
	    return 0;
}

// Get a CAN Frame from the RX Fifo Zone into message to RAM
uint8_t can_get_rx_message(uint32_t RxFifo, can_rx_header_typedef *pHeader, uint8_t aData[])
{
	// 1 Check the RX FIFO
	if (RxFifo == CAN_RX_FIFO0) // Rx element is assigned to Rx FIFO 0
	    {
	      /* Check that the Rx FIFO 0 is not empty */
	      if ((CAN1->RF0R & CAN_RF0R_FMP0) == 0U)
	      {
	        return 1;
	      }
	    }
	    else /* Rx element is assigned to Rx FIFO 1 */
	    {
	      /* Check that the Rx FIFO 1 is not empty */
	      if ((CAN1->RF1R & CAN_RF1R_FMP1) == 0U)
	      {
	        return 1;
	      }
	    }
	// Read the header information from the FIFO mailbox
	// Extract identifier, DLC, timestamp, etc.
	pHeader->ide = CAN_RI0R_IDE & CAN1->sFIFOMailBox[RxFifo].RIR;
	    if (pHeader->ide == CAN_ID_STD)
	    {
	      pHeader->std_id = (CAN_RI0R_STID & CAN1->sFIFOMailBox[RxFifo].RIR) >> CAN_TI0R_STID_Pos;
	    }
	    else
	    {
	      pHeader->ext_id = ((CAN_RI0R_EXID | CAN_RI0R_STID) & CAN1->sFIFOMailBox[RxFifo].RIR) >> CAN_RI0R_EXID_Pos;
	    }
	    pHeader->rtr = (CAN_RI0R_RTR & CAN1->sFIFOMailBox[RxFifo].RIR);
	    pHeader->dlc = (CAN_RDT0R_DLC & CAN1->sFIFOMailBox[RxFifo].RDTR) >> CAN_RDT0R_DLC_Pos;
	    pHeader->filter_match_index = (CAN_RDT0R_FMI & CAN1->sFIFOMailBox[RxFifo].RDTR) >> CAN_RDT0R_FMI_Pos;
	    pHeader->timestamp = (CAN_RDT0R_TIME & CAN1->sFIFOMailBox[RxFifo].RDTR) >> CAN_RDT0R_TIME_Pos;

	    // Extract the data from the FIFO mailbox
	    aData[0] = (uint8_t)((CAN_RDL0R_DATA0 & CAN1->sFIFOMailBox[RxFifo].RDLR) >> CAN_RDL0R_DATA0_Pos);
	    aData[1] = (uint8_t)((CAN_RDL0R_DATA1 & CAN1->sFIFOMailBox[RxFifo].RDLR) >> CAN_RDL0R_DATA1_Pos);
	    aData[2] = (uint8_t)((CAN_RDL0R_DATA2 & CAN1->sFIFOMailBox[RxFifo].RDLR) >> CAN_RDL0R_DATA2_Pos);
	    aData[3] = (uint8_t)((CAN_RDL0R_DATA3 & CAN1->sFIFOMailBox[RxFifo].RDLR) >> CAN_RDL0R_DATA3_Pos);
	    aData[4] = (uint8_t)((CAN_RDH0R_DATA4 & CAN1->sFIFOMailBox[RxFifo].RDHR) >> CAN_RDH0R_DATA4_Pos);
	    aData[5] = (uint8_t)((CAN_RDH0R_DATA5 & CAN1->sFIFOMailBox[RxFifo].RDHR) >> CAN_RDH0R_DATA5_Pos);
	    aData[6] = (uint8_t)((CAN_RDH0R_DATA6 & CAN1->sFIFOMailBox[RxFifo].RDHR) >> CAN_RDH0R_DATA6_Pos);
	    aData[7] = (uint8_t)((CAN_RDH0R_DATA7 & CAN1->sFIFOMailBox[RxFifo].RDHR) >> CAN_RDH0R_DATA7_Pos);

	    /* Release the FIFO */
	    if (RxFifo == CAN_RX_FIFO0) /* Rx element is assigned to Rx FIFO 0 */
	    {
	      /* Release RX FIFO 0 */
	      SET_BIT(CAN1->RF0R, CAN_RF0R_RFOM0);
	    }
	    else /* Rx element is assigned to Rx FIFO 1 */
	    {
	      /* Release RX FIFO 1 */
	      SET_BIT(CAN1->RF1R, CAN_RF1R_RFOM1);
	    }



	  return 0;// Message read successfully

	}


void can_filter_config(uint16_t std_id)
{
	//Set filter intiailization mode
	CAN1->FMR |= CAN_FMR_FINIT ;
	//Set the slave filter to start from 20
	/*****Filter activation sequence****/
	//Deactive filter 18
	CAN1->FA1R &=~(CAN_FA1R_FACT18);
	//Set 32-bit scale configuration
	CAN1->FS1R |= (CAN_FS1R_FSC18);
	//Configure filter mode to identifier mask mode
	CAN1->FM1R &=~(CAN_FM1R_FBM18);
	//Set the identifier
	CAN1->sFilterRegister[18].FR1 = (std_id << 21);
	//Set the identifier Mask
	CAN1->sFilterRegister[18].FR2 = (std_id << 21);
	//Assign filter 18 to FIFO0
	CAN1->FFA1R &=~(CAN_FFA1R_FFA18);
	//Activate filter 18
	CAN1->FA1R |=(CAN_FA1R_FACT18);
	// Clear Filter initialisation mode
	CAN1->FMR &=~ CAN_FMR_FINIT ;

}


