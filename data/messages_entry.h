/*******************************
 * Projet Cortex  -  Dronolab  *
 * http://dronolab.etsmtl.ca/  *
 *******************************/

/************************************************************************
	Le mechanisme des messages est detaille dans init_msg.h 
************************************************************************/

#ifndef MESSAGE_ENTRY_H
#define MESSAGE_ENTRY_H

#include <inttypes.h>

//namespace Messages {

/* message entry info */
class Entry;
extern Entry* list[];
extern uint8_t count;
extern uint16_t list_size;


class MessageHandler;


/*! @class Entry
	@ingroup MessageModule
    @brief Les Entry ajoutent un niveau de classement au message. Chaque Entry associe au message un type, un ID et une taille. 

	Chaque message du systeme doit avoir un Entry associe. Les classes de message ne sont jamais instancies 
	
	Voir Frederic Morin pour les details : frederic.morin.8@gmail.com
*/
class Entry {
public:
	enum Type {
		TELEMETRY, //
		DEBUG, //
		COMMAND, //
		POWER,
		NOT_TO_SEND
	};

	const uint8_t id;
	const Type type;
	const uint8_t size;
	const uint16_t offset;
	MessageHandler* handler;

	Entry(uint8_t id, Type type, uint8_t size) :
		id(id), type(type), size(size), offset(list_size), handler(0) {
		list[count++] = this;
		list_size += size;
	}

	operator uint8_t() {
		return id;
	}
};

//} // namespace Message

#endif // MESSAGE_ENTRY_H
