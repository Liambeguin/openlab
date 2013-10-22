/*******************************
 * Projet Cortex  -  Dronolab  *
 * http://dronolab.etsmtl.ca/  *
 *******************************/

/************************************************************************
	Le mechanisme des messages est detaille dans init_msg.h 
************************************************************************/

#ifndef MESSAGE_HANDLER_H
#define MESSAGE_HANDLER_H

#include "messages_def.h"


class Snapshot;
class FFPSender;


namespace Messages {


/*! @class MessageHandler
	@ingroup MessageModule
    @brief Classe qui gere la transmission des messages.

	Chacun des messages connaient son contenu et donc la maniere dont il devrait etre envoyer. Chaque message implemente donc son propre messageHanlder. 
	Ceux-ci sont instancies dans MsgInit. Chaque messageHandler utilise la fonction serialize qui est generique pour envoyer les messages. 
	Le protocole de communication utilise le handler n'a qu'a appele cette fonction.
	
	Voir Frederic Morin pour les details : frederic.morin.8@gmail.com
*/

class MessageHandler
{
protected:
	MessageHandler() {}
	
public:
	/*! 
		Generic fonction to send message
		@param senser FPPSenser instance used to send data
		@param snaphot DataStore Snapshot were the data is currently available
		
	*/
	virtual void serialize(FFPSender& sender, Snapshot& snapshot) = 0;
};

/** Macro to generate generic MessageHandler class implementation
 ** Here an example of the code generated

DRONOLAB_DECLARE_MESSAGE_HANDLER_DECLARATION(RcChannel); 

class RcChannelHandler : public MessageHandler
{
public:
	RcChannelHandler();
	virtual void serialize(FFPSender& sender, Snapshot& snapshot);
};	
**/

#define DRONOLAB_DECLARE_MESSAGE_HANDLER_DECLARATION(_Name) \
class _Name##Handler : public MessageHandler \
{ \
public: \
	_Name##Handler();  \
	virtual void serialize(FFPSender& sender, Snapshot& snapshot); \
}; \

DRONOLAB_DECLARE_MESSAGE_HANDLER_DECLARATION(RcChannel)
DRONOLAB_DECLARE_MESSAGE_HANDLER_DECLARATION(UCommands)
DRONOLAB_DECLARE_MESSAGE_HANDLER_DECLARATION(IMUData)
DRONOLAB_DECLARE_MESSAGE_HANDLER_DECLARATION(EstimatedInsPosition)
DRONOLAB_DECLARE_MESSAGE_HANDLER_DECLARATION(MotorSpeed)
DRONOLAB_DECLARE_MESSAGE_HANDLER_DECLARATION(Watchdog)
DRONOLAB_DECLARE_MESSAGE_HANDLER_DECLARATION(BatteryStatus)
DRONOLAB_DECLARE_MESSAGE_HANDLER_DECLARATION(EchoPose)
DRONOLAB_DECLARE_MESSAGE_HANDLER_DECLARATION(DebugFloat)
DRONOLAB_DECLARE_MESSAGE_HANDLER_DECLARATION(DatastoreTimestamp)
DRONOLAB_DECLARE_MESSAGE_HANDLER_DECLARATION(FirmwareVersion)

DRONOLAB_DECLARE_MESSAGE_HANDLER_DECLARATION(EchoInsCorrection)

DRONOLAB_DECLARE_MESSAGE_HANDLER_DECLARATION(RcTaskMonitoring)
DRONOLAB_DECLARE_MESSAGE_HANDLER_DECLARATION(ImuTaskMonitoring)
DRONOLAB_DECLARE_MESSAGE_HANDLER_DECLARATION(CtrlPosTaskMonitoring)
DRONOLAB_DECLARE_MESSAGE_HANDLER_DECLARATION(CtrlAttTaskMonitoring)
DRONOLAB_DECLARE_MESSAGE_HANDLER_DECLARATION(ObcTaskMonitoring)
//DRONOLAB_DECLARE_MESSAGE_HANDLER_DECLARATION(TelemetryTaskMonitoring)
DRONOLAB_DECLARE_MESSAGE_HANDLER_DECLARATION(InsTaskMonitoring)


//@todo : Finish Parameters module !!!
/*
DRONOLAB_DECLARE_MESSAGE_HANDLER_DECLARATION(SendDefaultParameters);
DRONOLAB_DECLARE_MESSAGE_HANDLER_DECLARATION(SendLiveParameters);
DRONOLAB_DECLARE_MESSAGE_HANDLER_DECLARATION(SendFlashParameters);
*/
//
	
	
#undef DRONOLAB_DECLARE_MESSAGE_HANDLER_DECLARATION
	 
} // namespace Message




#endif // MSG_DEF_H
