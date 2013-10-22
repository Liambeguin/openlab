/*******************************
 * Projet Cortex  -  Dronolab  *
 * http://dronolab.etsmtl.ca/  *
 *******************************/

#include "parameters.h"
//#include "../../core/flash/flash.h"
#include <core/console/console.h>
#include <singleton.h>
#include <stdio.h>
#include <string.h>
#include <math.h>


#if DEBUG_FLAG_DATA_PARAMETERS
#define param_print_dbg(x)		(Singleton::get()->console << (x))
#define param_print_dbg_hex(x)	(Singleton::get()->console << _HEX(x))
#else
#define param_print_dbg(x)
#define param_print_dbg_hex(x)
#endif


Parameters::Parameters(void) {
	
	
	//Drone V4 NO LRF !
	
	default_.version = 6;
	default_.params[PHYSICS_JR]			 = 0.000078187598;
	default_.params[PHYSICS_IXX]		 = 0.013;//
	default_.params[PHYSICS_IYY]		 = 0.013;//
	default_.params[PHYSICS_IZZ]		 = 0.024;

	default_.params[PHYSICS_L]			 = 0.2125f;				    // Distance d'un moteur au centre de masse en m
	default_.params[PHYSICS_B]			 = 0.00001418713138124377f; // Coéfficient de poussée vefirie avec O1=O2=O3=O4 = (environ) 455 rad/s, m = 1.198kg, g = 9.806647326217 m/s^2
	default_.params[PHYSICS_D]			 = 0.00000184;			    // Coéfficient de trainée (drag)
	default_.params[PHYSICS_M]			 = 1.10;					// Masse en kg

	default_.params[DT]					 = 0.005;// 5ms => 200 Hz

	default_.params[RC_FACTOR_THRUST]	 = 1.25;  //
	default_.params[RC_FACTOR_ROLLPITCH] = 0.52f; // 30 degrés
	default_.params[RC_FACTOR_YAW]		 = 3;     //
		
	default_.params[MOTOR_SPEED_MAX]	 = 750.0f;
	
	default_.params[U1_MAX]				 = (4.0f * default_.params[MOTOR_SPEED_MAX] * default_.params[MOTOR_SPEED_MAX] * default_.params[PHYSICS_B]);//

	default_.params[BIAS_U2]			 = 0;// Roll  -> gain positif corrige debalancement a droite
	default_.params[BIAS_U3]			 = 0;// Pitch -> gain positif corrige debalancement vers lavant
	default_.params[BIAS_U4]			 = 0;//    Yaw
	default_.params[BIAS_ROLL]			 = 0;//
	default_.params[BIAS_PITCH]			 = 0;//

	default_.params[BATTERIE_DT]		 = 0.005;//
	default_.params[BATTERIE_LSB2MA]	 = 88.04;//
	default_.params[BATTERIE_LSB2V]		 = 0.076665;//
	
	
	default_.params[MAX_CMD_ANG]		 =  0.174f; //10 deg
    
	//After this line all parameters are in the live structure.


	//@todo Replace this by real code after
	memcpy(&live, &default_, sizeof(stored_data));

	//@todo Debug this, make the chip die !!!!
	//check_version_compatibility();

	set_ctrl_physic_params();

	print_dbg("\t[Parameters] instance created \n\r");
}

inline void Parameters::set_ctrl_physic_params(){

	ctrl_physic_params[A1] = (live.params[PHYSICS_IYY] - live.params[PHYSICS_IZZ])/live.params[PHYSICS_IXX];
	ctrl_physic_params[A2] = live.params[PHYSICS_JR] / live.params[PHYSICS_IXX];
	ctrl_physic_params[A3] = (live.params[PHYSICS_IZZ] - live.params[PHYSICS_IXX])/live.params[PHYSICS_IYY];
	ctrl_physic_params[A4] = live.params[PHYSICS_JR] / live.params[PHYSICS_IYY];
	ctrl_physic_params[A5] = (live.params[PHYSICS_IXX] - live.params[PHYSICS_IYY])/live.params[PHYSICS_IZZ];
	ctrl_physic_params[B1] = live.params[PHYSICS_L] / live.params[PHYSICS_IXX];
	ctrl_physic_params[B2] = live.params[PHYSICS_L] / live.params[PHYSICS_IYY];
	ctrl_physic_params[B3] = 1.0f / live.params[PHYSICS_IZZ];

}

//@todo Replace this when flash is implemented !!!
//void Parameters::reset_parameters(void) {
	//memcpy(&live, &default_, sizeof(stored_data));
	//save_parameters();
	//param_print_dbg("[FLASH] Default parameters temporarily applied on live parameters\n");
	//param_print_dbg("size:");
	//param_print_dbg(sizeof(stored_data));
	//param_print_dbg(" version:");
	//param_print_dbg(live.version);
	//param_print_dbg("\n");
//
	//set_ctrl_physic_params();
//}
//
//void Parameters::clear_parameters(void) {
	//Singleton::get()->flash.read(&live, sizeof(stored_data));
	//param_print_dbg("[DRIVER] Flash data applied to live parameters\n");
//
	//set_ctrl_physic_params();
//}
//
//void Parameters::save_parameters(void) {
	//Singleton::get()->flash.write(&live, sizeof(stored_data));
	//param_print_dbg("[FLASH] Live parameters stored in flash memory\n");
//
	//set_ctrl_physic_params();
//}
//
//
//uint8_t Parameters::get_version(void* ptr) {
	//stored_data* data;
	//data = (stored_data*) ptr;
	//return data->version;
//
//}
//
//void Parameters::check_version_compatibility(void) {
	//param_print_dbg("[DRIVER] Checking stored parameters version...");
	//uint8_t version_default = get_version(&default_);
	//uint8_t version_flash = get_version(Singleton::get()->flash.flash_param_);
	//if (version_default == version_flash) {
//
		//param_print_dbg(" version ");
		//param_print_dbg((int) version_flash);
		//param_print_dbg(" OK\n");
//
		//clear_parameters();
	//} else {
//
		//param_print_dbg("\n[DRIVER]   version error: ");
		//param_print_dbg((int) version_default);
		//param_print_dbg(" != ");
		//param_print_dbg((int) version_flash);
		//param_print_dbg("... recovering default settings\n");
//
		//reset_parameters();
//
	//}
//}
//
//
//
//
//inline void Parameters::clone_default(stored_data& dest) {
	//memcpy(dest.params, this->default_.params, LAST_ENUM_PARAM * sizeof(float));
//}
//