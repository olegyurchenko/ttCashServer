/*----------------------------------------------------------------------------*/
/*"$Id: fm_errors.h,v 1.1 2004/04/21 12:19:50 oleg Exp $"*/
/**
* @pkg api_ERRORS
*/
/**
* Errors codes.
*
* Errors codes descriptions.
* (C) T&T, Kiev, Ukraine 2004.
* @pkgdoc api_ERRORS
* @author Oleg Yurchenko
* @version $Revision$
*/
/*----------------------------------------------------------------------------*/
#ifndef FM_ERRORS_H
#define FM_ERRORS_H

/**
* Common return type.
*
* Usualy, all fiscal module function returns 0 (FM_OK)if result Ok,
* or error code if error.
* Error cone contain 2 part: WHAT and WHERE
* ERROR = 1000 * WHERE + WHAT
*/
typedef int fmerr_t;
typedef int fmret_t;

#define FM_OK 0

#define FME_WHAT(err) ((err) % 1000)
#define FME_WHERE(err) ((err) / 1000)

#define FM_ERROR(what, where) ((what) + (FME_WHERE(what) ?  0 : (where) * 1000))

/*"WHAT" errors*/
#define FME_HARDWARE   1
#define FME_SOFTWARE   2
#define FME_OS         3
#define FME_INTERFACE  4

#define FME_RAM        5
#define FME_NV_RAM     6
#define FME_FLASH      7
#define FME_FISCAL     8
#define FME_EJ         9

#define FME_PRNERR     10
#define FME_PRNPAPER   11
#define FME_PRNBUSY    12
#define FME_POWER      13

#define FME_RTC        20 /*Real time clock error*/
#define FME_CD         21 /*Customer display error*/

