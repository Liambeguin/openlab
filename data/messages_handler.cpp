#include "messages_handler.h"

#include <data/datastore.h>
#include <driver/ffprotocol/ffp_sender.h>

#include <singleton.h>

namespace Messages {


/** Macro to generate generic MessageHandler class implementation
 ** Here an example of the code generated

DRONOLAB_DECLARE_MESSAGE_HANDLER_IMPLEMENTATION(RcChannel); 

RcChannelHandler::RcChannelHandler
{
	Singleton::get()->RcChannelMessageEntry.handler = this;
}	
**/
#define DRONOLAB_DECLARE_MESSAGE_HANDLER_IMPLEMENTATION(_Name)  \
_Name##Handler::_Name##Handler() \
{ 	Singleton::get()->_Name##MessageEntry.handler = this; }    \


DRONOLAB_DECLARE_MESSAGE_HANDLER_IMPLEMENTATION(RcChannel);
void RcChannelHandler::serialize(FFPSender& sender, Snapshot& snapshot) {
	sender << Singleton::get()->RcChannelMessageEntry;
	sender << snapshot.get(DataStore::RC_THROTTLE);
	sender << snapshot.get(DataStore::RC_ROLL);
	sender << snapshot.get(DataStore::RC_PITCH);
	sender << snapshot.get(DataStore::RC_YAW);
	sender << snapshot.get(DataStore::RC_MANUAL);
	sender << snapshot.get(DataStore::RC_KILL);
	sender << endtrame;
}

DRONOLAB_DECLARE_MESSAGE_HANDLER_IMPLEMENTATION(UCommands);
void UCommandsHandler::serialize(FFPSender& sender, Snapshot& snapshot) {
	sender << Singleton::get()->UCommandsMessageEntry;
	sender << snapshot.get(DataStore::CMD_U1);
	sender << snapshot.get(DataStore::CMD_U2);
	sender << snapshot.get(DataStore::CMD_U3);
	sender << snapshot.get(DataStore::CMD_U4);
	sender << snapshot.get(DataStore::CMD_ROLL);
	sender << snapshot.get(DataStore::CMD_PITCH);
	
	sender << endtrame;
}

DRONOLAB_DECLARE_MESSAGE_HANDLER_IMPLEMENTATION(IMUData);
void IMUDataHandler::serialize(FFPSender& sender, Snapshot& snapshot) {
	sender << Singleton::get()->IMUDataMessageEntry;
	sender << snapshot.get(DataStore::ROLL);
	sender << snapshot.get(DataStore::PITCH);  
	sender << snapshot.get(DataStore::YAW);
	sender << snapshot.get(DataStore::DOT_ROLL);  
	sender << snapshot.get(DataStore::DOT_PITCH);
	sender << snapshot.get(DataStore::DOT_YAW);
	sender << snapshot.get(DataStore::ACCEL_X); 
	sender << snapshot.get(DataStore::ACCEL_Y);
	sender << snapshot.get(DataStore::ACCEL_Z);
	sender << endtrame;
	
}

//Should NOT BE CALLED BY THIS !!!!!!
DRONOLAB_DECLARE_MESSAGE_HANDLER_IMPLEMENTATION(EstimatedInsPosition);
void EstimatedInsPositionHandler::serialize(FFPSender& sender, Snapshot& snapshot) {
	sender << Singleton::get()->EstimatedInsPositionMessageEntry;
	
	sender << snapshot.get(DataStore::INS_ESTIMATED_X);
	sender << snapshot.get(DataStore::INS_ESTIMATED_Y);
	sender << snapshot.get(DataStore::INS_ESTIMATED_Z);
	sender << snapshot.get(DataStore::INS_ESTIMATED_VX);
	sender << snapshot.get(DataStore::INS_ESTIMATED_VY);
	sender << snapshot.get(DataStore::INS_ESTIMATED_VZ);

	sender << endtrame;
	
	//@todo Remove this after debug ....
	Singleton::get()->console << " HELL NO SENDING POSITION OF INS NOT IN THE PROCESS HELL !!!! \n\r";
	
}

DRONOLAB_DECLARE_MESSAGE_HANDLER_IMPLEMENTATION(MotorSpeed);
void MotorSpeedHandler::serialize(FFPSender& sender, Snapshot& snapshot) {
	sender << Singleton::get()->MotorSpeedMessageEntry;
	sender << snapshot.get(DataStore::M1);
	sender << snapshot.get(DataStore::M2);
	sender << snapshot.get(DataStore::M3);
	sender << snapshot.get(DataStore::M4);
	sender << endtrame;
}

DRONOLAB_DECLARE_MESSAGE_HANDLER_IMPLEMENTATION(Watchdog);
void WatchdogHandler::serialize(FFPSender& sender, Snapshot& snapshot) {
	sender << Singleton::get()->WatchdogMessageEntry << (char) (snapshot.get_watchdogs()) << endtrame;
}

DRONOLAB_DECLARE_MESSAGE_HANDLER_IMPLEMENTATION(BatteryStatus);
void BatteryStatusHandler::serialize(FFPSender& sender, Snapshot& snapshot) {
	// @todo
}

DRONOLAB_DECLARE_MESSAGE_HANDLER_IMPLEMENTATION(EchoPose);
void EchoPoseHandler::serialize(FFPSender& sender, Snapshot& snapshot) {
	sender << Singleton::get()->EchoPoseMessageEntry;
	sender << snapshot.get(DataStore::PC_CURRENT_X);
	sender << snapshot.get(DataStore::PC_CURRENT_Y);
	sender << snapshot.get(DataStore::PC_CURRENT_Z);
	sender << snapshot.get(DataStore::PC_CURRENT_YAW);
	
	sender << snapshot.get(DataStore::PC_TARGET_X);
	sender << snapshot.get(DataStore::PC_TARGET_Y);
	sender << snapshot.get(DataStore::PC_TARGET_Z);
	sender << snapshot.get(DataStore::PC_TARGET_YAW);
	
	sender << snapshot.get(DataStore::PC_MOTOR_ON);
	sender << snapshot.get(DataStore::PC_GRAB_ON);
	sender << snapshot.get(DataStore::PC_RELEASE_ON);
	
	sender << snapshot.get(DataStore::GAIN_K1);
	sender << snapshot.get(DataStore::GAIN_K2);
	sender << snapshot.get(DataStore::GAIN_LAMBDA);
	
	sender << snapshot.get(DataStore::GAIN_K1_P);
	sender << snapshot.get(DataStore::GAIN_K2_P);
	sender << snapshot.get(DataStore::GAIN_LAMBDA_P);
	
		
	
	sender << endtrame;
}


DRONOLAB_DECLARE_MESSAGE_HANDLER_IMPLEMENTATION(EchoInsCorrection);
void EchoInsCorrectionHandler::serialize(FFPSender& sender, Snapshot& snapshot) {
	sender << Singleton::get()->EchoInsCorrectionMessageEntry;
	
	sender << snapshot.get(DataStore::INS_CORRECTION_X);
	sender << snapshot.get(DataStore::INS_CORRECTION_Y);
	sender << snapshot.get(DataStore::INS_CORRECTION_Z);
	sender << snapshot.get(DataStore::INS_CORRECTION_VX);
	sender << snapshot.get(DataStore::INS_CORRECTION_VY);
	sender << snapshot.get(DataStore::INS_CORRECTION_VZ);
		
	sender << endtrame;
}


DRONOLAB_DECLARE_MESSAGE_HANDLER_IMPLEMENTATION(DebugFloat);
void DebugFloatHandler::serialize(FFPSender& sender, Snapshot& snapshot) {
		
	sender << Singleton::get()->DebugFloatMessageEntry;
	sender << snapshot.get(DataStore::DEBUG_0);
	sender << snapshot.get(DataStore::DEBUG_1);
	sender << snapshot.get(DataStore::DEBUG_2);
	sender << snapshot.get(DataStore::DEBUG_3);	
	sender << snapshot.get(DataStore::DEBUG_4);
	
	sender << endtrame;
}



DRONOLAB_DECLARE_MESSAGE_HANDLER_IMPLEMENTATION(DatastoreTimestamp);
void DatastoreTimestampHandler::serialize(FFPSender& sender, Snapshot& snapshot) {
	sender << Singleton::get()->DatastoreTimestampMessageEntry;
	
	sender << snapshot.time;
	sender << endtrame;
}



DRONOLAB_DECLARE_MESSAGE_HANDLER_IMPLEMENTATION(RcTaskMonitoring);
void RcTaskMonitoringHandler::serialize(FFPSender& sender, Snapshot& snapshot) {
	sender << Singleton::get()->RcTaskMonitoringMessageEntry;
	
	sender << snapshot.get(DataStore::RC_AVRG_EXEC_US);
	sender << snapshot.get(DataStore::RC_AVRG_OFFSET_US);
	sender << snapshot.get(DataStore::RC_AVRG_PERIOD_US);
	sender << endtrame;
}

DRONOLAB_DECLARE_MESSAGE_HANDLER_IMPLEMENTATION(ImuTaskMonitoring);
void ImuTaskMonitoringHandler::serialize(FFPSender& sender, Snapshot& snapshot) {
	sender << Singleton::get()->ImuTaskMonitoringMessageEntry;
	
	sender << snapshot.get(DataStore::IMU_AVRG_EXEC_US);
	sender << snapshot.get(DataStore::IMU_AVRG_OFFSET_US);
	sender << snapshot.get(DataStore::IMU_AVRG_PERIOD_US);	
	sender << snapshot.get(DataStore::IMU_AVRG_ANG_RECEP_US);
	sender << snapshot.get(DataStore::IMU_AVRG_ACC_RECEP_US);
	sender << snapshot.get(DataStore::IMU_SUM_RECEIVING_US);	
	sender << endtrame;
}

DRONOLAB_DECLARE_MESSAGE_HANDLER_IMPLEMENTATION(CtrlAttTaskMonitoring);
void CtrlAttTaskMonitoringHandler::serialize(FFPSender& sender, Snapshot& snapshot) {
	sender << Singleton::get()->CtrlAttTaskMonitoringMessageEntry;
	
	sender << snapshot.get(DataStore::CTRL_ATT_AVRG_EXEC_US);
	sender << snapshot.get(DataStore::CTRL_ATT_AVRG_OFFSET_US);
	sender << snapshot.get(DataStore::CTRL_ATT_AVRG_PERIOD_US);
	sender << endtrame;
}


DRONOLAB_DECLARE_MESSAGE_HANDLER_IMPLEMENTATION(CtrlPosTaskMonitoring);
void CtrlPosTaskMonitoringHandler::serialize(FFPSender& sender, Snapshot& snapshot) {
	sender << Singleton::get()->CtrlPosTaskMonitoringMessageEntry;
		
	sender << snapshot.get(DataStore::CTRL_POS_AVRG_EXEC_US);
	sender << snapshot.get(DataStore::CTRL_POS_AVRG_EXEC_US);
	sender << snapshot.get(DataStore::CTRL_POS_AVRG_EXEC_US);
	sender << endtrame;
}


DRONOLAB_DECLARE_MESSAGE_HANDLER_IMPLEMENTATION(ObcTaskMonitoring);
void ObcTaskMonitoringHandler::serialize(FFPSender& sender, Snapshot& snapshot) {
	sender << Singleton::get()->ObcTaskMonitoringMessageEntry;
	
	sender << snapshot.get(DataStore::OBC_AVRG_EXEC_US);
	sender << snapshot.get(DataStore::OBC_AVRG_OFFSET_US);
	sender << snapshot.get(DataStore::OBC_AVRG_PERIOD_US);
	sender << snapshot.get(DataStore::OBC_SUM_RECEIVING_US);
	sender << snapshot.get(DataStore::OBC_AVRG_SENDING_US);
	sender << endtrame;
}

/*
DRONOLAB_DECLARE_MESSAGE_HANDLER_IMPLEMENTATION(TelemetryTaskMonitoring);
void TelemetryTaskMonitoringHandler::serialize(FFPSender& sender, Snapshot& snapshot) {
	sender << Singleton::get()->TelemetryTaskMonitoringMessageEntry;
	
	sender << snapshot.get(DataStore::TEL_AVRG_EXEC_US);
	sender << snapshot.get(DataStore::TEL_AVRG_OFFSET_US);
	sender << snapshot.get(DataStore::TEL_AVRG_PERIOD_US);
	sender << endtrame;
}
*/

DRONOLAB_DECLARE_MESSAGE_HANDLER_IMPLEMENTATION(InsTaskMonitoring);
void InsTaskMonitoringHandler::serialize(FFPSender& sender, Snapshot& snapshot) {
	sender << Singleton::get()->InsTaskMonitoringMessageEntry;
	
	sender << snapshot.get(DataStore::INS_AVRG_EXEC_US);
	sender << snapshot.get(DataStore::INS_AVRG_EXEC_US);
	sender << snapshot.get(DataStore::INS_AVRG_EXEC_US);
	sender << endtrame;
}



DRONOLAB_DECLARE_MESSAGE_HANDLER_IMPLEMENTATION(FirmwareVersion);
void FirmwareVersionHandler::serialize(FFPSender& sender, Snapshot& snapshot) {
	sender << Singleton::get()->FirmwareVersionMessageEntry;
	
	sender << snapshot.get(DataStore::FIRMWARE_VERSION);

	sender << endtrame;
}




//@todo : Finish Parameters module !!!
/*
DRONOLAB_DECLARE_MESSAGE_HANDLER_IMPLEMENTATION(SendDefaultParameters);
void SendDefaultParametersHandler::serialize(FFPSender& sender, Snapshot& snapshot) {
	sender << Singleton::get()->SendDefaultParametersMessageEntry;
	for (int i = 0; i < Singleton::get()->parameters.LAST_ENUM_PARAM; i++)
		sender << Singleton::get()->parameters.default_.params[i];
	sender << endtrame;
}

DRONOLAB_DECLARE_MESSAGE_HANDLER_IMPLEMENTATION(SendLiveParameters);
void SendLiveParametersHandler::serialize(FFPSender& sender, Snapshot& snapshot) {
	sender << Singleton::get()->SendLiveParametersMessageEntry;
	for (int i = 0; i < Singleton::get()->parameters.LAST_ENUM_PARAM; i++)
		sender << Singleton::get()->parameters.live.params[i];
	sender << endtrame;
}

DRONOLAB_DECLARE_MESSAGE_HANDLER_IMPLEMENTATION(SendFlashParameters);
void SendFlashParametersHandler::serialize(FFPSender& sender, Snapshot& snapshot) {
	sender << Singleton::get()->SendFlashParametersMessageEntry;
	for(int i=0; i<Singleton::get()->parameters.LAST_ENUM_PARAM; i++)
		sender << Singleton::get()->parameters.flash_param_->params[i];
	sender << endtrame;
}

*/
#undef DRONOLAB_DECLARE_MESSAGE_HANDLER_IMPLEMENTATION

} // namespace Message
