/*******************************
 * Projet Cortex  -  Dronolab  *
 * http://dronolab.etsmtl.ca/  *
 *******************************/

/************************************************************************
	Le mechanisme des messages est detaille dans init_msg.h 
************************************************************************/

#ifndef MESSAGE_DEF_H
#define MESSAGE_DEF_H

#include <inttypes.h>

#include "messages_entry.h"

namespace Messages {


//Telemetry purpose LLC => Telemetry

/*! @class RcChannelMessage
	@ingroup MessageModule
    @brief Les Entry ajoutent un niveau de classement au message. Chaque Entry associe au message un type, un ID et une taille. 
	
	Voir Frederic Morin pour les details : frederic.morin.8@gmail.com
*/
class RcChannelMessage {
public:
	static const uint8_t ID = 0x01;	
	float throttle;
	float roll;
	float pitch;
	float yaw;
	float manual;
	float kill;
}__attribute__ ((packed ));

//Telemetry purpose LLC ->Telemetry
class UCommandsMessage {
	public:
	static const uint8_t ID = 0x02;
	float u1;
	float u2;
	float u3;
	float u4;
	float rollD;
	float pitchD;	
}__attribute__ ((packed ));


//Telemetry + OBC 
// LLC => Telemetry
// LLC => OBC
class IMUDataMessage {
	public:
	static const uint8_t ID = 0x03;
	float roll;
	float pitch;
	float yaw;
	float dot_roll;
	float dot_pitch;
	float dot_yaw;
	float acceleration_x;
	float acceleration_y;
	float acceleration_z;
}__attribute__ ((packed ));

//Telemetry
// LLC => Telemetry
class EstimatedInsPositionMessage {
	public:
	static const uint8_t ID = 0x04;
	float ins_estimated_x;
	float ins_estimated_y;
	float ins_estimated_z;
	float ins_estimated_vx;
	float ins_estimated_vy;
	float ins_estimated_vz;
}__attribute__ ((packed ));

//Telemetry 
// LLC => Telemetry
class MotorSpeedMessage {
	public:
	static const uint8_t ID = 0x05;
	float m1;
	float m2;
	float m3;
	float m4;
}__attribute__ ((packed ));

//Telemetry
// LLC => Telemetry
class WatchdogMessage {
	public:
	static const uint8_t ID = 0x07;
	uint8_t rc :1;
	uint8_t pc :1;
	uint8_t imu :1;
	uint8_t batt :1;
	uint8_t kill :1;
	uint8_t :3;
}__attribute__ ((packed ));

//Telemetry
// LLC => Telemetry
class BatteryStatusMessage {
	public:
	static const uint8_t ID = 0x08;
	uint8_t status;
	float consumed_mah;
	float current;
	float voltage;
}__attribute__ ((packed ));


// Echo de confirmation de réception des données
// LLC => Telemetry
class EchoPoseMessage {
	public:
	static const uint8_t ID = 0x09;

	float current_x;
	float current_y;
	float current_z;
	float current_yaw;

	float target_x;
	float target_y;
	float target_z;
	float target_yaw;
		
	float motor_on;
	float grab_on;
	float release_on;
	
	float k1;		//@todo Remove after
	float k2;		//@todo Remove after
	float lambda;	//@todo Remove after
	
	float k1_p;		//@todo Remove after
	float k2_p;		//@todo Remove after
	float lambda_p;	//@todo Remove after
	
	
	
}__attribute__ ((packed ));


// Echo de confirmation de réception des données
// LLC => Telemetry
class EchoInsCorrectionMessage {
	public:
	static const uint8_t ID = 0x10;

	float ins_correction_x;
	float ins_correction_y;
	float ins_corretion_z;
	
	float ins_correction_vx;
	float ins_correction_vy;
	float ins_correction_vz;
	
}__attribute__ ((packed ));



// LLC => Telemetry
class DebugFloatMessage {
	public:
	static const uint8_t ID = 0x20;
	
	float debug_0;
	float debug_1;
	float debug_2;
	float debug_3;
	float debug_4;


}__attribute__ ((packed ));


class InsCorrectionMessage {
	public:
	static const uint8_t ID = 0x40;
	
	float ins_correction_x; 
	float ins_correction_y;
	float ins_corretion_z;
	
