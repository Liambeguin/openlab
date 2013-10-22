/*******************************
 * Projet Cortex  -  Dronolab  *
 * http://dronolab.etsmtl.ca/  *
 *******************************/

#ifndef DATASTORE_H
#define DATASTORE_H

#include <stdint.h>
#include <string.h> // memcpy

//Framework
#include <asf.h>

class Snapshot;

/*! @class DataStore	
	@brief Module qui gere toutes les donnes du systeme.
	
	Cette classe contient un grand tableau qui contient toutes les donnes pertinentes du systeme, dont principalement les mesures des capteurs ainsi que le calcul du controleur.
	L'acces est effectue par setter/getter, sans aucune protection d'acces. Le module offre aussi la possibilite de creer un clone avec la classe Snapshot.
	
	
	@defgroup DataStoreModule Driver - DataStore
	@brief Module qui gere toutes les donnes du systeme.
	*/
class DataStore
{
	friend class Snapshot;
public:
	//! Watchdog Key 
	enum bparam_e {//
		//
		RC_ALIVE 	 = 1 << 0, //!< 0 : No Rc detected = KILLSWITCH ON , 1 : Rc detected 
		PC_ALIVE 	 = 1 << 1, //!< 0 : No msg received from the OBC = KILLSWITCH ON , 1 : OBC msg received
		IMU_ALIVE 	 = 1 << 2, //!< 0 : No msg received form the IMU = KILLSWITCH ON , 1 : IMU msg received
		BATT_OK 	 = 1 << 3, //!< @todo Implement the power management module
		KILL_ENABLED = 1 << 4, //!< 1 : Killswtich ENABLE, control is not activated, motor are desactivated
		MANUAL_MODE  = 1 << 5, //!< 1 : manual mode ENABLE , 0 : autonomous mode ENABLE 
		IDLE		 = 1 << 6, //!< 1 : idle mode ENABLE, control is not activated, motor turns at minimal speed
		KALMAN_ALIVE = 1 << 7, //!< 1 : receiving INS correction form Kalman filter 
	};
	
	//! Task Monitoring DataStore Key
	enum uparam_e {
			
		//Task RC
		RC_AVRG_EXEC_US,			//!< temps moyen d'execution de la tache RC [us] 
		RC_AVRG_OFFSET_US,			//!< decalage moyen de la tache RC [us] 
		RC_AVRG_PERIOD_US,			//!< periode d'execution de la tache RC [us] 
			
		//Task IMU
		IMU_AVRG_EXEC_US,			//!< temps moyen d'execution de la tache IMU [us] 
		IMU_AVRG_OFFSET_US,			//!< decalage moyen de la tache IMU [us] 
		IMU_AVRG_PERIOD_US,			//!< periode d'execution de la tache IMU [us] 
		
		IMU_AVRG_ANG_RECEP_US,		//!< temps moyen de reception d'une position angulaire IMU [us] 
		IMU_AVRG_ACC_RECEP_US,		//!< temps moyen de reception des accelerations angulaire IMU [us] 
		IMU_SUM_RECEIVING_US,		//!< somme de temps passe a recevoir IMU [us] 

			
		//Task CTRL POS
		CTRL_POS_AVRG_EXEC_US,		//!< temps moyen d'execution de la tache Ctrl_pos [us] 
		CTRL_POS_AVRG_OFFSET_US,	//!< decalage moyen de la tache Ctrl_pos [us] 
		CTRL_POS_AVRG_PERIOD_US,	//!< periode d'execution de la tache Ctrl_pos [us] 
			
		//Task CTRL ATT
		CTRL_ATT_AVRG_EXEC_US,		//!< [us] temps moyen d'execution de la tache Ctrl_att
		CTRL_ATT_AVRG_OFFSET_US,	//!< [us] decalage moyen de la tache Ctrl_att
		CTRL_ATT_AVRG_PERIOD_US,	//!< [us] periode d'execution de la tache Ctrl_att
			
			
		//Task OBC
		OBC_AVRG_EXEC_US,			//!< temps moyen d'execution de la tache OBC [us] 
		OBC_AVRG_OFFSET_US,			//!< decalage moyen de la tache OBC [us] 
		OBC_AVRG_PERIOD_US,			//!< periode d'execution de la tache OBC [us] 
		
		OBC_SUM_RECEIVING_US,		//!< somme de temps passe a recevoir OBC [us] 
		OBC_AVRG_SENDING_US,		//!< moyenne de temps passe a envoye des donnes OBC [us] 
		
		/*	
		//Task TELEMETRY
		TEL_AVRG_EXEC_US,
		TEL_AVRG_OFFSET_US,
		TEL_AVRG_PERIOD_US,	
		*/
		
