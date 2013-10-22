/*******************************
 * Projet Cortex  -  Dronolab  *
 * http://dronolab.etsmtl.ca/  *
 *******************************/

#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <stdint.h>

// gravité à l'ÉTS
#define PHYSICS_G 9.806647326217f // @todo: Changer lors de la location a la COMPETITION.

// gravité à Grand Forks
//#define PHYSICS_G 9.808844993683307

// Constante de gravité du GX3
#define PHYSICS_GX3 9.80665f



/*! @class Parameters

    @brief Classe qui contient les parametres du drone
	
	Les parameters fonctionnent de la meme maniere que le DataStore. La classe contient une structure de donne accessible par getter/setter.
	Les parameters sont toutes les donnes fixes du systeme tel que la masse, inertie et gain des controleurs.
	 
	@todo : 	Ajouter la fonctionnalite pour setter les parametres dans la flash.

*/

class Parameters {

void set_ctrl_physic_params();

public:

	Parameters();

	// peut contenir un max de 127 paramètres

	enum param {

		PHYSICS_JR = 0,		 // rotor inertia [kg * m^2]
		PHYSICS_IXX,		 // inertia moment (x) [kg * m^2]
		PHYSICS_IYY,		 // inertia moment (y) [kg * m^2]
		PHYSICS_IZZ,		 // inertia moment (z) [kg * m^2]
		PHYSICS_L,			 // center-rotor distance [m]
		PHYSICS_B,			 // thrust factor [N*s^2 / rad^2]
		PHYSICS_D,			 // drag factor   [N*m*s^2 / rad^2]  
		PHYSICS_M,			 // drone weight [kg]

		DT,					 // time constant of the fastest element of the system [s]

		RC_FACTOR_THRUST,	 // correction factor for RC effort thrust []
		RC_FACTOR_ROLLPITCH, // correction factor for RC efforts roll and pitch [rad]
		RC_FACTOR_YAW,		 // correction factor for RC effort yaw [rad/s]

		MOTOR_SPEED_MAX,    // maximum speed of the motor [rad/s]
		U1_MAX,				// factor for max throttle [N]

		BIAS_U2,			// bias correction for u2    (Motors misalignment and thrust balancing) [N]
		BIAS_U3,			// bias correction for u3    (Motors misalignment and thrust balancing) [N]
		BIAS_U4,			// bias correction for u4    (Motors misalignment and thrust balancing) [N]
		BIAS_ROLL,			// bias correction for roll  (IMU misalignment) [rad]
		BIAS_PITCH,			// bias correction for pitch (IMU misalignment) [rad]

		BATTERIE_DT,		// battery sample time
		BATTERIE_LSB2MA,	// battery ADC_LSB versus mA
		BATTERIE_LSB2V,		// battery ADC_LSB versus V
		
		MAX_CMD_ANG,	    // Maximum angle that can be output by the position ctrl [rad]
		
		
		
		// *****WATCH OUT*****
		// Pour ajouter un élément, ou pour changer l'odre des éléments du énum,
		// il faut absolument changer le numéro de version dans la liste de param
		// default et updater redmine
		//******WATCH OUT*****
		
			
		
		
		LAST_ENUM_PARAM //Cet élément doit toujours être le dernier du enum.
	};

	enum ctrl_physic_param {

		A1,
		A2,
		A3,
		A4,
		A5,
		B1,
		B2,
		B3,
		NB_CTRL_PARAM
	};

	typedef struct stored_data {
		uint8_t version;
		float params[LAST_ENUM_PARAM];
		uint16_t checksum;
	} stored_data;

	float ctrl_physic_params[NB_CTRL_PARAM];

	stored_data default_;
	stored_data live;
	stored_data* flash_param_;

/*	typedef struct param_snapshot{
			stored_data default_, live, flash;
		};*/

	inline float get(enum param p) {
		return live.params[p];
	}

	inline float get(enum ctrl_physic_param p){
		return 	ctrl_physic_params[p];
	}
	
	
	//@todo reimplemented when flash is functional
	/*
	 LOOP

	 Cette fonction effectue un test d'écriture et de lecture de mémoire
	 flash puis tombe dans une boucle infinie.
	 */
	//void loop();

	/*
	 RESET_PARAMETERS

	 Cette fonction remplace toutes les valeurs live par les valeurs
	 par défaut hardcodés.
	 */
	//void reset_parameters(void);

	/*
	 CLEAR_PARAMETERS

	 Cette fonction emplace toutes les valeurs live par les valeurs en flash.
	 */
	//void clear_parameters(void);

	/*
	 SAVE_PARAMETERS

	 Cette fonction sauvegarde toutes les valeurs courantes dans la flash.
	 */
	//void save_parameters(void);

	//uint8_t get_version(void* ptr);
	//void check_version_compatibility(void);

	//inline void clone_default(stored_data& dest);
};

#endif