	float ins_correction_vx;
	float ins_correction_vy;
	float ins_correction_vz;
	
}__attribute__ ((packed ));



// LLC => Telemetry
// LLC => OBC
class DatastoreTimestampMessage {
	public:
	static const uint8_t ID = 0x41;
	uint32_t ms;
}__attribute__ ((packed ));

// OBC => LLC
class PoseCommandMessage {
	public:
	static const uint8_t ID = 0x64;
	
	float current_x;
	float current_y;
	float current_z;
	float current_yaw;
	
	float target_x;
	float target_y;
	float target_z;
	float target_yaw;
		
	float motor_on;
	float grab_on;
	float release_on;
	
}__attribute__ ((packed ));


class FirmwareVersionMessage {
	public:
	static const uint8_t ID = 0x42;
	
	float version;

}__attribute__ ((packed ));


// LLC => Telemetry
class RcTaskMonitoringMessage {
	public:
	static const uint8_t ID = 0x80;
	
	uint32_t avrg_exec_us;
	uint32_t avrg_offset_us;
	uint32_t avrg_period_us;
	
}__attribute__ ((packed ));

// LLC => Telemetry
class ImuTaskMonitoringMessage {
	public:
	static const uint8_t ID = 0x81;
	
	uint32_t avrg_exec_us;
	uint32_t avrg_offset_us;
	uint32_t avrg_period_us;
	
	uint32_t avrg_ang_recep_us;
	uint32_t avrg_acc_recep_us;
	uint32_t sum_receiving_us;
	
	
}__attribute__ ((packed ));

// LLC => Telemetry
class CtrlPosTaskMonitoringMessage {
	public:
	static const uint8_t ID = 0x82;
	
	uint32_t avrg_exec_us;
	uint32_t avrg_offset_us;
	uint32_t avrg_period_us;
	
}__attribute__ ((packed ));

// LLC => Telemetry
class CtrlAttTaskMonitoringMessage {
	public:
	static const uint8_t ID = 0x83;
	
	uint32_t avrg_exec_us;
	uint32_t avrg_offset_us;
	uint32_t avrg_period_us;
	
}__attribute__ ((packed ));



// LLC => Telemetry
class ObcTaskMonitoringMessage {
	public:
	static const uint8_t ID = 0x84;
	
	uint32_t avrg_exec_us;
	uint32_t avrg_offset_us;
	uint32_t avrg_period_us;
	
	uint32_t sum_receiving_us;
	uint32_t avrg_sending_us;
	
	
}__attribute__ ((packed ));

/*
// LLC => Telemetry
class TelemetryTaskMonitoringMessage {
	public:
	static const uint8_t ID = 0x85;
	
	uint32_t avrg_exec_us;
	uint32_t avrg_offset_us;
	uint32_t avrg_period_us;
	
}__attribute__ ((packed ));
*/


// LLC => Telemetry
class InsTaskMonitoringMessage {
	public:
	static const uint8_t ID = 0x86;
	
	uint32_t avrg_exec_us;
	uint32_t avrg_offset_us;
	uint32_t avrg_period_us;
	
}__attribute__ ((packed ));



class GainMessage {
	public:
	static const uint8_t ID = 0x97;
	
	float k1;
	float k2;
	float lambda;
	
	float k1_p;
	float k2_p;
	float lambda_p;
	
}__attribute__ ((packed ));


//@todo : Finish Parameters module !!!
/*
// Telemetry => LLC
class SetParameterMessage {
	public:
	static const uint8_t ID = 0x90;
	uint8_t id;
	float value;
}__attribute__ ((packed ));

// Telemetry => LCC
class SaveParameterMessage {
	public:
	static const uint8_t ID = 0x91;
}__attribute__ ((packed ));

//Telemetry => LLC
class ClearParameterMessage {
	public:
	static const uint8_t ID = 0x92;
}__attribute__ ((packed ));

//Telemetry => LLC
class ResetParameterMessage {
	public:
	static const uint8_t ID = 0x93;	
}__attribute__ ((packed ));

//Telemetry => LLC
class SendDefaultParametersMessage {
	public:
	static const uint8_t ID = 0x94;
};

//Telemetry => LLC
class SendLiveParametersMessage {
	public:
	static const uint8_t ID = 0x95;
};

//Telemetry => LLC
class SendFlashParametersMessage {
	public:
	static const uint8_t ID = 0x96;
};

//Telemetry => LLC
*/

} // namespace Message

#endif // MSG_DEF_H
