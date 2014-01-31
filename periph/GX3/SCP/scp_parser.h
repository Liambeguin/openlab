/*
 * This file is part of DronolabSTM32-embedded 
 *
 * DronolabSTM32-embedded is a student project designed 
 * for flying drones.
 *
 * Go to < dronolab.etsmtl.ca > to find out more about 
 * the project
 *
 *
 */

/*
 * scp_parser.h
 *
 *  Created on: Jan 20, 2014
 *      Author: liam 		<liambeguin.at.gmail.com>
 *      Author: Frederic 	<frederic.morin.8@gmail.com>
 */

#ifndef SCP_PARSER_H_
#define SCP_PARSER_H_


#include <stdint.h>
#include <string.h>

/*! @class SCPParser
	@ingroup SCPProtocol
    @brief Classe qui gere la reception de message via le protocole SSPProtocol.

	L'algorithme recherche dans un buffer de byte un ID valide. A partir de l'ID valide, l'algorithme connait le nombre de byte a traite.
	Il effectue la lecture ainsi que le calcul du checksum. Si la validation du checksum est positive, le contenu du buffer est copie dans la structure correspondant au message.


	Voir Frederic Morin pour les details : frederic.morin.8@gmail.com
 */


#define SCPPARSER_OK	(0)
#define SCPPARSER_ERROR	(1)

#define UNKNOWN_MESSAGE_ID (-1)

/** État du parseur **/
enum {
	LOOKING_FOR_KNOWN_MESSAGE_ID, //
	RECEIVING_KNOWN_MESSAGE
} state;


void SCP_reset();
void SCP_init();

inline void* SCP_get_payload();

uint8_t SCP_decode(const char data);
uint8_t SCP_handle_completed_message();


/** Doit retourner la longueur d'un message s'il est connu
 ** @note Doit utiliser "id" pour déterminer la valeur de "bytes_to_read"
 ** @return la longueur du payload du message ou -1 si le message n'existe pas
 **/
int8_t SCP_get_message_length(const uint8_t id);


/** Doit traiter un payload de message complet et valide.
 ** @note Les données à interpréter sont "id" pour l'identifiant et get() pour le contenu
 **/
//void SCP_process_complete_message(const uint8_t id);

/** Fonction appelée pour récupérer le payload
 ** @note L'appel ne s'effectue que si le message en buffer est valide
 **/
//inline int get(void* dst, int len) {
//	memcpy(dst, buff + 1, len);
//	return len;
//}


/*! Fonction qui sert a calcule combien de temps l'algorithme passe a traite les donnees en reception
	    @return La somme de temps passe a recevoir depuis le dernier appel de cette fonction.
 */
inline uint32_t SCP_sum_time_receiving();

#endif /* SCP_PARSER_H_ */