#define FME_OVERFLOAT       100 /*Numeric overfloat*/
#define FME_ZERODIVIDE      101 /*Divide by zero*/
#define FME_INVALIDPASS     102 /*Invalid clerk password*/
#define FME_PERMDENIED      103 /*Permission disabled*/
#define FME_NOSUCHCLERK     104 /*Clerk not programmed*/
#define FME_NOSUCHPLU       105 /*PLU not programmed*/
#define FME_NOSUCHDEPT      106 /*DEPT not programmed*/
#define FME_VALUEOUTOFRANGE 107 /*Value out of range*/
#define FME_STRLENGTH       108 /*String length out of range*/
#define FME_NOSERIALNUM     109 /*No serial number in fiscal memory*/
#define FME_NOFISCALMODE    110 /*No fiscalisation data in fiscal memory*/
#define FME_NOSUCHSTORE     111 /*Store not found*/
#define FME_NOSUCHDISPENS   112 /*Pump not found*/
#define FME_NOSUCHNOZZLE    113 /*Nozzle not found*/
#define FME_NOTINIT         114 /*Data not initialized*/
#define FME_INVALIDPARAM    115 /*Invalid parameter*/
#define FME_SETTINGS        116 /*Invalid settings*/
#define FME_INPROGRESS      117 /*Job in progress*/
#define FME_ABORTED         118 /*Aborted job*/
#define FME_FUNCNOTFOUND    119 /*Function not found*/
#define FME_NOSUCHGROUP     120 /*Product group not programmed*/
#define FME_PACKAGEREF      121 /*Invalid package reference*/
#define FME_REMAINLESS      122 /*Amount greater then remain*/
#define FME_BASKETNOTFOUND  123 /*Basket item not found*/
#define FME_BASKETCOUNT     124 /*Basket count out of range*/
#define FME_DISPENSEACTIVE  125 /*Dispense active*/
#define FME_PAYMENTCOUNT    126 /*Payment count out of range*/
#define FME_TRANSCOUNT      127 /*Transaction count out of range*/
#define FME_DISCOUNT        128 /*Discount error*/
#define FME_TAXNOTFOUND     129 /*Tax group not found*/
#define FME_INVALIDPAYFORM  130 /*Invalid payform*/
#define FME_INVALIDFORMATCPEC 131 /* Invalid format specifier */
#define FME_INVALIDOP       132 /*Invalid operation*/
#define FME_INVALIDNUMBER   133/*Invalid number format*/
#define FME_CTRLERROR       134/*Controller error*/
#define FME_CTL_EMAXDRV     135 /*CTRL: Driver count out of range*/
#define FME_CTL_EMAXPUMP    136 /*CTRL: Dispenser count out of range*/
#define FME_CTL_ENODRV      137 /*CTRL: Invalid driver index*/
#define FME_CTL_ENOPUMP     138 /*CTRL: Invalid dispenser index*/
#define FME_CTL_ENODRVF     139 /*CTRL: Driver not found*/
#define FME_CTL_ENOCOM      140 /*CTRL: Error open COM*/
#define FME_CTL_EFAILED     141 /*CTRL: Driver error*/
#define FME_CTL_ENOCMD      142 /*CTRL: Invalid command*/
#define FME_CTL_ESYS        143 /*CTRL: System error*/
#define FME_MULTIPAYMENT    144 /*Multipayment error*/
#define FME_PUMPBUSY        145 /*Pump busy*/
#define FME_BASKETLOCKED    146 /*Basket is locked*/
#define FME_INVALID_PAYDOUT 147 /*Paydout greater then drawer sum*/
#define FME_NOSUCHEMITENT   148 /*Emitent not programmed*/
#define FME_FMDATA          149 /*Fiscal memory data corrupted*/
#define FME_NOBUSINESATTR   150 /*Business attributes not programmed*/
#define FME_MAXCLEANCOUNT   151 /*Maximum registers clean count reached*/
#define FME_INVALIDREGEXP   152 /*Invalid regular expression*/
#define FME_INVALIDSTRING   153 /*String not in trapharet*/
#define FME_INVALIDPAYRET   154 /*Invalid pay return operation*/
#define FME_STRINGNOTFOUND  155 /*String resource not found*/
#define FME_DATELESSFM      156 /*Date less then last Fiscal record*/
#define FME_PLUATTRCHANGE   157 /*Try to change PLU attributes*/
#define FME_SESSIONTIME     158 /*Session time riched*/
#define FME_OPDISABLED      159 /*Operation disabled before Z*/
#define FME_FMFULL          160 /*Fiscal memory full*/
#define FME_EJFULL          161 /*Electronic journal full*/
#define FME_PRN_POWER_OFF   162	//Printer power off
#define FME_PRN_SYSTEM			163	//Printer system error
#define FME_PRN_INIT				164	//Printer init error
#define FME_PRN_COVER				165	//Printer cover open
#define FME_PRN_OVERHEAT		166	//Jverheat of printhead
#define FME_PUMP_IS_OFFLINE 167 //Pump is offline
#define FME_PLU_DELETE      168 //PLU delete error
#define FME_STORE_DELETE    169 //Store delete error
#define FME_CONTRACTNOTFOUD 170 //Contract not found
#define FME_CONTRACTBALANCE 171 //Contract limit reached
#define FME_CONTRACTPLU     172 //Contract product incompatibility
#define FME_INVALIDEXCISE   173 //Invalid excise operation



#define FME_OPENDATA        200 /*Error open file*/
#define FME_WRITEDATA       201 /*Error write file*/
#define FME_READDATA        202 /*Error read file*/
#define FME_MALLOC          203 /*Memory allocate error*/
#define FME_OPENCOM         204 /*Error open COM*/
#define FME_MUTEX           205 /*Error create mutex*/
#define FME_THREAD          206 /*Error create thread*/

#define FME_INVALIDHANDLE   300 /*Invalid handle*/
#define FME_INVALIDANSWER   301 /*Invalid answer*/
#define FME_BUFFERTOOSMALL  302 /*Buffer too small*/
#define FME_STX_TIMEOUT     303 /*STX timeout*/
#define FME_HEAD_TIMEOUT    304 /*Packet head timeout*/
#define FME_DATA_TIMEOUT    305 /*Packet data timeout*/
#define FME_CRC_TIMEOUT     306 /*Packet CRC timeout*/
#define FME_CRC_ERROR       307 /*Packet CRC error*/
#define FME_ETX_TIMEOUT     308 /*Packet ETX timeout*/
#define FME_SIZE_ERROR      309 /*Packet size error*/
#define FME_NAK_RECEIVED    310 /*NAK received*/
#define FME_HEAD_ERROR      311 /*Packet head error*/
#define FME_PACK_ID         312 /*Invalid packet ID*/
#define FME_CONNECTION      313 /*FM Connection error*/