		INS_AVRG_EXEC_US,			//!< temps moyen d'execution de la tache RC [us] 
		INS_AVRG_OFFSET_US,			//!< decalage moyen de la tache RC [us] 
		INS_AVRG_PERIOD_US,			//!< periode d'execution de la tache RC [us] 
		
		UPARAM_COUNT		
	};
	

	/** Différentes données flottantes disponibles **/
	enum fparam_e {
		ROLL, 				//!< position angulaire autours de l'axe x [rad] 
		PITCH, 				//!< position angulaire autours de l'axe y [rad] 
		YAW, 				//!< position angulaire autours de l'axe z [rad] 
		DOT_ROLL, 			//!< acceleration angulaire dans le repere du quadrotor : p [rad/sec] 
		DOT_PITCH, 			//!< acceleration angulaire dans le repere du quadrotor : q [rad/sec] 
		DOT_YAW, 			//!< acceleration angulaire dans le repere du quadrotor : r [rad/sec] 
				
		ACCEL_X,			//!<  acceleration dans le repere du quadrotor : u [m/s^2] 
		ACCEL_Y, 			//!<  acceleration dans le repere du quadrotor : v [m/s^2] 
		ACCEL_Z, 			//!<  acceleration dans le repere du quadrotor : w [m/s^2] 
		
		GYRO_BIAS_X,		//!<  bias du gyroscope x [rad/sec] 
		GYRO_BIAS_Y,		//!<  bias du gyroscope y [rad/sec] 
		GYRO_BIAS_Z,		//!<  bias du gyroscope z [rad/sec] 
		
		ACCEL_BIAS_X,		//!<  bias du accelerometre x [* G_GX3 = m/s^2] 
		ACCEL_BIAS_Y,		//!<  bias du accelerometre y [* G_GX3 = m/s^2] 
		ACCEL_BIAS_Z,		//!<  bias du accelerometre z [* G_GX3 = m/s^2] 
		
		RC_THROTTLE,		//!<  [0,1]  * Singleton::get->parameters.getParameters::RC_FACTOR_THRUST - 0.15
		RC_PITCH,			//!<  [-1,1] * Singleton::get->parameters.getParameters::RC_FACTOR_ROLLPITCH [rad]
		RC_ROLL,			//!<  [-1,1] * Singleton::get->parameters.getParameters::RC_FACTOR_ROLLPITCH [rad]
		RC_YAW,				//!<  [-1,1] * Singleton::get->parameters.getParameters::RC_FACTOR_YAW [rad]
		RC_MANUAL,			//!<  [-1,1] , > 0.5 = Manual
		RC_KILL,			//!<  [-1,1] , < 0.5 = KILL
		
		CMD_U1,				//!< Total thrust input	[N] 
		CMD_U2,				//!< Total roll torque input [Nm]   
		CMD_U3,			    //!< Total pitch torque input  [Nm]
		CMD_U4,             //!< Total yaw torque input    [Nm]
		CMD_ROLL,			//!< Roll  desired calculated by position ctrl  [rad]
		CMD_PITCH,			//!< Pitch desired calculated by position ctrl	[rad] 
		
		M1,					//!< vitesse calcule de rotation du motor 1 [rad/s] 
		M2,					//!< vitesse calcule de rotation du motor 2 [rad/s] 
		M3,					//!< vitesse calcule de rotation du motor 3 [rad/s] 
		M4,					//!< vitesse calcule de rotation du motor 4 [rad/s] 
		OMEGA_R,			//!< vitesse residuelle calcule des moteurs M1-M2+M3-M4 [rad/s] 
		
		PC_CURRENT_X,		//!< position x du quadrotor estime par le systeme de positionnement [m]	
		PC_CURRENT_Y,		//!< position y du quadrotor estime par le systeme de positionnement [m]	
		PC_CURRENT_Z, 		//!< position z du quadrotor estime par le systeme de positionnement [m]	
		PC_CURRENT_YAW,		//!< cap du quadrotor estime par le systeme de positionnement [rad]	
		PC_TARGET_X,		//!< valeur desire en roll [rad]   
		PC_TARGET_Y,		//!< valeur desire en pitch [rad]   
		PC_TARGET_Z,		//!< valeur desire d'altitude  ATTENTION, l'axe Z pointe vers le bas [m]     
		PC_TARGET_YAW,		//!< valeur desire en yaw [rad]   
		PC_MOTOR_ON,		//!< 0> : Ready to fly, 0 : Idle Motor are activated, and no control, 0< : Software KillSwtich
		PC_GRAB_ON,			//!< @todo Add this functionability
		PC_RELEASE_ON,		//!< @todo Add this functionability
		