#define FME_MODEM                400 /*Fiscal modem error*/
#define FME_MDM_NOT_READY        401 //"Modem not ready"
#define FME_MDM_NOT_PERSONALISED 402 //"Modem not personalised"
#define FME_MDM_INVALID_FORMAT   403 //"Modem: invalid format"
#define FME_MDM_UNCNOWN_DOC_TYPE 404 //"Modem: invalid doc type"
#define FME_MDM_UNRNOWN_REC_TYPE 405 //"Modem: invalid record type"
#define FME_MDM_INVALID_DATE     406 //"Modem: invalid date or time"
#define FME_MDM_INVALID_DOC_ID   409 //"Modem: invalid doc id"
#define FME_MDM_INVALID_REC_NUM  410 //"Modem: Invalid record number";
#define FME_MDM_LOST_RECORD      411 //"Modem: lost record(s)"
#define FME_MDM_RECNUM_RICHED    412 //"Modem: record nom reached"
#define FME_MDM_INVALID_COMMAND  413 //"Modem: invalid commend"
#define FME_MDM_72H_ERROR        414 //"Modem: doc transmit pause riched 72h"

#define FME_MDM_COM_OPEN_ERROR  420 /*Modem: Invalid handle*/
#define FME_MDM_INVALIDANSWER   421 /*Modem: Invalid answer*/
#define FME_MDM_BUFFERTOOSMALL  422 /*Modem: Buffer too small*/
#define FME_MDM_STX_TIMEOUT     423 /*Modem: STX timeout*/
#define FME_MDM_HEAD_TIMEOUT    424 /*Modem: Packet head timeout*/
#define FME_MDM_DATA_TIMEOUT    425 /*Modem: Packet data timeout*/
#define FME_MDM_ACK_TIMEOUT     426 /*Modem: ACK timeout*/
#define FME_MDM_CRC_ERROR       427 /*Modem: Packet CRC error*/
#define FME_MDM_ETX_TIMEOUT     428 /*Modem: Packet ETX timeout*/
#define FME_MDM_SIZE_ERROR      429 /*Modem: Packet size error*/
#define FME_MDM_NAK_RECEIVED    430 /*Modem: NAK received*/
#define FME_MDM_HEAD_ERROR      431 /*Modem: Packet head error*/
#define FME_MDM_PACK_ID         432 /*Modem: Invalid packet ID*/
#define FME_MDM_COM_WRITE       433 /*Modem: Serial write error*/

#define	FME_KSEF_NO_PERSON          500	//	KSEF: Module not prsonalised
#define	FME_KSEF_SAM_NO_PERSON      501	//	KSEF: SAM-module not prsonalised
#define	FME_KSEF_NT_SAM_LIMIT       502	//	KSEF: NT_SAM limit
#define	FME_KSEF_SD_BAD_FORMAT      503	//	KSEF: SD card bad format
#define	FME_KSEF_SD_ERROR           504	//	KSEF: SD error
#define	FME_KSEF_SESSION_LIMIT      505	//	KSEF: Session limit
#define	FME_KSEF_MEMORY_FAULT				506	//	KSEF: Memory limit
#define	FME_KSEF_SAM_FAULT					507	//	KSEF: SAM fault
#define	FME_KSEF_RTC_FAULT					508	//	KSEF: RTC fault
#define	FME_KSEF_ETH_NO_CONNECT 		509	//	KSEF: ETH no connect
#define	FME_KSEF_ETH_WRITE_FAULT 		510	//	KSEF: ETH write fault
#define	FME_KSEF_ETH_READ_FAULT 		511	//	KSEF: ETH read fault
#define	FME_KSEF_ETH_RSPNS_FAULT 		512	//	KSEF: Response fault
#define	FME_KSEF_STR_DATE_FAULT 		513	//	KSEF: Invalid date
#define	FME_KSEF_SRV_STR_DATE_FAULT 514	//	KSEF: Server invalid date
#define	FME_KSEF_XML_FAULT 					515	//	KSEF: XML error
#define	FME_KSEF_SAM_NOT_RESPONSE		516	//	KSEF: SAM not response
#define	FME_KSEF_HASHHARD_FAULT			517	//	KSEF: HASH hard fault
#define	FME_KSEF_HASH_FAULT					518	//	KSEF: HASH fault
#define	FME_KSEF_PARAM_FAULT				519	//	KSEF: invalid param
#define	FME_KSEF_DPA_TLG_LENGTH    	520	//	KSEF: Telegramm length
#define	FME_KSEF_DPA_TLG_SEQUENCE  	521	//	KSEF: Telegramm sequence
#define	FME_KSEF_DPA_TLG_UNKNOWN   	522	//	KSEF: Telegramm number unknown
#define	FME_KSEF_DPA_TLG_VERSION   	523	//	KSEF: Telegramm version
#define	FME_KSEF_DPA_TLG_RRO_ID    	524	//	KSEF: Telegramm RRO ID
#define	FME_KSEF_DPA_TLG_SAM_ID   	525	//	KSEF: Telegramm SAM ID
#define	FME_KSEF_DPA_TLG_NT_SAM   	526	//	KSEF: Telegramm NT_SAM
#define	FME_KSEF_DPA_TLG_MAC      	527	//	KSEF: Telegramm MAC
#define	FME_KSEF_DPA_TLG_WTX      	528	//	KSEF: Telegramm WTX
#define	FME_KSEF_DPA_TLG_BREAK    	529	//	KSEF: Telegramm break
#define	FME_KSEF_DPA_FAULT	    		530	//	KSEF: DPA fault
#define	FME_KSEF_SRV_ERROR    			531	//	KSEF: server returned an error message
#define	FME_KSEF_SRV_NOT_CONFIRM    532	//	KSEF: server did not return a confirmation document processing
#define	FME_KSEF_OS_FAULT           533	//	KSEF: OS error
#define	FME_KSEF_DPA_XML_FAULT			534	//	KSEF: invalid XML from DPA server
#define	FME_KSEF_DPA_NEED_REPEAT_TELEGRAMM 535	//KSEF: Connecting session error
#define	FME_KSEF_DOC_LIMIT          536	  //KSEF: Disk limit reached
#define FME_KSEF_DPA_TLG_NEXT       537   //KSEF: EXC_NEXT server exception
#define FME_KSEF_IDDEV_ABSENT	     	538	  //KSEF: ID_DEV absent on KSEF SD
#define FME_KSEF_IDSAM_ABSENT	     	539	  //KSEF: SAM not initialised
#define FME_KSEF_NOT_EQUAL_IDSAM_IDDEV	540	//KSEF: ID_DEV not equal ID_SAM
#define FME_KSEF_SAM_INIT_FAULT			541	 //KSEF: SAM init fault
#define FME_KSEF_SAM_BUSY           542  //KSEF busy. There is a communication session with the DPA server.

#define FME_KSEF_DI_ERROR           550   //KSEF: Document ID sequency error

#define FME_KSEF_XML_PARSE  598         /*KSEF: XML parse error*/
#define FME_KSEF_ERROR      599         /*KSEF: KSEF error*/