		INS_ESTIMATED_X,	//!< Best position estimate [m] 
		INS_ESTIMATED_Y,	//!< Best position estimate [m]
		INS_ESTIMATED_Z,	//!< Best position estimate [m]
		INS_ESTIMATED_VX,	//!< Best position velocity estimate [m/s] 
		INS_ESTIMATED_VY,	//!< Best position velocity estimate [m/s]
		INS_ESTIMATED_VZ,	//!< Best position velocity estimate [m/s]	
		
		INS_CORRECTION_X,	//!<  Integral correction from Kalman [m]
		INS_CORRECTION_Y,	//!<  Integral correction from Kalman [m]
		INS_CORRECTION_Z,	//!<  Integral correction from Kalman [m]
		INS_CORRECTION_VX,	//!<  Integral correction from Kalman [m/s]
		INS_CORRECTION_VY,	//!<  Integral correction from Kalman [m/s]
		INS_CORRECTION_VZ,	//!<  Integral correction from Kalman [m/s]
			
		
		BATTERY_CONSUMED_MAH, //!<  @todo Add functionality
		BATTERY_CURRENT,	  //!<  @todo Add functionality
		BATTERY_VOLTAGE,	  //!<  @todo Add functionality
			
		DEBUG_0,			  //!<  Debug purpose	
		DEBUG_1,			  //!<  Debug purpose
		DEBUG_2,			  //!<  Debug purpose
		DEBUG_3,			  //!<  Debug purpose	
		DEBUG_4,			  //!<  Debug purpose
										
		FIRMWARE_VERSION,	//!< Contains the firmware version of the system

		GAIN_K1,				//!< @todo Remove after
		GAIN_K2,				//!< @todo Remove after
		GAIN_LAMBDA,			//!< @todo Remove after
		
		
		GAIN_K1_P,		//!< @todo Remove after
		GAIN_K2_P,		//!< @todo Remove after
		GAIN_LAMBDA_P,	//!< @todo Remove after
		
		
		
		
		/** IMPORTANT THIS SHOULD BE LAST ENUM PARAM */
		FPARAM_COUNT
	};

private:
	/** Buffers des données **/
	float     fparam[FPARAM_COUNT];
	uint32_t  uparam[UPARAM_COUNT];
	uint8_t   watchdogs;

public:

	/** Initialise les pointeurs de données **/
	DataStore()
	{

		memset(this->fparam, 0.0f, FPARAM_COUNT * sizeof(float));
		fparam[DataStore::RC_KILL]   = 1.0f; //Par default la manette devrait toujours etre en kill   au demarrage
		fparam[DataStore::RC_MANUAL] = 1.0f; //Par default la manette devrait toujours etre en manuel au demarrage
		
		fparam[DataStore::PC_MOTOR_ON] = -1.0f; //Par default KillSwith Software est active
		
		//Set default parameters here ...
		fparam[DataStore::GAIN_K1]		= 10.0f; //@todo Remove this ...
		fparam[DataStore::GAIN_K2]		= 3.0f;  //@todo Remove this ...
		fparam[DataStore::GAIN_LAMBDA]	= 1.0f;  //@todo Remove this ...
		
		
		fparam[GAIN_K1_P]			= 1.35f;    //!< @todo Remove after
		fparam[GAIN_K2_P]			= 0.35f;	//!< @todo Remove after
		fparam[GAIN_LAMBDA_P]		= 0.0f;	    //!< @todo Remove after
		
		
		watchdogs = 0;
		
		memset(this->uparam, 0, UPARAM_COUNT * sizeof(uint32_t));
		
		print_dbg("\t[DataStore] instance created \n\r");
	}

	// getters
	inline uint8_t get_watchdogs() {
		return watchdogs;
	}
	inline bool get(enum bparam_e p) {
		return watchdogs & p;
	}
	inline float get(enum fparam_e p) {
		return fparam[p];
	}

	inline uint32_t get(enum uparam_e p) {
		return uparam[p];
	} 
		
	// setters
	inline void set(enum bparam_e p, bool v) {
		if (v)
			watchdogs |= p;
		else
			watchdogs &= ~p;
	}
	inline void set(enum fparam_e p, float v) {
		fparam[p] = v;
	}
	
	inline void set(enum uparam_e p , uint32_t v){
		uparam[p] = v; 
	}

};



/*! @class Snapshot	
	@brief Module qui gere toutes les donnes du systeme.
	
	Cette classe contient un grand tableau qui contient toutes les donnes pertinentes du systeme, dont principalement les mesures des capteurs ainsi que le calcul du controleur.
	L'acces est effectue par setter/getter, sans aucune protection d'acces. Le module offre aussi la possibilite de creer un clone avec la classe Snapshot.
	
	
	@ingroup DataStoreModule	
*/
class Snapshot: public DataStore {
public:
	uint32_t time; //Current system time when snapshot is taken

	void clone(const DataStore& source);
};

#endif