#define	FME_SAM_NO_REGISTRATION      601	//	KSEF: SAM ANSWER no registration
#define	FME_SAM_NT_SAM_LIMIT         602	//	KSEF: SAM ANSWER NT limit
#define	FME_SAM_KEY_NOT_SUPPORT      603	//	KSEF: SAM ANSWER key not supported
#define	FME_SAM_SYS_KEY_NOT_SUPPORT	604	//	KSEF: SAM ANSWER sys key not supported
#define	FME_SAM_MAC_ERROR            605	//	KSEF: SAM ANSWER MAC error
#define	FME_SAM_ID_TERM_NOT_VALID    606	//	KSEF: SAM ANSWER ID term not valid
#define	FME_SAM_FILE_END             607	//	KSEF: SAM ANSWER file end
#define	FME_SAM_OK_REPEAT            608	//	KSEF: SAM ANSWER OK repeat
#define	FME_SAM_BAD_AUTH             609	//	KSEF: SAM ANSWER bad auth
#define	FME_SAM_BAD_CRC              610	//	KSEF: SAM ANSWER bad CRC
#define	FME_SAM_BAD_LENGTH           611	//	KSEF: SAM ANSWER bad length
#define	FME_SAM_COMMAND_NOT_ALLOWED	612	//	KSEF: SAM ANSWER command not allowed
#define	FME_SAM_COMMAND_NOT_SUPPORT	613	//	KSEF: SAM ANSWER command not support
#define	FME_SAM_ACCESS_DENIED        614	//	KSEF: SAM ANSWER access denied
#define	FME_SAM_BLOCK_AUTH_METHOD    615	//	KSEF: SAM ANSWER auth metod blocked
#define	FME_SAM_BLOCK_FILE           616	//	KSEF: SAM ANSWER file blocked
#define	FME_SAM_POOR_CONDITIONS      617	//	KSEF: SAM ANSWER poor conditions
#define	FME_SAM_NOT_SECURE_MESSAGING	618	//	KSEF: SAM ANSWER not ecure messaging
#define	FME_SAM_BAD_SECURE_MESSAGING	619	//	KSEF: SAM ANSWER bad secure messaging
#define	FME_SAM_BAD_PARAMS           620	//	KSEF: SAM ANSWER bad params
#define	FME_SAM_FUNC_NOT_SUPPORT     621	//	KSEF: SAM ANSWER function not supported
#define	FME_SAM_FILE_NOT_FOUND       622	//	KSEF: SAM ANSWER file not found
#define	FME_SAM_REC_NOT_FOUND        623	//	KSEF: SAM ANSWER record not found
#define	FME_SAM_BAD_FILE_SIZE        624	//	KSEF: SAM ANSWER bad file size
#define	FME_SAM_BAD_P1_P2            625	//	KSEF: SAM ANSWER bad P! P2
#define	FME_SAM_DATA_NOT_FOUND       626	//	KSEF: SAM ANSWER data not found
#define	FME_SAM_NOT_SUPPORT_P1_P2    627	//	KSEF: SAM ANSWER not support P1 P2
#define	FME_SAM_INS_NOT_SUPPORT      628	//	KSEF: SAM ANSWER INS not support
#define	FME_SAM_CLA_NOT_SUPPORT      629	//	KSEF: SAM ANSWER CLA not support
#define	FME_SAM_INCORRECT_COMMAND    630	//	KSEF: SAM ANSWER incorrect command

#define FME_CARD_ERROR               700 //Card error
#define FME_REFUND_CARD              701 //Error refund payment card
#define FME_NEED_REFUND_FIRST        702 //At first refund payment card

/*"WHERE" errors*/
#define FMW_UNKNOWN         0
#define FMW_BASKET          1  /*Basket*/
#define FMW_BILL            2  /*Bill*/
#define FMW_CD              3  /*Customer display*/
#define FMW_CLERK           4  /*Clerk*/
#define FMW_DISPENSER       5  /*Dispenser*/
#define FMW_EJ              6  /*Electronic journal*/
#define FMW_EMITENT         7  /*Emitent*/
#define FMW_FINDATA         8  /*Fin data*/
#define FMW_FISCAL          9  /*Fiscal*/
#define FMW_GOODS           10 /*Goods*/
#define FMW_INIT            11 /*Init*/
#define FMW_PAYFORM         12 /*Pay forv*/
#define FMW_PROFILE         13 /*Profile*/
#define FMW_REPORT          14 /*Report*/
#define FMW_STORE           15 /*Store*/
#define FMW_TAX             16 /*Tax*/
#define FMW_TIME            17 /*Time*/
#define FMW_VARS            18 /*Vars*/
#define FMW_DISCOUNT        19 /*Discount*/
#define FMW_DOC             20 /*Doc*/
#define FMW_PAYMENT         21 /*Payment*/
#define FMW_PRINT           22 /*Print*/
#define FMW_API             23 /*API*/
#define FMW_CLIENT          24 /*Client*/
#define FMW_SERVER          25 /*Server*/
#define FMW_PSF             26 /*PSF*/
#define FMW_MODEM           27 /*Modem*/
#define FMW_KSEF            28 /*KSEF*/
#define FMW_KSEF_XML        29 /*KSEF_XML*/
#define FMW_KSEF_API        30 /*KSEF_API*/
#define FMW_NET             31 /*NET*/
#define FMW_VERIFONE        32 /*VERIFONE*/
#define FMW_BARCODE         33 /*BARCODE*/
#define FMW_CONTRACT        34 /*CONTACT*/



#ifdef __cplusplus
extern "C"
{
#endif  /*__cplusplus*/
  char *fm_str_error(fmret_t err);
  char *fm_str_where(fmret_t err);
#ifdef __cplusplus
};
#endif  /*__cplusplus*/

#endif /*FM_ERRORS_H*/
/*----------------------------------------------------------------------------*/

